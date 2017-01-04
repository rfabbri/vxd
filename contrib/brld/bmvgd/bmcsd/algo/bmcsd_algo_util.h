// This is bmcsd_algo_util.h
#ifndef bmcsd_algo_util_h
#define bmcsd_algo_util_h
//:
//\file
//\brief Misc. Utilities for photogrammetry library
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 01/15/07 14:00:30 EST
//

#include <vcl_vector.h>
#include <vgl/vgl_fwd.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vsol/vsol_box_2d.h>
#include <sdet/sdet_edgel.h>
#include <bdifd/bdifd_camera.h>
#include <bdifd/bdifd_rig.h>

#include <bmcsd/bmcsd_util.h>


//: Some utility functions for mw_algo
class bmcsd_algo_util {
public:
  //:  Move world coordinates to first camera, updating each camera matrix accordingly
  // \todo test this separately.
  static void
  move_world_to_1st_cam(vcl_vector<vpgl_perspective_camera<double> *> &cam);

  static void
  move_world_to_1st_cam(vcl_vector<vpgl_perspective_camera<double> *> &cam,
      vcl_vector<vgl_point_3d<double> > &pts);

  //: Computes an image-based reprojection error of points attributed with differential geometry, for
  // all pairs of views reprojected into the view \p v. 
  //
  // \param[out] n is how many reprojections were valid on view \p v
  // Caller MUST initialize dpos,dtheta,...,n (usually to ZERO).
  // \return true in case at least one reprojection into current view has non-degenerate geometry
  static bool
  dg_reprojection_error(
      vcl_vector<bdifd_3rd_order_point_2d> &pts, //:< pts[iv] points in view iv
      const vcl_vector<bdifd_camera> &cam,
      unsigned v,
      double &dpos,
      double &dtheta,
      double &dnormal_plus,
      double &dnormal_minus,
      double &dtangential_plus,
      double &dtangential_minus,
      double &dk,
      double &dkdot,
      unsigned &n
      );

  //: Computes an image-based reprojection error of points attributed with differential geometry. 
  // for each reference view v in the set of all views, do
  //   for all pairs of views 
  //      reproject into view v, and accumulate the errors
  //
  // \param[out] n number of valid points that were valid for reprojecting DG; in other words, the
  // number of points used to compute the d's
  //
  // \return false if all points lead to degenerate reprojection / singular geometry
  static bool
  dg_reprojection_error(
      vcl_vector<bdifd_3rd_order_point_2d> &pts, //:< pts[iv] points in view iv
      const vcl_vector<bdifd_camera> &cam,
      // no parameter v - do it for all v.
      double &dpos,
      double &dtheta,
      double &dnormal_plus,
      double &dnormal_minus,
      double &dtangential_plus,
      double &dtangential_minus,
      double &dk,
      double &dkdot,
      unsigned &n
      );

  //: Given 1st order bdifd point-tangents in image coordinates, 
  // returns the sdet_edgel with the correct position and orientation.
  static void bdifd_to_sdet(
      const bdifd_1st_order_curve_2d &crv_from, 
      vcl_vector<sdet_edgel> *crv_to
      )
  {
    crv_to->reserve(crv_from.size());
    for (unsigned i=0; i < crv_from.size(); ++i) {
      sdet_edgel edg;
      bdifd_to_sdet(crv_from[i], &edg);
      crv_to->push_back(edg);
    }
  }

  static void bdifd_to_sdet(const bdifd_1st_order_point_2d &from, sdet_edgel *to) 
  {
    to->pt.set(from.gama[0], from.gama[1]);
    to->tangent = bmcsd_util::angle0To2Pi(vcl_atan2(from.t[1], from.t[0]));
  }

  static void sdet_to_vsol(
      const sdet_edgel_list &edgels, 
      vcl_vector<vsol_point_2d_sptr> *pcurve) 
  {
    pcurve->reserve(edgels.size());
    for (unsigned j=0; j < edgels.size(); j++)
      pcurve->push_back(new vsol_point_2d(edgels[j]->pt));
  }

  static void extract_edgel_chain(const vsol_polyline_2d &pts, sdet_edgel_chain *ec);
};

#endif // bmcsd_algo_util_h
