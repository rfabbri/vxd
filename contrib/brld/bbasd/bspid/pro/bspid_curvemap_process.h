// This is basic/bspid/pro/bspid_curvemap_process.h
#ifndef bspid_curvemap_process_h_
#define bspid_curvemap_process_h_

//:
// \file
// \brief Process that computes a distance map for curves
// \author Based on original code by  Matt Leotta (@lems.brown.edu)
// \date 7/19/04
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>

//: Derived video process class for computing a distance map
//  image for vsol_digital_curve_2d
class bspid_curvemap_process : public bpro1_process {

public:

  bspid_curvemap_process();
  virtual ~bspid_curvemap_process();

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name();

  int input_frames();
  int output_frames();

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  bool execute();
  bool finish();

};

#endif
