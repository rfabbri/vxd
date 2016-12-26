// This is dbsol/dbsol_file_io.cxx
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma implementation
#endif

//:
// \file


#include "dbsol_file_io.h"

#include <vcl_iostream.h>
#include <vcl_cstring.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_line_2d_sptr.h>

#include <vnl/vnl_math.h>
#include <vul/vul_psfile.h>
#include <vul/vul_file.h>
#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_image_resource_sptr.h>
#include <vil/vil_image_view_base.h>

#include <dbsol/dbsol_config.h>

#ifdef HAS_BOOST
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#endif


// -----------------------------------------------------------------------------
//: Load a .CON file and save results to a vector of points and whether the 
// contour is closed. Return false if loading fails
bool dbsol_load_con_file(char const* filename, 
                         vcl_vector<vsol_point_2d_sptr >& points, 
                         bool & is_closed)
{
  // open file for reading
  vcl_ifstream infp(filename, vcl_ios_in);
  if (!infp) 
  {
    vcl_cerr << " Error opening file  " << filename << vcl_endl;
    return false;
  }

  // check header
  char line_buffer[2000]; //200
  infp.getline(line_buffer,2000);
  if (vcl_strncmp(line_buffer,"CONTOUR",7)) 
  {
    vcl_cerr << "Invalid File " << filename << vcl_endl
             << "Should be CONTOUR " << line_buffer << vcl_endl;
    return false;
  }

  // check whether contour is open or closed
  char open_flag[2000];
  infp.getline(open_flag,2000);
  if (!vcl_strncmp(open_flag,"OPEN",4))
    is_closed = false;
  else if (!vcl_strncmp(open_flag,"CLOSE",5))
    is_closed = true;
  else
  {
    vcl_cerr << "Invalid File " << filename << vcl_endl
             << "Should be OPEN/CLOSE " << open_flag << vcl_endl;
    return false;
  }


  // read coordinates of the points
  int num_pts;
  infp >> num_pts;

  // vector to store the points
  points.clear();
  points.reserve(num_pts);
  for (int i=0;i<num_pts;i++) 
  {
    float x,y;
    infp >> x >> y;
    vsol_point_2d_sptr new_pt = new vsol_point_2d (x,y);
    points.push_back(new_pt);
  }

  //close file
  infp.close();

  return true;
}


 
// ----------------------------------------------------------------------------
//: Load a .CON file and return a smart pointer to vsol_spatial_object_2d
// This can be either a polyline or a polygon. Use cast function to get the
// exact type
vsol_spatial_object_2d_sptr dbsol_load_con_file(char const* filename)
{  
  vcl_vector<vsol_point_2d_sptr > points;
  bool is_closed;
  if (!dbsol_load_con_file(filename, points, is_closed))
    return 0;

  
  // create a polyline or a polygon depending on whether the contour
  // file is labelled OPEN or CLOSE
  if (is_closed) 
  {
    return new vsol_polygon_2d (points);
  }
  else 
  {
    return new vsol_polyline_2d (points);
  }
}


// ----------------------------------------------------------------------------
//: Save a polyline to a .CON file. Return false if saving fails
bool dbsol_save_con_file(char const* filename, vsol_polyline_2d_sptr polyline)
{
  vcl_vector<vsol_point_2d_sptr > pts;
  pts.reserve(polyline->size());
  for (unsigned int i=0; i<polyline->size(); ++i)
  {
    pts.push_back(polyline->vertex(i));
  }
  return dbsol_save_con_file(filename, pts, false);
}


// ----------------------------------------------------------------------------
//: Save a polygon to a .CON file. Return false if saving fails
bool dbsol_save_con_file(char const* filename, vsol_polygon_2d_sptr polygon)
{
  vcl_vector<vsol_point_2d_sptr > pts;
  pts.reserve(polygon->size());
  for (unsigned int i=0; i<polygon->size(); ++i)
  {
    pts.push_back(polygon->vertex(i));
  }
  return dbsol_save_con_file(filename, pts, true);
}


