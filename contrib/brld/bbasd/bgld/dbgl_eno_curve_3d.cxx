// This is basic/dbgl/dbgl_eno_curve_3d.cxx
#include "dbgl_eno_curve_3d.h"
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_analytic_integrant.h>
#include <vnl/algo/vnl_adaptsimpson_integral.h>

dbgl_eno_curve_3d::dbgl_eno_curve_3d(vcl_vector<double> coefs_x, 
                                     vcl_vector<double> coefs_y, 
                                     vcl_vector<double> coefs_z,
                                     double start_t,
                                     double end_t)
{
  tolerance_ = 1e-6;

  for(int i=0; i<4; i++)
  {
    coefs_x_.push_back(coefs_x[i]);
    coefs_y_.push_back(coefs_y[i]);
    coefs_z_.push_back(coefs_z[i]);
  }
  start_t_ = start_t;
  end_t_ = end_t;

  len_ = length_at_t(end_t_);
};

dbgl_eno_curve_3d::~dbgl_eno_curve_3d()
{
  coefs_x_.clear();
  coefs_y_.clear();
  coefs_z_.clear();
}

// Algorithm is the same as in 2D case except the initialization of k_start and k_end
double dbgl_eno_curve_3d::s_to_t(double s, double accuracy) const
{
  // s cannot be smaller than zero or greater than the length of the curve
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double k_start = start_t_;
  double k_end = end_t_;
  while(1)
  {
    double k_guess = (k_start + k_end) / 2;
    double len = length_at_t(k_guess);
    if(vcl_fabs(s-len) < accuracy || vcl_fabs(k_start-k_end) < accuracy)
      return k_guess;
    else if(s < len)
      k_end = k_guess;
    else
      k_start = k_guess;
  }
  return 0;
}

// Used with Adaptive Simpson Integration
class my_integrant : public vnl_analytic_integrant
{
public:
  my_integrant(double A, double B, double C, double D, double E)
    : A_(A), B_(B), C_(C), D_(D), E_(E) {};

  double f_(double t)
  { 
    return vcl_sqrt(A_ * vcl_pow(t, 4.0) + B_ * vcl_pow(t, 3.0) + C_ * vcl_pow(t, 2.0) + D_ * t + E_);
  }
protected:
  double A_, B_, C_, D_, E_;
};

// Computes the length of the curve from start_t_ to the given t
// We use Adaptive Simpson integration to compute the length because Maple and Mathematica
// could not compute an explicit formula for the arclength, i.e. the integration of 
// sqrt(A*t^4+B*t^3+C*t^2+D*t+E) from t0 to t1
double dbgl_eno_curve_3d::length_at_t(double t) const
{
  assert((t >= start_t_-tolerance_) && (t <= end_t_+tolerance_));

  double a0 = coefs_x_[0];
  double a1 = coefs_x_[1];
  double a2 = coefs_x_[2];
  double a3 = coefs_x_[3];

  double b0 = coefs_y_[0];
  double b1 = coefs_y_[1];
  double b2 = coefs_y_[2];
  double b3 = coefs_y_[3];

  double c0 = coefs_z_[0];
  double c1 = coefs_z_[1];
  double c2 = coefs_z_[2];
  double c3 = coefs_z_[3];

  double A = 9 * (vcl_pow(a3,2.0) + vcl_pow(b3,2.0) + vcl_pow(c3,2.0));
  double B = 12 * (a2*a3 + b2*b3 + c2*c3);
  double C = 6 * (a1*a3 + b1*b3 + c1*c3) + 4 * (vcl_pow(a2,2.0) + vcl_pow(b2,2.0) + vcl_pow(c2,2.0));
  double D = 4 * (a1*a2 + b1*b2 + c1*c2);
  double E = vcl_pow(a1,2.0) + vcl_pow(b1,2.0) + vcl_pow(c1,2.0);

  my_integrant f(A, B, C, D, E);
  vnl_adaptsimpson_integral simpson_integral;
  double val = simpson_integral.integral(&f, start_t_, t, 1e-6);

  return val;
}

double dbgl_eno_curve_3d::length() const
{
  return len_;
}

vgl_point_3d<double> dbgl_eno_curve_3d::point_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return point_at_length(s * len_);
}

vgl_point_3d<double> dbgl_eno_curve_3d::point_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return point_at_t(t);
}

