// This is breye1/vidpro1/process/vidpro1_equalize_images_process.cxx

//:
// \file

#include "vidpro1_equalize_images_process.h"
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vil/vil_convert.h>
#include <vil/vil_image_view.h>
#include <bil/algo/bil_equalize.h>


//: Constructor
vidpro1_equalize_images_process::vidpro1_equalize_images_process()
{
  if( !parameters()->add( "Use Mask" ,    "-mask" ,   false ) ){
    vcl_cerr << "ERROR: Adding parameters in " __FILE__<< vcl_endl;
  }
}


//: Destructor
vidpro1_equalize_images_process::~vidpro1_equalize_images_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_equalize_images_process::clone() const
{
  return new vidpro1_equalize_images_process(*this);
}


//: Return the name of this process
vcl_string
vidpro1_equalize_images_process::name()
{
  return "Equalize Images";
}


//: Return the number of input frame for this process
int
vidpro1_equalize_images_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
vidpro1_equalize_images_process::output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > vidpro1_equalize_images_process::get_input_type()
{
  bool mask = false;
  parameters()->get_value( "-mask" , mask );
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  // add another input for the mask
  if(mask)
    to_return.push_back( "image" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > vidpro1_equalize_images_process::get_output_type()
{  
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Execute the process
bool
vidpro1_equalize_images_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cout << "In vidpro1_equalize_images_process::execute() - "
             << "not exactly one input images \n";
    return false;
  }

  // get image from the storage class
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);
  vil_image_resource_sptr image_sptr = frame_image->get_image();

  float dummy_float = 0.0f;
  vil_image_view_base_sptr img_sptr = vil_convert_stretch_range(dummy_float, image_sptr->get_view());
  vil_image_view<float> image = *img_sptr;
  //img_sptr = vil_convert_stretch_range(dummy_float, image2_sptr->get_view());
  //vil_image_view<float> output = *img_sptr;
  //vil_image_view<vxl_byte> image1, image2, output;
  //image1 = image1_sptr->get_view();
  //image2 = image2_sptr->get_view();
  //output.deep_copy(image2);
  
  bool use_mask = false;
  parameters()->get_value( "-mask" , use_mask );
  
  // if first frame use this frame as the model
  if(model_.size() == 0){
    model_ = image;
    if(use_mask){
      mask_ = vil_image_view<bool>(image.ni(), image.nj(), 1);
      vidpro1_image_storage_sptr frame_mask;
      frame_mask.vertical_cast(input_data_[0][1]);
      vil_image_view<vxl_byte> byte_mask = frame_mask->get_image()->get_view();
      for(unsigned int i=0; i<mask_.ni(); ++i){
        for(unsigned int j=0; j<mask_.nj(); ++j){
          mask_(i,j) = (byte_mask(i,j)<128);
        }
      }
    }
  }
  
  else{
    if(use_mask){
      vil_image_view<bool> curr_mask(image.ni(), image.nj(), 1);
      vidpro1_image_storage_sptr frame_mask;
      frame_mask.vertical_cast(input_data_[0][1]);
      vil_image_view<vxl_byte> byte_mask = frame_mask->get_image()->get_view();
      for(unsigned int i=0; i<curr_mask.ni(); ++i){
        for(unsigned int j=0; j<curr_mask.nj(); ++j){
          curr_mask(i,j) = (byte_mask(i,j)<128) && mask_(i,j);
        }
      }
      bil_equalize_linear(model_, image, curr_mask);
    }
    else
      bil_equalize_linear(model_, image);
  }

  // create the output storage class
  vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
  output_storage->set_image(vil_new_image_resource_of_view(image));

  output_data_[0].push_back(output_storage);
  return true;  
}



bool
vidpro1_equalize_images_process::finish()
{
  model_ = NULL;
  mask_ = NULL;
  return true;
}




