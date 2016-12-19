// This is brl/vidpro1/process/vidpro1_horn_schunck_process.cxx

//:
// \file

#include "vidpro1_horn_schunck_process.h"
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vil/vil_image_view.h>
#include <vil/vil_convert.h>
#include <vil/vil_math.h>
#include <vil/algo/vil_sobel_3x3.h>
#include <brip/brip_vil_float_ops.h>


//: Constructor
vidpro1_horn_schunck_process::vidpro1_horn_schunck_process()
{
  if( !parameters()->add( "Low Range" ,  "-horn_schunck_low" ,  0.0f ) ||
      !parameters()->add( "High Range" , "-horn_schunck_high" , 10000.0f ) ||
      !parameters()->add( "Alpha Coeff", "-alpha" ,  0.5f ) ||  
      !parameters()->add( "No of Iterations", "-no_of_iterations" ,  (int)10 )
      )
    {    
      vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
    }
}


//: Destructor
vidpro1_horn_schunck_process::~vidpro1_horn_schunck_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_horn_schunck_process::clone() const
{
  return new vidpro1_horn_schunck_process(*this);
}


//: Return the name of this process
vcl_string
vidpro1_horn_schunck_process::name()
{
  return "Compute Horn_Schunck";
}


//: Return the number of input frame for this process
int
vidpro1_horn_schunck_process::input_frames()
{
  return 2;
}


//: Return the number of output frames for this process
int
vidpro1_horn_schunck_process::output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > vidpro1_horn_schunck_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > vidpro1_horn_schunck_process::get_output_type()
{  
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}

void vidpro1_horn_schunck_process::
store_output(vil_image_view<vxl_byte> const& output)
{
  // create the output storage class
  vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
  output_storage->set_image(vil_new_image_resource_of_view(output));
  output_data_[0].push_back(output_storage);
}

//: Execute the process
bool
vidpro1_horn_schunck_process::execute()
{
  if ( input_data_.size() != 2 ){
    vcl_cout << "In vidpro1_horn_schunck_process::execute() - not exactly two"
             << " input images \n";
    return false;
  }
  this->parameters()->get_value("-horn_schunck_low", low_range_);
  this->parameters()->get_value("-horn_schunck_high", high_range_);
  this->parameters()->get_value("-alpha", alpha_);
  this->parameters()->get_value("-no_of_iterations", no_of_iter_);

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

  

  unsigned ni = grey_img1.ni(), nj = grey_img1.nj();
  vil_image_view<float> vx(ni, nj);
  vil_image_view<float> vy(ni, nj);
  vil_image_view<vxl_byte> output(ni, nj);
  output.fill(0);

  int result  = 
    brip_vil_float_ops::Horn_SchunckMotion(grey_img1, grey_img2,
                                          vx, vy, alpha_, no_of_iter_);

  if(result == -1)
    {
      vcl_cout << "Warning! input images are equal " 
               << "in vidpro1_horn_schunck_process\n";
      this->store_output(output);
      return true;
    }
  else if (result == -2)
    {
      vcl_cout << "Warning! some internal process image is all zero " 
               << "in vidpro1_horn_schunck_process\n";
      this->store_output(output);
      return true;
    }
  //the result was successful
  //compute the magnitude image
  vil_image_view<float> mag(ni, nj);
  vil_math_image_vector_mag(vx, vy, mag);  
  //scale the results and cast to a byte image;
  double scale =  (high_range_ - low_range_);
  vil_math_scale_and_offset_values(mag, scale, low_range_);
  vil_convert_cast(mag, output);
  this->store_output(output);
  return true;  
}
    


bool
vidpro1_horn_schunck_process::finish()
{
  return true;
}