// ----------------------------------------------------------------------------
//: Save a set of ordered points to a .CON file. Return false if saving fails
bool dbsol_save_con_file(char const* filename, 
                         const vcl_vector<vsol_point_2d_sptr >& pts,
                         bool is_closed)
{
  // 1) create the file
  //vcl_ifstream infp(filename, vcl_ios::in);
  vcl_ofstream outfp(filename, vcl_ios_out);
  if (!outfp)
  {
    vcl_cout << " Error writing file  " << filename << vcl_endl;
    return false;
  }

  // 2) start writing out the contour to the file
  
  // header
  outfp << "CONTOUR" << vcl_endl;

  // The contour can either be a polyline producing an open contour 
  // or a polygon producing a close contour          
  if (is_closed)
  {
    outfp << "CLOSE" << vcl_endl;
  }
  else
  {
    outfp << "OPEN" << vcl_endl;
  }

  // Number of points
  outfp << pts.size() << vcl_endl;

  // Write coordinates of the points
  for (unsigned int i=0; i<pts.size(); ++i)
  {
    vsol_point_2d_sptr pt = pts[i];
    outfp <<pt->x() << " " << pt->y() << vcl_endl;
  }

  // 3) close the file
  outfp.close();

  return true;
}


#ifdef HAS_BOOST
static bool dbsol_load_cem_gzip(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename);
static bool dbsol_save_cem_gzip(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename);
#endif

static bool dbsol_load_cem_ascii(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename);
static bool dbsol_save_cem_ascii(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename);

bool dbsol_load_cem(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename)
{
  vcl_string ext = vul_file::extension(filename);

  if (ext == ".gz") {
#ifdef HAS_BOOST
    return dbsol_load_cem_gzip(contours, filename);
#else
    vcl_cerr << "Error: .gz compressed file was provided, but boost wasn't found\n";
#endif
  } else
    return dbsol_load_cem_ascii(contours, filename);
}

bool dbsol_save_cem(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename)
{
  vcl_string ext = vul_file::extension(filename);

  if (ext == ".gz") {
#ifdef HAS_BOOST
    return dbsol_save_cem_gzip(vsol_list, filename);
#else
    vcl_cerr << "Error: .gz compressed filename was provided, but boost wasn't found\n";
#endif
  } else
    return dbsol_save_cem_ascii(vsol_list, filename);
}


