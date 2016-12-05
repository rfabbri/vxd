// This is brl/vidpro1/process/vidpro1_load_image_process.h
#ifndef vidpro1_load_image_process_h_
#define vidpro1_load_image_process_h_

//:
// \file
// \brief A process for loading an image into a frame
// \author Amir Tamrakar
// \date 06/06/04
//
// \verbatim
//  Modifications
// \endverbatim

#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

//: Loads an image into a video frame by creating a 
// vidpro1_image_storage class at that frame

class vidpro1_load_image_process : public bpro1_process
{
 public:
  
  vidpro1_load_image_process();
 ~vidpro1_load_image_process();

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

#endif //vidpro1_load_image_process_h_
