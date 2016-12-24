//: This is bgld/bgld_curve_smoothing.cxx
//  Ricardo
//  Ming-Ching Chang

#include <vcl_cfloat.h>
#include <vcl_algorithm.h>
#include <vcl_iostream.h>
#include <vul/vul_printf.h>
#include <vgl/vgl_distance.h>
#include <vnl/algo/vnl_gaussian_kernel_1d.h>
#include <vnl/vnl_vector_fixed.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vnl/vnl_cross.h>
#include <vnl/vnl_rotation_matrix.h>
#include <vnl/vnl_math.h>
#include "bgld_curve_smoothing.h"


//: A simple discrete curvature smoothing technique. Simply move each sample point according to an
// estimated "pseudo-normal" direction (bisector at each sample point) and strength.
//
// \param[in,out] c: digital curve (sequence of points). Endpoints remain fixed.
// \param[in] psi: how much to evolve at each step. If in interval [0,1], then it is guaranteed that
// the smoothed curve will not cross itself.
// \param[in] nsteps : number of smoothing iterations
//
void
bgld_csm(
    vcl_vector<vgl_point_2d<double> > &c,
    double psi,
    unsigned nsteps
    )
{
  unsigned n = c.size();
  assert (n >= 3);

  vcl_vector<vgl_point_2d<double> > cs;
  cs.resize(n);

  cs[0] = vgl_point_2d<double> (c[0]);
  cs[n-1] = vgl_point_2d<double> (c[n-1]);

  for (unsigned ns=0; ns < nsteps; ++ns) {
    for (unsigned i=1; i < n-1; ++i) {
      vnl_vector_fixed<double,2> middle_p(c[i].x(),c[i].y());
      vnl_vector_fixed<double,2> left_p(c[i-1].x(),c[i-1].y());
      vnl_vector_fixed<double,2> right_p(c[i+1].x(),c[i+1].y());

      vnl_vector_fixed<double,2> v1 = left_p - middle_p;
      vnl_vector_fixed<double,2> v2 = right_p - middle_p;

      double nv1 = v1.two_norm();
      double nv2 = v2.two_norm();

      vnl_vector_fixed<double,2> v1n = v1/nv1;
      vnl_vector_fixed<double,2> v2n = v2/nv2;

      vnl_vector_fixed<double,2> nrm;
      if (nv1 < nv2) {
        nrm = (v1 + v2n*nv1) / 4.0;
      } else {
        nrm = (v2 + v1n*nv2) / 4.0;
      }

      vnl_vector_fixed<double,2> cs_i = middle_p + psi*nrm;
      cs[i] = vgl_point_2d<double> (cs_i[0],cs_i[1]);
    }
    c = cs;
  }
}

//: A simple discrete curvature smoothing technique. Simply move each sample point according to an
// estimated "pseudo-normal" direction (bisector at each sample point) and strength.
//
// \param[in,out] c: digital curve (sequence of points). Endpoints remain fixed.
// \param[in] psi: how much to evolve at each step. If in interval [0,1], then it is guaranteed that
// the smoothed curve will not cross itself.
// \param[in] nsteps : number of smoothing iterations
//
bool bgld_curve_shorten_3d (vcl_vector<vgl_point_3d<double> > &c,
                            double psi, unsigned nsteps)
{
  unsigned n = c.size();
  if (n < 3)
    return false;

  vcl_vector<vgl_point_3d<double> > cs;
  cs.resize(n);

  cs[0] = vgl_point_3d<double> (c[0]);
  cs[n-1] = vgl_point_3d<double> (c[n-1]);

  for (unsigned ns=0; ns < nsteps; ++ns) {
    for (unsigned i=1; i < n-1; ++i) {
      vnl_vector_fixed<double,3> middle_p(c[i].x(),  c[i].y(),   c[i].z());
      vnl_vector_fixed<double,3> left_p(c[i-1].x(),  c[i-1].y(), c[i-1].z());
      vnl_vector_fixed<double,3> right_p(c[i+1].x(), c[i+1].y(), c[i+1].z());

      vnl_vector_fixed<double,3> v1 = left_p - middle_p;
      vnl_vector_fixed<double,3> v2 = right_p - middle_p;

      double nv1 = v1.two_norm();
      double nv2 = v2.two_norm();

      vnl_vector_fixed<double,3> v1n = v1/nv1;
      vnl_vector_fixed<double,3> v2n = v2/nv2;

      vnl_vector_fixed<double,3> nrm;
      if (nv1 < nv2) {
        nrm = (v1 + v2n*nv1) / 4.0;
      } else {
        nrm = (v2 + v1n*nv2) / 4.0;
      }

      vnl_vector_fixed<double,3> cs_i = middle_p + psi*nrm;
      cs[i] = vgl_point_3d<double> (cs_i[0],cs_i[1],cs_i[2]);
    }
    c = cs;
  }
  return true;
}

