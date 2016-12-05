// This is brl/vidpro1/process/vidpro1_VD_edge_process.h
#ifndef vidpro1_VD_edge_process_h_
#define vidpro1_VD_edge_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief Computes VD edges on each video frame
//
// \author
//   J.L. Mundy
//
// \verbatim
//  Modifications:
//   J.L. Mundy October 11, 2002    Initial version.
//   Amir Tamrakar Nov 11, 2003     Update version for brown eyes
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_vector.h>
#include <vcl_string.h>
#include <sdet/sdet_detector_params.h>
#include <bpro1/bpro1_process.h>


class vidpro1_VD_edge_process : public bpro1_process 
{
public:

  vidpro1_VD_edge_process();
  virtual ~vidpro1_VD_edge_process();

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
  sdet_detector_params dp;

};


#endif // vidpro1_VD_edge_process_h_
