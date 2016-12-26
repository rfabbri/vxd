// This is basic/dbsol/pro/dbsol_scan_polygon_process.h
#ifndef dbsol_scan_polygon_process_h_
#define dbsol_scan_polygon_process_h_

//:
// \file
// \brief Process that scan converts polygons a label image
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 3/14/06
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>
#include <vcl_vector.h>
#include <vcl_string.h>

//: Process that scan converts polygons a label image
class dbsol_scan_polygon_process : public bpro1_process {

public:

  dbsol_scan_polygon_process();
  virtual ~dbsol_scan_polygon_process();

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
