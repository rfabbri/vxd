// This is basic/dbnl/algo/pro/dbnl_function_sampling_process_h_
#ifndef dbnl_function_sampling_process_h_
#define dbnl_function_sampling_process_h_

//:
// \file
// \brief A process for upsampling a dbnl_eno object (or other continuous
// functions). Interpolates input vsol's
//
// \author Ricardo Fabbri
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

class dbnl_function_sampling_process : public bpro1_process
{
public:
  dbnl_function_sampling_process();
  ~dbnl_function_sampling_process() {}
  
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

#endif // dbnl_function_sampling_process_h_
