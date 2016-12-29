// This is brcv/mvg/dvpgl/algo/pro/dvpgl_image_lens_warp_process.cxx

//:
// \file

#include "dvpgl_image_lens_warp_process.h"
#include <vgl/vgl_point_2d.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_convert.h>
#include <vil/vil_warp.h>
#include <vil/vil_bilin_interp.h>
#include <vil/vil_new.h>
#include <vpgl/vpgl_poly_radial_distortion.h>
#include <vpgl/algo/vpgl_lens_warp_mapper.h>
#include <bpgl/algo/bpgl_vsol_lens_warp.h>


//: Constructor
dvpgl_image_lens_warp_process::dvpgl_image_lens_warp_process()
{
  if( !parameters()->add( "center x" , "-cx" ,  160.0 ) ||
      !parameters()->add( "center y" , "-cy" ,  120.0 ) ||
      !parameters()->add( "k1" , "-k1" ,  0.0 ) ||
      !parameters()->add( "k2" , "-k2" ,  5.9019720572961e-6 ) ||
      !parameters()->add( "k3" , "-k3" ,  0.0 ) ||
      !parameters()->add( "k4" , "-k4" ,  4.6894031771092e-11 ) ||
      !parameters()->add( "k5" , "-k5" ,  0.0 ) ||
      !parameters()->add( "k6" , "-k6" ,  0.0 ) || 
      !parameters()->add( "invert" , "-invert" ,  false ) ||
      !parameters()->add( "resize" , "-resize" ,  true  ) ||
      !parameters()->add( "number of vsol storage" , "-numvsol" ,  (unsigned int)0  ) ||
      !parameters()->add( "midpoint threshold" , "-mpthresh" ,  -1.0 ) ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  } 
}


//: Clone the process
bpro1_process* 
dvpgl_image_lens_warp_process::clone() const
{
  return new dvpgl_image_lens_warp_process(*this);
}


//: Return the name of the process
vcl_string
dvpgl_image_lens_warp_process::name()
{
  return "Lens Distort Image";
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > 
dvpgl_image_lens_warp_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  unsigned int num=0;
  parameters()->get_value( "-numvsol" , num );
  for(unsigned int i=0; i<num; ++i)
    to_return.push_back( "vsol2D" );
  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string > 
dvpgl_image_lens_warp_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  unsigned int num=0;
  parameters()->get_value( "-numvsol" , num );
  for(unsigned int i=0; i<num; ++i)
    to_return.push_back( "vsol2D" );
  return to_return;
}


//: Returns the number of input frames to this process
int
dvpgl_image_lens_warp_process::input_frames()
{
  return 1;
}


//: Returns the number of output frames from this process
int
dvpgl_image_lens_warp_process::output_frames()
{
  return 1;
}


static 
float 
interpolator(vil_image_view<float> const& view,
             double x, double y, unsigned p)
{
  return static_cast<float>(vil_bilin_interp_safe(view, x, y, p));
}

  

//: Run the process on the current frame
bool
dvpgl_image_lens_warp_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cerr << __FILE__ << " - not exactly one input frame" << vcl_endl;
    return false;
  }

  double k[6], cx=0, cy=0, mpthresh=0;
  bool invert=false, resize=false;
  unsigned int num_vsol=0;
  parameters()->get_value( "-cx" , cx );
  parameters()->get_value( "-cy" , cy );
  parameters()->get_value( "-k1" , k[0] );
  parameters()->get_value( "-k2" , k[1] );
  parameters()->get_value( "-k3" , k[2] );
  parameters()->get_value( "-k4" , k[3] );
  parameters()->get_value( "-k5" , k[4] );
  parameters()->get_value( "-k6" , k[5] );
  parameters()->get_value( "-invert" , invert );
  parameters()->get_value( "-resize" , resize );
  parameters()->get_value( "-numvsol" , num_vsol );
  parameters()->get_value( "-mpthresh" , mpthresh );
  vgl_point_2d<double> center(cx,cy);

  // find the order of the polynomial
  unsigned int order = 6;
  while(order > 0 && k[order--] == 0.0);
  
  
  // get image from the storage class
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);
  vil_image_resource_sptr image_sptr = frame_image->get_image();

  vil_image_view<float> image = *vil_convert_stretch_range(float(), image_sptr->get_view());
  unsigned int w=0, h=0;
  if(resize){
    w = image.ni();
    h = image.nj();
  }
    
  vpgl_lens_distortion<double>* lens = 0;
  switch (order)
  {
  case 1:
    lens = new vpgl_poly_radial_distortion<double,1>(center,k);
    break;
  case 2:
    lens = new vpgl_poly_radial_distortion<double,2>(center,k);
    break;
  case 3:
    lens = new vpgl_poly_radial_distortion<double,3>(center,k);
    break;
  case 4:
    lens = new vpgl_poly_radial_distortion<double,4>(center,k);
    break;
  case 5:
    lens = new vpgl_poly_radial_distortion<double,5>(center,k);
    break;
  case 6:
    lens = new vpgl_poly_radial_distortion<double,6>(center,k);
    break;
   default:
    return false;
  }

  vil_image_view<float> out_image;
  if(resize){
    if(invert)
      out_image = vpgl_lens_unwarp_resize(image, float(), *lens, interpolator);
    else
      out_image = vpgl_lens_warp_resize(image, float(), *lens, interpolator);
  }
  else{
    out_image = vil_image_view<float>(image.ni(), image.nj(), image.nplanes());
    if(invert)
      vpgl_lens_unwarp(image, out_image, *lens, interpolator);
    else
      vpgl_lens_warp(image, out_image, *lens, interpolator);
  }
  
  // create the output storage class
  vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
  output_storage->set_image(vil_new_image_resource_of_view(out_image));
  output_data_[0].push_back(output_storage);
  
  for(unsigned int i=0; i<num_vsol; ++i)
  {
    // get the input vsol data
    vidpro1_vsol2D_storage_sptr frame_vsol;
    frame_vsol.vertical_cast(input_data_[0][i+1]);
    
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
  }
 
 
  delete lens;
  return true;
}



//: Finish
bool
dvpgl_image_lens_warp_process::finish()
{
  return true;
}




