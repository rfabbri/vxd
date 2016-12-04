#include "dbdif_transfer.h"


//: Given two point-tangents with a tangential error band, compute a point-tangent and tangential
//error band in the third view with an error band given by the first two.
//
// param[in] t_err : error in t0 and t1, in [0,pi/2) range.
//
// The input tangents are *not* assumed to be oriented (i.e., they are up to a sign).
//
// param[out] theta_min_reproj and theta_max_reproj: sector of tolerance represented by theta_min
// and theta_max, counter-clockwise, with theta_min in [0,vnl_math::pi), and theta_max in
// [theta_min,theta_min+vnl_math::pi].
//
// \return false if there were any degeneracies detected in the computation.
//
// \seealso my notes from May 15 2008 B
bool dbdif_transfer::
transfer_tangent_band(
    const dbdif_1st_order_point_2d &p0, 
    const dbdif_1st_order_point_2d &p1, 
    double t_err,
    double *theta_min_reproj,
    double *theta_max_reproj,
    const dbdif_camera &cam, 
    const dbdif_rig &rig)
{

  // determine 4 possible combinations in 3D

  double c = vcl_cos(t_err);
  double s = vcl_sin(t_err);
  
  dbdif_vector_3d t0_min (c*p0.t[0] + s*p0.t[1], -s*p0.t[0] + c*p0.t[1], 0);
  dbdif_vector_3d t0_max (c*p0.t[0] - s*p0.t[1], s*p0.t[0] + c*p0.t[1], 0);

  dbdif_vector_3d t1_min (c*p1.t[0] + s*p1.t[1], -s*p1.t[0] + c*p1.t[1], 0);
  dbdif_vector_3d t1_max (c*p1.t[0] - s*p1.t[1], s*p1.t[0] + c*p1.t[1], 0);

  dbdif_3rd_order_point_2d p_t_0_min = p0; 
  p_t_0_min.t = t0_min;

  dbdif_3rd_order_point_2d p_t_1_min = p1; 
  p_t_1_min.t = t1_min;

  dbdif_3rd_order_point_2d p_t_0_max = p0; 
  p_t_0_max.t = t0_max;

  dbdif_3rd_order_point_2d p_t_1_max = p1; 
  p_t_1_max.t = t1_max;

  dbdif_3rd_order_point_2d p0_mw3 = p0; 
  dbdif_3rd_order_point_2d p1_mw3 = p1; 


  dbdif_3rd_order_point_3d Prec;

  dbdif_3rd_order_point_2d 
    p_reproj_mm,
    p_reproj_mp,
    p_reproj_pm,
    p_reproj_pp,
    p_reproj_mid;


  bool valid = transfer_by_reconstruct_and_reproject (p_t_0_min, p_t_1_min, p_reproj_mm, Prec, cam, rig);
  valid = valid && transfer_by_reconstruct_and_reproject (p_t_0_min, p_t_1_max, p_reproj_mp, Prec, cam, rig);
  valid = valid && transfer_by_reconstruct_and_reproject (p_t_0_max, p_t_1_min, p_reproj_pm, Prec, cam, rig);
  valid = valid && transfer_by_reconstruct_and_reproject (p_t_0_max, p_t_1_max, p_reproj_pp, Prec, cam, rig);

  valid = valid && transfer_by_reconstruct_and_reproject (p0_mw3, p1_mw3, p_reproj_mid, Prec, cam, rig);

 
  //  xxx
//  assert((p_reproj_mp.gama -p_reproj_mm.gama).two_norm() < 1e-4 
//      && (p_reproj_pm.gama -p_reproj_mm.gama).two_norm() < 1e-4
//      && (p_reproj_pp.gama -p_reproj_mm.gama).two_norm() < 1e-4 );

  // - After the above process, we end up with set of correspondents; get the hull

  dbdif_vector_3d tmid(p_reproj_mid.t), tmid_perp(-tmid[1],tmid[0],0);

  // orient to match tmid direction
  if ( dot_product(p_reproj_mm.t,tmid) < 0)
    p_reproj_mm.t *= -1;

  if ( dot_product(p_reproj_mp.t,tmid) < 0)
    p_reproj_mp.t *= -1;

  if ( dot_product(p_reproj_pp.t,tmid) < 0)
    p_reproj_pp.t *= -1;

  if ( dot_product(p_reproj_pm.t,tmid) < 0)
    p_reproj_pm.t *= -1;
   

  double smin,smax;
  smin = smax = dot_product(p_reproj_mm.t,tmid_perp);
  dbdif_vector_3d *tmin, *tmax;

  tmin = tmax = &(p_reproj_mm.t);

  double ss = dot_product (p_reproj_mp.t,tmid_perp);
  if ( ss < smin) {
    smin = ss;
    tmin = &(p_reproj_mp.t);
  } else if ( ss > smax) {
    smax = ss;
    tmax = &(p_reproj_mp.t);
  }

  ss = dot_product (p_reproj_pm.t,tmid_perp);
  if ( ss < smin) {
    smin = ss;
    tmin = &(p_reproj_pm.t);
  } else if ( ss > smax) {
    smax = ss;
    tmax = &(p_reproj_pm.t);
  }

  ss = dot_product (p_reproj_pp.t,tmid_perp);
  if ( ss < smin) {
    smin = ss;
    tmin = &(p_reproj_pp.t);
  } else if ( ss > smax) {
    smax = ss;
    tmax = &(p_reproj_pp.t);
  }

  //: weird - in very special cases, all perturbed reprojections don't include the mid
  // reprojection. In this case, give a little leeway around tmid and use the
  // max of the remaining reprojection as the other extreme for the band.
  if (smin*smax > 0) {
      if (smin < 0) {
        tmax = &(tmid);
        tmid = dbdif_vector_3d (c*tmid[0] - s*tmid[1], s*tmid[0] + c*tmid[1], 0);
      } else {
        tmin = &(tmid);
        tmid = dbdif_vector_3d(c*tmid[0] + s*tmid[1], -s*tmid[0] + c*tmid[1], 0);
      }
  }


  *theta_min_reproj = atan((*tmin)[1]/(*tmin)[0]);
  *theta_max_reproj = atan((*tmax)[1]/(*tmax)[0]);

  if (*theta_max_reproj < *theta_min_reproj)
    *theta_max_reproj += vnl_math::pi;

  if (*theta_min_reproj < 0) {
    *theta_min_reproj += vnl_math::pi;
    *theta_max_reproj += vnl_math::pi;
  }

  return valid;
}

