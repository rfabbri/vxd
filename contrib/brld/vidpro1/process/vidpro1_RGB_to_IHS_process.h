// This is brl/vidpro1/process/vidpro1_RGB_to_IHS_process.h
#ifndef vidpro1_RGB_to_IHS_process_h_
#define vidpro1_RGB_to_IHS_process_h_

//:
// \file
// \brief Process that converts RGB image to IHS image
// \author Amir Tamrakar
// \date 10/31/05
//
// \verbatim
//  Modifications
//    Amir Tamrakar     Added an option to ouput either as component images or as a single image with 3 planes
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>

//: Process that converts RGB image to IHS image
class vidpro1_RGB_to_IHS_process : public bpro1_process {

public:
  //: Constructor
  vidpro1_RGB_to_IHS_process();
  //: Destructor
  virtual ~vidpro1_RGB_to_IHS_process();

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
