// This is brl/vidpro1/process/vidpro1_harris_corners_process.cxx

//:
// \file

#include "vidpro1_harris_corners_process.h"

#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>

#include <vil1/vil1_vil.h>
#include <vil1/vil1_memory_image_of.h>

#include <vil/vil_image_view.h>
#include <vil/vil_convert.h>

#include <vsol/vsol_point_2d.h>
#include <sdet/sdet_harris_detector_params.h>
#include <sdet/sdet_harris_detector.h>


//: Constructor
vidpro1_harris_corners_process::vidpro1_harris_corners_process() : bpro1_process()
{
  if( !parameters()->add( "Sigma" ,                     "-hcsigma" ,  1.0f ) ||
      !parameters()->add( "Threshold" ,                 "-hcthresh" , 2.0f ) ||
      !parameters()->add( "N = 2n+1, (n)" ,             "-hcn" ,      (int)1 ) ||
      !parameters()->add( "Max No. Corners (percent)" , "-hcmax" ,    80.0f ) ||
      !parameters()->add( "Scale Factor" ,              "-hcscale" ,  0.04f ) ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__<< vcl_endl;
  } 
}


//: Destructor
vidpro1_harris_corners_process::~vidpro1_harris_corners_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_harris_corners_process::clone() const
{
  return new vidpro1_harris_corners_process(*this);
}


//: Return the name of the process
vcl_string
vidpro1_harris_corners_process::name()
{
  return "Harris Corners";
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > vidpro1_harris_corners_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string > vidpro1_harris_corners_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Run the process on the current frame
bool
vidpro1_harris_corners_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cout << "In vidpro1_harris_corner_process::execute() - not exactly one"
          << " input image \n";
    return false;
  }
  clear_output();

    // get image from the storage class

  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image_sptr = frame_image->get_image();
  vil_image_view< unsigned char > image_view = image_sptr->get_view(0, image_sptr->ni(), 0, image_sptr->nj() );
  vil_image_view< unsigned char > greyscale_view;

  if( image_view.nplanes() == 3 ) {
    vil_convert_planes_to_grey( image_view , greyscale_view );
  }
  else if ( image_view.nplanes() == 1 ) {
    greyscale_view = image_view;
  } else {
    vcl_cerr << "Returning false. nplanes(): " << image_view.nplanes() << vcl_endl;
    return false;
  }

  vil1_memory_image_of< unsigned char > img = vil1_from_vil_image_view( greyscale_view );

  sdet_harris_detector_params temp_params;

  parameters()->get_value( "-hcsigma" , temp_params.sigma_ );
  parameters()->get_value( "-hcthresh" , temp_params.thresh_ );
  parameters()->get_value( "-hcn" , temp_params.n_ );
  parameters()->get_value( "-hcmax" , temp_params.percent_corners_ );
  parameters()->get_value( "-hcscale" , temp_params.scale_factor_ );

  sdet_harris_detector harris_detector( temp_params );
  harris_detector.set_image(img);
  harris_detector.extract_corners();

  vcl_vector<vsol_point_2d_sptr>& points = harris_detector.get_points();
  int N = points.size();
  if (!N)
    return false;

  vcl_vector< vsol_spatial_object_2d_sptr > harris_points;
  for( unsigned int i = 0 ; i < points.size() ; i++ ) {
    vsol_spatial_object_2d_sptr point = points[i]->cast_to_spatial_object();
    harris_points.push_back(point);
  }

  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_vsol->add_objects(harris_points, "harris corner point");
  output_data_[0].push_back(output_vsol);

  return true;
}


//: Finish
bool
vidpro1_harris_corners_process::finish()
{
  return true;
}


//: Returns the number of input frames to this process
int
vidpro1_harris_corners_process::input_frames()
{
  return 1;
}


//: Returns the number of output frames from this process
int
vidpro1_harris_corners_process::output_frames()
{
  return 1;
}



