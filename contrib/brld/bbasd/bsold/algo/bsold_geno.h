// This is bbasd/bsold/algo/bsold_geno.h
#ifndef bsold_geno_h
#define bsold_geno_h
//:
//\file
//\brief Compute class for GENO interpolation 
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 03/02/2005 11:55:39 AM EST
//

#include <bsold/bsold_geno_curve_2d.h>
#include <bgld/bgld_arc.h>

//: GENO interpolation
// GENO stands for Geometric Non-Oscillatory interpolation.
// See the file test_geno.cxx in the tests directory for an example usage.
// This is a compute class in the spirit of VNL, e.g. vnl_svd
class bsold_geno {
public:
  static void interpolate(bsold_geno_curve_2d *c, 
      vcl_vector<vsol_point_2d_sptr> const &pts, bool closed);
  static void interpolate3_approx(bsold_geno_curve_2d *c, 
      vcl_vector<vsol_point_2d_sptr> const &pts, bool closed);

  static void interpolate3_from_tangents(
      bsold_geno_curve_2d *c, 
      vcl_vector<vsol_point_2d_sptr> const &pts, 
      vcl_vector<double> const &tangent_angles,
      bool closed);


   /* TODO:
  //: version of GENO that accepts a vector stating which _samples_ are
  // corners/shocks (useful for e.g. re-doing the interpolation after some shock
  // detection)
  static bsold_interp_curve_2d_sptr 
      interpolate_geno(vcl_vector<vsol_point_2d_sptr> const &pts, bool closed);

  //: version of GENO that accepts a vector stating which _intervals_ contain
  // corners/shocks (useful for e.g. re-doing the interpolation after some shock
  // detection)

  static bsold_interp_curve_2d_sptr 
      interpolate_geno(vcl_vector<vsol_point_2d_sptr> const &pts, bool closed);
  */

  //: 2nd order GENO interpolation of an interval from four points.
  // The central interval is the one to be interpolated.
  static bgld_arc * interpolate_interval(
         vgl_point_2d<double> const & pts0, vgl_point_2d<double> const & pts1, 
         vgl_point_2d<double> const & pts2, vgl_point_2d<double> const & pts3, 
         bool *up);

};


// Implementation of Inlines: ----------------------------------------------

//: 2nd order GENO interpolation of an interval from four points.
// The central interval is the one to be interpolated.
// \param[out] lower true if the interpolant uses first three pts
inline bgld_arc * bsold_geno::
interpolate_interval(
      vgl_point_2d<double> const &  pts0, vgl_point_2d<double> const &  pts1, 
      vgl_point_2d<double> const &  pts2, vgl_point_2d<double> const &  pts3, bool *up)
{
   bgld_arc lower(pts1, pts2, pts0);
   bgld_arc upper(pts1, pts2, pts3);

   if (lower.curvature() < upper.curvature()) {
      *up=false;
      return new bgld_arc(lower);
   } else {
      *up=true;
      return new bgld_arc(upper);
   }
}

#endif // bsold_geno_h
