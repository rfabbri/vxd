// This is brl/vidpro1/process/vidpro1_brightness_contrast_process.h
#ifndef vidpro1_brightness_contrast_process_h_
#define vidpro1_brightness_contrast_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief live vidpro1_brightness_contrast_process
//
//  A generic video processor that is called from the live_video_manager
//  to carry out algorithms on the live video frames.
// \author
//   J.L. Mundy
//
// \verbatim
//  Modifications:
//   J.L. Mundy October 9, 2002    Initial version.
// \endverbatim
//--------------------------------------------------------------------------------


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>


class vidpro1_brightness_contrast_process : public bpro1_process
{
 public:
  
  vidpro1_brightness_contrast_process();
 ~vidpro1_brightness_contrast_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  int input_frames();
  int output_frames();
  
  bool execute();
  bool finish();
 
 protected:
};

#endif // vidpro1_brightness_contrast_process_h_
