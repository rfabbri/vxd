#include "dbdif_analytic.h"
#include <vnl/vnl_cross.h>
#include <vnl/vnl_random.h>
#include <vnl/vnl_rotation_matrix.h>

//: Shorthand without the tangent.
void dbdif_analytic::
sphere_occluding_contour(
    double rho,
    const dbdif_vector_3d &s0,
    const dbdif_vector_3d &c,
    vcl_vector<dbdif_vector_3d> &Gamma,
    dbdif_vector_3d &Gamma_center,
    double &Gamma_radius
    )
{
  vcl_vector<dbdif_3rd_order_point_3d> crv3d;
  sphere_occluding_contour(rho, s0, c, crv3d, Gamma_center, Gamma_radius);
  Gamma.resize(crv3d.size());
  for (unsigned i=0; i < crv3d.size(); ++i)
    Gamma[i] = crv3d[i].Gama;
}

//: Generates the occluding contour of an analytic sphere, as seen from a given camera
// center.
// \param[in] rho - sphere radius
// \param[in] s0 - sphere center
// \param[in] c - camera center
// \param[out] Gamma_radius - radius of occluding contour
// \param[out] crv3d - the 1st order geometry of the 3D curve. The higher order
// terms are ignored.
//
void dbdif_analytic::
sphere_occluding_contour(
    double rho,
    const dbdif_vector_3d &s0,
    const dbdif_vector_3d &c,
    vcl_vector<dbdif_3rd_order_point_3d> &crv3d,
    dbdif_vector_3d &Gamma_center,
    double &Gamma_radius
    )
{
  dbdif_vector_3d nn = s0 - c;

  // choose the largest of the coordinates; make sure it is suff. greater than
  // zero.
  double tol=1e-7;
  unsigned ntheta = 628;
  unsigned i_big;

  if ( vcl_fabs(nn(0)) > vcl_fabs(nn(1)) ) {
    i_big = 0;
  } else 
    i_big = 1;

  if ( vcl_fabs(nn(2)) > vcl_fabs(nn(i_big)) )
    i_big = 2;

  if (dbdif_util::near_zero(nn(i_big),tol)) {
    vcl_cout << "Error: camera center too close to sphere center\n";
  }


  double 
    norm_nn = nn.two_norm();

  double 
    y = norm_nn - rho*rho/norm_nn;
  
  Gamma_center = c + y*nn/norm_nn;

  double 
    aux = rho/norm_nn;

  Gamma_radius = rho*sqrt(1 - aux*aux); // "x" in my scrap notation

  double 
    dtheta = 2*vnl_math::pi / ((double)ntheta);

  crv3d.resize(ntheta);

  unsigned 
    i_o1 = (i_big+1)%3;
  unsigned 
    i_o2 = (i_big+2)%3;

  nn.normalize();

  // Define u,v orthonormal basis to the plane of the 3D circle
  dbdif_vector_3d u,v,vrad;
  u(i_o1)  = 1;
  u(i_o2)  = 0;
  u(i_big) = -nn(i_o1)/nn(i_big);
  u.normalize();

  v = vnl_cross_3d(nn,u);

  // sample from 3D circle parametric equation, which is obtained from 
  // Gamma_center, Gamma_radius, and nn a normal to the circle plane
  for (unsigned k=0; k < ntheta; ++k) {
    double theta = k*dtheta;
    dbdif_vector_3d vrad;

    double c = vcl_cos(theta);
    double s = vcl_sin(theta);

    vrad = c*u + s*v;

    vrad *= Gamma_radius;
    crv3d[k].Gama = Gamma_center + vrad;
    crv3d[k].T = -s*u + c*v;
  }
}

//:
// \param[in] radius Radius of the circle
// \param[out] curve 3D Curve geometry for each theta value parameters
// \param[out] t The actual vector of parameter values (in radians)
// \param[in] t_initial, step, range: values in degrees
void dbdif_analytic::
circle_curve(
    double radius,
    vcl_vector<dbdif_3rd_order_point_3d> &C, 
    vcl_vector<double> &t,
    double t_initial, double step, double range
    )
{
  const dbdif_vector_3d translation(0,0,0);

  circle_curve(radius, translation, C, t, t_initial, step, range);
}

