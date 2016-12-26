// This is bbasd/bsold/pro/bsold_scan_polygon_process.h
#ifndef bsold_scan_polygon_process_h_
#define bsold_scan_polygon_process_h_

//:
// \file
// \brief Process that scan converts polygons a label image
// \author Based on original code by  Matt Leotta (@lems.brown.edu)
// \date 3/14/06
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>

//: Process that scan converts polygons a label image
class bsold_scan_polygon_process : public bpro1_process {

public:

  bsold_scan_polygon_process();
  virtual ~bsold_scan_polygon_process();

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
