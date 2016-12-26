// This is basic/bild/pro/bild_wshed2d_process.h
#ifndef bild_wshed2d_process_h_
#define bild_wshed2d_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief   this implements the watershed transform of an image
//
// \author Based on original code by 
//   H.C. Aras
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

class bild_wshed2d_process : public bpro1_process 
{
public:

  bild_wshed2d_process();
  virtual ~bild_wshed2d_process();

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


#endif // bild_wshed2d_process_h_
