// This is basic/bild/pro/bild_distance_transform_process.h
#ifndef bild_distance_transform_process_h_
#define bild_distance_transform_process_h_

//:
// \file
// \brief A process to compute the distance transform for a binary image
// \author Based on original code by  Vishal Jain
// \date 09/06/05
//
// \verbatim
//  Modifications
//  04/28/2009  Ricardo Fabbri: added more algorithms
// \endverbatim


#include <vcl_vector.h>
#include <vcl_string.h>
#include <bpro1/bpro1_process.h>


//: Computes the distance transform of an input image.
//
// \todo square the output of CEDT and don't convert it to byte anymore.
// \todo label output for CEDT
class bild_distance_transform_process : public bpro1_process
{
public:
  //: Constructor
  bild_distance_transform_process();
  //: Destructor
  virtual ~bild_distance_transform_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  //: Returns the name of this process
  vcl_string name() { return "Distance Transform"; }

  int input_frames();
  int output_frames();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  //: Execute this process
  bool execute();
  bool finish();
};

#endif // bild_distance_transform_process_h_
