// This is brl/vidpro1/process/vidpro1_frame_diff_process.h
#ifndef vidpro1_frame_diff_process_h_
#define vidpro1_frame_diff_process_h_

//:
// \file
// \brief Process that computes absolute difference between frames
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 11/11/03
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>

//: Derived video process class for computing frame difference
class vidpro1_frame_diff_process : public bpro1_process {

public:

  vidpro1_frame_diff_process();
  virtual ~vidpro1_frame_diff_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  int input_frames();
  int output_frames();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();

};

#endif
