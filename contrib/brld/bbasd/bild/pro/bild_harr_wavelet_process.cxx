// This is basic/dbil/pro/dbil_harr__process.cxx

//:
// \file

#include "dbil_harr_wavelet_process.h"

#include <vil/vil_image_resource.h>
#include <vil/vil_image_resource_sptr.h>
#include <vil/vil_convert.h>
#include <vil/vil_new.h>

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <bil/algo/bil_harr_wavelet_transform.h>

//: Constructor
dbil_harr_wavelet_process::dbil_harr_wavelet_process()
{
  if( !parameters()->add( "Restore"    , "-restore" ,  false   ) 
    ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}

//: Destructor
dbil_harr_wavelet_process::~dbil_harr_wavelet_process()
{
}

bpro1_process*
dbil_harr_wavelet_process::clone() const
{
  return new dbil_harr_wavelet_process(*this);
}

//: Return the name of this process
vcl_string 
dbil_harr_wavelet_process::name()
{
  return "Harr Wavelet";
}

int
dbil_harr_wavelet_process::input_frames()
{
  return 1;
}

int
dbil_harr_wavelet_process::output_frames()
{
  return 1;
}

vcl_vector< vcl_string >
dbil_harr_wavelet_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}

vcl_vector< vcl_string >
dbil_harr_wavelet_process::get_output_type()
{
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}

bool
dbil_harr_wavelet_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cout << "In vidpro1_can_test_process::execute() - not exactly one" << " input image" << vcl_endl;
    return false;
  }
  clear_output();


  // get image from the storage class
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);
  
  vil_image_resource_sptr image_rsc = frame_image->get_image();
  vil_image_view<float> img = *vil_convert_cast(float(), image_rsc->get_view());
  if(image_rsc->pixel_format() == VIL_PIXEL_FORMAT_BYTE)
    vil_math_scale_values(img,1.0/255.0);

  bool restore=false;
  parameters()->get_value( "-restore" , restore );

  
  vil_image_view< float > result;
  if(restore)
    bil_harr_wavelet_inverse(img,result);
  else
    bil_harr_wavelet_transform(img,result);

  vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
  output_storage->set_image(vil_new_image_resource_of_view(result));
  output_data_[0].push_back(output_storage);

  return true;
}

bool
dbil_harr_wavelet_process::finish()
{
  return true;
}