void bgld_curve_shorten_z (vcl_vector<vgl_point_3d<double> > &c,
                           double psi, unsigned nsteps)
{
  unsigned n = c.size();
  assert (n >= 3);

  vcl_vector<vgl_point_3d<double> > cs;
  cs.resize(n);

  cs[0] = vgl_point_3d<double> (c[0]);
  cs[n-1] = vgl_point_3d<double> (c[n-1]);

  for (unsigned ns=0; ns < nsteps; ++ns) {
    for (unsigned i=1; i < n-1; ++i) {
      vnl_vector_fixed<double,3> middle_p(c[i].x(),  c[i].y(),   c[i].z());
      vnl_vector_fixed<double,3> left_p(c[i-1].x(),  c[i-1].y(), c[i-1].z());
      vnl_vector_fixed<double,3> right_p(c[i+1].x(), c[i+1].y(), c[i+1].z());

      vnl_vector_fixed<double,3> v1 = left_p - middle_p;
      vnl_vector_fixed<double,3> v2 = right_p - middle_p;

      double nv1 = v1.two_norm();
      double nv2 = v2.two_norm();

      vnl_vector_fixed<double,3> v1n = v1/nv1;
      vnl_vector_fixed<double,3> v2n = v2/nv2;

      vnl_vector_fixed<double,3> nrm;
      if (nv1 < nv2) {
        nrm = (v1 + v2n*nv1) / 4.0;
      } else {
        nrm = (v2 + v1n*nv2) / 4.0;
      }

      vnl_vector_fixed<double,3> cs_i = middle_p + psi*nrm;
      //cs[i] = vgl_point_3d<double> (cs_i[0],cs_i[1],cs_i[2]);
      cs[i] = vgl_point_3d<double> (c[i].x(), c[i].y(), cs_i[2]);
    }
    c = cs;
  }
}

void bgld_curve_shorten_3d_th (vcl_vector<vgl_point_3d<double> > &c, const double kernel_th,
                               double stepsize, unsigned int nsteps)
{  
  //For curve c, look for consecutive group of points from js to je
  unsigned int js = 0;
  while (js < c.size()) {
    //find je
    unsigned int jep = js;
    unsigned int je = js+1;
    while (je < c.size()-1 && vgl_distance (c[jep], c[je]) < kernel_th) {
      jep++;
      je++;
    }

    if (je >= c.size())
      je = c.size()-1;

    //smooth curve[js, je] with end points fixed
    vcl_vector<vgl_point_3d<double> > curve;
    curve.clear();
    for (unsigned int j=js; j<=je; j++)
      curve.push_back (c[j]);

    if (curve.size() > 2)
      bgld_curve_shorten_3d (curve, stepsize, nsteps);

    for (unsigned int j=js; j<=je; j++)
      c[j] = curve[j-js];

    //set new js
    js = je+1;
  }
}

// ###########################################################################
//: Gaussian curve smoothing.
// Ming: gaussian 3D curve smoothing

#define GK_SIZE 3

