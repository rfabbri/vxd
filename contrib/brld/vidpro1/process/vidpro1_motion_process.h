// This is brl/vidpro1/process/vidpro1_motion_process.h
#ifndef vidpro1_motion_process_h_
#define vidpro1_motion_process_h_

//:
// \file
// \brief Process that computes a linear motion image between frames
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 11/11/03
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>
#include <vil/vil_image_view.h>

//: Derived video process class for computing motion
class vidpro1_motion_process : public bpro1_process {

public:

  vidpro1_motion_process();
  virtual ~vidpro1_motion_process();

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
  //: compute the motion between two images
  void compute_motion( const vil_image_view<float>& img1,
                       const vil_image_view<float>& img2,
                             vil_image_view<vxl_byte>& output );

  float low_range_;
  float high_range_;
  float sigma_;
};

#endif