//:
// \param[in] radius Radius of the circle
// \param[out] curve 3D Curve geometry for each theta value parameters
// \param[out] t The actual vector of parameter values (in radians)
// \param[in] t_initial, step, range: values in _degrees_
void dbdif_analytic::
circle_curve(
    double radius,
    const dbdif_vector_3d &translation,
    vcl_vector<dbdif_3rd_order_point_3d> &C, 
    vcl_vector<double> &t,
    double t_initial, double step, double range
    )
{

  double sz=range/step + 1;
  C.resize((unsigned) sz);
  t.resize((unsigned) sz);

  unsigned i;
  double theta=t_initial;
  for (i=0; theta<=range+t_initial; theta+=step) {
    assert (i < t.size());
     t[i] = (theta/180.0)*vnl_math::pi;

     dbdif_3rd_order_point_3d &Frame = C[i];

     Frame.Gama[0] = radius*cos(t[i])+ translation[0];
     Frame.Gama[1] = radius*sin(t[i])+ translation[1];
     Frame.Gama[2] = 0               + translation[2];

     Frame.N[0] = -radius*cos(t[i]);
     Frame.N[1] = -radius*sin(t[i]);
     Frame.N[2] = 0;
     Frame.N.normalize();

     Frame.T[0] =  Frame.N[1];
     Frame.T[1] = -Frame.N[0];
     Frame.T[2] =  Frame.N[2];

     Frame.B = vnl_cross_3d(Frame.T,Frame.N);
     Frame.K = 1/radius;

     Frame.Kdot = 0;
     Frame.Tau  = 0;

     ++i;
  }

  if (i < t.size()) { // can happen with numerical errors
    C.resize(i);
    t.resize(i);
  } else if (i > t.size()) {
    vcl_cerr << "Error: vector overflow happened in circle code\n";
    abort();
  }

}

//: Same but 2D now
// \param[in] radius Radius of the circle
// \param[out] curve 3D Curve geometry for each theta value parameters
// \param[out] t The actual vector of parameter values (in radians)
// \param[in] t_initial, step, range: values in _degrees_
void dbdif_analytic::
circle_curve(
    double radius,
    const dbdif_vector_2d &translation,
    vcl_vector<dbdif_3rd_order_point_2d> &C, 
    vcl_vector<double> &t,
    double t_initial, double step, double range
    )
{

  double sz=range/step + 1;
  C.resize((unsigned) sz);
  t.resize((unsigned) sz);

  unsigned i;
  double theta=t_initial;
  for (i=0; theta<=range+t_initial && i < t.size(); theta+=step) {
     t[i] = (theta/180.0)*vnl_math::pi;

     dbdif_3rd_order_point_2d &frame = C[i];

     frame.gama[0] = radius*cos(t[i])+ translation[0];
     frame.gama[1] = radius*sin(t[i])+ translation[1];
     frame.gama[2] = 0; //:< not used

     frame.n[0] = -radius*cos(t[i]);
     frame.n[1] = -radius*sin(t[i]);
     frame.n[2] = 0;
     frame.n.normalize();

     frame.t[0] =  frame.n[1];
     frame.t[1] = -frame.n[0];
     frame.t[2] =  0;

     frame.k = 1/radius;
     frame.kdot = 0;

     ++i;
  }

  if (i < t.size()) { // can happen with numerical errors
    C.resize(i);
    t.resize(i);
  } else if (i > t.size()) {
    vcl_cerr << "Error: vector overflow happened in circle code\n";
    abort();
  }

}

//: Same but 2D now
// \param[in] radius Radius of the circle
// \param[out] curve 3D Curve geometry for each theta value parameters
// \param[out] t The actual vector of parameter values (in radians)
// \param[in] t_initial, step, range: values in _degrees_
void dbdif_analytic::
ellipse(
    double a,
    double b,
    const dbdif_vector_2d &translation,
    vcl_vector<dbdif_3rd_order_point_2d> &C, 
    vcl_vector<double> &t,
    double t_initial, double step, double range
    )
{

  double sz=range/step + 1;
  C.resize((unsigned) sz);
  t.resize((unsigned) sz);

  double ab = a*b;
  double b2 = b*b;
  double a2 = a*a;
  unsigned i;
  double theta=t_initial;
  for (i=0; theta<=range+t_initial && i < t.size(); theta+=step) {
     t[i] = (theta/180.0)*vnl_math::pi;

     double cost = vcl_cos(t[i]);
     double sint = vcl_sin(t[i]);
     double cost2 = cost*cost;
     double g = vcl_sqrt(a2*sint*sint + b2*cost2);

     dbdif_3rd_order_point_2d &frame = C[i];

     frame.gama[0] = a*cost+ translation[0];
     frame.gama[1] = b*sint+ translation[1];
     frame.gama[2] = 0; //:< not used

     frame.t[0] =  -a*sint/g;
     frame.t[1] =   b*cost/g;
     frame.t[2] =   0;


     frame.n[0] = -frame.t[1];
     frame.n[1] =  frame.t[0];
     frame.n[2] =  0;

     double denom = b2*cost2 + a2*(1.0 - cost2);
     double denom3 = denom*denom*denom;
     double denom_1_5 = vcl_sqrt(denom3);
     frame.k = ab/denom_1_5;

     frame.kdot = -3*ab*cost*sint*(a2 - b2);

     denom *= denom_1_5;

     frame.kdot /= (denom*g);

     ++i;
  }

  if (i < t.size()) { // can happen with numerical errors
    C.resize(i);
    t.resize(i);
  } else if (i > t.size()) {
    vcl_cerr << "Error: vector overflow happened in circle code\n";
    abort();
  }

}

