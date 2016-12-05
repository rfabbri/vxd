// This is brl/vidpro1/process/vidpro1_load_video_process.cxx



#include "vidpro1_load_video_process.h"


#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <vidl/vidl_istream_sptr.h>
#include <vidl/vidl_image_list_istream.h>
#include <vidl/vidl_convert.h>

#include <vil/vil_new.h>
#include <vil/vil_image_resource_sptr.h>
#include <vil/vil_image_view.h>
#include <vil/vil_crop.h>
#include <vil/vil_load.h>

#include <vul/vul_file_iterator.h>
#include <vul/vul_file.h>
#include <vnl/vnl_math.h>
#include <vcl_iostream.h>
#include <vcl_algorithm.h>


#if HAS_DSHOW
#include <vcl_cstdio.h>
#include <vil/vil_flip.h>
#include <vil/vil_save.h>
#include <vil/vil_decimate.h>
#include <vil/algo/vil_gauss_filter.h>
#include <vidl/vidl_frame.h>
#include <vidl/vidl_convert.h>
#include <vidl/vidl_image_list_istream.h>
#include <vidl/vidl_image_list_ostream.h>
#include <vidl/vidl_dshow_file_istream.h>
#endif


//: Constructor
vidpro1_load_video_process::vidpro1_load_video_process() : bpro1_process(), num_frames_(0)
{
    if( !parameters()->add( "Video file <filename...>" , "-video_filename", bpro1_filepath("","*") )||
#if HAS_DSHOW
        !parameters()->add( "Load with direct show? (clip it, otherwise may take forever!" , "-dshow",bool(true) )||
        !parameters()->add( "Is Decimated" , "-isdec",bool(false) )||
        !parameters()->add( "factor: " , "-factor",int(2) )||
        !parameters()->add( "smoothing sigma: " , "-sigma",float(1.0f) )||
#endif
        !parameters()->add( "Is ROI" , "-isroi",bool(false) )||
        !parameters()->add( "ROI(top x)" , "-topx",int(-1) )||
        !parameters()->add( "ROI(top y)" , "-topy",int(-1) )||
        !parameters()->add( "ROI(Length x)" , "-lenx",int(-1) )||
        !parameters()->add( "ROI(Length y)" , "-leny",int(-1) ) ||
        //: update vidl1 in vxl for this option to be effective
        !parameters()->add( "Is Clipped" , "-isclipped",bool(false) )||
        !parameters()->add( "Start Frame: " , "-sf",int(0) )||
        !parameters()->add( "End Frame: " , "-ef",int(10) )

        )
    {
        vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
    }
}


//: Destructor
vidpro1_load_video_process::~vidpro1_load_video_process()
{
}


//: Clone the process
bpro1_process*
vidpro1_load_video_process::clone() const
{
    return new vidpro1_load_video_process(*this);
}


//: Return the name of the process
vcl_string vidpro1_load_video_process::name()
{
    return "Load Video";
}


