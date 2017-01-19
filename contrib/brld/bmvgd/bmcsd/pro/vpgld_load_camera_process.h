// This is vidpro1/process/vpgld_load_camera_process_h_

#ifndef vpgld_load_camera_process_h_
#define vpgld_load_camera_process_h_

//:
//\file
//\brief Process to load a camera from Ascii files
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri@gmail.com)
//\date 05/05/2005 06:13:38 PM EDT
//

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vcl_vector.h>


//: This process loads a text file into camera matrices
// TODO: for now its just loading according to the .intrinsic/.extrinsic
// convention I did for myself.
// \seealso new methods in vpgl to do this.
class vpgld_load_camera_process : public bpro1_process
{
public:
  vpgld_load_camera_process();
  ~vpgld_load_camera_process() {}

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name() {
    return "Load camera file";
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

#endif // vpgld_load_camera_process_h_
