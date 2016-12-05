// This is brl/vidpro1/process/vidpro1_horn_schunck_process.h
#ifndef vidpro1_horn_schunck_process_h_
#define vidpro1_horn_schunck_process_h_

//:
// \file
// \brief Process that computes horn_schunck optical flow image between frames
// \author J.L. Mundy
// \date June 28, 2005
//
// \verbatim
//  Modifications (None)
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>
#include <vil/vil_image_view.h>

//: Derived video process class for computing horn_schunck
class vidpro1_horn_schunck_process : public bpro1_process {

public:

  vidpro1_horn_schunck_process();
  virtual ~vidpro1_horn_schunck_process();

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name();

  int input_frames();
  int output_frames();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();

private:
  //: store the output frame
void store_output(vil_image_view<vxl_byte> const& output);

  //: members
  //: range for display
  float low_range_;
  float high_range_;
  //: alpha regularization coefficient
  float alpha_;
  //: no of iterations 
  int no_of_iter_;
};

#endif
