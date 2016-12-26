// This is bbasd/bsold/pro/bsold_roi_image_process.h

#ifndef bsold_roi_image_process_h_
#define bsold_roi_image_process_h_

//:
// \file
// \brief Process that takes in a list of vsol regions objects
// (polygons and ellipse) and turn into a binary image
// \author Based on original code by  Nhon Trinh (@lems.brown.edu)
// \date 12/07/2005
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>


//: Process that takes in a list of vsol regions objects
// (polygons and ellipse) and turn into a binary image
class bsold_roi_image_process : public bpro1_process 
{

public:

  bsold_roi_image_process();
  virtual ~bsold_roi_image_process();

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

#endif //bsold_roi_image_process_h_
