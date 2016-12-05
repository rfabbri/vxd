// This is brl/vidpro1/process/vidpro1_save_video_process.h

#ifndef vidpro1_save_video_process_h_
#define vidpro1_save_video_process_h_

//:
// \file
// \brief A process for saving a video into a series of frames
// \author Vishal Jain
// \date 04/23/05
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>
#include <vcl_list.h>
#include <vil/vil_image_resource_sptr.h>

class vidpro1_save_video_process : public bpro1_process
{
 public:
  
  vidpro1_save_video_process();
 ~vidpro1_save_video_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  void clear_output();

  int input_frames();
  int output_frames();
  
  bool execute();
  bool finish();
 
 protected:
  unsigned sample_counter_;
  bool first_frame_;
  vcl_list<vil_image_resource_sptr> list_images;
  int num_frames_;
  vcl_vector<vcl_string> list_to_vector(vcl_list<vcl_string>  v);
};

#endif // vidpro1_save_video_process
