// This is vidpro1/process/vidpro1_save_con_process_h_
#ifndef vidpro1_save_con_process_h_
#define vidpro1_save_con_process_h_

//:
// \file
// \brief A process for saving geometry as .CON contour files from the current frame
//        Exactly same as save_cem process
// \author Ozge Can Ozcanli
// \date 07/07/04
//
// Note: It only saves contour type geometry, i.e., vsol2D_curve type and ignores the others
//
// \verbatim
//  Modifications
//  Amir Tamrakar 02/22/2005   This process will now save both
//                             vsol_polyline_2d object and vsol_polygon_2d
//                             objects and specify the [OPEN/CLOSE] attribute accordingly
//                             on the file header.
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vcl_vector.h>

//: This class saves the geometry from the current frame into a .CON file.
// It only saves vsol2D_curve types and ignores the rest
class vidpro1_save_con_process : public bpro1_process
{
public:
  vidpro1_save_con_process();
  ~vidpro1_save_con_process() {}
  
  vcl_string name() {
    return "Save .CON File";
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
  bool finish();

  bool saveCON (vcl_string con_file_rootname, 
                bool output_poly_file, int video_id, 
                int num_frames, vcl_string poly_filename);

protected:
  bool first_frame_;
};

#endif // vidpro1_save_con_process_h_
