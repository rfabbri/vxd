// This is bbasd/bnld/algo/pro/bnld_function_sampling_process_h_
#ifndef bnld_function_sampling_process_h_
#define bnld_function_sampling_process_h_

//:
// \file
// \brief A process for upsampling a bnld_eno object (or other continuous
// functions). Interpolates input vsol's
//
// \author Based on original code by  Ricardo Fabbri
// \date 03/26/05
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vcl_vector.h>

class bnld_function_sampling_process : public bpro1_process
{
public:
  bnld_function_sampling_process();
  ~bnld_function_sampling_process() {}
  
  vcl_string name() {
    return "Sample Function";
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

  bool function_upsample(int new_size, int type);

protected:
};

#endif // bnld_function_sampling_process_h_
