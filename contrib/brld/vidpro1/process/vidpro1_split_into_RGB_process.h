// This is brl/vidpro1/process/vidpro1_split_into_RGB_process.h
#ifndef vidpro1_split_into_RGB_process_h_
#define vidpro1_split_into_RGB_process_h_

//:
// \file
// \brief Process that splits an RGB image into separate color channel images
// \author Amir Tamrakar
// \date 01/22/06
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>

//: Process that splits an RGB image into separate color channel images
class vidpro1_split_into_RGB_process : public bpro1_process {

public:
  //: Constructor
  vidpro1_split_into_RGB_process();
  //: Destructor
  virtual ~vidpro1_split_into_RGB_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  //: Returns the name of this process
  vcl_string name();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  int input_frames();
  int output_frames();

  bool execute();
  bool finish();

};

#endif