void dbdif_analytic::
ellipse(
    double a,
    double b,
    const dbdif_vector_3d &translation,
    vcl_vector<dbdif_3rd_order_point_3d> &C, 
    vcl_vector<double> &t,
    double t_initial, double step, double range
    )
{
  dbdif_vector_2d translation_2(0,0);
  vcl_vector<dbdif_3rd_order_point_2d> c2d; 

  ellipse(a, b, translation_2, c2d, t, t_initial, step, range);
  C.resize(c2d.size());

  for (unsigned  i=0; i < c2d.size(); ++i) {
    C[i].Gama[0] = c2d[i].gama[0] + translation[0];
    C[i].Gama[1] = c2d[i].gama[1] + translation[1];
    C[i].Gama[2] = translation[2];

    C[i].T[0] = c2d[i].t[0];
    C[i].T[1] = c2d[i].t[1];
    C[i].T[2] = 0;

    dbdif_vector_3d Ntmp;
    Ntmp[0] = c2d[i].n[0];
    Ntmp[1] = c2d[i].n[1];
    Ntmp[2] = 0;

    C[i].set_normal(Ntmp);

    C[i].K = c2d[i].k;
    C[i].Kdot = c2d[i].kdot;

    C[i].Tau = 0;
  }
}

//: Defines the curve (rcos(t), rsint(t), alpha*t/2pi)
// \param[in] radius Radius of the circle
// \param[in] alpha: controls the pitch. Equals the increase in Z per revolution
// \param[out] curve 3D Curve geometry for each theta value parameters
// \param[out] t The actual vector of parameter values (in radians)
// \param[in] t_initial, step, range: values in degrees
void dbdif_analytic::
helix_curve(
    double radius, double alpha,
    dbdif_vector_3d &translation,
    vcl_vector<dbdif_3rd_order_point_3d> &C, 
    vcl_vector<double> &t,
    double t_initial, double step, double range
    )
{

  double sz=range/step + 1;
  C.resize((unsigned) sz);
  t.resize((unsigned) sz);

  alpha /=(2*vnl_math::pi);

  double theta=t_initial;
  unsigned i;
  for (i=0; theta<range+t_initial; theta+=step ) {
    assert (i < t.size());
     t[i] = (theta/180.0)*vnl_math::pi;

     dbdif_3rd_order_point_3d &Frame = C[i];

     Frame.Gama[0] = radius*cos(t[i])      + translation[0];
     Frame.Gama[1] = radius*sin(t[i])      + translation[1];
     Frame.Gama[2] = alpha * (t[i]-t_initial) + translation[2];

     Frame.T[0] =  -sin(t[i]);
     Frame.T[1] =   cos(t[i]);
     Frame.T[2] =  alpha/radius;
     Frame.T.normalize();

     Frame.N[0] =  -cos(t[i]);
     Frame.N[1] =  -sin(t[i]);
     Frame.N[2] =  0;


     Frame.B = vnl_cross_3d(Frame.T,Frame.N);

     Frame.K = 1. / ((alpha*alpha +1) * radius);
     Frame.Tau = alpha*Frame.K;

     Frame.Kdot = 0;
     ++i;
  }

  if (i < t.size()) { // can happen with numerical errors
    C.resize(i);
    t.resize(i);
  } else if (i > t.size()) {
    vcl_cerr << "Error: vector overflow happened in circle code\n";
    abort();
  }
}


