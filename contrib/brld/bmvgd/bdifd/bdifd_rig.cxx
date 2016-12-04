#include "dbdif_rig.h"

#include <vnl/algo/vnl_svd.h>

dbdif_rig::
dbdif_rig(const vpgl_perspective_camera <double> &P1, const vpgl_perspective_camera <double> &P2)
     : cam(2)
{
  cam[0].set_p(P1), cam[1].set_p(P2);
  compute_extra_info();
}

void dbdif_rig::
compute_extra_info()
{
  f12.set_matrix(cam[0].Pr_, cam[1].Pr_);
}

double dbdif_rig::
reconstruct_point_lsqr(
   const vsol_point_2d_sptr &xi1, const vsol_point_2d_sptr &xi2,
   dbdif_vector_3d *Cpt_v) const
{
  // convert to world coordinates
  dbdif_vector_3d gamma1, gamma2;

  cam[0].get_gama(xi1->x(), xi1->y(), &gamma1);
  cam[1].get_gama(xi2->x(), xi2->y(), &gamma2);

  return reconstruct_point_lsqr(gamma1, gamma2, Cpt_v);
}

//: returns error norm, ie, minimum distance btw the backprojection rays
double dbdif_rig::
reconstruct_point_lsqr(
      const dbdif_vector_3d &gama1,
      const dbdif_vector_3d &gama2,
      dbdif_vector_3d *Cpt_v) const
{
   // global: vnl_matrix_fixed<double,3,2> A;
 
   for (unsigned i=0; i<3; ++i) {
      A_(i,0) = gama1[i];
      A_(i,1) = -gama2[i];
   }
 
   vnl_svd<double> svd(A_);
   lambda_ = svd.solve(cam[1].c - cam[0].c);
 
   *Cpt_v = cam[0].c + lambda_(0)*gama1;

   return (A_*lambda_ + cam[0].c - cam[1].c).two_norm();
}

//: input is a vector of points in image coordinates
// Curve 1 has to be the same size as crv 2.
void dbdif_rig::
reconstruct_3d_curve(
  vcl_vector<dbdif_vector_3d> *Crv3d,
  const vcl_vector<vsol_point_2d_sptr> &crv1, 
  const vcl_vector<vsol_point_2d_sptr> &crv2)
{
  assert(crv1.size() == crv2.size());

  // convert to world coordinates
  vcl_vector<dbdif_vector_3d> crv1_v, crv2_v;

  crv1_v.resize(crv1.size()); crv2_v.resize(crv1.size());

  for (unsigned i=0; i<crv1.size(); ++i) {
    cam[0].get_gama(crv1[i]->x(),crv1[i]->y(),&(crv1_v[i]));
    cam[1].get_gama(crv2[i]->x(),crv2[i]->y(),&(crv2_v[i]));
  }

  reconstruct_3d_curve(Crv3d,crv1_v,crv2_v);
}

//: input is a vector of image points in 3D world coordinates
void dbdif_rig::
reconstruct_3d_curve(
  vcl_vector<dbdif_vector_3d> *Crv3d,
  const vcl_vector<dbdif_vector_3d> &crv1, 
  const vcl_vector<dbdif_vector_3d> &crv2)
{
  assert(crv1.size() == crv2.size());
  Crv3d->resize(crv1.size());

  for (unsigned i=0; i<crv1.size(); ++i) {
    reconstruct_point_lsqr(crv1[i],crv2[i],&((*Crv3d)[i]));
  }
}

void dbdif_rig::
reconstruct_tangent(
      const dbdif_vector_3d &gama1,
      const dbdif_vector_3d &gama2,
      const vgl_vector_2d<double> &t1_img,
      const vgl_vector_2d<double> &t2_img,
      dbdif_vector_3d *T_rec) const
{
   dbdif_vector_3d t1_world;
   cam[0].img_to_world_vector(t1_img.x(),t1_img.y(), &t1_world);

   dbdif_vector_3d t2_world;
   cam[1].img_to_world_vector(t2_img.x(),t2_img.y(), &t2_world);

   reconstruct_tangent(gama1,gama2,t1_world,t2_world,T_rec);
}

