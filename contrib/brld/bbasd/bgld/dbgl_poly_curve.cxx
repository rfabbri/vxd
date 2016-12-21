// This is basic/dbgl/dbgl_poly_curve.cxx
#include "dbgl_poly_curve.h"
#include <vcl_cmath.h>

//: polynomial is parametrized by s (runs in [-infinity,+infinity]). 
//  n --> order
//  x(s) = coef_x[0] + coef_x[1]*s + coef_x[2]*s^2 + ... + coef_x[n]*s^n 
//  y(s) = coef_y[0] + coef_y[1]*s + coef_y[2]*s^2 + ... + coef_y[n]*s^n 

//  s can run in [0-1] or in [0-length] depending on the constructor
//  constructor will know what s stands for and use accordingly

// Property functions ----------------------------------------------

//: length of the polynomial piece between start and end points
// one might implement this numerically by integration 
// assuming that s=0 is the start and s=1 is the end
// then dbgl_poly_curve won't be an abstract class anymore
//virtual double length() const = 0;  

//: Compute the point at arclength s using polynomials 
vgl_point_2d<double> dbgl_poly_curve::point_at(double s) const 
{
  return 
  vgl_point_2d<double> (evaluate_x(s), evaluate_y(s));
}

//: Compute the tangent using derivatives of the polynomials at given point
vgl_vector_2d<double> dbgl_poly_curve::tangent_at(double s) const
{
  double alpha = tangent_angle_at(s);
  return vgl_vector_2d<double>(cos(alpha), sin(alpha));
}

//: Compute the angle using derivatives of the polynomials at given point
double dbgl_poly_curve::tangent_angle_at(double s) const 
{
  double tangent_x = evaluate_derivative_x(s);
  double tangent_y = evaluate_derivative_y(s);

  return vcl_atan2(tangent_y, tangent_x);
}

//: evaluate at s
double dbgl_poly_curve::evaluate_x(double s) const 
{
  double sum=0;
  for (int i = 0; i<order_+1; i++) 
    sum += coefs_x_[i]*vcl_pow(s, i);
  return sum;
}

double dbgl_poly_curve::evaluate_y(double s) const
{
  double sum=0;
  for (int i = 0; i<order_+1; i++) 
    sum += coefs_y_[i]*vcl_pow(s, i);
  return sum;
}

double dbgl_poly_curve::evaluate_derivative_x(double s) const
{
  double sum=0;
  for (int i = 1; i<order_+1; i++) 
    sum += i*coefs_x_[i]*vcl_pow(s, i-1);
  return sum;
}

double dbgl_poly_curve::evaluate_derivative_y(double s) const
{
  double sum=0;
  for (int i = 1; i<order_+1; i++) 
    sum += i*coefs_y_[i]*vcl_pow(s, i-1);
  return sum;
}

//: comparison operator.
//  Comparison is on the curve, two parametric curves are identical if their
//  equations are equivalent
bool dbgl_poly_curve::operator==(dbgl_poly_curve const& c) const 
{
  return (coefs_x_== c.coefficients_x() && 
          coefs_y_== c.coefficients_y() && 
          order_ == c.order());
}

dbgl_poly_curve& dbgl_poly_curve::operator=( dbgl_poly_curve const& c) 
{
  order_ = c.order();
  len_ = c.length();
  coefs_x_ = c.coefficients_x();
  coefs_y_ = c.coefficients_y();
  return *this;
}