void bgld_gaussian_smooth_curve_3d (vcl_vector<vgl_point_3d<double> >& curve, 
                                    const float sigma, const int nsteps)
{
  vnl_gaussian_kernel_1d gauss_1d ((double) sigma);
  
  //add 3 extra points in the front and the end.
  curve.insert(curve.begin(),curve[0]);
  curve.insert(curve.begin(),curve[0]);
  curve.insert(curve.begin(),curve[0]);

  curve.insert(curve.end(),curve[curve.size()-1]);
  curve.insert(curve.end(),curve[curve.size()-1]);
  curve.insert(curve.end(),curve[curve.size()-1]);

  for (int s=0; s<nsteps; s++) {
    for (int i=GK_SIZE; i+GK_SIZE < int(curve.size()); ++i) {
      //kernel size: -3, -2, -1, 0, 1, 2, 3
      double sum_gauss=0, sum_x=0, sum_y=0, sum_z=0;

      for (int j=-GK_SIZE; j<=GK_SIZE; j++) {
        double g = gauss_1d[vnl_math::abs(j)];
        sum_gauss += g;
        sum_x += curve[i+j].x() * g;
        sum_y += curve[i+j].y() * g;
        sum_z += curve[i+j].z() * g;
      }
      sum_x /= sum_gauss;
      sum_y /= sum_gauss;
      sum_z /= sum_gauss;
      curve[i].set (sum_x, sum_y, sum_z);
    }
  }

  curve.erase(curve.begin());
  curve.erase(curve.begin());
  curve.erase(curve.begin());
  curve.erase(curve.end()-1);
  curve.erase(curve.end()-1);
  curve.erase(curve.end()-1);
}


void bgld_gaussian_smooth_curve_3d_th (vcl_vector<vgl_point_3d<double> >& curve, 
                                       const double kernel_th, const float sigma, const int nsteps)
{
  vnl_gaussian_kernel_1d gauss_1d ((double) sigma);
  
  //add 3 extra points in the front and the end.
  curve.insert(curve.begin(),curve[0]);
  curve.insert(curve.begin(),curve[0]);
  curve.insert(curve.begin(),curve[0]);

  curve.insert(curve.end(),curve[curve.size()-1]);
  curve.insert(curve.end(),curve[curve.size()-1]);
  curve.insert(curve.end(),curve[curve.size()-1]);

  for (int s=0; s<nsteps; s++) {
    for (int i=GK_SIZE; i+GK_SIZE < int(curve.size()); ++i) {
      //kernel size: -3, -2, -1, 0, 1, 2, 3
      double sum_gauss=0, sum_x=0, sum_y=0, sum_z=0;

      for (int j=-GK_SIZE; j<=GK_SIZE; j++) {
        double d = vgl_distance (curve[i+j], curve[i]);
        if (d > kernel_th) 
          continue; //thresholding
        double g = gauss_1d[vnl_math::abs(j)];
        sum_gauss += g;
        sum_x += curve[i+j].x() * g;
        sum_y += curve[i+j].y() * g;
        sum_z += curve[i+j].z() * g;
      }
      sum_x /= sum_gauss;
      sum_y /= sum_gauss;
      sum_z /= sum_gauss;
      curve[i].set (sum_x, sum_y, sum_z);
    }
  }

  curve.erase(curve.begin());
  curve.erase(curve.begin());
  curve.erase(curve.begin());
  curve.erase(curve.end()-1);
  curve.erase(curve.end()-1);
  curve.erase(curve.end()-1);
}

