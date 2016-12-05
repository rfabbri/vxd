// This is breye1/vidpro1/process/vidpro1_image_gradient_process.h
#ifndef vidpro1_image_gradient_process_h_
#define vidpro1_image_gradient_process_h_

//:
// \file
// \brief A process that computes the gradient of an image
// \author Amir Tamrakar
// \date 09/26/06
//
// \verbatim
//  Modifications
//   
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>


class vidpro1_image_gradient_process : public bpro1_process {

public:

  vidpro1_image_gradient_process();
  virtual ~vidpro1_image_gradient_process();

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

private:

};

#endif // vidpro1_image_gradient_process_h_