//: Call the parent function and reset num_frames_
void
vidpro1_load_video_process::clear_output(int resize)
{
  num_frames_ = 0;
  bpro1_process::clear_output(resize);  
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string >
vidpro1_load_video_process::get_input_type()
{
    vcl_vector< vcl_string > to_return;
    // no input type required
    to_return.clear();

    return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string >
vidpro1_load_video_process::get_output_type()
{
    vcl_vector< vcl_string > to_return;
    to_return.push_back( "image" );

    return to_return;
}


//: Returns the number of input frames to this process
int
vidpro1_load_video_process::input_frames()
{
    return 0;
}


//: Returns the number of output frames from this process
int
vidpro1_load_video_process::output_frames()
{
    return num_frames_;
}


//: Run the process on the current frame
bool
vidpro1_load_video_process::execute()
{
  bpro1_filepath video_path;
  static int topx_, topy_;
  static int lenx_, leny_;
  static bool isroi, isclipped;
  static int start_frame_, end_frame_;
  parameters()->get_value( "-video_filename" , video_path );
  parameters()->get_value( "-topx" , topx_ );
  parameters()->get_value( "-topy" , topy_ );
  parameters()->get_value( "-lenx" , lenx_ );
  parameters()->get_value( "-leny" , leny_ );
  parameters()->get_value( "-isroi" , isroi );

  //: update vidl1 in vxl for this option to be effective
  parameters()->get_value( "-sf" , start_frame_ );
  parameters()->get_value( "-ef" , end_frame_ );
  parameters()->get_value( "-isclipped" , isclipped );

#if HAS_DSHOW
    static bool use_dshow;
    static int upper_cnt;
    parameters()->get_value( "-dshow" , use_dshow );

    static int factor; static float sigma;
    static bool decimate;
    parameters()->get_value( "-isdec", decimate);
    parameters()->get_value( "-factor",factor);
    parameters()->get_value( "-sigma",sigma);

    if (!use_dshow) {
#endif



  if ( video_path.path == "")
    return false;


  vcl_string path = video_path.path;
  
  // determine whether the input path is a video file or a list of image files
  bool glob = false;
  if ( vul_file::is_directory(path) || path.find_first_of('*') != vcl_string::npos)
     glob = true;
  
  // if the input path is an image file, then we load all images with the same extention, i.e.,
  // if the input path has the form /image_folder/image_01.jpg
  // then we will load all images of the form /image_folder/*.jpg
  if(!glob && vil_load_image_resource(path.c_str()))
  {
    vcl_string ex = vul_file::extension(path);
    path = vul_file::dirname(path) + "/*" + ex;;
    glob = true;
  }

  // Load the video stream
  vidl_istream_sptr video_istr = 0;
  if(glob) //we are opening an image list
  {
    video_istr = new vidl_image_list_istream(path);
  }

  // Check if video loading was successful
  if( !video_istr || !video_istr->is_open() ) 
    return false;

  // Load the images input video stream and put them into the image storages
  vcl_cout << "\nLoading video frames into memory ....";
  vcl_vector<vidpro1_image_storage_sptr > image_storage_list;
  while (video_istr->advance())
  {
    vidl_frame_sptr frame = video_istr->current_frame();
    
    // convert the frame to an image_view, which we can work with
    vil_image_view<vxl_byte > image_view;
    vidl_convert_to_view(*frame, image_view);
    
    // put the image into a storage
    vidpro1_image_storage_sptr image_storage = vidpro1_image_storage_new();
    vil_image_resource_sptr img = vil_new_image_resource_of_view(image_view);

    // crop the frame if necessary
    if(isroi)
    {
      int ni=img->ni();
      int nj=img->nj();
      
      if(topx_<ni)
        topx_=vnl_math::max(0,topx_);
      else
        topx_=0;

      if(topy_<nj)
        topy_=vnl_math::max(0,topy_);
      else
        topy_=0;

  
      if(lenx_>0)
        lenx_=vnl_math::min(ni-topx_,lenx_);
      else
        lenx_=0;

      if(leny_>0)
        leny_=vnl_math::min(nj-topy_,leny_);
      else
        leny_=0;

      vil_image_resource_sptr cropimg=vil_crop(img,topx_,lenx_,topy_,leny_);
      image_storage->set_image( cropimg );
    }
    else
    {
      image_storage->set_image( img );
    }

    // save to the global list
    image_storage_list.push_back(image_storage);
  }
  vcl_cout << image_storage_list.size() << " frames loaded.\n";

  // push the image storage list to the repository. Need to put the last frame first.
  for (vcl_vector<vidpro1_image_storage_sptr >::reverse_iterator iter = 
    image_storage_list.rbegin(); iter != image_storage_list.rend(); ++iter)
  {
    output_data_.push_back(vcl_vector< bpro1_storage_sptr >(1, *iter));
    ++this->num_frames_;
  }

#if HAS_DSHOW

    } else {  // using dshow
      vcl_vector< vil_image_resource_sptr > temp_output;
      
      vcl_vector<vcl_string> video_files;
      for(vul_file_iterator fn = video_path.path; fn; ++fn)
        video_files.push_back(fn());
      while(!video_files.empty())
      {
        vcl_cout << "Using direct show to open: " << video_files.back() << vcl_endl;
        vidl_dshow_file_istream istream(video_files.back());

        if (!istream.is_open()) {
          vcl_cout << "Failed to open the input stream\n";
          return false;
        }
        
        int increment = 0;
        if (istream.is_seekable() && isclipped && start_frame_ >= 0 && end_frame_ >= start_frame_) {
          istream.seek_frame(start_frame_);
          increment = 1;
        } else {
          if (istream.is_seekable())
            istream.seek_frame(0);
          else {
            vcl_cout << "The stream is not seekable! Quitting!\n";
            return false;
          }
          start_frame_ = end_frame_ = 0;
        }
        
          for (int kk = start_frame_; kk <= end_frame_; kk += increment) {
            //: read frame will advance after processing this frame
            vidl_frame_sptr frame = istream.current_frame();
           
            vil_image_resource_sptr img_r;
            if (frame) {
              int height = frame->nj();
              int width = frame->ni();
              vcl_cout << "orig frame number: " << istream.frame_number() << " width: " << width << " height: " << height << vcl_endl;
              
              vil_image_view<vxl_byte> img, img_ud, dummy;
              vidl_convert_to_view(*frame,img);
              //itab_->set_image_view(img);
              img_ud = vil_flip_ud(img);
              //vil_image_resource_sptr dummy;
              if(isroi)
              {  
                if(topx_<width)
                  topx_=vcl_max(0,topx_);
                else
                  topx_=0;
  
                if(topy_<height)
                  topy_=vcl_max(0,topy_);
                else
                  topy_=0;
  
                if(lenx_>0)
                  lenx_=vcl_min(width-topx_,lenx_);
                else
                  lenx_=0;
  
                if(leny_>0)
                  leny_=vcl_min(height-topy_,leny_);
                else
                  leny_=0;
                dummy = vil_crop(img_ud, topx_, lenx_, topy_, leny_);
              } else
                dummy = img_ud;
              
              if (decimate) {
                vil_image_view<vxl_byte> dummy2;
                vil_gauss_filter_2d(dummy, dummy2, sigma, unsigned(vcl_floor(3*sigma+0.5)));
                img_r = vil_decimate(vil_new_image_resource_of_view(dummy2), factor, factor);
              }
              else
                img_r = vil_new_image_resource_of_view(dummy);


            } else {
              vcl_cout << "Either the frame is corrupted stopping, or more likely end of file is reached!\n";
              break;
            }
            
            //vcl_vector< bpro1_storage_sptr > tmp(1,image_storage);
            //temp_output.push_back(tmp);
            temp_output.push_back(img_r);
            num_frames_ ++;
            //: advance the stream
            istream.advance();
          }

        video_files.pop_back();
      }

      for (int k = int(temp_output.size()-1); k >= 0; k--) {
        vidpro1_image_storage_sptr image_storage = vidpro1_image_storage_new();
        if (temp_output[k])
          image_storage->set_image(temp_output[k]);
        output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,image_storage));
      }
      vcl_cout << "num_frames_: " << num_frames_ << " output size: " << output_data_.size() << vcl_endl;

    }
#endif

    return true;   
}


//: Finish
bool
vidpro1_load_video_process::finish() 
{
    return true;
}



