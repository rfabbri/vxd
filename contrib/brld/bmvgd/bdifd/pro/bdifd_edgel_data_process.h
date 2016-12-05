// This is brcv/seg/dbdet/pro/bdifd_edgel_data_process.h
#ifndef bdifd_edgel_data_process_h_
#define bdifd_edgel_data_process_h_

//:
// \file
// \brief Process to generate synthetic subpixel edgels for testing
// \author Ricardo Fabbri
// \date Tue May  9 10:00:51 EDT 2006
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1d/bpro1d_process.h>
#include <bpro1d/bpro1d_parameters.h>

//: Process generates subpixel edgels sampled along some analytic curves
class bdifd_edgel_data_process : public bpro1d_process 
{
public:

  bdifd_edgel_data_process();
  virtual ~bdifd_edgel_data_process();

  //: Clone the process
  virtual bpro1d_process* clone() const;

  vcl_string name();

  int input_frames();
  int output_frames();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();

};

#endif
