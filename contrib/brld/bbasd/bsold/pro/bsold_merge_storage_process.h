// This is bbasd/bsold/pro/bsold_merge_storage_process.h

#ifndef bsold_merge_storage_process_h_
#define bsold_merge_storage_process_h_

//:
// \file
// \brief A process to smooth contours
// \author Based on original code by  Nhon Trinh (@lems.brown.edu)
// \date 11/16/2006
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>


//: Process to Gaussian-smooth contours
class bsold_merge_storage_process : public bpro1_process 
{

public:

  bsold_merge_storage_process();
  virtual ~bsold_merge_storage_process();

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

#endif //bsold_merge_storage_process_h_