void bgld_gaussian_smooth_curve_z_th (vcl_vector<vgl_point_3d<double> >& curve, 
                                      const double kernel_th, const float sigma, const int nsteps)
{
  vnl_gaussian_kernel_1d gauss_1d ((double) sigma);
  
  //add 3 extra points in the front and the end.
  curve.insert(curve.begin(),curve[0]);
  curve.insert(curve.begin(),curve[0]);
  curve.insert(curve.begin(),curve[0]);

  curve.insert(curve.end(),curve[curve.size()-1]);
  curve.insert(curve.end(),curve[curve.size()-1]);
  curve.insert(curve.end(),curve[curve.size()-1]);

  for (int s=0; s<nsteps; s++) {
    for (int i=GK_SIZE; i+GK_SIZE < int(curve.size()); ++i) {
      //kernel size: -3, -2, -1, 0, 1, 2, 3
      double sum_gauss=0, sum_z=0;

      for (int j=-GK_SIZE; j<=GK_SIZE; j++) {
        double d = vgl_distance (curve[i+j], curve[i]);
        if (d > kernel_th) 
          continue; //thresholding
        double g = gauss_1d[vnl_math::abs(j)];
        sum_gauss += g;
        sum_z += curve[i+j].z() * g;
      }
      sum_z /= sum_gauss;
      curve[i].set (curve[i].x(), curve[i].y(), sum_z);
    }
  }

  curve.erase(curve.begin());
  curve.erase(curve.begin());
  curve.erase(curve.begin());
  curve.erase(curve.end()-1);
  curve.erase(curve.end()-1);
  curve.erase(curve.end()-1);
}

bool bgld_gaussian_smooth_curve_3d_fixedendpt (vcl_vector<vgl_point_3d<double> >& curve, 
                                               const float sigma, const int nsteps)
{
  if (curve.size() < 2)
    return false;

  vgl_point_3d<double> sPt = curve[0];
  vgl_point_3d<double> ePt = curve[curve.size()-1];
  unsigned int sz = curve.size();
  double r = sqrt ((ePt.x()-sPt.x())*(ePt.x()-sPt.x()) +
                   (ePt.y()-sPt.y())*(ePt.y()-sPt.y()) +
                   (ePt.z()-sPt.z())*(ePt.z()-sPt.z()));

  //smooth noise curve n times and write to original file 
  bgld_gaussian_smooth_curve_3d (curve, sigma, nsteps);
  assert (sz == curve.size());

  //If starting and ending points are the same (r==0), return true.
  if (r < 1E-4)
    return true;

  //1)compute the r_new, get the scale.
  double r_new = sqrt ((curve[sz-1].x()-curve[0].x())*(curve[sz-1].x()-curve[0].x()) +
                       (curve[sz-1].y()-curve[0].y())*(curve[sz-1].y()-curve[0].y()) +
                       (curve[sz-1].z()-curve[0].z())*(curve[sz-1].z()-curve[0].z()));
  double ScaleFactor = r/r_new;

  //2)Move to the relative coord (curve[0] as origin).
  //3)Scale the curve
  for (unsigned int i=1; i<curve.size(); i++) {
    double x = (curve[i].x() - curve[0].x())*ScaleFactor;
    double y = (curve[i].y() - curve[0].y())*ScaleFactor;
    double z = (curve[i].z() - curve[0].z())*ScaleFactor;
    curve[i].set (x, y, z);
  }
  curve[0].set (0, 0, 0);

  //Now we have two vector, X0=(dx0, dy0, dz0) and X=(dx, dy, dz)
  //We want to rotate X to X0
  vnl_vector_fixed<double, 3> X0 (ePt.x()-sPt.x(), ePt.y()-sPt.y(), ePt.z()-sPt.z());
  vnl_vector_fixed<double, 3> X (curve[sz-1].x(), curve[sz-1].y(), curve[sz-1].z());

  //The rotation axis is cross(X, X0)
  vnl_vector<double> rot_vector = vnl_cross_3d (X, X0);

  //4)The rotation angle is computed form dot product.
  double rot_angle = angle (X0, X);   
  if (!(rot_angle > -100000.0)) //assert (rot_angle > -100000.0); a simple check.
    rot_angle = 0;

  //Note: if rot_angle is zero, just skip the rotation part.
  //      Don't multiple the matrix. Will have numerical error!
  if (rot_angle !=0) {   
    //Normalize rot_vector, and make it's norm equal to rot_angle
    rot_vector /= rot_vector.magnitude();
    rot_vector *= rot_angle;

    // The result is a (special) orthogonal 3x3 matrix which is a
    // rotation about the axis, by an angle equal to ||axis||.
    vnl_matrix_fixed<double, 3, 3> R = vnl_rotation_matrix (rot_vector);

    //5)Rotate each point on the curve to the right place.
    for (unsigned int i=1; i<curve.size(); i++) {
      vnl_vector_fixed<double, 3> xx (curve[i].x(), curve[i].y(), curve[i].z());
      xx = R*xx;
      curve[i].set (xx(0), xx(1), xx(2));
    }
  }

  //6)Shift back from the relative coord, adding sPt.
  for (unsigned int i=0; i<curve.size(); i++)
    curve[i].set (curve[i].x() + sPt.x(), curve[i].y() + sPt.y(), curve[i].z() + sPt.z());

  return true;
}

