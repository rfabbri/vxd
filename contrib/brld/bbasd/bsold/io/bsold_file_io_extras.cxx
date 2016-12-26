// This is bsold/io/bsold_file_io_extras.cxx
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma implementation
#endif

//:
// \file


#include "bsold_file_io_extras.h"

#include <vcl_iostream.h>
#include <vcl_cstring.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_line_2d_sptr.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_polygon_2d_sptr.h>

#include <vnl/vnl_math.h>
#include <vul/vul_psfile.h>
#include <vul/vul_file.h>
#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_image_resource_sptr.h>
#include <vil/vil_image_view_base.h>

// -----------------------------------------------------------------------------
//: Write an image and a set of polygons to a ps file
// TODO: we currently only handle polygons. Need to handle other types
bool bsold_save_ps_file(const vcl_string& filename,
                        const vil_image_resource_sptr& img, 
                        const vcl_vector<vsol_spatial_object_2d_sptr >& vsol_data,
                        const vcl_vector<vil_rgb<float > >& colors,
                        float line_width,
                        float point_size)
{
  vcl_cout << "Save to postscript file\n";

  //1)If file open fails, return.
  vul_psfile psfile(filename.c_str(), false);

  if (!psfile){
    vcl_cout << "  Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  if (!(vsol_data.size() == colors.size())) {
    vcl_cout << "  vsol_data vector is not the same size as color vector!!!\n";
    return false;
  }

  // write image
  vil_image_view<vxl_byte> image = img->get_view(0, img->ni(), 0, img->nj());
  int sizex = image.ni();
  int sizey = image.nj();
  int planes = image.nplanes();

  unsigned char *buf = new unsigned char[sizex*sizey*3];
  if (planes == 3) 
  {
    vcl_cout << "  processing color image ...";
    for (int x=0; x<sizex; ++x) 
    {
      for (int y=0; y<sizey; ++y) 
      {
        buf[3*(x+sizex*y)  ] = image(x,y,0);
        buf[3*(x+sizex*y)+1] = image(x,y,1);
        buf[3*(x+sizex*y)+2] = image(x,y,2);
      }
    }
  } 
  else if (planes == 1) 
  {
    vcl_cout << "  processing grey image ...";
    for (int x=0; x<sizex; ++x) 
    {
      for (int y=0; y<sizey; ++y) 
      {
        buf[3*(x+sizex*y)  ] = image(x,y,0);
        buf[3*(x+sizex*y)+1] = image(x,y,0);
        buf[3*(x+sizex*y)+2] = image(x,y,0);
      }
    }
  }
  psfile.print_color_image(buf,sizex,sizey);
  delete [] buf;
  vcl_cout << "done.\n";


  // write vsol objects
  // we can only handle several types of vsol objects

  vcl_cout << "  Processing vsol2D objects ... ";
  psfile.set_scale_x(50);
  psfile.set_scale_y(50);

  // set background color for all vsol2D objects 
  // (so that they stand out from the image)
  vil_rgb<float > bg_color(1.0f, 1.0f, 1.0f); // white background

  // iterate thru the vsol objects  and write out one by one
  for (unsigned k =0; k < vsol_data.size(); ++k)
  {
    vsol_spatial_object_2d_sptr obj = vsol_data[k];
    vil_rgb<float > color = colors[k];
    
    // point
    if (obj->cast_to_point())
    {
      vsol_point_2d_sptr pt = obj->cast_to_point();

      // white background for object (2x thicker than the linewidth)
      psfile.set_fg_color(bg_color.R(), bg_color.G(), bg_color.B());
      psfile.point(float(pt->x()), float(pt->y()), 2*point_size);

      // the actual point
      psfile.set_fg_color(color.R(), color.G(), color.B());
      psfile.point(float(pt->x()), float(pt->y()), 2*point_size);
    }

    // line
    else if (obj->cast_to_curve() && obj->cast_to_curve()->cast_to_line())
    {
      vsol_line_2d_sptr line = obj->cast_to_curve()->cast_to_line();
      vsol_point_2d_sptr p0 = line->p0();
      vsol_point_2d_sptr p1 = line->p1();

      // ignore degenerate line
      if (!p0 || !p1)
        continue;

      // background for the object (2x thicker than the linewidth)
      psfile.set_fg_color(bg_color.R(), bg_color.G(), bg_color.B());
      psfile.set_line_width(2*line_width);
      psfile.line( float(p0->x()), float(p0->y()), float(p1->x()), float(p1->y()) );

      // the line
      psfile.set_fg_color(color.R(), color.G(), color.B());
      psfile.set_line_width(line_width);
      psfile.line( float(p0->x()), float(p0->y()), float(p1->x()), float(p1->y()) );
    }

    // polygon
    else if (obj->cast_to_region() && obj->cast_to_region()->cast_to_polygon())
    {
      vsol_polygon_2d_sptr polygon = obj->cast_to_region()->cast_to_polygon();
      
      // ignore degenerate polygon
      if (polygon->size() < 2)
        continue;

      // white background for the object (2x thicker than the linewidth)
      psfile.set_fg_color(bg_color.R(), bg_color.G(), bg_color.B());
      psfile.set_line_width(2*line_width);
      for (unsigned i=0; i< polygon->size(); ++i)
      {
        int next_i = (i+1) % polygon->size();
        vsol_point_2d_sptr p1 = polygon->vertex(i);
        vsol_point_2d_sptr p2 = polygon->vertex(next_i);
        psfile.line((float)p1->x(), (float)p1->y(), (float)p2->x(), (float)p2->y());
      }
    
      // the polygon, with specified color
      psfile.set_line_width(line_width);
      psfile.set_fg_color(color.R(), color.G(), color.B());
      for (unsigned i =0; i< polygon->size(); ++i)
      {
        int next_i = (i+1) % polygon->size();
        vsol_point_2d_sptr p1 = polygon->vertex(i);
        vsol_point_2d_sptr p2 = polygon->vertex(next_i);
        psfile.line((float)p1->x(), (float)p1->y(), (float)p2->x(), (float)p2->y());
      }
    }
    
    // polyline
    else if (obj->cast_to_curve() && obj->cast_to_curve()->cast_to_polyline())
    {
      vsol_polyline_2d_sptr polyline = obj->cast_to_curve()->cast_to_polyline();

      // ignore degenerate polyline
      if (polyline->size() < 2)
        continue;

      // white background for the object (2x thicker than the linewidth)
      psfile.set_fg_color(bg_color.R(), bg_color.G(), bg_color.B());
      psfile.set_line_width(2*line_width);
      for (unsigned i =0; i< (polyline->size()-1); ++i)
      {
        int next_i = i+1;
        vsol_point_2d_sptr p1 = polyline->vertex(i);
        vsol_point_2d_sptr p2 = polyline->vertex(next_i);
        psfile.line((float)p1->x(), (float)p1->y(), (float)p2->x(), (float)p2->y());
      }
    
      // the polyline, with specified color
      psfile.set_line_width(line_width);
      psfile.set_fg_color(color.R(), color.G(), color.B());
      for (unsigned i =0; i< (polyline->size()-1); ++i)
      {
        int next_i = i+1;
        vsol_point_2d_sptr p1 = polyline->vertex(i);
        vsol_point_2d_sptr p2 = polyline->vertex(next_i);
        psfile.line((float)p1->x(), (float)p1->y(), (float)p2->x(), (float)p2->y());
      }
    }
  }
  vcl_cout << " done.\n";

  ////close file
  //psfile1.close();
  return true;
}
