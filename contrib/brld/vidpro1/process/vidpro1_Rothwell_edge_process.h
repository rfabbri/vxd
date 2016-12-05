// This is brl/vidpro1/process/vidpro1_Rothwell_edge_process.h
#ifndef vidpro1_Rothwell_edge_process_h_
#define vidpro1_Rothwell_edge_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief Computes Rothwell-Mundy edges on each video frame
//
// \author Amir Tamrakar
// \date 03/29/06
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_vector.h>
#include <vcl_string.h>
#include <osl/osl_canny_rothwell_params.h>
#include <bpro1/bpro1_process.h>

class vidpro1_Rothwell_edge_process : public bpro1_process 
{
public:

  vidpro1_Rothwell_edge_process();
  virtual ~vidpro1_Rothwell_edge_process();

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
  osl_canny_rothwell_params dp;

};


#endif // vidpro1_Rothwell_edge_process_h_
