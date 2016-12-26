// This is bspid_curve_distance.h
#ifndef bspid_curve_distance_h
#define bspid_curve_distance_h
//:
//\file
//\brief Functions to integrate distance costs along a curve
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 04/28/2009 03:56:36 PM EDT
//

#include <vsol/vsol_digital_curve_2d.h>
#include <vil/vil_image_view.h>
#include <vcl_set.h>

class bspid_curve_distance {
public:
  //: returns the number of distinct feature points within a distance threshold
  // from the input curve.  We assume c is such that a point (x,y) has dt(x,y)
  // and label(x,y). All curve samples are assumed to lie inside the image.
  //
  // \param[in] dt_threshold: squared distance threshold
  static unsigned num_inliers_dt(
      const vsol_digital_curve_2d &c, double d_threshold,
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label
      );

  //: This variant returns the inlier set.
  // \return same as num_inliers_dt
  static unsigned inliers_dt(
      const vsol_digital_curve_2d &c, double d_threshold,
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<vxl_uint_32> &label,
      vcl_set<unsigned> *inliers
      );

  //: Sums up the squared distances at each sample of the curve.
  // We assume c is such that a point (x,y) has dt(x,y). The user might want to divide by the length
  // of the curve.
  static unsigned long
  sum_dt(
      const vsol_digital_curve_2d &c,
      const vil_image_view<vxl_uint_32> &dt);
};

#endif // bspid_curve_distance_h
