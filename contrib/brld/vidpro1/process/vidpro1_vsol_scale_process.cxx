// This is brcv/mvg/dvpgl/algo/pro/vidpro1_vsol_scale_process.cxx

//:
// \file

#include "vidpro1_vsol_scale_process.h"
#include <vgl/vgl_point_2d.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_line_2d_sptr.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_digital_curve_2d.h>
#include <vsol/vsol_digital_curve_2d_sptr.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polygon_2d_sptr.h>


//: Constructor
vidpro1_vsol_scale_process::vidpro1_vsol_scale_process()
{
  if( !parameters()->add( "scale" , "-scale" ,  0.5f )  ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  } 
}


//: Clone the process
bpro1_process* 
vidpro1_vsol_scale_process::clone() const
{
  return new vidpro1_vsol_scale_process(*this);
}


//: Return the name of the process
vcl_string
vidpro1_vsol_scale_process::name()
{
  return "Scale vsol";
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > 
vidpro1_vsol_scale_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string > 
vidpro1_vsol_scale_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Returns the number of input frames to this process
int
vidpro1_vsol_scale_process::input_frames()
{
  return 1;
}


//: Returns the number of output frames from this process
int
vidpro1_vsol_scale_process::output_frames()
{
  return 1;
}

static vsol_point_2d_sptr
scale_vsol(const vsol_point_2d_sptr& pt, float scale)
{
  vgl_point_2d<double> p = pt->get_p(); 
  return new vsol_point_2d( vgl_point_2d<double>(p.x()*scale, p.y()*scale) );
}

static vsol_spatial_object_2d_sptr
scale_vsol(const vsol_spatial_object_2d_sptr& so, float scale)
{
  // FIXME: fill in missing vsol2D objects
  if(vsol_point_2d_sptr pt = so->cast_to_point())
  {
    return scale_vsol(pt, scale).ptr();
  }
  else if(vsol_curve_2d* curve = so->cast_to_curve())
  {
    if(vsol_line_2d_sptr line = curve->cast_to_line())
    {
      vsol_point_2d_sptr p0 = scale_vsol(line->p0(), scale);
      vsol_point_2d_sptr p1 = scale_vsol(line->p1(), scale);
      return new vsol_line_2d(p0,p1);
    }
    else if(vsol_polyline_2d_sptr pline = curve->cast_to_polyline())
    {
      vcl_vector<vsol_point_2d_sptr> pts;
      for(unsigned int i=0; i<pline->size(); ++i)
        pts.push_back(scale_vsol(pline->vertex(i),scale));
      return new vsol_polyline_2d(pts);
    }
    else if(vsol_digital_curve_2d_sptr dc = curve->cast_to_digital_curve())
    {
      vcl_vector<vsol_point_2d_sptr> pts;
      for(unsigned int i=0; i<dc->size(); ++i)
        pts.push_back(scale_vsol(dc->point(i),scale));
      return new vsol_digital_curve_2d(pts);
    }
  }
  else if(vsol_region_2d* region = so->cast_to_region())
  {
    if(vsol_polygon_2d_sptr pg = region->cast_to_polygon())
    {
      vcl_vector<vsol_point_2d_sptr> pts;
      for(unsigned int i=0; i<pg->size(); ++i)
        pts.push_back(scale_vsol(pg->vertex(i),scale));
      return new vsol_polygon_2d(pts);
    }
  }
  return NULL;
}


//: Run the process on the current frame
bool
vidpro1_vsol_scale_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cerr << __FILE__ << " - not exactly one input frame" << vcl_endl;
    return false;
  }

  float scale=0;
  parameters()->get_value( "-scale" , scale );


  // get the input vsol data
  vidpro1_vsol2D_storage_sptr frame_vsol;
  frame_vsol.vertical_cast(input_data_[0][0]);
  
  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_data_[0].push_back(output_vsol);
  
  vcl_vector<vcl_string> groups = frame_vsol->groups();
  for(unsigned g=0; g<groups.size(); ++g)
  {
    vcl_vector<vsol_spatial_object_2d_sptr> objs 
      = frame_vsol->data_named(groups[g]);
    for(unsigned v=0; v<objs.size(); ++v){
      output_vsol->add_object(scale_vsol(objs[v],scale), 
                              groups[g]);
    }
  }
  
  return true;
}



//: Finish
bool
vidpro1_vsol_scale_process::finish()
{
  return true;
}




