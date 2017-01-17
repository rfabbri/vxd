// This is brcv/seg/sdetd/pro/bdifd_edgel_data_process.h
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

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>

//: Process generates subpixel edgels sampled along some analytic curves
class bdifd_edgel_data_process : public bpro1_process 
{
public:

  bdifd_edgel_data_process();
  virtual ~bdifd_edgel_data_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  int input_frames();
  int output_frames();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();

};

#endif