//: Defines the curve (rcos(t), rsin(t), rcos(2*t))
// \param[in] radius Radius of the circle
// \param[out] curve 3D Curve geometry for each theta value parameters
// \param[out] t The actual vector of parameter values (in radians)
// \param[in] t_initial, step, range: values in degrees
void dbdif_analytic::
space_curve1(
    double radius, 
    dbdif_vector_3d &translation,
    vcl_vector<dbdif_3rd_order_point_3d> &C, 
    vcl_vector<double> &t,
    double t_initial, double step, double range
    )
{

  double theta=t_initial;
  double sz=range/step + 1;
  C.resize((unsigned) sz);
  t.resize((unsigned) sz);
  unsigned i;
  for (i=0; theta<range; theta+=step, ++i) {      assert (i < t.size());
     t[i] = (theta/180.0)*vnl_math::pi;

     dbdif_3rd_order_point_3d &Frame = C[i];

     //: Cache computations --------------------
     double cos_t = cos(t[i]); double cos_2t = cos(2*t[i]); 
     double cos_t_sq = cos_t*cos_t;
     double cos_t_4 = cos_t*cos_t*cos_t*cos_t;

     double sin_t = sin(t[i]); double sin_2t = sin(2*t[i]);

     double aux2 = sqrt(16.*(cos_t_sq -cos_t_4) + 1.);

     Frame.Gama[0] = radius*cos_t  + translation[0];
     Frame.Gama[1] = radius*sin_t  + translation[1];
     Frame.Gama[2] = radius*cos_2t + translation[2];

     Frame.T[0] =  -sin_t / aux2;
     Frame.T[1] =   cos_t / aux2;
     Frame.T[2] =   - 2.*sin_2t / aux2;


     /* code from Maple: */

     Frame.K = sqrt(0.48e2 * vcl_pow(vcl_cos(t[i]), 0.4e1) - 0.48e2 * vcl_pow(vcl_cos(t[i]), 0.2e1) + 0.17e2) * vcl_pow(0.16e2 * vcl_pow(vcl_cos(t[i]), 0.2e1) + 0.1e1 - 0.16e2 * vcl_pow(vcl_cos(t[i]), 0.4e1), -0.3e1 / 0.2e1) / radius;

     dbdif_vector_3d Nrm;

     Nrm[0] = -0.1e1 / Frame.K * (0.17e2 + 0.16e2 * vcl_pow(vcl_cos(t[i]), 0.4e1) - 0.32e2 * vcl_pow(vcl_cos(t[i]), 0.2e1)) * vcl_cos(t[i]) / radius / (0.224e3 * vcl_pow(vcl_cos(t[i]), 0.4e1) + 0.32e2 * vcl_pow(vcl_cos(t[i]), 0.2e1) - 0.512e3 * vcl_pow(vcl_cos(t[i]), 0.6e1) + 0.1e1 + 0.256e3 * vcl_pow(vcl_cos(t[i]), 0.8e1)); 

     Nrm[1] = -0.1e1 / Frame.K * vcl_sin(t[i]) * (0.16e2 * vcl_pow(vcl_cos(t[i]), 0.4e1) + 0.1e1) / radius / (0.224e3 * vcl_pow(vcl_cos(t[i]), 0.4e1) + 0.32e2 * vcl_pow(vcl_cos(t[i]), 0.2e1) - 0.512e3 * vcl_pow(vcl_cos(t[i]), 0.6e1) + 0.1e1 + 0.256e3 * vcl_pow(vcl_cos(t[i]), 0.8e1));

     Nrm[2] = -0.4e1 / Frame.K * (0.2e1 * vcl_pow(vcl_cos(t[i]), 0.2e1) - 0.1e1) / radius / (0.224e3 * vcl_pow(vcl_cos(t[i]), 0.4e1) + 0.32e2 * vcl_pow(vcl_cos(t[i]), 0.2e1) - 0.512e3 * vcl_pow(vcl_cos(t[i]), 0.6e1) + 0.1e1 + 0.256e3 * vcl_pow(vcl_cos(t[i]), 0.8e1));

     Frame.set_normal(Nrm);

     Frame.Tau = 0.12e2 * vcl_cos(t[i]) * vcl_sin(t[i]) / (0.48e2 * vcl_pow(vcl_cos(t[i]), 0.4e1) - 0.48e2 * vcl_pow(vcl_cos(t[i]), 0.2e1) + 0.17e2) / radius;

     Frame.Kdot = 0.96e2 * vcl_sin(t[i]) * vcl_cos(t[i]) * (-0.48e2 * vcl_pow(vcl_cos(t[i]), 0.4e1) + 0.34e2 * vcl_pow(vcl_cos(t[i]), 0.2e1) - 0.9e1 + 0.32e2 * vcl_pow(vcl_cos(t[i]), 0.6e1)) * vcl_pow(radius, -0.2e1) * vcl_pow(0.48e2 * vcl_pow(vcl_cos(t[i]), 0.4e1) - 0.48e2 * vcl_pow(vcl_cos(t[i]), 0.2e1) + 0.17e2, -0.1e1 / 0.2e1) / (-0.1e1 - 0.48e2 * vcl_pow(vcl_cos(t[i]), 0.2e1) - 0.720e3 * vcl_pow(vcl_cos(t[i]), 0.4e1) - 0.2560e4 * vcl_pow(vcl_cos(t[i]), 0.6e1) + 0.11520e5 * vcl_pow(vcl_cos(t[i]), 0.8e1) - 0.12288e5 * vcl_pow(vcl_cos(t[i]), 0.10e2) + 0.4096e4 * vcl_pow(vcl_cos(t[i]), 0.12e2));

  }

  if (i < t.size()) { // can happen with numerical errors
    C.resize(i);
    t.resize(i);
  } else if (i > t.size()) {
    vcl_cerr << "Error: vector overflow happened in circle code\n";
    abort();
  }

}

