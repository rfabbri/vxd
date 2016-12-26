// This is basic/dbsol/algo/dbsol_geno.h
#ifndef dbsol_geno_h
#define dbsol_geno_h
//:
//\file
//\brief Compute class for GENO interpolation 
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 03/02/2005 11:55:39 AM EST
//

#include <dbsol/dbsol_geno_curve_2d.h>
#include <dbgl/dbgl_arc.h>

//: GENO interpolation
// GENO stands for Geometric Non-Oscillatory interpolation.
// See the file test_geno.cxx in the tests directory for an example usage.
// This is a compute class in the spirit of VNL, e.g. vnl_svd
class dbsol_geno {
public:
  static void interpolate(dbsol_geno_curve_2d *c, 
      vcl_vector<vsol_point_2d_sptr> const &pts, bool closed);
  static void interpolate3_approx(dbsol_geno_curve_2d *c, 
      vcl_vector<vsol_point_2d_sptr> const &pts, bool closed);

  static void interpolate3_from_tangents(
      dbsol_geno_curve_2d *c, 
      vcl_vector<vsol_point_2d_sptr> const &pts, 
      vcl_vector<double> const &tangent_angles,
      bool closed);


   /* TODO:
  //: version of GENO that accepts a vector stating which _samples_ are
  // corners/shocks (useful for e.g. re-doing the interpolation after some shock
  // detection)
  static dbsol_interp_curve_2d_sptr 
      interpolate_geno(vcl_vector<vsol_point_2d_sptr> const &pts, bool closed);

  //: version of GENO that accepts a vector stating which _intervals_ contain
  // corners/shocks (useful for e.g. re-doing the interpolation after some shock
  // detection)

  static dbsol_interp_curve_2d_sptr 
      interpolate_geno(vcl_vector<vsol_point_2d_sptr> const &pts, bool closed);
  */

  //: 2nd order GENO interpolation of an interval from four points.
  // The central interval is the one to be interpolated.
  static dbgl_arc * interpolate_interval(
         vgl_point_2d<double> const & pts0, vgl_point_2d<double> const & pts1, 
         vgl_point_2d<double> const & pts2, vgl_point_2d<double> const & pts3, 
         bool *up);

};


// Implementation of Inlines: ----------------------------------------------

//: 2nd order GENO interpolation of an interval from four points.
// The central interval is the one to be interpolated.
// \param[out] lower true if the interpolant uses first three pts
inline dbgl_arc * dbsol_geno::
interpolate_interval(
      vgl_point_2d<double> const &  pts0, vgl_point_2d<double> const &  pts1, 
      vgl_point_2d<double> const &  pts2, vgl_point_2d<double> const &  pts3, bool *up)
{
   dbgl_arc lower(pts1, pts2, pts0);
   dbgl_arc upper(pts1, pts2, pts3);

   if (lower.curvature() < upper.curvature()) {
      *up=false;
      return new dbgl_arc(lower);
   } else {
      *up=true;
      return new dbgl_arc(upper);
   }
}

#endif // dbsol_geno_h
