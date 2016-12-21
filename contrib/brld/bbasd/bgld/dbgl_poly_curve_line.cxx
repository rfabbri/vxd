// This is basic/dbgl/dbgl_poly_curve_line.cxx
#include "dbgl_poly_curve_line.h"
#include <vcl_cmath.h>

//: construct from two points so that s runs in [0,1]
dbgl_poly_curve_line::dbgl_poly_curve_line(vgl_point_2d<double> start, vgl_point_2d<double> end)
{
  len_ = (start-end).length();
  coefs_x_.push_back(start.x());
  coefs_x_.push_back(end.x()-start.x());

  coefs_y_.push_back(start.y());
  coefs_y_.push_back(end.y()-start.y());

  order_ = 1;
}

double dbgl_poly_curve_line::length() const 
{
  return len_;
}

vgl_point_2d<double> dbgl_poly_curve_line::point_at_length(double l) const
{
  double s = l/len_;
  return point_at(s);
}

//: Get tangent of the point at arc length s away from starting point
vgl_vector_2d<double> dbgl_poly_curve_line::tangent_at_length(double l) const
{
  double s = l/len_;
  return tangent_at(s);
}

//: Get curvature of the point at s arc length away from starting point.
double dbgl_poly_curve_line::curvature_at_length(double l) const
{
  double s = l/len_;
  return curvature_at(s);
}

//: Compute the angle using derivatives of the polynomials at given length
double dbgl_poly_curve_line::tangent_angle_at_length(double l) const
{
  double s = l/len_;
  return tangent_angle_at(s);
}
