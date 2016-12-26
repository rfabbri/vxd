// This is basic/dbil/pro/dbil_harr_wavelet_process.h
#ifndef dbil_harr_wavelet_process_h_
#define dbil_harr_wavelet_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief  A process for the Harr Wavelet Transform and its inverse
//
// \author Matt Leotta
// \date 10/20/05
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

class dbil_harr_wavelet_process : public bpro1_process 
{
public:

  dbil_harr_wavelet_process();
  virtual ~dbil_harr_wavelet_process();

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


#endif // dbil_harr_wavelet_process_h_
