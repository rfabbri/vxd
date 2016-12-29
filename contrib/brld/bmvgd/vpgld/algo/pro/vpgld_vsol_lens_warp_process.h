// This is bmvgd/vpgld/algo/pro/vpgld_vsol_lens_warp_process.h
#ifndef vpgld_vsol_lens_warp_process_h_
#define vpgld_vsol_lens_warp_process_h_

//:
// \file
// \brief A process that applies lens distortion to vsol
// \author Based on original code by  Matt Leotta (@lems.brown.edu)
// \date 8/24/05
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

#include <bpro1/bpro1_parameters.h>


class vpgld_vsol_lens_warp_process : public bpro1_process
{
public:

  vpgld_vsol_lens_warp_process();
  ~vpgld_vsol_lens_warp_process(){}

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name();

  int input_frames();
  int output_frames();
  
  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();


private:

};

#endif // vpgld_vsol_lens_warp_process_h_