// -----------------------------------------------------------------------------
// Load .CEM file
bool dbsol_load_cem_ascii(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename)
{
  float x, y;
  char lineBuffer[1024];
  int numContours, numTotalEdges, numEdges;
  int ix, iy;
  double idir, iconf, dir, conf;

  //1)If file open fails, return.
  vcl_ifstream infp(filename.c_str(), vcl_ios::in);

  if (!infp){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  //2)Read in each line
  while (infp.getline(lineBuffer,1024)) {

    //ignore comment lines and empty lines
    if (strlen(lineBuffer)<2 || lineBuffer[0]=='#')
      continue;

    //read the line with the contour count info
    if (!strncmp(lineBuffer, "CONTOUR_COUNT=", sizeof("CONTOUR_COUNT=")-1)){
      sscanf(lineBuffer,"CONTOUR_COUNT=%d",&(numContours));
      //vcl_cout << numContours << vcl_endl;
      continue;
    }

    //read the line with the edge count info
    if (!strncmp(lineBuffer, "TOTAL_EDGE_COUNT=", sizeof("TOTAL_EDGE_COUNT=")-1)){
      sscanf(lineBuffer,"TOTAL_EDGE_COUNT=%d",&(numTotalEdges));
      //vcl_cout << numTotalEdges << vcl_endl;
      continue;
    }

    //read the beginning of a contour block
    if (!strncmp(lineBuffer, "[BEGIN CONTOUR]", sizeof("[BEGIN CONTOUR]")-1)){

      //discarding other information for now...should really be outputting edgels
      vcl_vector< vsol_point_2d_sptr > points;

      infp.getline(lineBuffer,1024);
      sscanf(lineBuffer,"EDGE_COUNT=%d",&(numEdges));
      //vcl_cout << numEdges << vcl_endl;

      for (int j=0; j< numEdges; j++){
        //the rest should have data that goes into the current contour
        infp.getline(lineBuffer,1024);
        sscanf(lineBuffer," [%d, %d]\t%f\t%f\t[%f, %f]\t%f\t%f",&(ix), &(iy),
              &(idir), &(iconf), &(x), &(y), &(dir), &(conf));

        //VJ's current CEM is in degrees rather than radians so need to convert
        dir += 90;
        dir *= vnl_math::pi/180;

        vsol_point_2d_sptr newPt = new vsol_point_2d (x,y);
        points.push_back(newPt);
      }

      infp.getline(lineBuffer,1024);

    while (strncmp(lineBuffer, "[END CONTOUR]", sizeof(" [END CONTOUR]")-1)) {

      infp.getline(lineBuffer,1024);

    }
      //if (strncmp(lineBuffer, "[END CONTOUR]", sizeof(" [END CONTOUR]")-1))
        //assert(false);

      vsol_polyline_2d_sptr newContour = new vsol_polyline_2d (points);
      contours.push_back(newContour->cast_to_spatial_object());
    }
  }
  infp.close();

  vcl_cout << "Loaded: " << filename.c_str() << ".\n";
  return true;
}

#ifdef HAS_BOOST
// -----------------------------------------------------------------------------
// Load .CEM file compressed with zlib, gzip style.
// TODO: use templating and/or istream inheritance to avoid duplicating almost
// identical code to dbsol_load_cem_ascii
bool dbsol_load_cem_gzip(vcl_vector< vsol_spatial_object_2d_sptr >& contours, vcl_string filename)
{
  float x, y;
  char lineBuffer[1024];
  int numContours, numTotalEdges, numEdges;
  int ix, iy;
  double idir, iconf, dir, conf;

  //1)If file open fails, return.
  vcl_ifstream infp_orig(filename.c_str(), vcl_ios::in | vcl_ios::binary);

  if (!infp_orig){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  boost::iostreams::filtering_istream infp;
  infp.push(boost::iostreams::gzip_decompressor());
  infp.push(infp_orig);

  //2)Read in each line
  while (infp.getline(lineBuffer,1024)) {

    //ignore comment lines and empty lines
    if (strlen(lineBuffer)<2 || lineBuffer[0]=='#')
      continue;

    //read the line with the contour count info
    if (!strncmp(lineBuffer, "CONTOUR_COUNT=", sizeof("CONTOUR_COUNT=")-1)){
      sscanf(lineBuffer,"CONTOUR_COUNT=%d",&(numContours));
      //vcl_cout << numContours << vcl_endl;
      continue;
    }

    //read the line with the edge count info
    if (!strncmp(lineBuffer, "TOTAL_EDGE_COUNT=", sizeof("TOTAL_EDGE_COUNT=")-1)){
      sscanf(lineBuffer,"TOTAL_EDGE_COUNT=%d",&(numTotalEdges));
      //vcl_cout << numTotalEdges << vcl_endl;
      continue;
    }

    //read the beginning of a contour block
    if (!strncmp(lineBuffer, "[BEGIN CONTOUR]", sizeof("[BEGIN CONTOUR]")-1)){

      //discarding other information for now...should really be outputting edgels
      vcl_vector< vsol_point_2d_sptr > points;

      infp.getline(lineBuffer,1024);
      sscanf(lineBuffer,"EDGE_COUNT=%d",&(numEdges));
      //vcl_cout << numEdges << vcl_endl;

      for (int j=0; j< numEdges; j++){
        //the rest should have data that goes into the current contour
        infp.getline(lineBuffer,1024);
        sscanf(lineBuffer," [%d, %d]\t%f\t%f\t[%f, %f]\t%f\t%f",&(ix), &(iy),
              &(idir), &(iconf), &(x), &(y), &(dir), &(conf));

        //VJ's current CEM is in degrees rather than radians so need to convert
        dir += 90;
        dir *= vnl_math::pi/180;

        vsol_point_2d_sptr newPt = new vsol_point_2d (x,y);
        points.push_back(newPt);
      }

      infp.getline(lineBuffer,1024);

    while (strncmp(lineBuffer, "[END CONTOUR]", sizeof(" [END CONTOUR]")-1)) {

      infp.getline(lineBuffer,1024);

    }
      //if (strncmp(lineBuffer, "[END CONTOUR]", sizeof(" [END CONTOUR]")-1))
        //assert(false);

      vsol_polyline_2d_sptr newContour = new vsol_polyline_2d (points);
      contours.push_back(newContour->cast_to_spatial_object());
    }
  }

  vcl_cout << "Loaded: " << filename.c_str() << ".\n";
  return true;
}
#endif //! HAS_BOOST

// -----------------------------------------------------------------------------
//: Save .CEM file
bool dbsol_save_cem_ascii(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename)
{
  //1)If file open fails, return.
  vcl_ofstream outfp(filename.c_str(), vcl_ios::out);

  if (!outfp){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  // output header information
   outfp <<"# CONTOUR_EDGE_MAP : Logical-Linear + Shock_Grouping"<<vcl_endl;
   outfp <<"# .cem files"<<vcl_endl;
   outfp <<"#"<<vcl_endl;
   outfp <<"# Format :"<<vcl_endl;
   outfp <<"# Each contour block will consist of the following"<<vcl_endl;
   outfp <<"# [BEGIN CONTOUR]"<<vcl_endl;
   outfp <<"# EDGE_COUNT=num_of_edges"<<vcl_endl;
   outfp <<"# [Pixel_Pos]  Pixel_Dir Pixel_Conf  [Sub_Pixel_Pos] Sub_Pixel_Dir Sub_Pixel_Conf "<<vcl_endl;
   outfp <<"# ..."<<vcl_endl;
   outfp <<"# ..."<<vcl_endl;
   outfp <<"# [END CONTOUR]"<<vcl_endl;

  outfp<<vcl_endl;

  // Note: this count is currently missing
   outfp <<"CONTOUR_COUNT="<< vcl_endl;
   outfp <<"TOTAL_EDGE_COUNT="<<vcl_endl;

  // parse through all the vsol classes and save curve objects only
   
  
  for (unsigned int b = 0 ; b < vsol_list.size() ; b++ )
  {
    if( vsol_list[b]->cast_to_curve())
    {
      if( vsol_list[b]->cast_to_curve()->cast_to_polyline() )
      {
        outfp <<"[BEGIN CONTOUR]"<<vcl_endl;
        outfp <<"EDGE_COUNT="<< vsol_list[b]->cast_to_curve()->cast_to_polyline()->size() <<vcl_endl;
        
        for (unsigned int i=0; i<vsol_list[b]->cast_to_curve()->cast_to_polyline()->size();i++)
        {
          vsol_point_2d_sptr pt = vsol_list[b]->cast_to_curve()->cast_to_polyline()->vertex(i);
          //output as subpixel contours
          // [%d, %d]\t%lf\t%lf\t[%lf, %lf]\t%lf\t%lf
          outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;
        }
        outfp <<"[END CONTOUR]"<<vcl_endl<<vcl_endl;
      }
    } else if (vsol_list[b]->cast_to_region()) {
      if (vsol_list[b]->cast_to_region()->cast_to_polygon() )
      {
        outfp <<"[BEGIN CONTOUR]"<<vcl_endl;
        outfp <<"EDGE_COUNT="<< vsol_list[b]->cast_to_region()->cast_to_polygon()->size()+1 <<vcl_endl;
        
        for (unsigned int i=0; i<vsol_list[b]->cast_to_region()->cast_to_polygon()->size();i++)
        {
          vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(i);
          //output as subpixel contours
          // [%d, %d]\t%lf\t%lf\t[%lf, %lf]\t%lf\t%lf
          outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;
        }
        vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(0);  // print the first point once more to include last edge
        outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;

        outfp <<"[END CONTOUR]"<<vcl_endl<<vcl_endl;
      }
    }
  }
  
  return true;
}

#ifdef HAS_BOOST
// -----------------------------------------------------------------------------
//: Save .CEM file
bool dbsol_save_cem_gzip(vcl_vector< vsol_spatial_object_2d_sptr >& vsol_list, vcl_string filename)
{
  //1)If file open fails, return.
  vcl_ofstream outfp_orig(filename.c_str(), vcl_ios::out | vcl_ios::binary);

  if (!outfp_orig){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  boost::iostreams::filtering_ostream outfp;
  outfp.push(boost::iostreams::gzip_compressor());
  outfp.push(outfp_orig);

  // output header information
   outfp <<"# CONTOUR_EDGE_MAP : Logical-Linear + Shock_Grouping"<<vcl_endl;
   outfp <<"# .cem files"<<vcl_endl;
   outfp <<"#"<<vcl_endl;
   outfp <<"# Format :"<<vcl_endl;
   outfp <<"# Each contour block will consist of the following"<<vcl_endl;
   outfp <<"# [BEGIN CONTOUR]"<<vcl_endl;
   outfp <<"# EDGE_COUNT=num_of_edges"<<vcl_endl;
   outfp <<"# [Pixel_Pos]  Pixel_Dir Pixel_Conf  [Sub_Pixel_Pos] Sub_Pixel_Dir Sub_Pixel_Conf "<<vcl_endl;
   outfp <<"# ..."<<vcl_endl;
   outfp <<"# ..."<<vcl_endl;
   outfp <<"# [END CONTOUR]"<<vcl_endl;

  outfp<<vcl_endl;

  // Note: this count is currently missing
   outfp <<"CONTOUR_COUNT="<< vcl_endl;
   outfp <<"TOTAL_EDGE_COUNT="<<vcl_endl;

  // parse through all the vsol classes and save curve objects only
   
  
  for (unsigned int b = 0 ; b < vsol_list.size() ; b++ )
  {
    if( vsol_list[b]->cast_to_curve())
    {
      if( vsol_list[b]->cast_to_curve()->cast_to_polyline() )
      {
        outfp <<"[BEGIN CONTOUR]"<<vcl_endl;
        outfp <<"EDGE_COUNT="<< vsol_list[b]->cast_to_curve()->cast_to_polyline()->size() <<vcl_endl;
        
        for (unsigned int i=0; i<vsol_list[b]->cast_to_curve()->cast_to_polyline()->size();i++)
        {
          vsol_point_2d_sptr pt = vsol_list[b]->cast_to_curve()->cast_to_polyline()->vertex(i);
          //output as subpixel contours
          // [%d, %d]\t%lf\t%lf\t[%lf, %lf]\t%lf\t%lf
          outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;
        }
        outfp <<"[END CONTOUR]"<<vcl_endl<<vcl_endl;
      }
    } else if (vsol_list[b]->cast_to_region()) {
      if (vsol_list[b]->cast_to_region()->cast_to_polygon() )
      {
        outfp <<"[BEGIN CONTOUR]"<<vcl_endl;
        outfp <<"EDGE_COUNT="<< vsol_list[b]->cast_to_region()->cast_to_polygon()->size()+1 <<vcl_endl;
        
        for (unsigned int i=0; i<vsol_list[b]->cast_to_region()->cast_to_polygon()->size();i++)
        {
          vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(i);
          //output as subpixel contours
          // [%d, %d]\t%lf\t%lf\t[%lf, %lf]\t%lf\t%lf
          outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;
        }
        vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(0);  // print the first point once more to include last edge
        outfp <<" [0, 0]  0.0  0.0  [" << pt->x() << ", " << pt->y() << "]  0.0  0.0"  << vcl_endl;

        outfp <<"[END CONTOUR]"<<vcl_endl<<vcl_endl;
      }
    }
  }
  return true;
}
#endif //!HAS_BOOST





// -----------------------------------------------------------------------------
//: Write an image and a set of polygons to a ps file
// TODO: we currently only handle polygons. Need to handle other types
bool dbsol_save_ps_file(const vcl_string& filename,
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