// Given point and direction, and t_final
void dbdif_analytic::
line(
    dbdif_vector_3d &translation,
    dbdif_vector_3d &direction,
    vcl_vector<dbdif_3rd_order_point_3d> &C, 
    vcl_vector<double> &t,
    double t_final, double step
    )
{
  double sz=t_final/step + 1;
  C.resize((unsigned) sz);
  t.resize((unsigned) sz);
  direction.normalize();

  unsigned i;
  double lambda=0;
  for (i=0; lambda<=t_final; lambda+=step) {
    assert (i < t.size());

    dbdif_3rd_order_point_3d &Frame = C[i];

    Frame.Gama = lambda*direction + translation;

    Frame.T = direction;

    //    These are undefined:
//    Frame.N = -Frame.Gama;
//    Frame.N.normalize();
//    Frame.B = vvnl_cross_3d(Frame.T,Frame.N);
//    User must always check whether the curvature is 0 for a given curve before
//    attempting to use the normal or anything else
    Frame.K = 0;

    Frame.Kdot = 0;
    Frame.Tau  = 0;

    ++i;
  }

  if (i < t.size()) { // can happen with numerical errors
    C.resize(i);
    t.resize(i);
  } else if (i > t.size()) {
    vcl_cerr << "Error: vector overflow happened in circle code\n";
    abort();
  }
}

static double
d_sqr(double x1, double y1, double x2, double y2)
{
   double dx= x1-x2, dy = y1-y2;
   return dx*dx + dy*dy;
}

//: Make sure that, for consecutive points, 0.5 <= dist <= sqrt(2).
// It only enforces 0.5 <= dist, but occurrence of dist <= sqrt(2) is only signaled.
// \return true if all points are bellow a distance of sqrt(2)
bool dbdif_analytic::
limit_distance(
    const vcl_vector<dbdif_3rd_order_point_2d> &C, 
    vcl_vector<dbdif_3rd_order_point_2d> &C_limited)
{
  bool retval=true;
  C_limited.reserve(C.size());
  C_limited.push_back(C[0]);
  for (unsigned i=1; i < C.size(); ++i) {
    double d = d_sqr(C[i].gama[0], C[i].gama[1], C_limited.back().gama[0], C_limited.back().gama[1]);
    if (d > 2) //:< squared distance
      retval=false;
    if (d >= 0.5*0.5) {
      C_limited.push_back(C[i]);
    }
  }

  return retval;
}

static vnl_random myrand;

//: Perturb each datapoint randomly in all directions
double dbdif_analytic::
perturb( double coord, double dp)
{
  return coord + myrand.drand64(-dp,dp);
}

//: Perturb each datapoint randomly in all directions
void dbdif_analytic::
perturb( vgl_vector_2d<double> &t, double dt)
{
  dt = myrand.drand64(-dt,dt);

  double sint = vcl_sin(dt);
  double cost = vcl_cos(dt);
  t.set( cost*t.x() -sint*t.y(), 
         sint*t.x() +cost*t.y() );
}

//: rotate all points around specified  axis, by angle theta equal to \|axis\|
void dbdif_analytic::
rotate( vcl_vector<dbdif_3rd_order_point_3d> &c, const dbdif_vector_3d &axis)
{
  vnl_matrix<double> R(3,3);

  vnl_rotation_matrix(axis,  R);

  for (unsigned  i=0; i < c.size(); ++i) {
    c[i].Gama = R* (c[i].Gama);
    c[i].T = R* (c[i].T);
    c[i].N = R* (c[i].N);
    c[i].B = R* (c[i].B);
  }
}

void dbdif_analytic::
translate( vcl_vector<dbdif_3rd_order_point_3d> &c, const dbdif_vector_3d &transl)
{
  for (unsigned  i=0; i < c.size(); ++i) {
    c[i].Gama = c[i].Gama + transl;
  }
}
