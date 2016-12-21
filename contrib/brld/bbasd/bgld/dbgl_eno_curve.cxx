// This is basic/dbgl/dbgl_eno_curve.cxx
#include "dbgl_eno_curve.h"
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>
#include <limits>

dbgl_eno_curve::dbgl_eno_curve(vcl_vector<double> coefs_x, vcl_vector<double> coefs_y, double start_t, double end_t)
{
  tolerance_ = 10e-6;

  for(int i=0; i<3; i++)
  {
    coefs_x_.push_back(coefs_x[i]);
    coefs_y_.push_back(coefs_y[i]);
  }

  start_t_ = start_t;
  end_t_ = end_t;

  len_until_start_ = -1; //not set yet
  len_until_start_ = length_at(start_t_);
  len_ = length_at(end_t_);
};

dbgl_eno_curve::~dbgl_eno_curve()
{
  coefs_x_.clear();
  coefs_y_.clear();
}

double dbgl_eno_curve::s_to_t(double s, double accuracy) const
{
  // s cannot be smaller than zero or greater than the length of the curve
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double k_start = start_t_;;
  double k_end = end_t_;
  while(1)
  {
    double k_guess = (k_start + k_end) / 2;
    double len = length_at(k_guess);
    if(vcl_fabs(s-len) < accuracy || vcl_fabs(k_start-k_end) < accuracy)
      return k_guess;
    else if(s < len)
      k_end = k_guess;
    else
      k_start = k_guess;
  }

  return 0;
}

// formula obtained by using Maple
double dbgl_eno_curve::length_at(double t) const
{
  double val = 0;
  // t has to be in [start_t_, end_t_] range
  assert(t >= start_t_ - tolerance_ && t <= end_t_ + tolerance_);

  double A = 4 * (vcl_pow(coefs_x_[2], 2.0) + vcl_pow(coefs_y_[2], 2.0));
  double B = 4 * (coefs_x_[1] * coefs_x_[2] + coefs_y_[1] * coefs_y_[2]);
  double C = vcl_pow(coefs_x_[1], 2.0) + vcl_pow(coefs_y_[1], 2.0);

  if(A == 0)
  {
    if(B == 0)
    {
      val = vcl_sqrt(C) * t;
    }
    else
    {
      double temp1 = vcl_pow(C, 1.5);
      double temp2 = vcl_pow(B * t + C, 1.5);
      val = 2 * (-temp1 + temp2) / (3*B);
    }
  }
  else
  {
    double rootA = vcl_sqrt(A);
    double rootC = vcl_sqrt(C);
    double sqB = vcl_pow(B,2.0);

    double inter1 = vcl_log((B + 2 * rootC * rootA) / rootA);
    double inter2 = vcl_sqrt(A*vcl_pow(t,2.0) + B*t+ C);
    double inter3 = vcl_log((B + 2*A*t + 2*inter2*rootA) / rootA);

    if(vcl_fabs(inter1) == std::numeric_limits<double>::infinity()) inter1 = 0;
    if(vcl_fabs(inter2) == std::numeric_limits<double>::infinity()) inter2 = 0;
    if(vcl_fabs(inter3) == std::numeric_limits<double>::infinity()) inter3 = 0;

    if(my_isnan(inter1)) inter1 = 0;
    if(my_isnan(inter2)) inter2 = 0;
    if(my_isnan(inter3)) inter3 = 0;

    double temp1 = 2 * rootC * rootA * B;
    double temp2 = -sqB * inter1;
    double temp3 = 4 * C * A * inter1;
    double temp4 = -2 * inter2 * rootA * B;
    double temp5 = sqB * inter3;
    double temp6 = -4 * inter2 * vcl_pow(A,1.5) * t;
    double temp7 = -4 * C * A * inter3;

    val = (temp1+temp2+temp3+temp4+temp5+temp6+temp7) / (-8*vcl_pow(A,1.5));
  }
  
  if(len_until_start_ != -1)
    return val - len_until_start_;
  else
    return val; //enters this only for the constructor before len_until_start_ is set
}

double dbgl_eno_curve::length() const
{
  return len_;
}

vgl_point_2d<double> dbgl_eno_curve::point_at(double t) const
{
  assert(t >= start_t_ - tolerance_ && t <= end_t_ + tolerance_);
  double x = evaluate_x(t);
  double y = evaluate_y(t);
  return vgl_point_2d<double>(x, y);
}

vgl_point_2d<double> dbgl_eno_curve::point_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return point_at(t);
}

vgl_vector_2d<double> dbgl_eno_curve::tangent_at(double t) const
{
  assert(t >= start_t_ - tolerance_ && t <= end_t_ + tolerance_);
  double angle = tangent_angle_at(t);
  return vgl_vector_2d<double> (vcl_cos(angle), vcl_sin(angle));
}

vgl_vector_2d<double> dbgl_eno_curve::tangent_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return tangent_at(t);
}

double dbgl_eno_curve::tangent_angle_at(double t) const
{
  assert(t >= start_t_ - tolerance_ && t <= end_t_ + tolerance_);
  double dx = evaluate_first_derivative_x(t);
  double dy = evaluate_first_derivative_y(t);

  double angle = vcl_atan2(dy, dx);
  if(angle < 0)
    angle += 2*vnl_math::pi;

  return angle;
}

double dbgl_eno_curve::tangent_angle_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return tangent_angle_at(t);
}

double dbgl_eno_curve::curvature_at(double t) const
{
  assert(t >= start_t_ - tolerance_ && t <= end_t_ + tolerance_);
  double x_t = evaluate_first_derivative_x(t);
  double y_t = evaluate_first_derivative_y(t);

  double x_tt = evaluate_second_derivative_x(t);
  double y_tt = evaluate_second_derivative_y(t);

  double temp1 = x_t * y_tt - y_t * x_tt;
  double temp2 = vcl_pow(vcl_pow(x_t, 2.0) + vcl_pow(y_t, 2.0), 1.5);

  return temp1 / temp2;
}

double dbgl_eno_curve::curvature_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);
  double t = s_to_t(s, S_T_ACCURACY);
  return curvature_at(t);
}

double dbgl_eno_curve::evaluate_x(double t) const
{
  return coefs_x_[0] + coefs_x_[1] * t + coefs_x_[2] * vcl_pow(t, 2.0);
}

double dbgl_eno_curve::evaluate_y(double t) const
{
  return coefs_y_[0] + coefs_y_[1] * t + coefs_y_[2] * vcl_pow(t, 2.0);
}

double dbgl_eno_curve::evaluate_first_derivative_x(double t) const
{
  return coefs_x_[1] + 2 * coefs_x_[2] * t;
}

double dbgl_eno_curve::evaluate_first_derivative_y(double t) const
{
  return coefs_y_[1] + 2 * coefs_y_[2] * t;
}

double dbgl_eno_curve::evaluate_second_derivative_x(double t) const
{
  return 2 * coefs_x_[2];
}

double dbgl_eno_curve::evaluate_second_derivative_y(double t) const
{
  return 2 * coefs_y_[2];
}