vgl_point_3d<double> dbgl_eno_curve_3d::point_at_t(double t) const
{
  assert((t >= start_t_-tolerance_) && (t <= end_t_+tolerance_));

  double x = evaluate_x(t);
  double y = evaluate_y(t);
  double z = evaluate_z(t);

  return vgl_point_3d<double>(x, y, z);
}

vgl_vector_3d<double> dbgl_eno_curve_3d::tangent_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return tangent_at_length(s * len_);
}

vgl_vector_3d<double> dbgl_eno_curve_3d::tangent_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return tangent_at_t(t);
}

vgl_vector_3d<double> dbgl_eno_curve_3d::tangent_at_t(double t) const
{
  assert((t >= start_t_-tolerance_) && (t <= end_t_+tolerance_));
  struct tangent_angles ta = tangent_angles_at_t(t);
  double phi = ta.phi_;
  double theta = ta.theta_;
  return vgl_vector_3d<double> (vcl_sin(phi)*vcl_cos(theta), 
                                vcl_sin(phi)*vcl_sin(theta), 
                                vcl_cos(phi));
}

vgl_vector_3d<double> dbgl_eno_curve_3d::normal_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return normal_at_length(s * len_);
}

vgl_vector_3d<double> dbgl_eno_curve_3d::normal_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return normal_at_t(t);
}

vgl_vector_3d<double> dbgl_eno_curve_3d::normal_at_t(double t) const
{
  assert((t >= start_t_-tolerance_) && (t <= end_t_+tolerance_));

  double dx = evaluate_first_derivative_x(t);
  double dy = evaluate_first_derivative_y(t);
  double dz = evaluate_first_derivative_z(t);

  double ddx = evaluate_second_derivative_x(t);
  double ddy = evaluate_second_derivative_y(t);
  double ddz = evaluate_second_derivative_z(t);

  vgl_vector_3d<double> df(dx, dy, dz);
  vgl_vector_3d<double> ddf(ddx, ddy, ddz);

  vgl_vector_3d<double> temp1(ddf * df.length());
  vgl_vector_3d<double> temp2(df * dot_product(df, ddf) / df.length());
  vgl_vector_3d<double> temp3(temp1 - temp2);

  vgl_vector_3d<double> normal(normalize(temp3));

  return normal;
}

struct tangent_angles dbgl_eno_curve_3d::tangent_angles_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return tangent_angles_at_length(s * len_);
}

struct tangent_angles dbgl_eno_curve_3d::tangent_angles_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return tangent_angles_at_t(t);
}

struct tangent_angles dbgl_eno_curve_3d::tangent_angles_at_t(double t) const
{
  assert((t >= start_t_-tolerance_) && (t <= end_t_+tolerance_));
  double dx = evaluate_first_derivative_x(t);
  double dy = evaluate_first_derivative_y(t);
  double dz = evaluate_first_derivative_z(t);

  struct tangent_angles ta;

  ta.theta_ = vcl_atan2(dy, dx);
  if(ta.theta_ < 0)
    ta.theta_ += 2*vnl_math::pi;

  ta.phi_ = vcl_atan2(vcl_sqrt(vcl_pow(dx,2.0) + vcl_pow(dy,2.0)), dz);
  if(ta.phi_ < 0)
    ta.phi_ += 2*vnl_math::pi;

  return ta;
}

double dbgl_eno_curve_3d::curvature_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return curvature_at_length(s * len_);
}

double dbgl_eno_curve_3d::curvature_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return curvature_at_t(t);
}

// Using the formula:
// K = |c' x c''| / (|c'|^3) where c is the curve, x denotes vector product
double dbgl_eno_curve_3d::curvature_at_t(double t) const
{
  assert((t >= start_t_-tolerance_) && (t <= end_t_+tolerance_));

  // c' components
  double u1 = evaluate_first_derivative_x(t);
  double u2 = evaluate_first_derivative_y(t);
  double u3 = evaluate_first_derivative_z(t);
  // c'' components
  double v1 = evaluate_second_derivative_x(t);
  double v2 = evaluate_second_derivative_y(t);
  double v3 = evaluate_second_derivative_z(t);
  // c' x c'' components
  double val1 = u2*v3 - u3*v2;
  double val2 = -u1*v3 + u3*v1;
  double val3 = u1*v2 - u2*v1;
  // |c' x c''| value
  double n = vcl_sqrt(vcl_pow(val1, 2.0) + vcl_pow(val2, 2.0) + vcl_pow(val3, 2.0));
  // |c'| value
  double d = vcl_sqrt(vcl_pow(u1, 2.0) + vcl_pow(u2, 2.0) + vcl_pow(u3, 2.0));

  return n / (vcl_pow(d, 3.0));
}

