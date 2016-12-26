// This is basic/dbsol/pro/dbsol_translate_2d_process.h

#ifndef dbsol_translate_2d_process_h_
#define dbsol_translate_2d_process_h_

//:
// \file
// \brief Process that translate all vsol objects with a vector
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date 4/12/2005
//
// \verbatim
//  Modifications
//    2005/4/12 Nhon Trinh    Creation
// \endverbatim

#include <bpro1/bpro1_process.h>


//: Derived video process class for translating vsol_2d objects
class dbsol_translate_2d_process : public bpro1_process {

public:

  dbsol_translate_2d_process();
  virtual ~dbsol_translate_2d_process();

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

#endif //dbsol_translate_2d_process_h_
