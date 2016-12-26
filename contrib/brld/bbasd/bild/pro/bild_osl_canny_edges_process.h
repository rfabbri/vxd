// This is basic/dbil/pro/dbil_osl_canny_edges_process.h
#ifndef dbil_osl_canny_edges_process_h
#define dbil_osl_canny_edges_process_h


//:
// \file
// \brief  A process for detecting edges of an image using
// U. Oxford's Canny edge detector
//
// \author Vishal Jain (vj)
// \date 06/27/2003
//
// \verbatim
//  Modifications:
// \endverbatim
//----------------------------------------------------------------------

#include <bpro1/bpro1_process.h>

class dbil_osl_canny_edges_process : public bpro1_process {

public:

  dbil_osl_canny_edges_process();
  ~dbil_osl_canny_edges_process();

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

#endif