double dbgl_eno_curve_3d::torsion_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return torsion_at_length(s * len_);
}

double dbgl_eno_curve_3d::torsion_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return torsion_at_t(t);
}

// Using the formula:
// T = ((c' x c'').c''') / (|c' x c''|^2) where c is the curve,
// x denotes vector product, . denotes dot product
double dbgl_eno_curve_3d::torsion_at_t(double t) const
{
  assert((t >= start_t_-tolerance_) && (t <= end_t_+tolerance_));

  // c' components
  double u1 = evaluate_first_derivative_x(t);
  double u2 = evaluate_first_derivative_y(t);
  double u3 = evaluate_first_derivative_z(t);
  // c'' components
  double v1 = evaluate_second_derivative_x(t);
  double v2 = evaluate_second_derivative_y(t);
  double v3 = evaluate_second_derivative_z(t);
  // c''' components
  double t1 = evaluate_third_derivative_x(t);
  double t2 = evaluate_third_derivative_y(t);
  double t3 = evaluate_third_derivative_z(t);
  // c' x c'' components
  double val1 = u2*v3 - u3*v2;
  double val2 = -u1*v3 + u3*v1;
  double val3 = u1*v2 - u2*v1;
  // |c' x c''|^2 value
  double d = vcl_pow(val1, 2.0) + vcl_pow(val2, 2.0) + vcl_pow(val3, 2.0);
  // (c' x c'').c''' computation
  double n = val1 * t1 + val2 * t2 + val3 * t3;

  return n/d;
}

double dbgl_eno_curve_3d::evaluate_x(double t) const
{
  return coefs_x_[0] + coefs_x_[1] * t + coefs_x_[2] * vcl_pow(t, 2.0) + coefs_x_[3] * vcl_pow(t, 3.0);
}

double dbgl_eno_curve_3d::evaluate_y(double t) const
{
  return coefs_y_[0] + coefs_y_[1] * t + coefs_y_[2] * vcl_pow(t, 2.0) + coefs_y_[3] * vcl_pow(t, 3.0);
}

double dbgl_eno_curve_3d::evaluate_z(double t) const
{
  return coefs_z_[0] + coefs_z_[1] * t + coefs_z_[2] * vcl_pow(t, 2.0) + coefs_z_[3] * vcl_pow(t, 3.0);
}

double dbgl_eno_curve_3d::evaluate_first_derivative_x(double t) const
{
  return coefs_x_[1] + 2 * coefs_x_[2] * t + 3 * coefs_x_[3] * vcl_pow(t, 2.0);
}

double dbgl_eno_curve_3d::evaluate_first_derivative_y(double t) const
{
  return coefs_y_[1] + 2 * coefs_y_[2] * t + 3 * coefs_y_[3] * vcl_pow(t, 2.0);
}

double dbgl_eno_curve_3d::evaluate_first_derivative_z(double t) const
{
  return coefs_z_[1] + 2 * coefs_z_[2] * t + 3 * coefs_z_[3] * vcl_pow(t, 2.0);
}

double dbgl_eno_curve_3d::evaluate_second_derivative_x(double t) const
{
  return 2 * coefs_x_[2] + 6 * coefs_x_[3] * t;
}

double dbgl_eno_curve_3d::evaluate_second_derivative_y(double t) const
{
  return 2 * coefs_y_[2] + 6 * coefs_y_[3] * t;
}

double dbgl_eno_curve_3d::evaluate_second_derivative_z(double t) const
{
  return 2 * coefs_z_[2] + 6 * coefs_z_[3] * t;
}

double dbgl_eno_curve_3d::evaluate_third_derivative_x(double t) const
{
  return 6 * coefs_x_[3];
}

double dbgl_eno_curve_3d::evaluate_third_derivative_y(double t) const
{
  return 6 * coefs_y_[3];
}

double dbgl_eno_curve_3d::evaluate_third_derivative_z(double t) const
{
  return 6 * coefs_z_[3];
}