#ifndef NDEBUG 
static bool ran_dbg_=false;
#endif

void dbdif_rig::
reconstruct_1st_order(
  const dbdif_1st_order_point_2d &fr1, 
  const dbdif_1st_order_point_2d &fr2,
  dbdif_1st_order_point_3d *Fr) const
{
  // TODO use optimal Euclidean reconstruction here
  double err = reconstruct_point_lsqr(fr1.gama, fr2.gama, &(Fr->Gama));

  Gama1_ = Fr->Gama - cam[0].c; 
  Gama2_ = Fr->Gama - cam[1].c;

  double lambda1 = lambda_(0), 
         lambda2 = lambda_(1);

#ifndef NDEBUG 
  if (!ran_dbg_) {
    double lambda1_err, lambda2_err;
    lambda1_err = vcl_fabs(lambda1 - dot_product(Gama1_,cam[0].F));
    lambda2_err = vcl_fabs(lambda2 - dot_product(Gama2_,cam[1].F));
    if (lambda1_err > 0.1  || lambda2_err > 0.1) {
      vcl_cerr << "Warning: very imprecise measurements and/or reconstruction in " << __FILE__ << vcl_endl;
      vcl_cerr << "reconstr lsq err: " << err << 
        "\tlambda1 error: " << lambda1_err << "\tlambda2 error: " << lambda2_err << vcl_endl;
      ran_dbg_ = true;
    }
  }
#endif
  
  bool valid_t = reconstruct_tangent(fr1.gama, fr2.gama, fr1.t, fr2.t, &(Fr->T));
  Fr->valid = Fr->valid && valid_t;
  Fr->T.normalize();

  g1_ = cam[0].speed(*Fr,lambda1);
  g2_ = cam[1].speed(*Fr,lambda2);
}

void dbdif_rig::
reconstruct_2nd_order(
    const dbdif_2nd_order_point_2d &fr1, 
    const dbdif_2nd_order_point_2d &fr2,
    dbdif_2nd_order_point_3d *Fr) const
{
  reconstruct_1st_order(fr1, fr2, Fr);

  double lambda1 = lambda_(0), 
         lambda2 = lambda_(1);

  U1_ = vnl_cross_3d(fr1.gama,fr1.t);
  U2_ = vnl_cross_3d(fr2.gama,fr2.t);

  dbdif_vector_3d Const;

  n_dot_U1_ = dot_product(fr1.n,U1_);
  n_dot_U2_ = dot_product(fr2.n,U2_);

  g1g1_ = g1_*g1_;
  g2g2_ = g2_*g2_;
  Const[0] = n_dot_U1_*lambda1*g1g1_*fr1.k;
  Const[1] = n_dot_U2_*lambda2*g2g2_*fr2.k;
  Const[2] = 0;

  Coefs_(0,0) = U1_(0); Coefs_(0,1) = U1_(1); Coefs_(0,2) = U1_(2);
  Coefs_(1,0) = U2_(0); Coefs_(1,1) = U2_(1); Coefs_(1,2) = U2_(2);
  Coefs_(2,0) = Fr->T(0);  Coefs_(2,1) = Fr->T(1);  Coefs_(2,2) = Fr->T(2);

  vnl_svd<double> svd(Coefs_);
  dbdif_vector_3d NK;
  NK = svd.solve(Const);

  double residual = (Coefs_*NK - Const).two_norm();

#ifndef NDEBUG
  if ( !dbdif_util::near_zero(residual, 1e-8) ) {
    vcl_cout << "Imprecision in normal reconstruction!\n";
  }
#endif

  Fr->K = NK.two_norm();
  Fr->set_normal(NK);

//  if ( !dbdif_util::near_zero(Fr->K,1e-15) )
  //: else leave any finite value on the normal; it is undefined
}

