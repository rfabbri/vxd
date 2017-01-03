// This is bcsid_curve_distance.h
#ifndef bcsid_curve_distance_h
#define bcsid_curve_distance_h
//:
//\file
//\brief Functions to integrate distance/grouping costs along a curve
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 05/06/2009 02:13:37 PM EDT
//

#include <vsol/vsol_digital_curve_2d.h>
#include <vil/vil_image_view.h>
#include <vcl_vector.h>
#include <dbdet/sel/dbdet_curvelet.h>
#include <dbdet/sel/dbdet_curvelet_map.h>
#include <vcl_set.h>

class dbdet_edgel_chain;
class dbdet_edgemap;

typedef vcl_vector<dbdet_edgel> bcsid_edgel_seq;
typedef vcl_vector<dbdet_edgel>::iterator bcsid_edgel_seq_iter;
typedef vcl_vector<dbdet_edgel>::const_iterator bcsid_edgel_seq_const_iter;

class bcsid_curve_distance {
public:
  //: \return the number of distinct edgels points within a distance threshold
  // from the input curve, that also pass an orientation distance threshold.  We
  // assume c is such that a point (x,y) has dt(x,y) and label(x,y). All curve
  // samples are assumed to lie inside the image.
  //
  // \param[in] dt_threshold: squared distance threshold
  static unsigned inliers_dt_oriented(
      const bcsid_edgel_seq &c, double d_threshold, double dtheta_threshold,
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      const dbdet_edgemap &em,
      vcl_set<unsigned> *inliers
    );

  //: Convenience function that does not return the inlier set.
  static unsigned num_inliers_dt_oriented(
      const bcsid_edgel_seq &c, double d_threshold, double dtheta_threshold,
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      const dbdet_edgemap &em
    );

  //: \return the number of edgels per sample within a distance threshold
  // from the input curve, that also pass an orientation distance threshold.  We
  // assume c is such that a point (x,y) has dt(x,y) and label(x,y). All curve
  // samples are assumed to lie inside the image.
  //
  // This version accumulates counts for each curve sample if there is a closest
  // edgel passing the thresholds. If so, this edge is counted once. For a
  // different curve sample the same edgel might be counted twice here, because
  // a list is used. In the version withou the suffix all_samples, the same
  // edgel will not be counted twice due to the use of a set.
  //
  // \param[in] dt_threshold: squared distance threshold
  static unsigned inliers_dt_oriented_all_samples(
      const bcsid_edgel_seq &c, double d_threshold, double dtheta_threshold,
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      const dbdet_edgemap &em,
      vcl_list<unsigned> *inliers
    );

  //: Convenience function that does not return the inlier set.
  static unsigned num_inliers_dt_oriented_all_samples(
      const bcsid_edgel_seq &c, double d_threshold, double dtheta_threshold,
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      const dbdet_edgemap &em
    );

  //: \return the number of distinct curvelets all of whose edgels are within a
  // distance threshold from the input curve. We assume c is such that a point
  // (x,y) has dt(x,y) and label(x,y). All curve samples are assumed to lie
  // inside the image. The dt and label are computed distance and
  // closest label maps on the underlying edge map of the curvelet map
  //
  // \param[in] dt_threshold: squared distance threshold
  static unsigned inlier_curvelets_dt(
      const vsol_digital_curve_2d &crv, double d_threshold, 
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      const dbdet_curvelet_map &cm,
      curvelet_list *inlier_curvelets
      );

  //: \return the number of distinct curvelets all of whose edgels are within a
  // distance threshold from the input curve. We assume c is such that a point
  // (x,y) has dt(x,y) and label(x,y). All curve samples are assumed to lie
  // inside the image. The dt and label are computed distance and
  // closest label maps on the underlying edge map of the curvelet map
  //
  // \param[in] dt_threshold: squared distance threshold
  static unsigned inlier_curvelets_dt_oriented(
      const bcsid_edgel_seq &c, double d_threshold, double dtheta_threshold, 
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      const dbdet_curvelet_map &cm,
      unsigned min_num_inlier_edgels,
      curvelet_list *inlier_curvelets
      );

  static unsigned num_inlier_curvelets_dt_oriented(
      const bcsid_edgel_seq &c, double d_threshold, double dtheta_threshold, 
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      const dbdet_curvelet_map &cm,
      unsigned min_num_inlier_edgels
      )
  {
    curvelet_list inlier_curvelets;
    return inlier_curvelets_dt_oriented(
        c, d_threshold, dtheta_threshold, dt, label, cm, min_num_inlier_edgels,
        &inlier_curvelets);
  }
  
  /*
  //: Convenience variant
  static unsigned inlier_curvelets_dt_oriented(
      const vsol_digital_curve_2d &crv, double d_threshold, 
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      const dbdet_curvelet_map &cm,
      curvelet_list *inlier_curvelets
      )
  {
     return inlier_curvelets_dt_oriented(
        crv, d_threshold, dt, label, cm, 0, inlier_curvelets);
  }

  //: Convenience variant without theta threshold
  static unsigned inlier_curvelets_dt_oriented(
      const vsol_digital_curve_2d &crv, double d_threshold, 
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      const dbdet_curvelet_map &cm,
      unsigned min_num_inlier_edgels,
      curvelet_list *inlier_curvelets
      )
  {
     return inlier_curvelets_dt_oriented(
        crv, d_threshold, vnl_math::pi*0.5,
        dt, label, cm, min_num_inlier_edgels, inlier_curvelets);
  }
  */
};

#endif // bcsid_curve_distance_h
