// This is vidpro1/process/vidpro1_save_vsol_process_h_
#ifndef vidpro1_save_vsol_process_h_
#define vidpro1_save_vsol_process_h_

//:
// \file
// \brief A process for saving vsols storages in binary format
// \author Ricardo Fabbri
// \date Fri Apr 28 12:22:38 EDT 2006
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vcl_vector.h>

//: A process for saving vsols storages in binary format. User can then load the
// vsol in any other application by using the vsol storage binary r/w methods
class vidpro1_save_vsol_process : public bpro1_process
{
public:
  vidpro1_save_vsol_process();
  ~vidpro1_save_vsol_process() {}

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name() {
    return "Save vsol storage";
  }
  
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

protected:
};

#endif // vidpro1_save_vsol_process_h_
