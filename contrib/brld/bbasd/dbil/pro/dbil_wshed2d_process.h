// This is basic/dbil/pro/dbil_wshed2d_process.h
#ifndef dbil_wshed2d_process_h_
#define dbil_wshed2d_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief   this implements the watershed transform of an image
//
// \author
//   H.C. Aras
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

class dbil_wshed2d_process : public bpro1_process 
{
public:

  dbil_wshed2d_process();
  virtual ~dbil_wshed2d_process();

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


#endif // dbil_wshed2d_process_h_
