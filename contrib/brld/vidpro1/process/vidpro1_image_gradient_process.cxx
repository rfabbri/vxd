// This is brl/vidpro1/process/vidpro1_image_gradient_process.cxx

//:
// \file

#include "vidpro1_image_gradient_process.h"

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <vil/vil_image_resource.h>
#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vil/vil_convert.h>
#include <vil/algo/vil_sobel_1x3.h>
#include <vil/algo/vil_sobel_3x3.h>
#include <vil/algo/vil_gauss_filter.h>
#include <vil/algo/vil_orientations.h>


//: Constructor
vidpro1_image_gradient_process::vidpro1_image_gradient_process()
{
  vcl_vector<vcl_string> gradient_operator_choices;
  gradient_operator_choices.push_back("Sobel (1x3)");    //0
  gradient_operator_choices.push_back("Sobel (3x3)");    //1
  gradient_operator_choices.push_back("Gaussian");       //2

  vcl_vector<vcl_string> output_choices;
  output_choices.push_back("|grad I|");                  //0
  output_choices.push_back("dI/dx only");                //1
  output_choices.push_back("dI/dy only");                //2
  output_choices.push_back("dI/dx and dI/dy");           //3
  output_choices.push_back("dI/dx, dI/dy and |grad I|"); //4

  if( !parameters()->add( "Gradient Operator"   , "-grad_op" , gradient_operator_choices, 2) ||
      !parameters()->add( "Sigma (Gaussian)"    , "-gbsigma" , 1.0f ) ||
      !parameters()->add( "Output these:"   , "-output_type" , output_choices, 4)) 
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
vidpro1_image_gradient_process::~vidpro1_image_gradient_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_image_gradient_process::clone() const
{
  return new vidpro1_image_gradient_process(*this);
}


//: Return the name of the process
vcl_string
vidpro1_image_gradient_process::name()
{
  return "Compute Gradients";
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > vidpro1_image_gradient_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string > vidpro1_image_gradient_process::get_output_type()
{
  unsigned out_type=0;
  parameters()->get_value( "-output_type" , out_type );

  vcl_vector< vcl_string > to_return;
  to_return.push_back("image");
  if (out_type>=3)
    to_return.push_back("image");//another one
  if (out_type>=4)
    to_return.push_back("image");//yet another one
  return to_return;
}


//: Returns the number of input frames to this process
int
vidpro1_image_gradient_process::input_frames()
{
  return 1;
}


//: Returns the number of output frames from this process
int
vidpro1_image_gradient_process::output_frames()
{
  unsigned out_type=0;
  parameters()->get_value( "-output_type" , out_type );

  if (out_type==3)
    return 2;
  if (out_type==4)
    return 3;

  return 1; //default
}


//: Run the process on the current frame
bool
vidpro1_image_gradient_process::execute()
{
  if ( input_data_.size() != 1 )
  {
      vcl_cout << "In vidpro1_image_gradient_process::execute() - not exactly one"
               << " input image \n";
      return false;
  }
  clear_output();

  //get image
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);
  vil_image_resource_sptr image_sptr = frame_image->get_image();

  //convert to grayscale
  vil_image_view<vxl_byte> image_view = image_sptr->get_view(0, image_sptr->ni(), 0, image_sptr->nj() );
  vil_image_view<vxl_byte> greyscale_view;

  if(image_view.nplanes() == 3){
    vil_convert_planes_to_grey(image_view, greyscale_view );
  }
  else if (image_view.nplanes() == 1){
    greyscale_view = image_view;
  }

  ////////////////////////////////////////////
  //get type of gradient operator 
  unsigned grad_op=0;
  parameters()->get_value( "-grad_op" , grad_op );

  //declare arrays to hold the gradients
  vil_image_view<float> grad_x, grad_y, orient_im, grad_mag;

  switch(grad_op)
  {
  case 0: // Sobel (1x3)
    vil_sobel_1x3 <vxl_byte, float> (greyscale_view, grad_x, grad_y);
    vil_orientations(grad_x, grad_y, orient_im, grad_mag);

    break;
  case 1: // Sobel (3x3)
    vil_sobel_3x3 <vxl_byte, float> (greyscale_view, grad_x, grad_y);
    vil_orientations(grad_x, grad_y, orient_im, grad_mag);

    break;
  case 2: // Gaussian
    float sigma;
    parameters()->get_value( "-gbsigma" , sigma );
    vil_gauss_filter_5tap_params params(sigma);

    vil_gauss_filter_5tap(greyscale_view, grad_mag, params ); 

    break;
  }

  ///////////////////////////////////////////////////////////////////
  unsigned out_type=0;
  parameters()->get_value( "-output_type" , out_type );

  if (out_type==1 || out_type==3 || out_type==4)
  {
    // create the output storage class for dI/dx
    vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
    output_storage->set_image(vil_new_image_resource_of_view(grad_x));
    output_data_[0].push_back(output_storage);
  }

  if (out_type==2 || out_type==3 || out_type==4)
  {
    // create the output storage class for dI/dy
    vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
    output_storage->set_image(vil_new_image_resource_of_view(grad_y));
    output_data_[0].push_back(output_storage);
  }

  if (out_type==0 || out_type==4)
  {
    // create the output storage class for |grad I|
    vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
    output_storage->set_image(vil_new_image_resource_of_view(grad_mag));
    output_data_[0].push_back(output_storage);
  }
  
  return true;
}


//: Finish
bool
vidpro1_image_gradient_process::finish()
{
  return true;
}


