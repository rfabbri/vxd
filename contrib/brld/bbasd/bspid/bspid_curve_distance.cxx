#include "dbspi_curve_distance.h"
#include <vsol/vsol_point_2d.h>

unsigned dbspi_curve_distance::
num_inliers_dt(
    const vsol_digital_curve_2d &c, double d_threshold,
    const vil_image_view<vxl_uint_32> &dt,
    const vil_image_view<vxl_uint_32> &label) 
{
  vcl_set<unsigned> inliers;

  return inliers_dt(c, d_threshold, dt, label, &inliers);
}

unsigned dbspi_curve_distance::
inliers_dt(
    const vsol_digital_curve_2d &c, double d_threshold,
    const vil_image_view<vxl_uint_32> &dt,
    const vil_image_view<vxl_uint_32> &label,
    vcl_set<unsigned> *inliers
    ) 
{
  for (unsigned i=0; i < c.size();  ++i) {
    unsigned p_i = static_cast<unsigned>(c.point(i)->x()+0.5);
    unsigned p_j = static_cast<unsigned>(c.point(i)->y()+0.5);

    if (dt(p_i, p_j) < d_threshold)
      inliers->insert(label(p_i, p_j));
  }
  return inliers->size();
}

unsigned long dbspi_curve_distance::
sum_dt(
    const vsol_digital_curve_2d &c,
    const vil_image_view<vxl_uint_32> &dt
    )
{
  unsigned d=0;
  for (unsigned i=0; i < c.size();  ++i) {
    unsigned p_i = static_cast<unsigned>(c.point(i)->x()+0.5);
    unsigned p_j = static_cast<unsigned>(c.point(i)->y()+0.5);

    d += dt(p_i, p_j);
  }
  return d;
}
