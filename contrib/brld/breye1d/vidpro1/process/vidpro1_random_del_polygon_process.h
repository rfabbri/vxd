// This is vidpro1/process/vidpro1_random_del_polygon_process_h_
#ifndef vidpro1_random_del_polygon_process_h_
#define vidpro1_random_del_polygon_process_h_

//:
// \file
// \brief A process for removing random segments from a polygon
//        first load a polygon from a .CON file
//
// \author Ozge Can Ozcanli
// \date 11/20/07
//
//
// \verbatim
//  Modifications
//
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <vcl_vector.h>

//: This class saves the geometry from the current frame into a .CON file.
// It only saves vsol2D_curve types and ignores the rest
class vidpro1_random_del_polygon_process : public bpro1_process
{
public:
  vidpro1_random_del_polygon_process();
  ~vidpro1_random_del_polygon_process() {}
  
  vcl_string name() {
    return "Create random segments";
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

protected:
  bool first_frame_;
};

#endif // vidpro1_random_del_polygon_process_h_
