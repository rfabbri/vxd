// This is vidpro1/process/vidpro1_load_cem_process_h_
#ifndef vidpro1_load_cem_process_h_
#define vidpro1_load_cem_process_h_

//:
// \file
// \brief A process for loading an .CEM contour files into a frame
// \author Amir Tamrakar
// \date 06/06/04
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vcl_vector.h>

//: This process loads a .CEM file and produces vsol2D_storage class
// \remarks The most recent and well-maintained version of the .CEM loader is in
// dbdet/pro/dbdet_load_cem_process.h
//
// Use this version *only* if you don't want to link to dbdet.
class vidpro1_load_cem_process : public bpro1_process
{
public:
  vidpro1_load_cem_process();
  ~vidpro1_load_cem_process() {}

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name() {
    return "Load .CEM File (vsol)";
  }
  
  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();
  
  int input_frames() {
    return 1;
  }
  int output_frames() {
    return num_frames_;
  }
  
  bool execute();
  bool finish() {
    return true;
  }

protected:
  int num_frames_;
};

#endif // vidpro1_load_cem_process_h_
