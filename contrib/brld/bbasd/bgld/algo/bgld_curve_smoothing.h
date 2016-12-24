// This is bgld_curve_smoothing.h
#ifndef bgld_curve_smoothing_h_
#define bgld_curve_smoothing_h_
//:
//\file
//\brief Smoothing of digital curves
//\author Based on original code by  Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 02/05/07 14:52:58 EST
//

#include <vcl_vector.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>

//: A simple discrete curvature smoothing technique. Simply move each sample point according to a
// roughly normal direction (bisector at each sample point) and strength.
//
// \param[in,out] c: digital curve (sequence of points). Endpoints remain fixed.
// \param[in] psi: how much to evolve at each step. If in interval [0,1], then it is guaranteed that
// the smoothed curve will not cross itself.
// \param[in] nsteps : number of smoothing iterations
//
// \remarks The algorithm may generate cusps. For example, if the input shape is the cursive "l"
// letter, with one self-intersection, then after some steps a cusp is formed, before desintangling
// the curve. Prof. Ben Kimia called this method "discrete curve shortening".
//
// 
//
//
void bgld_csm( vcl_vector<vgl_point_2d<double> > &c, double psi, unsigned nsteps);

bool bgld_curve_shorten_3d (vcl_vector<vgl_point_3d<double> > &c, double psi, unsigned nsteps);

void bgld_curve_shorten_3d_th (vcl_vector<vgl_point_3d<double> > &c, const double kernel_th,
                               double psi, unsigned nsteps);

//: curve shortening only in z(x,y) value.
void bgld_curve_shorten_z (vcl_vector<vgl_point_3d<double> > &c, double psi, unsigned nsteps);

// ###############################################################
// Ming: gaussian 3D curve smoothing

//: Gaussian curve smoothing.
void bgld_gaussian_smooth_curve_3d (vcl_vector<vgl_point_3d<double> > &curve, 
                                    const float sigma, const int nsteps);

//: Gaussian curve smoothing with a specified kernel_th.
void bgld_gaussian_smooth_curve_3d_th (vcl_vector<vgl_point_3d<double> >& curve, 
                                       const double kernel_th, const float sigma, const int nsteps);

//: Gaussian curve smoothing with a specified kernel_th.
void bgld_gaussian_smooth_curve_z_th (vcl_vector<vgl_point_3d<double> >& curve, 
                                      const double kernel_th, const float sigma, const int nsteps);

bool bgld_gaussian_smooth_curve_3d_fixedendpt (vcl_vector<vgl_point_3d<double> >& curve, 
                                               const float sigma, int nsteps);

void bgld_gaussian_smooth_curve_3d_th_fixedendpt (vcl_vector<vgl_point_3d<double> >& curve, 
                                                  const double kernel_th, 
                                                  const float sigma, const int nsteps);

void bgld_gaussian_smooth_curve_z_th_fixedendpt (vcl_vector<vgl_point_3d<double> >& curve, 
                                                 const double kernel_th, 
                                                 const float sigma, const int nsteps);

//2D Gaussian smoothing on 2D scalar function.
void bgld_gaussian_smooth_2d_th (vcl_vector<vcl_vector<double> >& range, 
                                 const float d_intra, const float d_inter, 
                                 const int nsteps,
                                 const float G_sigma_ratio,
                                 const float G_radius_ratio,
                                 const float G_th_ratio);

#endif // bgld_curve_smoothing_h

