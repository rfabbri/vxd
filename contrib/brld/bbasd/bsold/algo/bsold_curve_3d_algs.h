#ifndef bsold_curve_algs_3d_h_
#define bsold_curve_algs_3d_h_
//-----------------------------------------------------------------------------
//:
// \file
// \author Based on original code by  H. Can Aras
// \brief  bsold_interp_curve_3d algorithms
// \date   2006-12-04
//
// \verbatim

// \endverbatim
//
//-----------------------------------------------------------------------------
#include <bsold/bsold_interp_curve_3d.h>
#include <vsol/vsol_point_3d.h>
#include <vsol/vsol_polygon_3d_sptr.h>
#include <vgl/vgl_point_3d.h>

//: Interpolators and other algorithms constructing or acting on
//  bsold_intepr_curve_3d. They have not been made methods of the curve class so
//  that the curve class can be independent of them. For example, changing their
//  implementation or interface should not require a change of the curve class
//  declaration or a rebuild of the basic bsold library nor of software that use it.
//

class bsold_curve_3d_algs {
public:
  
  ~bsold_curve_3d_algs();

  //:Sample the curve and produce given number of samples
  static bool sample(bsold_interp_curve_3d const &c, int size,
                     vcl_vector<vsol_point_3d_sptr>& pts);

  static bool interpolate_linear_3d(bsold_interp_curve_3d *c,
                                    vcl_vector<vsol_point_3d_sptr> const &pts);

  //: If the last input parameter is an empty vector, simply-computed arclength
  //  (Euclidian distance between consecutive points) is used as the sampling 
  //  parameter for the ENO computations
  static bool interpolate_eno_3d(bsold_interp_curve_3d *c,
                                 vcl_vector<vsol_point_3d_sptr> const &pts,
                                 vcl_vector<double> &sample_pts);

 private:
  bsold_curve_3d_algs();
};


#endif  // bsold_curve_3d_algs_h