void bgld_gaussian_smooth_curve_3d_th_fixedendpt (vcl_vector<vgl_point_3d<double> >& curve, 
                                                  const double kernel_th, 
                                                  const float sigma, const int nsteps)
{
  vgl_point_3d<double> sPt = curve[0];
  vgl_point_3d<double> ePt = curve[curve.size()-1];
  unsigned int sz = curve.size();
  double r = sqrt ((ePt.x()-sPt.x())*(ePt.x()-sPt.x()) +
                   (ePt.y()-sPt.y())*(ePt.y()-sPt.y()) +
                   (ePt.z()-sPt.z())*(ePt.z()-sPt.z()));

  // smooth noise curve n times and write to original file 
  bgld_gaussian_smooth_curve_3d_th (curve, kernel_th, sigma, nsteps);
  assert (sz == curve.size());

  //1)compute the r_new, get the scale.
  double r_new = sqrt ((curve[sz-1].x()-curve[0].x())*(curve[sz-1].x()-curve[0].x()) +
                       (curve[sz-1].y()-curve[0].y())*(curve[sz-1].y()-curve[0].y()) +
                       (curve[sz-1].z()-curve[0].z())*(curve[sz-1].z()-curve[0].z()));
  double ScaleFactor = r/r_new;

  //2)Move to the relative coord (curve[0] as origin).
  //3)Scale the curve
  for (unsigned int i=1; i<curve.size(); i++) {
    double x = (curve[i].x() - curve[0].x())*ScaleFactor;
    double y = (curve[i].y() - curve[0].y())*ScaleFactor;
    double z = (curve[i].z() - curve[0].z())*ScaleFactor;
    curve[i].set (x, y, z);
  }
  curve[0].set (0, 0, 0);

  //Now we have two vector, X0=(dx0, dy0, dz0) and X=(dx, dy, dz)
  //We want to rotate X to X0
  vnl_vector_fixed<double, 3> X0 (ePt.x()-sPt.x(), ePt.y()-sPt.y(), ePt.z()-sPt.z());
  vnl_vector_fixed<double, 3> X (curve[sz-1].x(), curve[sz-1].y(), curve[sz-1].z());

  //The rotation axis is cross(X, X0)
  vnl_vector<double> rot_vector = vnl_cross_3d (X, X0);

  //4)The rotation angle is computed form dot product.
  double rot_angle = angle (X0, X);   
  if (!(rot_angle > -100000.0)) //assert (rot_angle > -100000.0); a simple check.
    rot_angle = 0;

  //Note: if rot_angle is zero, just skip the rotation part.
  //      Don't multiple the matrix. Will have numerical error!
  if (rot_angle !=0) {   
    //Normalize rot_vector, and make it's norm equal to rot_angle
    rot_vector /= rot_vector.magnitude();
    rot_vector *= rot_angle;

    // The result is a (special) orthogonal 3x3 matrix which is a
    // rotation about the axis, by an angle equal to ||axis||.
    vnl_matrix_fixed<double, 3, 3> R = vnl_rotation_matrix (rot_vector);

    //5)Rotate each point on the curve to the right place.
    for (unsigned int i=1; i<curve.size(); i++) {
      vnl_vector_fixed<double, 3> xx (curve[i].x(), curve[i].y(), curve[i].z());
      xx = R*xx;
      curve[i].set (xx(0), xx(1), xx(2));
    }
  }

  //6)Shift back from the relative coord, adding sPt.
  for (unsigned int i=0; i<curve.size(); i++)
    curve[i].set (curve[i].x() + sPt.x(), curve[i].y() + sPt.y(), curve[i].z() + sPt.z());
}


