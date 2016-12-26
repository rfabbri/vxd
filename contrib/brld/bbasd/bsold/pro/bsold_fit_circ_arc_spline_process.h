// This is bbasd/bsold/pro/bsold_fit_circ_arc_spline_process.h

#ifndef bsold_fit_circ_arc_spline_process_h_
#define bsold_fit_circ_arc_spline_process_h_

//:
// \file
// \brief A process to fit a polyline or polygon with an arc spline
// \author Based on original code by  Nhon Trinh (@lems.brown.edu)
// \date Sep 8, 2008
//
// \verbatim
//  Modifications
// \endverbatim

#include <bpro1/bpro1_process.h>


//: Process to fit a polyline or polygon with a circular arc spline
class bsold_fit_circ_arc_spline_process : public bpro1_process 
{
public:
  bsold_fit_circ_arc_spline_process(void);
  virtual ~bsold_fit_circ_arc_spline_process();

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

#endif //bsold_fit_circ_arc_spline_process_h_
