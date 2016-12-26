// This is basic/dbsol/pro/dbsol_roi_image_process.h

#ifndef dbsol_roi_image_process_h_
#define dbsol_roi_image_process_h_

//:
// \file
// \brief Process that takes in a list of vsol regions objects
// (polygons and ellipse) and turn into a binary image
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date 12/07/2005
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>


//: Process that takes in a list of vsol regions objects
// (polygons and ellipse) and turn into a binary image
class dbsol_roi_image_process : public bpro1_process 
{

public:

  dbsol_roi_image_process();
  virtual ~dbsol_roi_image_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  int input_frames();
  int output_frames();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();

protected:

};

#endif //dbsol_roi_image_process_h_
