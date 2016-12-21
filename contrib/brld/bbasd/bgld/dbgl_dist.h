// This is basic/dbgl/dbgl_dist.h

#ifndef dbgl_dist_h_
#define dbgl_dist_h_

#include <vcl_cmath.h>
#include <vnl/vnl_math.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_box_3d.h>

//:
// \file
// \brief Basic distance and squared distance computation.
//
// \author 
//    Ming-Ching Chang Mar 07, 2007    
//
// \verbatim  
//  Modifications:
// \endverbatim

inline double dbgl_dist_3d (const double& x1, const double& y1, const double& z1,
                                  const double& x2, const double& y2, const double& z2)
{
   return vcl_sqrt ( ((double) x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1) );
}

inline double dbgl_sqdist_3d (const double& x1, const double& y1, const double& z1,
                                    const double& x2, const double& y2, const double& z2)
{
   return ((double) x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1);
}

inline double dbgl_sqdist_3d (const vgl_point_3d<double>& pt1, 
                                    const vgl_point_3d<double>& pt2)
{
   return dbgl_sqdist_3d (pt1.x(), pt1.y(), pt1.z(),
                          pt2.x(), pt2.y(), pt2.z());
}

vgl_box_3d<double> dbgl_reduce_box (const vgl_box_3d<double>& box, const double red);
vgl_box_3d<double> dbgl_extend_box (const vgl_box_3d<double>& box, const double ext);
vgl_box_3d<double> dbgl_extend_box_by_factor (const vgl_box_3d<double>& box, const float factor);

//: Computes the smallest distance between two angles representing straight
// lines through the origin.
//
// \param[in] t1,t2: angles \in [0 2pi)
// \param[out]: the smallest angle \in [0,pi/2) between the underlying straight
// lines.
//
inline double
dbgl_undirected_angle_distance(double t1, double t2)
{
  assert (t1 < 2*vnl_math::pi && t2 < 2*vnl_math::pi);
  if (t1 > vnl_math::pi)
    t1 -= vnl_math::pi;

  if (t2 > vnl_math::pi)
    t2 -= vnl_math::pi;

  double dt = fabs (t1 - t2);
  return (dt > vnl_math::pi_over_2)? vnl_math::pi-dt : dt;
}

#endif
