// This is brcv/mvg/dvpgl/algo/pro/dvpgl_vsol_lens_warp_process.cxx

//:
// \file

#include "dvpgl_vsol_lens_warp_process.h"
#include <vgl/vgl_point_2d.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vpgl/vpgl_poly_radial_distortion.h>
#include <vpgl/algo/vpgl_lens_warp_mapper.h>
#include <bpgl/algo/bpgl_vsol_lens_warp.h>


//: Constructor
dvpgl_vsol_lens_warp_process::dvpgl_vsol_lens_warp_process()
{
  if( !parameters()->add( "center x" , "-cx" ,  0.0 ) ||
      !parameters()->add( "center y" , "-cy" ,  0.0 ) ||
      !parameters()->add( "k1" , "-k1" ,  0.0 ) ||
      !parameters()->add( "k2" , "-k2" ,  0.0 ) ||
      !parameters()->add( "k3" , "-k3" ,  0.0 ) ||
      !parameters()->add( "k4" , "-k4" ,  0.0 ) ||
      !parameters()->add( "k5" , "-k5" ,  0.0 ) ||
      !parameters()->add( "k6" , "-k6" ,  0.0 ) ||
      !parameters()->add( "midpoint threshold" , "-mpthresh" ,  -1.0 ) ||
      !parameters()->add( "invert" , "-invert" ,  false ) ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  } 
}


//: Clone the process
bpro1_process* 
dvpgl_vsol_lens_warp_process::clone() const
{
  return new dvpgl_vsol_lens_warp_process(*this);
}


//: Return the name of the process
vcl_string
dvpgl_vsol_lens_warp_process::name()
{
  return "Lens Distort VSOL";
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > 
dvpgl_vsol_lens_warp_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string > 
dvpgl_vsol_lens_warp_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Returns the number of input frames to this process
int
dvpgl_vsol_lens_warp_process::input_frames()
{
  return 1;
}


//: Returns the number of output frames from this process
int
dvpgl_vsol_lens_warp_process::output_frames()
{
  return 1;
}


//: Run the process on the current frame
bool
dvpgl_vsol_lens_warp_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cerr << __FILE__ << " - not exactly one input frame" << vcl_endl;
    return false;
  }

  double k[6], cx=0, cy=0, mpthresh=0;
  bool invert=false;
  parameters()->get_value( "-cx" , cx );
  parameters()->get_value( "-cy" , cy );
  parameters()->get_value( "-k1" , k[0] );
  parameters()->get_value( "-k2" , k[1] );
  parameters()->get_value( "-k3" , k[2] );
  parameters()->get_value( "-k4" , k[3] );
  parameters()->get_value( "-k5" , k[4] );
  parameters()->get_value( "-k6" , k[5] );
  parameters()->get_value( "-mpthresh" , mpthresh );
  parameters()->get_value( "-invert" , invert );
  vgl_point_2d<double> center(cx,cy);

  // find the order of the polynomial
  unsigned int order = 6;
  while(order > 0 && k[order--] == 0.0);
  
  vpgl_lens_distortion<double>* lens = 0;
 
  switch(order){
    case 1: lens = new vpgl_poly_radial_distortion<double,1>(center,k); break;
    case 2: lens = new vpgl_poly_radial_distortion<double,2>(center,k); break;
    case 3: lens = new vpgl_poly_radial_distortion<double,3>(center,k); break;
    case 4: lens = new vpgl_poly_radial_distortion<double,4>(center,k); break;
    case 5: lens = new vpgl_poly_radial_distortion<double,5>(center,k); break;
    case 6: lens = new vpgl_poly_radial_distortion<double,6>(center,k); break;
    default: return false;
  }

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
    for(unsigned v=0; v<objs.size(); ++v)
      output_vsol->add_object(bpgl_vsol_lens_warp(objs[v], *lens, invert, mpthresh), 
                              groups[g]);
  }
  
  delete lens;
  return true;
}



//: Finish
bool
dvpgl_vsol_lens_warp_process::finish()
{
  return true;
}




