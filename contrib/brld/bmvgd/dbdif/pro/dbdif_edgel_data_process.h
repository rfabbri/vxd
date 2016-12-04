// This is brcv/seg/dbdet/pro/dbdif_edgel_data_process.h
#ifndef dbdif_edgel_data_process_h_
#define dbdif_edgel_data_process_h_

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
class dbdif_edgel_data_process : public bpro1_process 
{
public:

  dbdif_edgel_data_process();
  virtual ~dbdif_edgel_data_process();

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
