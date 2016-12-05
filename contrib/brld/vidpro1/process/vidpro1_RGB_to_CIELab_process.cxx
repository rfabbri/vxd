// This is basic/dbil/pro/vidpro1_RGB_to_CIELab_process.cxx

//:
// \file

#include "vidpro1_RGB_to_CIELab_process.h"
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vil/vil_image_view.h>
#include <vil/vil_math.h>
#include <brip/brip_vil_float_ops.h>

//: Constructor
vidpro1_RGB_to_CIELab_process::vidpro1_RGB_to_CIELab_process()
{
  if( !parameters()->add( "Output component images", "-bComps", true ))
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
vidpro1_RGB_to_CIELab_process::~vidpro1_RGB_to_CIELab_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_RGB_to_CIELab_process::clone() const
{
  return new vidpro1_RGB_to_CIELab_process(*this);
}

//: Returns the name of this process
vcl_string 
vidpro1_RGB_to_CIELab_process::name()
{ 
  return "RGB to CIE-Lab"; 
}

//: Provide a vector of required input types
vcl_vector< vcl_string > 
vidpro1_RGB_to_CIELab_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > 
vidpro1_RGB_to_CIELab_process::get_output_type()
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
vidpro1_RGB_to_CIELab_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
vidpro1_RGB_to_CIELab_process::output_frames()
{
  return 1;
}

//: Execute this process
bool
vidpro1_RGB_to_CIELab_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cerr << "In vidpro1_RGB_to_CIELab_process::execute() - "
             << "not exactly one input images" << vcl_endl;
    return false;
  }

  // get images from the storage classes
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image_sptr = frame_image->get_image();
  vil_image_view<vxl_byte> col_image = image_sptr->get_view();

  //make sure the image is an rgb image
  if (col_image.nplanes() != 3){
    vcl_cout << "In vidpro1_RGB_to_CIELab_process::execute() - image must be RGB! \n";
    return false;
  }

  vil_image_view<float> L_image;
  vil_image_view<float> a_image;
  vil_image_view<float> b_image;

  convert_to_Lab(col_image, L_image, a_image, b_image);

  bool bComps;
  parameters()->get_value( "-bComps", bComps);

  if (bComps){
    // create resources for the component images
    vil_image_resource_sptr out_img_L =  vil_new_image_resource_of_view(brip_vil_float_ops::convert_to_byte(L_image));
    vil_image_resource_sptr out_img_a =  vil_new_image_resource_of_view(brip_vil_float_ops::convert_to_byte(a_image));
    vil_image_resource_sptr out_img_b =  vil_new_image_resource_of_view(brip_vil_float_ops::convert_to_byte(b_image));

    // create the output storage class
    vidpro1_image_storage_sptr output_storage1 = vidpro1_image_storage_new();
    vidpro1_image_storage_sptr output_storage2 = vidpro1_image_storage_new();
    vidpro1_image_storage_sptr output_storage3 = vidpro1_image_storage_new();
    output_storage1->set_image(out_img_L);
    output_storage2->set_image(out_img_a);
    output_storage3->set_image(out_img_b);

    output_data_[0].push_back(output_storage1);
    output_data_[0].push_back(output_storage2);
    output_data_[0].push_back(output_storage3);
  }
  else {
    //create a resource for the output image (Lab image by combining the L,a,b planes)
    vil_image_resource_sptr out_Lab_img =  vil_new_image_resource_of_view(
                                              brip_vil_float_ops::combine_color_planes(
                                                brip_vil_float_ops::convert_to_byte(L_image),
                                                brip_vil_float_ops::convert_to_byte(a_image),
                                                brip_vil_float_ops::convert_to_byte(b_image)));

    // create the output storage class
    vidpro1_image_storage_sptr output_storage1 = vidpro1_image_storage_new();
    output_storage1->set_image(out_Lab_img);

    output_data_[0].push_back(output_storage1);
  }

  return true;
}

bool
vidpro1_RGB_to_CIELab_process::finish()
{
  return true;
}

void vidpro1_RGB_to_CIELab_process::rgb2lab(double R, double G, double B, double &L , double &a, double &b)
{
  //1) First convert RGB (assuming sRGB) to 1931 CIE-XYZ space

  //convert to [0,1] range
  double var_R = (R/255.0);   //R = From 0 to 255
  double var_G = (G/255.0);   //G = From 0 to 255
  double var_B = (B/255.0);   //B = From 0 to 255

  if ( var_R > 0.04045 ) var_R = vcl_pow( (var_R+0.055)/1.055, 2.4);
  else                   var_R = var_R /12.92;

  if ( var_G > 0.04045 ) var_G = vcl_pow( (var_G+0.055)/1.055, 2.4);
  else                   var_G = var_G /12.92;

  if ( var_B > 0.04045 ) var_B = vcl_pow( (var_B+0.055)/1.055, 2.4);
  else                   var_B = var_B /12.92;

  double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
  double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
  double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

  //2) now convert 1931 CIE-XYZ to 1976 CIE-Lab
  // Assuming Observer = 2°, Illuminant = D65

  double var_X = X * 100.0/95.047;       
  double var_Y = Y * 100.0/100.000;
  double var_Z = Z * 100.0/108.883;

  if ( var_X > 0.008856 ) var_X = vcl_pow(var_X, 1.0/3.0);
  else                    var_X = 7.787 * var_X + 16.0/116.0;

  if ( var_Y > 0.008856 ) var_Y = vcl_pow(var_Y ,1.0/3.0);
  else                    var_Y = 7.787 * var_Y + 16.0/ 116.0;

  if ( var_Z > 0.008856 ) var_Z = vcl_pow(var_Z, 1.0/3.0);
  else                    var_Z = 7.787 * var_Z + 16.0/ 116.0;

  //Finally
  L = 116.0 * var_Y  - 16.0;
  a = 500.0 * ( var_X - var_Y );
  b = 200.0 * ( var_Y - var_Z );
}

void vidpro1_RGB_to_CIELab_process::convert_to_Lab(vil_image_view<unsigned char> const& image,
                                                  vil_image_view<float>& L_img,
                                                  vil_image_view<float>& a_img,
                                                  vil_image_view<float>& b_img)
{
  unsigned w = image.ni(), h = image.nj();
  L_img.set_size(w,h);
  a_img.set_size(w,h);
  b_img.set_size(w,h);
  for (unsigned r = 0; r < h; r++){
    for (unsigned c = 0; c < w; c++)
    {
      double L, a, b;
      rgb2lab(image(c,r,0),image(c,r,1),image(c,r,2), L, a, b);
      L_img(c,r) = L;
      a_img(c,r) = a;
      b_img(c,r) = b;
    }
  }
}




