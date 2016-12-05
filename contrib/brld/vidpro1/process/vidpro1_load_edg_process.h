// This is vidpro1/process/vidpro1_load_edg_process_h_

#ifndef vidpro1_load_edg_process_h_
#define vidpro1_load_edg_process_h_

//:
// \file
// \brief A process for loading a .EDG file into the current frame
// \author Amir Tamrakar
// \date 06/06/04
//
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vcl_vector.h>

//: This process loads a .EDG file into the current frame
// \remarks The most recent and well-maintained version of the .EDG loader is in
// dbdet/pro/dbdet_load_edg_process.h
//
// Use this version only if you don't want to link to dbdet.
class vidpro1_load_edg_process : public bpro1_process
{
public:
  vidpro1_load_edg_process();
  ~vidpro1_load_edg_process() {}

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name() {
    return "Load .EDG File";
  }
  
  vcl_vector< vcl_string > get_input_type() {
    vcl_vector< vcl_string > to_return;
    return to_return;
  }

  vcl_vector< vcl_string > get_output_type() {
    vcl_vector< vcl_string > to_return;
    to_return.push_back( "vsol2D" );
    return to_return;
  }
  
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

#endif // vidpro1_load_edg_process_h_