void dbdif_rig::
reconstruct_3rd_order(
    const dbdif_3rd_order_point_2d &fr1, 
    const dbdif_3rd_order_point_2d &fr2,
    dbdif_3rd_order_point_3d *Fr) const
{
  reconstruct_2nd_order(fr1,fr2,Fr);

  if (Fr->K == 0) {
    Fr->Kdot = 0;
    Fr->Tau  = 0;
    return;
  }

  double g1_prime, g2_prime, lambda1_dot, lambda2_dot;

  lambda1_dot = dot_product(Fr->T,cam[0].F);
  lambda2_dot = dot_product(Fr->T,cam[1].F);
  g1_prime = cam[0].tangential_accel(Fr,lambda_[0],lambda1_dot,g1_,fr1.t);
  g2_prime = cam[1].tangential_accel(Fr,lambda_[1],lambda2_dot,g2_,fr2.t);

  // compute non-homogeneous term in system
  dbdif_vector_3d Const;
  Const[0]  = 3*g1g1_*fr1.k*lambda1_dot;
  Const[0] += lambda_[0]*( 3*g1_*g1_prime*fr1.k + g1g1_*g1_*fr1.kdot);
  Const[0] *= dot_product(fr1.n,U1_);

  Const[1]  = 3*g2g2_*fr2.k*lambda2_dot;
  Const[1] += lambda_[1]*( 3*g2_*g2_prime*fr2.k + g2g2_*g2_*fr2.kdot);
  Const[1] *= dot_product(fr2.n,U2_);

  Const[2]  = 0;
  
  // solve
  vnl_svd<double> svd(Coefs_);
  dbdif_vector_3d V;
  V = svd.solve(Const);
  Fr->Kdot = dot_product(V,Fr->N);
  Fr->Tau  = dot_product(V,Fr->B) / Fr->K;
  assert(vnl_math_isfinite(Fr->Tau));
}

// -------------------------------------------------------------------------------
// Code for differential imagery and occluding contours
// -------------------------------------------------------------------------------

//: Reconstruction of occluding contours, assuming nearby cameras / small
// baseline. This uses a straightforward discretization of the differential 
// formula in Giblin and in my notes.
//
// Input: gama, gama_s, gama_t, where gama and gama_s may also be any vector
// multiple of them.
//
// \return false if point could not be reconstructed
// \param[out] (todo) estimate of error
// \param[out] Cpt_v reconstructed point in world coordinates relative to world
// origin
//
bool dbdif_rig::
reconstruct_occluding_contour_point(
      const dbdif_vector_3d &gamma,
      const dbdif_vector_3d &gamma_s,
      dbdif_vector_3d delta_gamma_t,
      dbdif_vector_3d *Cpt_v)
{
  lambda_.set_size(1);

  //: straightforward tangent estimation:
  dbdif_vector_3d delta_c = cam[1].c - cam[0].c;
  double delta_c_norm = delta_c.two_norm();
  delta_c /= delta_c_norm;


  double delta_gamma_t_norm = delta_gamma_t.two_norm();
  delta_gamma_t /=delta_gamma_t_norm;

  double factor = delta_c_norm / delta_gamma_t_norm;

  dbdif_vector_3d N = vnl_cross_3d(gamma,gamma_s); // Actually any multiple will do


  N.normalize();

  double numerator   = dot_product(delta_c,N);
  double denominator = dot_product(delta_gamma_t,N);
  static const double tol = vnl_math::pi/5.0;

  lambda_(0) = (-numerator/denominator)*factor;
  *Cpt_v = lambda_(0)*gamma + cam[0].c;

  //: Frontier points. 
  if (dbdif_util::near_zero(numerator,tol)) {
  //    vcl_cout << "Frontier point detected; c_t\\cdot\\N: ";
    return false;
  }

  if (dbdif_util::near_zero(lambda_(0)) || dbdif_util::near_zero(1.0/lambda_(0))) {
    vcl_cout << "WARNING: Invalid depth" << vcl_endl;

    if (dbdif_util::near_zero(factor,tol) || dbdif_util::near_zero(1.0/factor,tol))
      vcl_cout << "Factor leads to zero or infinite depth\n";

    if (dbdif_util::near_zero(numerator,tol))
      vcl_cout << "Numerator near zero; ";

    if (dbdif_util::near_zero(denominator,tol)) {
      vcl_cout << "Denominator near zero:\n";
      vcl_cout << "denom:  " << denominator
        << " numer: " << numerator  << vcl_endl
        << " delta_gamma_t: " << delta_gamma_t 
        << " N: " << N << vcl_endl;
    }

    vcl_cout << vcl_endl;

    return false;
  }

  return true;
}

