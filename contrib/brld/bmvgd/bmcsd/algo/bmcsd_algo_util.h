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
#include <sdet/sdet_frenet_keypoint.h>
#include <sdet/sel/sdet_edgel.h>
#include <bdifd/bdifd_camera.h>
#include <bdifd/bdifd_rig.h>

#include <bmcsd/bmcsd_util.h>
#include <mw/mw_subpixel_point_set.h>


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
  
  //: convert from sdet to bdifd (point with differential geometry attributes)
  inline static bdifd_3rd_order_point_2d
    mw_get_3rd_order_point_2d(const sdet_frenet_keypoint &kp);

  //: convert from bdifd to sdet (point with differential geometry attributes)
  inline static sdet_frenet_keypoint
    mw_get_frenet_keypoint(const bdifd_3rd_order_point_2d &fp);

  //: Determines proper bbox to be passed to becld_epiband, given that the bbox of the vsols (edgel
  // set) is pb, and that we have a subpixel point set sp
  static inline vsol_box_2d_sptr
    determine_right_bbox(const vsol_box_2d_sptr &/*poly_box*/, const mw_subpixel_point_set *sp) 
    {
      vsol_box_2d_sptr box = new vsol_box_2d();

      box->add_point(0.0,0.0);
      box->add_point(
          (double)sp->ncols()-1.0, 
          (double)sp->nrows()-1.0);

      // TODO: why the settings bellow give asymmetric tuplets?
      //  box->add_point(
      //      vcl_max(vcl_floor(pb->get_min_x())-5.0, 0.0), 
      //      vcl_max(vcl_floor(pb->get_min_y())-5.0, 0.0)
      //      );
      //  box->add_point(
      //      vcl_min(vcl_ceil(pb->get_max_x())+5.0, (double)sp->ncols()-1.0), 
      //      vcl_min(vcl_ceil(pb->get_max_y())+5.0, (double)sp->nrows()-1.0)
      //      );
      return box;
    }

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

inline bdifd_3rd_order_point_2d bmcsd_algo_util::
mw_get_3rd_order_point_2d(const sdet_frenet_keypoint &kp)
{
  bdifd_3rd_order_point_2d p;

  p.gama[0] = kp.x();
  p.gama[1] = kp.y();
  p.gama[2] = 0;
  p.t[0] = kp.tx_;
  p.t[1] = kp.ty_;
  p.t[2] = 0;
  p.n[0] = kp.nx_;
  p.n[1] = kp.ny_;
  p.n[2] = 0;
  p.valid = kp.valid_;
  p.k = kp.k_;
  p.kdot = kp.kdot_;

  return p;
}

inline sdet_frenet_keypoint bmcsd_algo_util::
mw_get_frenet_keypoint(const bdifd_3rd_order_point_2d &fp)
{
  sdet_frenet_keypoint kp;

  kp.set(fp.gama[0],fp.gama[1]);

  kp.tx_= fp.t[0];
  kp.ty_= fp.t[1];
  kp.nx_= fp.n[0];
  kp.ny_= fp.n[1];
  kp.k_=  fp.k;
  kp.kdot_= fp.kdot;
  kp.valid_= fp.valid;

  return kp;
}

#endif // bmcsd_algo_util_h
