// This is basic/bild/pro/bild_osl_canny_edges_process.h
#ifndef bild_osl_canny_edges_process_h
#define bild_osl_canny_edges_process_h


//:
// \file
// \brief  A process for detecting edges of an image using
// U. Oxford's Canny edge detector
//
// \author Based on original code by  Vishal Jain (vj)
// \date 06/27/2003
//
// \verbatim
//  Modifications:
// \endverbatim
//----------------------------------------------------------------------

#include <bpro1/bpro1_process.h>

class bild_osl_canny_edges_process : public bpro1_process {

public:

  bild_osl_canny_edges_process();
  ~bild_osl_canny_edges_process();

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
