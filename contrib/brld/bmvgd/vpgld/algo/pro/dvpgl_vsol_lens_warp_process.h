// This is brcv/mvg/dvpgl/algo/pro/dvpgl_vsol_lens_warp_process.h
#ifndef dvpgl_vsol_lens_warp_process_h_
#define dvpgl_vsol_lens_warp_process_h_

//:
// \file
// \brief A process that applies lens distortion to vsol
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 8/24/05
//
// \verbatim
//  Modifications
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>

#include <bpro1/bpro1_parameters.h>


class dvpgl_vsol_lens_warp_process : public bpro1_process
{
public:

  dvpgl_vsol_lens_warp_process();
  ~dvpgl_vsol_lens_warp_process(){}

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

#endif // dvpgl_vsol_lens_warp_process_h_
