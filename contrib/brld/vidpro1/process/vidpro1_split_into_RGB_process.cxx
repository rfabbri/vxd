// This is basic/dbil/pro/vidpro1_split_into_RGB_process.cxx

//:
// \file

#include "vidpro1_split_into_RGB_process.h"
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vil/vil_image_view.h>
#include <vil/vil_math.h>
#include <brip/brip_vil_float_ops.h>

//: Constructor
vidpro1_split_into_RGB_process::vidpro1_split_into_RGB_process()
{
}


//: Destructor
vidpro1_split_into_RGB_process::~vidpro1_split_into_RGB_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_split_into_RGB_process::clone() const
{
  return new vidpro1_split_into_RGB_process(*this);
}

//: Returns the name of this process
vcl_string 
vidpro1_split_into_RGB_process::name()
{ 
  return "Split into RGB"; 
}

//: Provide a vector of required input types
vcl_vector< vcl_string > 
vidpro1_split_into_RGB_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > 
vidpro1_split_into_RGB_process::get_output_type()
{
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "image" );
  to_return.push_back( "image" );
  to_return.push_back( "image" );
  return to_return;
}

//: Return the number of input frames for this process
int
vidpro1_split_into_RGB_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
vidpro1_split_into_RGB_process::output_frames()
{
  return 1;
}

//: Execute this process
bool
vidpro1_split_into_RGB_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cerr << "In vidpro1_split_into_RGB_process::execute() - "
             << "not exactly one input images" << vcl_endl;
    return false;
  }

  // get images from the storage classes
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image_sptr = frame_image->get_image();
  vil_image_view<vxl_byte> image_view = image_sptr->get_view(0, image_sptr->ni(), 0, image_sptr->nj() );

  vil_image_resource_sptr R_image, G_image, B_image;

  if(image_view.nplanes() == 3){
    R_image = vil_new_image_resource_of_view(vil_plane(image_view, 0)); 
    G_image = vil_new_image_resource_of_view(vil_plane(image_view, 1));
    B_image = vil_new_image_resource_of_view(vil_plane(image_view, 2));
  }
  else if (image_view.nplanes() == 1){
    R_image = vil_new_image_resource_of_view(image_view); 
    G_image = vil_new_image_resource_of_view(image_view);
    B_image = vil_new_image_resource_of_view(image_view);
  }

  // create the output storage class
  vidpro1_image_storage_sptr output_storage1 = vidpro1_image_storage_new();
  vidpro1_image_storage_sptr output_storage2 = vidpro1_image_storage_new();
  vidpro1_image_storage_sptr output_storage3 = vidpro1_image_storage_new();
  output_storage1->set_image(R_image);
  output_storage2->set_image(G_image);
  output_storage3->set_image(B_image);

  output_data_[0].push_back(output_storage1);
  output_data_[0].push_back(output_storage2);
  output_data_[0].push_back(output_storage3);

  return true;
}

bool
vidpro1_split_into_RGB_process::finish()
{
  return true;
}





