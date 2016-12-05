// This is breye1/vidpro1/process/vidpro1_equalize_images_process.h
#ifndef vidpro1_equalize_images_process_h_
#define vidpro1_equalize_images_process_h_

//:
// \file
// \brief Process that linearly adjusts pixels to match another image
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 6/27/05
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>
#include <vil/vil_image_view.h>

//: Derived video process class for equalizing
class vidpro1_equalize_images_process : public bpro1_process {

public:

  vidpro1_equalize_images_process();
  virtual ~vidpro1_equalize_images_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  int input_frames();
  int output_frames();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();

protected:
  vil_image_view<float> model_;
  vil_image_view<bool> mask_;

};

#endif