void bgld_gaussian_smooth_curve_z_th_fixedendpt (vcl_vector<vgl_point_3d<double> >& curve, 
                                                 const double kernel_th, 
                                                 const float sigma, const int nsteps)
{
  vgl_point_3d<double> sPt = curve[0];
  vgl_point_3d<double> ePt = curve[curve.size()-1];
  unsigned int sz = curve.size();
  double r = sqrt ((ePt.x()-sPt.x())*(ePt.x()-sPt.x()) +
                   (ePt.y()-sPt.y())*(ePt.y()-sPt.y()) +
                   (ePt.z()-sPt.z())*(ePt.z()-sPt.z()));

  // smooth noise curve n times and write to original file 
  bgld_gaussian_smooth_curve_z_th (curve, kernel_th, sigma, nsteps);
  assert (sz == curve.size());

  //1)compute the r_new, get the scale.
  double r_new = sqrt ((curve[sz-1].x()-curve[0].x())*(curve[sz-1].x()-curve[0].x()) +
                       (curve[sz-1].y()-curve[0].y())*(curve[sz-1].y()-curve[0].y()) +
                       (curve[sz-1].z()-curve[0].z())*(curve[sz-1].z()-curve[0].z()));
  double ScaleFactor = r/r_new;

  //2)Move to the relative coord (curve[0] as origin).
  //3)Scale the curve
  for (unsigned int i=1; i<curve.size(); i++) {
    double x = (curve[i].x() - curve[0].x())*ScaleFactor;
    double y = (curve[i].y() - curve[0].y())*ScaleFactor;
    double z = (curve[i].z() - curve[0].z())*ScaleFactor;
    curve[i].set (x, y, z);
  }
  curve[0].set (0, 0, 0);

  //Now we have two vector, X0=(dx0, dy0, dz0) and X=(dx, dy, dz)
  //We want to rotate X to X0
  vnl_vector_fixed<double, 3> X0 (ePt.x()-sPt.x(), ePt.y()-sPt.y(), ePt.z()-sPt.z());
  vnl_vector_fixed<double, 3> X (curve[sz-1].x(), curve[sz-1].y(), curve[sz-1].z());

  //The rotation axis is cross(X, X0)
  vnl_vector<double> rot_vector = vnl_cross_3d (X, X0);

  //4)The rotation angle is computed form dot product.
  double rot_angle = angle (X0, X);   
  if (!(rot_angle > -100000.0)) //assert (rot_angle > -100000.0); a simple check.
    rot_angle = 0;

  //Note: if rot_angle is zero, just skip the rotation part.
  //      Don't multiple the matrix. Will have numerical error!
  if (rot_angle !=0) {   
    //Normalize rot_vector, and make it's norm equal to rot_angle
    rot_vector /= rot_vector.magnitude();
    rot_vector *= rot_angle;

    // The result is a (special) orthogonal 3x3 matrix which is a
    // rotation about the axis, by an angle equal to ||axis||.
    vnl_matrix_fixed<double, 3, 3> R = vnl_rotation_matrix (rot_vector);

    //5)Rotate each point on the curve to the right place.
    for (unsigned int i=1; i<curve.size(); i++) {
      vnl_vector_fixed<double, 3> xx (curve[i].x(), curve[i].y(), curve[i].z());
      xx = R*xx;
      curve[i].set (xx(0), xx(1), xx(2));
    }
  }

  //6)Shift back from the relative coord, adding sPt.
  for (unsigned int i=0; i<curve.size(); i++)
    curve[i].set (curve[i].x() + sPt.x(), curve[i].y() + sPt.y(), curve[i].z() + sPt.z());
}