bool dbdif_rig::
reconstruct_occluding_contour_point(
      const dbdif_vector_3d &gamma,
      const dbdif_vector_3d &gamma_s,
      dbdif_vector_3d delta_gamma_t,
      dbdif_vector_3d *Cpt_v,
      dbdif_vector_3d delta_c)
{
  lambda_.set_size(1);

  double delta_c_norm = delta_c.two_norm();
  delta_c /= delta_c_norm;

  double delta_gamma_t_norm = delta_gamma_t.two_norm();
  delta_gamma_t /=delta_gamma_t_norm;

  double factor = delta_c_norm / delta_gamma_t_norm;

  dbdif_vector_3d N = vnl_cross_3d(gamma,gamma_s); // Actually any multiple will do


  N.normalize();

  double numerator   = dot_product(delta_c,N);
  double denominator = dot_product(delta_gamma_t,N);
  static const double tol = vnl_math::pi/5.0;

  lambda_(0) = (-numerator/denominator)*factor;
  *Cpt_v = lambda_(0)*gamma + cam[0].c;

  //: Frontier points. 
  if (dbdif_util::near_zero(numerator,tol)) {
  //    vcl_cout << "Frontier point detected; c_t\\cdot\\N: ";
    return false;
  }

  if (dbdif_util::near_zero(lambda_(0)) || dbdif_util::near_zero(1.0/lambda_(0))) {
    vcl_cout << "WARNING: Invalid depth" << vcl_endl;

    if (dbdif_util::near_zero(factor,tol) || dbdif_util::near_zero(1.0/factor,tol))
      vcl_cout << "Factor leads to zero or infinite depth\n";

    if (dbdif_util::near_zero(numerator,tol))
      vcl_cout << "Numerator near zero; ";

    if (dbdif_util::near_zero(denominator,tol)) {
      vcl_cout << "Denominator near zero:\n";
      vcl_cout << "denom:  " << denominator
        << " numer: " << numerator  << vcl_endl
        << " delta_gamma_t: " << delta_gamma_t 
        << " N: " << N << vcl_endl;
    }

    vcl_cout << vcl_endl;

    return false;
  }

  return true;
}

//: input are vectors of matching image points in 2D image coordinates
//
void dbdif_rig::
reconstruct_3d_occluding_contour(
  vcl_vector<dbdif_vector_3d> *Crv3d,
  vcl_vector<bool> &valid,
  const vcl_vector<vsol_point_2d_sptr> &crv1, 
  const vcl_vector<vsol_point_2d_sptr> &crv2)
{
  assert(crv1.size() == crv2.size());

  // convert to world coordinates
  vcl_vector<dbdif_vector_3d> crv1_v, crv2_v;

  crv1_v.resize(crv1.size()); crv2_v.resize(crv1.size());

  for (unsigned i=0; i<crv1.size(); ++i) {
    cam[0].get_gama(crv1[i]->x(),crv1[i]->y(),&(crv1_v[i]));
    cam[1].get_gama(crv2[i]->x(),crv2[i]->y(),&(crv2_v[i]));
  }

  reconstruct_3d_occluding_contour(Crv3d,valid,crv1_v,crv2_v);
}