#if 0
//: Intersects two orientation ranges rooted at a common point.
//
// \param[in] tx_a, ty_a : coordinates of unit tangent direction in the center of a band of width
// 2*t_err_a.
//
// \param[in] theta_min_b and theta_max_b - orientation range specified by counter-clockwise angles.
// Represents similar info to tangent + angular error representation, but we use this form for
// convenience when used in multiview transfer algorithms
//
// such that theta_min_b in [0,pi)  range and theta_max_b in [theta_min,theta_min+pi)  range.
//
bool dbdif_transfer::
intersect_tangent_band(
    double tx_a, double ty_a, double t_err_a, 
    double theta_min_b, double theta_max_b, 
    double *theta_min_comb, double *theta_max_comb)
{

  double theta_mid = atan(ty_a/tx_a);

  theta_min_a = theta_mid - t_err_a;
  theta_max_a = theta_mid + t_err_a;

  // put within conventioned range
  if (theta_min < 0) {
    theta_min += vnl_math::pi;
    theta_max += vnl_math::pi;
  }

  if (theta_max_a < vnl_math::pi) {
    if (theta_max_b < vnl_math::pi) {
      // case 1 : both normal range

      if (theta_min_a < theta_min_b) {
        *theta_min_comb = theta_min_b;
      } else {
        *theta_min_comb = theta_min_a;
      }

      if (theta_max_a > theta_max_b) {
        *theta_max_comb = theta_max_b;
      } else {
        *theta_max_comb = theta_max_a;
      }

      return theta_max_comb >= theta_min_comb;
    } else {
      // case 2 : sector 1 normal range and sector 2 crossing vnl_math::pi

      // invert sector 2

      theta_max_b -= vnl_math::pi;
      double tmp  = theta_min_b;
      theta_min_b = theta_max_b;
      theta_max_b = tmp;

      // intersect sector 1 and inverse of sector 2

      if (theta_min_a < theta_min_b) {
        *theta_min_comb = theta_min_b;
      } else {
        *theta_min_comb = theta_min_a;
      }

      if (theta_max_a > theta_max_b) {
        *theta_max_comb = theta_max_b;
      } else {
        *theta_max_comb = theta_max_a;
      }

      // Now subtract from s_1

    }
  } else {
    if (theta_max_b < vnl_math::pi) {
      // case 3 : sector 2 normal range and sector 1 crossing vnl_math::pi
    } else {
      // case 4 : sector 2 and sector 1 crossing vnl_math::pi
    }
  }
}
#endif

