// This is vidpro1/process/vidpro1_load_vsol_process_h_

#ifndef vidpro1_load_vsol_process_h_
#define vidpro1_load_vsol_process_h_
//:
// \file
// \brief A process for loading vsols storages in binary format
// \author Ricardo Fabbri
// \date Fri Apr 28 2006
//
// \verbatim
//  Modifications
// \endverbatim



#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vcl_vector.h>

//: This process loads a .CON file into a vidpro1_vsol_storage class
class vidpro1_load_vsol_process : public bpro1_process
{
public:
  vidpro1_load_vsol_process();
  ~vidpro1_load_vsol_process() {}

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name() {
    return "Load vsol storage";
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

};

#endif // vidpro1_load_vsol_process_h_