void dbdif_rig::
reconstruct_3d_occluding_contour(
  vcl_vector<dbdif_vector_3d> *Crv3d,
  vcl_vector<bool> &valid,
  const vcl_vector<vsol_point_2d_sptr> &crv1, 
  const vcl_vector<vsol_point_2d_sptr> &crv2,
  const dbdif_vector_3d &dC
  )
{
  assert(crv1.size() == crv2.size());

  // convert to world coordinates
  vcl_vector<dbdif_vector_3d> crv1_v, crv2_v;

  crv1_v.resize(crv1.size()); crv2_v.resize(crv1.size());

  for (unsigned i=0; i<crv1.size(); ++i) {
    cam[0].get_gama(crv1[i]->x(),crv1[i]->y(),&(crv1_v[i]));
    cam[1].get_gama(crv2[i]->x(),crv2[i]->y(),&(crv2_v[i]));
  }

  reconstruct_3d_occluding_contour(Crv3d,valid,crv1_v,crv2_v,dC);
}

//: input are vectors of matching image points in 3D world coordinates
// Order is assumed in vector "crv1" at least.  crv2 is not necessarily ordered,
// though given small baseline, and a correct differentiable curve matching, it
// should be.
//
// \param[inout] valid : has the same size as Crv3d, and validity[i] is true if
//                        Crv3d[i] is an acceptable reconstruction. As an input,
//                        valid[i] indicates whether point[i] in crv1 has valid
//                        match in crv2.
//
void dbdif_rig::
reconstruct_3d_occluding_contour(
  vcl_vector<dbdif_vector_3d> *Crv3d,
  vcl_vector<bool> &valid,
  const vcl_vector<dbdif_vector_3d> &crv1, 
  const vcl_vector<dbdif_vector_3d> &crv2)
{
  assert(crv1.size() == crv2.size());
  Crv3d->resize(crv1.size());
  if (valid.size() != crv1.size())
    valid.resize(crv1.size());

  //: Our numerical method, for now, is upwind (kind of dumb)
  for (unsigned i=0; i<crv1.size()-1; ++i) {
    if (valid[i]) {
      dbdif_vector_3d gamma   = crv1[i];
      dbdif_vector_3d gamma_s = crv1[i+1] - crv1[i];
      dbdif_vector_3d gamma_t = crv2[i]   - crv1[i];

      valid[i] = reconstruct_occluding_contour_point( gamma, gamma_s, gamma_t, &((*Crv3d)[i]) );
    }
  }

  unsigned i = crv1.size()-1;
  if (valid[i]) {
      dbdif_vector_3d gamma   = crv1[i];
      dbdif_vector_3d gamma_s = crv1[i] - crv1[i-1];
      dbdif_vector_3d gamma_t = crv2[i]   - crv1[i];

      valid[i] = reconstruct_occluding_contour_point( gamma, gamma_s, gamma_t, &((*Crv3d)[i]) );
  }
}

//: input are vectors of matching image points in 3D world coordinates
// Order is assumed in vector "crv1" at least.  crv2 is not necessarily ordered,
// though given small baseline, and a correct differentiable curve matching, it
// should be.
//
// \param[inout] valid : has the same size as Crv3d, and validity[i] is true if
//                        Crv3d[i] is an acceptable reconstruction. As an input,
//                        valid[i] indicates whether point[i] in crv1 has valid
//                        match in crv2.
// \param[in] dC : estimate of tangent vector (or delta C) to camera path
//
void dbdif_rig::
reconstruct_3d_occluding_contour(
  vcl_vector<dbdif_vector_3d> *Crv3d,
  vcl_vector<bool> &valid,
  const vcl_vector<dbdif_vector_3d> &crv1, 
  const vcl_vector<dbdif_vector_3d> &crv2,
  const dbdif_vector_3d &dC
  )
{
  assert(crv1.size() == crv2.size());
  Crv3d->resize(crv1.size());
  if (valid.size() != crv1.size())
    valid.resize(crv1.size());

  //: Our numerical method, for now, is upwind (kind of dumb)
  for (unsigned i=0; i<crv1.size()-1; ++i) {
    if (valid[i]) {
      dbdif_vector_3d gamma   = crv1[i];
      dbdif_vector_3d gamma_s = crv1[i+1] - crv1[i];
      dbdif_vector_3d gamma_t = crv2[i]   - crv1[i];

      valid[i] = reconstruct_occluding_contour_point( gamma, gamma_s, gamma_t, &((*Crv3d)[i]), dC);
    }
  }

  unsigned i = crv1.size()-1;
  if (valid[i]) {
      dbdif_vector_3d gamma   = crv1[i];
      dbdif_vector_3d gamma_s = crv1[i] - crv1[i-1];
      dbdif_vector_3d gamma_t = crv2[i]   - crv1[i];

      valid[i] = reconstruct_occluding_contour_point( gamma, gamma_s, gamma_t, &((*Crv3d)[i]), dC );
  }
}