//2D Gaussian smoothing on 2D scalar function.
void bgld_gaussian_smooth_2d_th (vcl_vector<vcl_vector<double> >& range, 
                                 const float d_intra, const float d_inter, 
                                 const int nsteps,
                                 const float G_sigma_ratio,
                                 const float G_radius_ratio,
                                 const float G_th_ratio)
{ 
  int s, i, j, k, l;

  //range2 to store the smoothed result.
  vcl_vector<vcl_vector<double> > range2;
  range2.resize (range.size());
  for (i=0; i<int(range.size()); i++)
    range2[i].resize(range[i].size());

  //Gaussian kernel parameters.
  double sigma = vcl_max (d_intra, d_inter) * G_sigma_ratio;
  vnl_gaussian_kernel_1d gauss1 ((double) sigma);
  vnl_gaussian_kernel_1d gauss2 ((double) sigma);
 
  double ksize = vcl_max (d_intra, d_inter) * G_radius_ratio;
  double kth = vcl_max (d_intra, d_inter) * G_th_ratio;

  int GK2_SIZE_JL = int (ksize / d_intra); //intra_scanline kernel size
  int GK2_SIZE_IK = int (ksize / d_inter); //inter_scanline kernel size

  vul_printf (vcl_cout, "\nbgld_gaussian_smooth_2d_th():\n", sigma);
  vul_printf (vcl_cout, "\td_intra: %f, d_inter: %f.\n", d_intra, d_inter);
  vul_printf (vcl_cout, "\tsigma: %f, kernel_size: %f, kernel_th: %f.\n", sigma, ksize, kth);
  vul_printf (vcl_cout, "\tkstep_intra: %d, kstep_inter: %d.\n", GK2_SIZE_JL, GK2_SIZE_IK);
  vul_printf (vcl_cout, "\tApply Gaussian smoothing %d times.\n", nsteps);
  
  for (s=0; s<nsteps; s++) {
    for (i=0; i<int(range.size()); i++) {
      for (j=0; j<int(range[i].size()); j++) {
        if (range[i][j] == DBL_MAX)
          continue; //skip non-data points.

        double sum_gauss = 0, sum_z = 0;
        for (k=-GK2_SIZE_IK; k<=GK2_SIZE_IK; k++) {
          int ik = i+k;
          if (ik<0 || ik>int(range.size())-1)
            continue; //ignore out of range points
          for (l=-GK2_SIZE_JL; l<=GK2_SIZE_JL; l++) {
            int jl = j+l;
            if (jl<0 || jl>int(range[ik].size())-1)
              continue; //ignore out of range points
            
            if (range[ik][jl] == DBL_MAX)
              continue; //ignore non-data points

            if (vcl_fabs(range[ik][jl] - range[i][j]) > kth)
              continue; //ignore out of threshold points

            //double gauss_2d = gauss1[vcl_abs(k*)] * gauss2[vcl_abs(l)];
            double gauss_2d = gauss1.G(k*d_inter) * gauss2.G(l*d_intra);
            sum_gauss += gauss_2d;
            sum_z += range[ik][jl] * gauss_2d;
          } //end kernel l
        } //end kernel k

        if (sum_gauss != 0) { //final result.
          sum_z /= sum_gauss;
          range2[i][j] = sum_z;
        }
      } //end i
    } //end j

    //write range2 back to range    
    for (i=0; i<int(range.size()); i++)
      for (j=0; j<int(range[i].size()); j++)
          range[i][j] = range2[i][j];

  } //end iterations

  vul_printf (vcl_cout, "\tdone.\n");
}
