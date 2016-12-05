// This is brl/vidpro1/process/vidpro1_save_video_process.cxx

//:
// \file

#include <vidpro1/process/vidpro1_save_video_process.h>
#include <vcl_iostream.h>

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <vil/vil_new.h>
#include <vil/vil_convert.h>

#include <vul/vul_sprintf.h>
#include <vil/vil_convert.h>
#include <vil/vil_save.h>

//: Constructor
vidpro1_save_video_process::vidpro1_save_video_process() 
 : bpro1_process(), sample_counter_(0), first_frame_(true), num_frames_(0)
{
  if( !parameters()->add( "Video file <filename...>" , "-video_filename", bpro1_filepath("","*") ) ||
      !parameters()->add( "Type" , "-video_type", vcl_string("tiff") ) ||

      !parameters()->add( "Frame Sample Interval" , "-frame_sample_interval",
                          unsigned(1) ) )
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
vidpro1_save_video_process::~vidpro1_save_video_process()
{
}


//: Clone the process
bpro1_process*
vidpro1_save_video_process::clone() const
{
  return new vidpro1_save_video_process(*this);
}


//: Return the name of the process
vcl_string vidpro1_save_video_process::name()
{
  return "Save Video";
}


//: Call the parent function and reset num_frames_
void
vidpro1_save_video_process::clear_output()
{
  bpro1_process::clear_output();
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string >
vidpro1_save_video_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  // image type required
  to_return.push_back( "image" );

  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string >
vidpro1_save_video_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.clear();

  return to_return;
}


//: Returns the number of input frames to this process
int
vidpro1_save_video_process::input_frames()
{
  return 1;
}


//: Returns the number of output frames from this process
int
vidpro1_save_video_process::output_frames()
{
  return 0;
}


//: Run the process on the current frame
bool
vidpro1_save_video_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cout << "In vidpro1_background_diff_process::execute() - "
             << "not exactly two input images \n";
    return false;
  }
  clear_output();
  unsigned sample_interval=0;
  parameters()->get_value( "-frame_sample_interval" , sample_interval );
  vidpro1_image_storage_sptr foreground_image;
  foreground_image.vertical_cast(input_data_[0][0]);
  vil_image_resource_sptr image_sptr = foreground_image->get_image();

  // vil_save expects a byte image so we must first make a byte
  // image out of whatever we have
  vil_image_view<vxl_byte> save_image;
  
  switch(image_sptr->pixel_format())
  {
    case VIL_PIXEL_FORMAT_BYTE:
    {
      save_image = image_sptr->get_view();
      break;
    }
    case VIL_PIXEL_FORMAT_UINT_16:
    {
      vil_image_view<vxl_uint_16> uint_image = image_sptr->get_view();
      vil_image_view<double> double_image;
      vil_convert_stretch_range_limited(uint_image, double_image,
                                        vxl_uint_16(0),
                                        vxl_uint_16(65535),
                                        0.0, 255.999);
      vil_convert_cast(double_image, save_image);
      break;
    }
   
    case VIL_PIXEL_FORMAT_BOOL:
    {
      vil_image_view<bool> bool_image = image_sptr->get_view();
      vil_convert_cast(bool_image, save_image);
      vil_math_scale_values(save_image,255.0);
      break;
    }
    
    case VIL_PIXEL_FORMAT_FLOAT:
    {
      vil_image_view<float> float_image = image_sptr->get_view();
      vil_image_view<double> double_image;
      vil_convert_stretch_range_limited(float_image, double_image,
                                        0.0f, 1.0f, 0.0, 255.999);
      vil_convert_cast(double_image, save_image);
      break;
    }
  
    case VIL_PIXEL_FORMAT_DOUBLE:
    {
      vil_image_view<double> double_image = image_sptr->get_view();
      vil_convert_stretch_range_limited(double_image, double_image,
                                        0.0, 1.0, 0.0, 255.999);
      vil_convert_cast(double_image, save_image);
      break;
    }
  
    default:
    {
      // stretch and convert to a byte image for display
      save_image = *vil_convert_stretch_range(vxl_byte(), image_sptr->get_view());
    }
  }
  
  
  if(first_frame_)
    {
      sample_counter_ = 0;
      list_images.push_back(vil_new_image_resource_of_view(save_image));
      first_frame_ = false;
      ++sample_counter_;
      return true;
    }
  if(sample_counter_ == sample_interval)
    {
      sample_counter_ = 0;
      list_images.push_back(vil_new_image_resource_of_view(save_image));
    }
  ++sample_counter_;
  return true;  
}


//: Finish
bool
vidpro1_save_video_process::finish() 
{
    
  bpro1_filepath video_path;
  vcl_string video_type;
  parameters()->get_value( "-video_filename" , video_path );
  parameters()->get_value( "-video_type" , video_type );
  vcl_string video_filename = video_path.path;


   // Create the extension for filenames
  vcl_string extension = video_type.substr(0, video_type.size()-5); // To get rid of "Image" string

  int n=list_images.size();
  for (int i=0;i<n;i++)
  {

    // Create a name for the current image to be saved
    vcl_string currentname = vul_sprintf("%s%05d.%s", video_filename.c_str(),
                                         i,extension.c_str());

    vil_image_view<vxl_byte> save_image;
        // Get the image from the frame

    vil_image_resource_sptr image_sptr=list_images.front();
    switch(image_sptr->pixel_format())
        {
        case VIL_PIXEL_FORMAT_BYTE: 
            {
            save_image = image_sptr->get_view();
            break;
            }
        case VIL_PIXEL_FORMAT_UINT_16:
            {
            vil_image_view<vxl_uint_16> uint_image = image_sptr->get_view();
            vil_image_view<double> double_image;
            vil_convert_stretch_range_limited(uint_image, double_image,
                vxl_uint_16(0),
                vxl_uint_16(65535), 
                0.0, 255.999);
            vil_convert_cast(double_image, save_image);
            break;
            }

        case VIL_PIXEL_FORMAT_BOOL: 
            {
            vil_image_view<bool> bool_image = image_sptr->get_view();
            vil_convert_cast(bool_image, save_image);
            vil_math_scale_values(save_image,255.0);
            break;
            }

        case VIL_PIXEL_FORMAT_FLOAT:
            {
            vil_image_view<float> float_image = image_sptr->get_view();
            vil_image_view<double> double_image;
            vil_convert_stretch_range_limited(float_image, double_image,
                0.0f, 1.0f, 0.0, 255.999);
            vil_convert_cast(double_image, save_image);
            break;
            }

        case VIL_PIXEL_FORMAT_DOUBLE:
            {
            vil_image_view<double> double_image = image_sptr->get_view();
            vil_convert_stretch_range_limited(double_image, double_image,
                0.0, 1.0, 0.0, 255.999);
            vil_convert_cast(double_image, save_image);
            break;
            }

        default:
            {
            // stretch and convert to a byte image for display
            save_image = *vil_convert_stretch_range(vxl_byte(), image_sptr->get_view());
            }
        }
    bool saved_image = vil_save(save_image, currentname.c_str(), video_type.c_str());

    if (!saved_image)
      return false;

    list_images.pop_front();
  }

  return true;
}


vcl_vector<vcl_string> vidpro1_save_video_process::list_to_vector(vcl_list<vcl_string>  v)
{
    vcl_vector<vcl_string> vec;
    vcl_list<vcl_string>::iterator iter;
    for(iter=v.begin();iter!=v.end();iter++)
        vec.push_back(*iter);

    return vec;
}

