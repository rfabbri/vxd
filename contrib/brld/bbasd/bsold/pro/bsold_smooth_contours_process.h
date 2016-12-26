// This is basic/dbsol/pro/dbsol_smooth_contours_process.h

#ifndef dbsol_smooth_contours_process_h_
#define dbsol_smooth_contours_process_h_

//:
// \file
// \brief A process to smooth contours
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date 11/16/2006
//
// \verbatim
//  Modifications
//    2/5/2007  Ricardo Fabbri  Added a discrete curve smoothing scheme
// \endverbatim

#include <bpro1/bpro1_process.h>


//: Process to smooth contours
// User can choose Gaussian or Discrete Curvature Smoothing.
//
// For discrete curvature smoothing, psi is the stepsize. For strong smoothings, the user should 
// set it to 1, and just change the number of times the smoothing is performed. If the stepsize is
// greater than 1, there is no guarantee that the resulting curve will be nice. Otherwise, the curve
// will always simplify; one can even enter an entangled curve and the smoothing will disentangle
// it after a sufficient number of times.
//
class dbsol_smooth_contours_process : public bpro1_process 
{

public:

  dbsol_smooth_contours_process(void);
  virtual ~dbsol_smooth_contours_process();

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

#endif //dbsol_smooth_contours_process_h_
