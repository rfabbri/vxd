// This is bbasd/bsold/pro/bsold_curve_sampling_process_h_
#ifndef bsold_curve_sampling_process_h_
#define bsold_curve_sampling_process_h_

//:
// \file
// \brief A process for upsampling a bsold_interp_curve_2d object
// \author Based on original code by  Ozge Can Ozcanli
// \date 07/07/04
//
// Note: It only saves contour type geometry, i.e., vsol2D_curve type and ignores the others
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vcl_vector.h>

class bsold_curve_sampling_process : public bpro1_process
{
public:
  bsold_curve_sampling_process();
  ~bsold_curve_sampling_process() {}
  
  vcl_string name() {
    return "Sample Curve";
  }

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();
  
  int input_frames() {
    return 1;
  }
  int output_frames() {
    return 1;
  }
  
  bool execute();
  bool finish() {
    return true;
  }

  bool curveUpsample (int new_size, int type);

protected:
};

#endif // bsold_curve_sampling_process_h_
