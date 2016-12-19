// This is basic/dbil/pro/vidpro1_RGB_to_IHS_process.cxx

//:
// \file

#include "vidpro1_RGB_to_IHS_process.h"
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vil/vil_image_view.h>
#include <vil/vil_math.h>
#include <brip/brip_vil_float_ops.h>

//: Constructor
vidpro1_RGB_to_IHS_process::vidpro1_RGB_to_IHS_process()
{
  if( !parameters()->add( "Output component images", "-bComps", true ))
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
vidpro1_RGB_to_IHS_process::~vidpro1_RGB_to_IHS_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_RGB_to_IHS_process::clone() const
{
  return new vidpro1_RGB_to_IHS_process(*this);
}

//: Returns the name of this process
vcl_string 
vidpro1_RGB_to_IHS_process::name()
{ 
  return "RGB to IHS"; 
}

//: Provide a vector of required input types
vcl_vector< vcl_string > 
vidpro1_RGB_to_IHS_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > 
vidpro1_RGB_to_IHS_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;

  bool bComps;
  parameters()->get_value( "-bComps", bComps);

  if (bComps){ //output 3 component images
    to_return.push_back( "image" );
    to_return.push_back( "image" );
    to_return.push_back( "image" );
  }
  else //single IHS image
    to_return.push_back( "image" );

  return to_return;
}

//: Return the number of input frames for this process
int
vidpro1_RGB_to_IHS_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
vidpro1_RGB_to_IHS_process::output_frames()
{
  return 1;
}

//: Execute this process
bool
vidpro1_RGB_to_IHS_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cerr << "In vidpro1_RGB_to_IHS_process::execute() - "
             << "not exactly one input images" << vcl_endl;
    return false;
  }

  // get images from the storage classes
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image_sptr = frame_image->get_image();
  vil_image_view<vil_rgb<vxl_byte> > image = image_sptr->get_view();

  vil_image_view<float> Int_image;
  vil_image_view<float> Hue_image;
  vil_image_view<float> Sat_image;

  brip_vil_float_ops::convert_to_IHS(image, Int_image, Hue_image, Sat_image);

  bool bComps;
  parameters()->get_value( "-bComps", bComps);

  if (bComps){
    // create resources for the component images
    vil_image_resource_sptr out_imgI =  vil_new_image_resource_of_view(brip_vil_float_ops::convert_to_byte(Int_image));
    vil_image_resource_sptr out_imgH =  vil_new_image_resource_of_view(brip_vil_float_ops::convert_to_byte(Hue_image));
    vil_image_resource_sptr out_imgS =  vil_new_image_resource_of_view(brip_vil_float_ops::convert_to_byte(Sat_image));

    // create the output storage class
    vidpro1_image_storage_sptr output_storage1 = vidpro1_image_storage_new();
    vidpro1_image_storage_sptr output_storage2 = vidpro1_image_storage_new();
    vidpro1_image_storage_sptr output_storage3 = vidpro1_image_storage_new();
    output_storage1->set_image(out_imgI);
    output_storage2->set_image(out_imgH);
    output_storage3->set_image(out_imgS);

    output_data_[0].push_back(output_storage1);
    output_data_[0].push_back(output_storage2);
    output_data_[0].push_back(output_storage3);
  }
  else {
    //create a resource for the output image (IHS image by combining the IHS planes)
    vil_image_resource_sptr out_IHS_img =  vil_new_image_resource_of_view(
                                              brip_vil_float_ops::combine_color_planes(
                                                brip_vil_float_ops::convert_to_byte(Int_image),
                                                brip_vil_float_ops::convert_to_byte(Hue_image),
                                                brip_vil_float_ops::convert_to_byte(Sat_image)));

    // create the output storage class
    vidpro1_image_storage_sptr output_storage1 = vidpro1_image_storage_new();
    output_storage1->set_image(out_IHS_img);

    output_data_[0].push_back(output_storage1);
  }

  return true;
}

bool
vidpro1_RGB_to_IHS_process::finish()
{
  return true;
}





