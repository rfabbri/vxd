// This is vidpro1/process/vidpro1_save_ps_process_h_
#ifndef vidpro1_save_ps_process_h_
#define vidpro1_save_ps_process_h_

//:
// \file
// \brief A process for saving geometry as a .PS image from the current frame
// \author Ozge Can Ozcanli
// \date 06/18/04
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

//: This class saves the geometry from the current frame into a .PS file.
// It only saves vsol2D_curve types and ignores the rest

class vidpro1_save_ps_process : public bpro1_process
{
public:
  vidpro1_save_ps_process();
  ~vidpro1_save_ps_process() {}

  //: Clone the process
  virtual bpro1_process* clone() const;
  
  vcl_string name() {
    return "Save .PS File";
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

  bool savePS (vcl_string filename);

protected:
};

#endif // vidpro1_save_ps_process_h_
