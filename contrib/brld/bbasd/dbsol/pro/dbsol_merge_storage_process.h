// This is basic/dbsol/pro/dbsol_merge_storage_process.h

#ifndef dbsol_merge_storage_process_h_
#define dbsol_merge_storage_process_h_

//:
// \file
// \brief A process to smooth contours
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date 11/16/2006
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>


//: Process to Gaussian-smooth contours
class dbsol_merge_storage_process : public bpro1_process 
{

public:

  dbsol_merge_storage_process();
  virtual ~dbsol_merge_storage_process();

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

#endif //dbsol_merge_storage_process_h_
