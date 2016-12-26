#include "dbil_wshed2d_process.h"

#include <vcl_cstdio.h>

#include <vul/vul_timer.h>

#include <vil/vil_image_resource.h>
#include <vil/vil_image_resource_sptr.h>
#include <vil/vil_new.h>
#include <vil/vil_save.h>
#include <vil/vil_convert.h>

#include <bpro1/bpro1_parameters.h>
#include <bpro1/bpro1_storage.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <bil/algo/bil_wshed2d.h>

//: Constructor
dbil_wshed2d_process::dbil_wshed2d_process()
{
  if( !parameters()->add( "Gaussian Smoothing Sigma On Input Image"    , "-gssigma1" , 2.0f   ) ||
      !parameters()->add( "Gaussian Smoothing Sigma On Gradient Image" , "-gssigma2" , 0.0f   ) ||
      !parameters()->add( "X_Min"                                      , "-min_x"    , (int)0 ) ||
      !parameters()->add( "Y_Min"                                      , "-min_y"    , (int)0 ) ||
      !parameters()->add( "X_Max"                                      , "-max_x"    , (int)0 ) ||
      !parameters()->add( "Y_Max"                                      , "-max_y"    , (int)0 )
    ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}

//: Destructor
dbil_wshed2d_process::~dbil_wshed2d_process()
{
}

bpro1_process*
dbil_wshed2d_process::clone() const
{
  return new dbil_wshed2d_process(*this);
}

//: Return the name of this process
vcl_string 
dbil_wshed2d_process::name()
{
  return "Watershed Transform 2D";
}

int
dbil_wshed2d_process::input_frames()
{
  return 1;
}

int
dbil_wshed2d_process::output_frames()
{
  return 1;
}

vcl_vector< vcl_string >
dbil_wshed2d_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}

vcl_vector< vcl_string >
dbil_wshed2d_process::get_output_type()
{
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "image" );
  to_return.push_back( "image" );
  return to_return;
}

