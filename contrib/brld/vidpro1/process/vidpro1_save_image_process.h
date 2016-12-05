// This is brl/vidpro1/process/vidpro1_save_image_process.h

#ifndef vidpro1_save_image_process_h_
#define vidpro1_save_image_process_h_

//:
// \file
// \brief A process for saving an image
// \author Matt Leotta
// \date 5/5/05
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>
#include <vcl_list.h>
#include <vil/vil_image_resource_sptr.h>

class vidpro1_save_image_process : public bpro1_process
{
 public:

  vidpro1_save_image_process();
 ~vidpro1_save_image_process();

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

};

#endif // vidpro1_save_image_process
