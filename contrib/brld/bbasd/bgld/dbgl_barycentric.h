// This is basic/dbgl/dbgl_barycentric.h
#ifndef dbgl_barycentric_h_
#define dbgl_barycentric_h_
//:
// \file
// \brief  Some 3D barycentric functions.
// \author Ming-Ching Chang (mcchang@lems.brown.edu)
// \date   Jan/03/2007
//
// \verbatim
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vcl_cmath.h>
#include <vgl/vgl_point_3d.h>

//: Compute the 16_delta^2 from given distance d.
inline double _delta2_16_d (const double& d12, const double& d23, const double& d13)
{
  return (d12+d23+d13)*(d23+d13-d12)*(d12+d13-d23)*(d12+d23-d13);
}

//: Compute the 16_delta^2 from given square distance u.
inline double _delta2_16_u (const double& u12, const double& u23, const double& u13)
{
  return vcl_fabs (u12 * (u12-u23-u13) + u13 * (u13-u12-u23) + u23 * (u23-u12-u13));
}

//: Compute triangle area from given distance d[3].
inline double tri_area_d (double* d)
{
  return vcl_sqrt (_delta2_16_d(d[0], d[1], d[2])) * 0.25;
}

//: Compute triangle area from given square distance u[3].
inline double tri_area_u (double* u)
{
  return vcl_sqrt (_delta2_16_u(u[0], u[1], u[2])) * 0.25;
}

//: Compute triangle compactness from given distane d[3] and area.
double tri_compactness (double* d, double& area);

//: Compute the barycentric coordinate of (G1, G2, G3)
//  Return false if numerical instability detected.
bool get_barycentric (const vgl_point_3d<double>& G1,
                      const vgl_point_3d<double>& G2,
                      const vgl_point_3d<double>& G3,
                      double& alpha, double& beta, double& gamma);

//: Compute the barycentric coordinate of (G1, G2, G3)
//  Input: d12, d23, d13.
//  Return false if numerical instability detected.
bool _get_barycentric (const double& u12, const double& u23, const double& u13,
                       double& alpha, double& beta, double& gamma);

void compute_tri_angles (const vgl_point_3d<double>& A, 
                         const vgl_point_3d<double>& B,
                         const vgl_point_3d<double>& C, 
                         double angle[3]);

vgl_point_3d<double> circum_center_3pts (const vgl_point_3d<double>& A, 
                                         const vgl_point_3d<double>& B,
                                         const vgl_point_3d<double>& C);


#endif // dbgl_barycentric_h_
