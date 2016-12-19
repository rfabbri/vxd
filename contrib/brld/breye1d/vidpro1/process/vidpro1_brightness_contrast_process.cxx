// This is brl/vidpro1/process/vidpro1_brightness_contrast_process.cxx

//:
// \file

#include <vidpro1/process/vidpro1_brightness_contrast_process.h>
#include <vcl_iostream.h>

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vcl_cmath.h>


vidpro1_brightness_contrast_process::vidpro1_brightness_contrast_process() : bpro1_process()
{
  if( !parameters()->add( "Brightness" , "-brightness" , (int)0 ) ||
      !parameters()->add( "Contrast" ,   "-contrast" ,   1.0f ) ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  
  }
}

vidpro1_brightness_contrast_process::~vidpro1_brightness_contrast_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_brightness_contrast_process::clone() const
{
  return new vidpro1_brightness_contrast_process(*this);
}


vcl_string vidpro1_brightness_contrast_process::name()
{
  return "Brightness/Contrast";
}

vcl_vector< vcl_string > vidpro1_brightness_contrast_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}

vcl_vector< vcl_string > vidpro1_brightness_contrast_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  // no return type
  to_return.clear();

  return to_return;
}

bool vidpro1_brightness_contrast_process::execute()
{
  if ( input_data_.size() != 1 )
  {
      vcl_cout << "In vidpro1_brightness_contrast_process::execute() - not exactly one"
               << " input image \n";
      return false;
  }
  clear_output();

  int brightness=0;
  float contrast=0;

  parameters()->get_value( "-brightness" , brightness );
  parameters()->get_value( "-contrast" , contrast );

  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image_sptr = frame_image->get_image();
  vil_image_view< unsigned char > image_view = image_sptr->get_view(0, image_sptr->ni(), 0, image_sptr->nj() );

  float distance = 0;
  
  for( unsigned int x = 0 ; x < image_sptr->ni() ; x++ ) {
    for( unsigned int y = 0 ; y < image_sptr->nj() ; y++ ) {
      for( unsigned int p = 0 ; p < image_sptr->nplanes() ; p++ ) {
        
        distance = vcl_abs((float) ( image_view(x,y,p) - 127 ));
        if( image_view(x,y,p) >= 127 ) {
          if( 127 + contrast * distance + brightness > 255 )
            image_view(x,y,p) = 255;
          else if ( 127 + contrast * distance + brightness < 0 )
            image_view(x,y,p) = 0;
          else
            image_view(x,y,p) = 127 + (unsigned char)(contrast*distance) + brightness;
        } else {
          if( 127 - contrast * distance + brightness > 255 )
            image_view(x,y,p) = 255;
          else if ( 127 - contrast * distance + brightness < 0 )
            image_view(x,y,p) = 0;
          else
            image_view(x,y,p) = 127 - (unsigned char)(contrast*distance) + brightness;
        }

      }
    }
  }

  vil_image_resource_sptr output_image = vil_new_image_resource_of_view( image_view );

  // this process does not have an output type. It just modifies the input
  frame_image->set_image(output_image);

  return true;
}

bool vidpro1_brightness_contrast_process::finish() 
{
  return true;
}

/*************************************************************************
 * Function Name: vidpro1_brightness_contrast_process::input_frames
 * Parameters: 
 * Returns: int
 * Effects: 
 *************************************************************************/
int
vidpro1_brightness_contrast_process::input_frames()
{
  return 1;
}


/*************************************************************************
 * Function Name: vidpro1_brightness_contrast_process::output_frames
 * Parameters: 
 * Returns: int
 * Effects: 
 *************************************************************************/
int
vidpro1_brightness_contrast_process::output_frames()
{
  return 1;
}