double dbdif_rig::
angle_with_epipolar_plane(const dbdif_vector_3d &v_world, const dbdif_vector_3d &gama) const
{
  dbdif_vector_3d Nep = vnl_cross_3d((cam[0].c - cam[1].c), gama);
  
  assert(!dbdif_util::near_zero(Nep.two_norm()) &&
      dbdif_util::near_zero(cam[0].F.two_norm()-1,1e-5) && 
      !dbdif_util::near_zero(v_world.two_norm()));

  return vcl_acos(dbdif_util::clump_to_acos(dot_product(v_world,Nep)/(v_world.two_norm()*Nep.two_norm())));
}

double dbdif_rig:: 
angle_with_epipolar_line(
      const dbdif_vector_3d &v_img, 
      const dbdif_vector_3d &pos_img, 
      const vpgl_fundamental_matrix<double> &F)
{
  dbdif_vector_2d epi_line_v;
  // - get direction of epipolar line through v_img

  vgl_homg_line_2d<double> ep_l = 
  dbdif_rig::l_epipolar_line(
      F, vgl_homg_point_2d<double>(pos_img[0],pos_img[1]) );

  assert(!dbdif_util::near_zero(ep_l.c()));

  epi_line_v[0] = ep_l.a()/ep_l.c();
  epi_line_v[1] = ep_l.b()/ep_l.c();

  double epipolar_angle = 
    vcl_acos(dbdif_util::clump_to_acos( (epi_line_v[0]*v_img[0] + epi_line_v[1]*v_img[1])/(v_img.two_norm()*epi_line_v.two_norm()) ));

  if (epipolar_angle > vnl_math::pi/2.0)
    epipolar_angle = vnl_math::pi - epipolar_angle;

  return vnl_math::pi/2.0 - epipolar_angle;
}

//: Compute epipolar line in left image from an epipolar line in the right
// image, given fundamental matrix from left to right image. Zisserman 2nd
// edition p. 247
//
vgl_homg_line_2d<double> dbdif_rig::
l_epipolar_line(
    const vpgl_fundamental_matrix<double> &F, 
    const vgl_homg_line_2d<double> &ep_r)
{

  vgl_homg_point_2d<double> e, e_prime;
  
  F.get_epipoles(e,e_prime);

  // The following is a line guaranteed not to pass through e_prime
  vgl_homg_line_2d<double> e_prime_line(e_prime.x(), e_prime.y(), e_prime.w());

  // The intersection of this line with ep_r provides a point, different
  // from the epipole, that is in ep_r. (remember: in the projective plane, two
  // lines always intersect). Now use the fundamental matrix to
  // find ep_l from this point.

  vnl_vector_fixed<double,3> ep_l =
    F.get_matrix().transpose() * 
    vgl_homg_operators_2d<double>::get_vector(
        vgl_homg_operators_2d<double>::intersection( e_prime_line ,ep_r));

  return vgl_homg_line_2d<double>(ep_l[0], ep_l[1], ep_l[2]);
}

//: Compute epipolar line in left image from a point in the left
// image, given fundamental matrix from left to right image. 
//
vgl_homg_line_2d<double> dbdif_rig::
l_epipolar_line(
    const vpgl_fundamental_matrix<double> &F, 
    const vgl_homg_point_2d<double>& pl
    )
{
  vgl_homg_point_2d<double> e, e_prime;
  F.get_epipoles(e,e_prime);
  return vgl_homg_operators_2d<double>::join(pl,e);
}
