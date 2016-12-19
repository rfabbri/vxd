// This is brl/vidpro1/process/vidpro1_gaussian_blur_process.cxx

//:
// \file

#include "vidpro1_gaussian_blur_process.h"

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vil/algo/vil_gauss_filter.h>


//: Constructor
vidpro1_gaussian_blur_process::vidpro1_gaussian_blur_process()
{
  if( !parameters()->add( "Gaussian Blur Sigma" , "-gbsigma" , 1.0f ) ||
      !parameters()->add( "Modify Source Image" , "-gbmodify", false ) ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
vidpro1_gaussian_blur_process::~vidpro1_gaussian_blur_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_gaussian_blur_process::clone() const
{
  return new vidpro1_gaussian_blur_process(*this);
}


//: Return the name of the process
vcl_string
vidpro1_gaussian_blur_process::name()
{
  return "Gaussian Blur";
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > vidpro1_gaussian_blur_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string > vidpro1_gaussian_blur_process::get_output_type()
{
  bool modify = false;
  parameters()->get_value( "-gbmodify" , modify );
  vcl_vector< vcl_string > to_return;
  if(!modify)
    to_return.push_back("image");
  return to_return;
}


//: Returns the number of input frames to this process
int
vidpro1_gaussian_blur_process::input_frames()
{
  return 1;
}


//: Returns the number of output frames from this process
int
vidpro1_gaussian_blur_process::output_frames()
{
  return 1;
}


//: Run the process on the current frame
bool
vidpro1_gaussian_blur_process::execute()
{
  if ( input_data_.size() != 1 )
  {
      vcl_cout << "In vidpro1_gaussian_blur_process::execute() - not exactly one"
               << " input image \n";
      return false;
  }
  clear_output();

  float sigma=0;
  bool modify = false;
  parameters()->get_value( "-gbsigma" , sigma );
  parameters()->get_value( "-gbmodify" , modify );

  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image_sptr = frame_image->get_image();
  vil_image_resource_sptr output_sptr = vil_new_image_resource( image_sptr->ni() ,
                                                                image_sptr->nj() ,
                                                                image_sptr->nplanes() ,
                                                                image_sptr->pixel_format() );
  vil_gauss_filter_5tap_params params( sigma );

  switch ( vil_pixel_format_component_format(image_sptr->pixel_format()) )
  {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define macro(F , T) \
     case F: {vil_image_view<T > output_view = output_sptr->get_view(); \
     vil_gauss_filter_5tap( vil_image_view<T >(*image_sptr->get_view()) , \
                            output_view , params ); break; }
 
#if VXL_HAS_INT_64
    //macro( VIL_PIXEL_FORMAT_UINT_64, vxl_uint_64 );
#endif
    //macro( VIL_PIXEL_FORMAT_UINT_32, vxl_uint_32 );
    //macro( VIL_PIXEL_FORMAT_UINT_16, vxl_uint_16 );
    macro( VIL_PIXEL_FORMAT_BYTE, vxl_byte );
    macro( VIL_PIXEL_FORMAT_FLOAT, float );
    //macro( VIL_PIXEL_FORMAT_DOUBLE, double );
#undef macro
#endif // DOXYGEN_SHOULD_SKIP_THIS
    default:
      output_sptr = NULL;
  }
  

  if(!modify){
    // create the output storage class
    vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
    output_storage->set_image(output_sptr);
    output_data_[0].push_back(output_storage);
  }
  else
    frame_image->set_image(output_sptr);

  return true;
}


//: Finish
bool
vidpro1_gaussian_blur_process::finish()
{
  return true;
}


