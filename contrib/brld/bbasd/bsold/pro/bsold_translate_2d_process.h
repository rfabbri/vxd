// This is bbasd/bsold/pro/bsold_translate_2d_process.h

#ifndef bsold_translate_2d_process_h_
#define bsold_translate_2d_process_h_

//:
// \file
// \brief Process that translate all vsol objects with a vector
// \author Based on original code by  Nhon Trinh (@lems.brown.edu)
// \date 4/12/2005
//
// \verbatim
//  Modifications
//    2005/4/12 Nhon Trinh    Creation
// \endverbatim

#include <bpro1/bpro1_process.h>


//: Derived video process class for translating vsol_2d objects
class bsold_translate_2d_process : public bpro1_process {

public:

  bsold_translate_2d_process();
  virtual ~bsold_translate_2d_process();

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

};

#endif //bsold_translate_2d_process_h_
