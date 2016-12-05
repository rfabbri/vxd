// This is brl/vidpro1/process/vidpro1_motion_process.cxx

//:
// \file

#include "vidpro1_motion_process.h"
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vil/vil_image_view.h>
#include <vil/vil_convert.h>
#include <vil/vil_math.h>
#include <vil/algo/vil_sobel_3x3.h>
#include <brip/brip_vil_ops.h>


//: Constructor
vidpro1_motion_process::vidpro1_motion_process()
{
  if( !parameters()->add( "Low Range" ,  "-motion_low" ,  0.0f ) ||
      !parameters()->add( "High Range" , "-motion_high" , 10000.0f ) ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
vidpro1_motion_process::~vidpro1_motion_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_motion_process::clone() const
{
  return new vidpro1_motion_process(*this);
}


//: Return the name of this process
vcl_string
vidpro1_motion_process::name()
{
  return "Compute Motion";
}


//: Return the number of input frame for this process
int
vidpro1_motion_process::input_frames()
{
  return 2;
}


//: Return the number of output frames for this process
int
vidpro1_motion_process::output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > vidpro1_motion_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > vidpro1_motion_process::get_output_type()
{  
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Execute the process
bool
vidpro1_motion_process::execute()
{
  if ( input_data_.size() != 2 ){
    vcl_cout << "In vidpro1_motion_process::execute() - not exactly two"
             << " input images \n";
    return false;
  }
  clear_output();

  // get image from the storage class
  vidpro1_image_storage_sptr frame_image1, frame_image2;
  frame_image1.vertical_cast(input_data_[1][0]);
  frame_image2.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image1_sptr = frame_image1->get_image();
  vil_image_resource_sptr image2_sptr = frame_image2->get_image();

  vil_image_view< float > image1, image2, grey_img1, grey_img2;
  image1 = vil_convert_stretch_range((float)0.0, image1_sptr->get_view());
  image2 = vil_convert_stretch_range((float)0.0, image2_sptr->get_view());


  // Convert to greyscale images if needed
  if( image1.nplanes() == 3 ) {
    vil_convert_planes_to_grey( image1 , grey_img1 );
  }
  else if ( image1.nplanes() == 1 ) {
    grey_img1 = image1;
  } else {
    vcl_cerr << "Returning false. image1.nplanes(): " << image1.nplanes() << vcl_endl;
    return false;
  }

  if( image2.nplanes() == 3 ) {
    vil_convert_planes_to_grey( image2 , grey_img2 );
  }
  else if ( image2.nplanes() == 1 ) {
    grey_img2 = image2;
  } else {
    vcl_cerr << "Returning false. image2.nplanes(): " << image2.nplanes() << vcl_endl;
    return false;
  }


  vcl_string output_filename;
  vcl_string input_filename;

  parameters()->get_value( "-motion_low" ,   low_range_ );
  parameters()->get_value( "-motion_high" ,  high_range_ );

  // create the output storage class
  vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
  vil_image_view<vxl_byte> output;

  this->compute_motion(grey_img1, grey_img2, output);

  output_storage->set_image(vil_new_image_resource_of_view(output));

  output_data_[0].push_back(output_storage);
  return true;  
}
    


bool
vidpro1_motion_process::finish()
{
  return true;
}


//: compute the motion between two images
void
vidpro1_motion_process::compute_motion( const vil_image_view<float>& img1,
                                       const vil_image_view<float>& img2,
                                             vil_image_view<vxl_byte>& output )
{
  //Get sqrt of the gradient matrix on a 3x3 neighborhood
  vil_image_view<float> sing_img;
  brip_sqrt_grad_singular_values(img1, sing_img, 1);

  //Get the discrete time derivative
  vil_image_view<float> diff_img;
  vil_math_image_abs_difference(img1, img2, diff_img); 

  //form the motion image
  vil_image_view<float> motion;
  vil_math_image_product(diff_img, sing_img, motion);

  //scale the results and cast to a byte image;
  double scale =  (high_range_ - low_range_);
  vil_math_scale_and_offset_values(motion, scale, low_range_);
  vil_convert_cast(motion, output);
}


