// This is basic/dbsol/pro/dbsol_fit_circ_arc_spline_process.h

#ifndef dbsol_fit_circ_arc_spline_process_h_
#define dbsol_fit_circ_arc_spline_process_h_

//:
// \file
// \brief A process to fit a polyline or polygon with an arc spline
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date Sep 8, 2008
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>


//: Process to fit a polyline or polygon with a circular arc spline
class dbsol_fit_circ_arc_spline_process : public bpro1_process 
{
public:
  dbsol_fit_circ_arc_spline_process(void);
  virtual ~dbsol_fit_circ_arc_spline_process();

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

#endif //dbsol_fit_circ_arc_spline_process_h_
