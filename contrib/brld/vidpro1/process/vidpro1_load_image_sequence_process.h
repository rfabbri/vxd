// This is brl/vidpro1/process/vidpro1_load_image_sequence_process.h

#ifndef vidpro1_load_image_sequence_process_h_
#define vidpro1_load_image_sequence_process_h_

//:
// \file
// \brief A process for loading a sequence of images, given the folder containing
//        the images and a file containing the list of image names (each line in
//        the file contains the name of one image)
// \author Nhon Trinh
// \date Sep 11, 2008
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

class vidpro1_load_image_sequence_process : public bpro1_process
{
 public:
  vidpro1_load_image_sequence_process();
  ~vidpro1_load_image_sequence_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  virtual void clear_output(int resize = -1);

  int input_frames();
  int output_frames();
  
  bool execute();
  bool finish();

  int num_frames() {return num_frames_;}
 
protected:
   // number of images loaded
   int num_frames_;
};

#endif // vidpro1_load_image_sequence_process