bool
dbil_wshed2d_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cout << "In vidpro1_can_test_process::execute() - not exactly one" << " input image" << vcl_endl;
    return false;
  }
  clear_output();

  //start the timer
  vul_timer t;

  // get image from the storage class
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image_sptr = frame_image->get_image();
  vil_image_view< unsigned char > image_view = image_sptr->get_view(0, image_sptr->ni(), 0, image_sptr->nj() );
  vil_image_view< unsigned char > greyscale_view;

  if( image_view.nplanes() == 3 ) {
    vil_convert_planes_to_grey( image_view , greyscale_view );
    vcl_cout << "converted" << vcl_endl;
  }
  else if ( image_view.nplanes() == 1 ) {
    greyscale_view = image_view;
    vcl_cout << "not converted" << vcl_endl;
  } 
  else {
    vcl_cerr << "Returning false. nplanes(): " << image_view.nplanes() << vcl_endl;
    return false;
  }

  float sigma1=0, sigma2=0;
  int min_x=0, min_y=0, max_x=0, max_y=0;
  parameters()->get_value( "-gssigma1" , sigma1 );
  parameters()->get_value( "-gssigma2" , sigma2 );
  parameters()->get_value( "-min_x" , min_x );
  parameters()->get_value( "-min_y" , min_y );
  parameters()->get_value( "-max_x" , max_x );
  parameters()->get_value( "-max_y" , max_y );

  //default case of no boundaries when the user do not change these values
  if(min_x == 0 && max_x == 0 && min_y == 0 && max_y == 0)
  {
    max_x = image_view.ni();
    max_y = image_view.nj();
    vcl_cout << "All image is being segmented" << vcl_endl;
  } 
  //error checks
  else if(min_x < 0)
  {
    vcl_cerr << "X_Min is smaller than zero" << vcl_endl;
    return false;
  }
  else if(unsigned(min_x) > image_view.ni())
  {
    vcl_cerr << "X_Min is larger than image width" << vcl_endl;
    return false;
  }
  else if(max_x < 0)
  {
    vcl_cerr << "X_Max is smaller than zero" << vcl_endl;
    return false;
  }
  else if(unsigned(max_x) > image_view.ni())
  {
    vcl_cerr << "X_Max is larger than image width" << vcl_endl;
    return false;
  }
  else if(min_y < 0)
  {
    vcl_cerr << "Y_Min is smaller than zero" << vcl_endl;
    return false;
  }
  else if(unsigned(min_y) > image_view.nj())
  {
    vcl_cerr << "Y_Min is larger than image height" << vcl_endl;
    return false;
  }
  else if(max_y < 0)
  {
    vcl_cerr << "Y_Max is smaller than zero" << vcl_endl;
    return false;
  }
  else if(unsigned(max_y) > image_view.nj())
  {
    vcl_cerr << "Y_Max is larger than image height" << vcl_endl;
    return false;
  }
  else if(min_x >= max_x)
  {
    vcl_cerr << "X_Min is greater than or equal to X_Max" << vcl_endl;
    return false;
  }
  else if(min_y >= max_y)
  {
    vcl_cerr << "Y_Min is greater than or equal to Y_Max" << vcl_endl;
    return false;
  }

  bil_wshed_2d dbil_wshed_2d;    
  vcl_vector< vil_image_view< unsigned char > > watershed_result_vector;
  vil_image_view< unsigned char > watershed_result;
  vil_image_view< vil_rgb< unsigned char > > watershed_result_rgb(image_view.ni(), image_view.nj());
  vil_image_view< vil_rgb< unsigned char > > watershed_result_rgb_2(image_view.ni(), image_view.nj());
  watershed_result_vector = dbil_wshed_2d.bil_wshed_2d_main(greyscale_view, sigma1, sigma2, min_x, min_y, max_x, max_y);
  watershed_result = watershed_result_vector.front();
  for(unsigned j = 0; j < image_view.nj(); j++)
  {
    for(unsigned i = 0; i < image_view.ni(); i++)
    {
      if(watershed_result(i, j) == WSHED)
      {
        watershed_result_rgb(i, j).r = 255;
        watershed_result_rgb(i, j).g = 0;
        watershed_result_rgb(i, j).b = 0;
      }
      else
      {
        watershed_result_rgb(i, j).r = greyscale_view(i, j);
        watershed_result_rgb(i, j).g = greyscale_view(i, j);
        watershed_result_rgb(i, j).b = greyscale_view(i, j);
       }
    }
  }
  for(int j = min_y ; j < max_y; j++)
  {
    watershed_result_rgb(min_x, j).r = 0;
    watershed_result_rgb(min_x, j).g = 255;
    watershed_result_rgb(min_x, j).b = 0;

    watershed_result_rgb(max_x-1, j).r = 0;
    watershed_result_rgb(max_x-1, j).g = 255;
    watershed_result_rgb(max_x-1, j).b = 0;
  }
  for(int i = min_x ; i < max_x; i++)
  {
     watershed_result_rgb(i, min_y).r = 0;
     watershed_result_rgb(i, min_y).g = 255;
     watershed_result_rgb(i, min_y).b = 0;

     watershed_result_rgb(i, max_y-1).r = 0;
     watershed_result_rgb(i, max_y-1).g = 255;
     watershed_result_rgb(i, max_y-1).b = 0;
  }
  
  vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
  output_storage->set_image(vil_new_image_resource_of_view(watershed_result_rgb));
  output_data_[0].push_back(output_storage);

  watershed_result = watershed_result_vector.back();
  
  for(unsigned j = 0; j < image_view.nj(); j++)
  {
    for(unsigned i = 0; i < image_view.ni(); i++)
    {
      int region_id = watershed_result(i, j);
      if(region_id == 0)
      {
        watershed_result_rgb_2(i, j).r = greyscale_view(i, j);
        watershed_result_rgb_2(i, j).g = greyscale_view(i, j);
        watershed_result_rgb_2(i, j).b = greyscale_view(i, j);
      }
      else
      {
        double region_mean_value = dbil_wshed_2d.wshed_regions_[region_id].mean_intensity;
        watershed_result_rgb_2(i, j).r = 0;//(int)region_mean_value;
        watershed_result_rgb_2(i, j).g = (int)region_mean_value;
        watershed_result_rgb_2(i, j).b = 0;//(int)region_mean_value;
      }
    }
  }

  vidpro1_image_storage_sptr output_storage_2 = vidpro1_image_storage_new();
  output_storage_2->set_image(vil_new_image_resource_of_view(watershed_result_rgb_2));
  output_data_[0].push_back(output_storage_2);

    return true;
}

bool
dbil_wshed2d_process::finish()
{
  return true;
}
