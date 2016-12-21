// This is basic/dbgl/dbgl_linear_curve_3d.cxx
#include "dbgl_linear_curve_3d.h"
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_analytic_integrant.h>
#include <vnl/algo/vnl_adaptsimpson_integral.h>

dbgl_linear_curve_3d::dbgl_linear_curve_3d(vgl_vector_3d<double> start,
                                           vgl_vector_3d<double> end)
{
  tolerance_ = 10e-6;

  start_ = start;
  end_ = end;

  vgl_vector_3d<double> diff(end-start);

  len_ = diff.length();

  intercepts_[0] = start.x();
  intercepts_[1] = start.y();
  intercepts_[2] = start.z();

  slopes_[0] = diff.x() / len_;
  slopes_[1] = diff.y() / len_;
  slopes_[2] = diff.z() / len_;
};

dbgl_linear_curve_3d::~dbgl_linear_curve_3d()
{
}

double dbgl_linear_curve_3d::length() const
{
  return len_;
}

vgl_point_3d<double> dbgl_linear_curve_3d::point_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return point_at_length(s * len_);
}

vgl_point_3d<double> dbgl_linear_curve_3d::point_at_length(double s) const
{
  assert(s >= -tolerance_ && s <= len_ + tolerance_);

  double x = slopes_[0] * s + intercepts_[0];
  double y = slopes_[1] * s + intercepts_[1];
  double z = slopes_[2] * s + intercepts_[2];
  
  return vgl_point_3d<double> (x,y,z);
}

vgl_vector_3d<double> dbgl_linear_curve_3d::tangent_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return tangent_at_length(s * len_);
}

vgl_vector_3d<double> dbgl_linear_curve_3d::tangent_at_length(double s) const
{
  assert((s >= -tolerance_) && (s <= len_+tolerance_));
//  struct tangent_angles ta = tangent_angles_at_length(s);
//  double phi = ta.phi_;
//  double theta = ta.theta_;
//  return vgl_vector_3d<double> (vcl_sin(phi)*vcl_cos(theta), 
//                                vcl_sin(phi)*vcl_sin(theta), 
//                                vcl_cos(phi));
  vgl_vector_3d<double> tangent(slopes_[0], slopes_[1], slopes_[2]);
  normalize(tangent);
  return tangent;
}

vgl_vector_3d<double> dbgl_linear_curve_3d::normal_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return normal_at_length(s * len_);
}

vgl_vector_3d<double> dbgl_linear_curve_3d::normal_at_length(double s) const
{
  vcl_cout << "dbgl_linear_curve_3d::normal_at_length NOT IMPLEMENTED!" << vcl_endl;
  return vgl_vector_3d<double>(0, 0, 0);
}

struct tangent_angles dbgl_linear_curve_3d::tangent_angles_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return tangent_angles_at_length(s * len_);
}

struct tangent_angles dbgl_linear_curve_3d::tangent_angles_at_length(double s) const
{
  assert((s >= -tolerance_) && (s <= len_+tolerance_));
  double dx = slopes_[0];
  double dy = slopes_[1];
  double dz = slopes_[2];

  struct tangent_angles ta;

  ta.theta_ = vcl_atan2(dy, dx);
  if(ta.theta_ < 0)
    ta.theta_ += 2*vnl_math::pi;

  ta.phi_ = vcl_atan2(vcl_sqrt(vcl_pow(dx,2.0) + vcl_pow(dy,2.0)), dz);
  if(ta.phi_ < 0)
    ta.phi_ += 2*vnl_math::pi;

  return ta;
}

double dbgl_linear_curve_3d::curvature_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return curvature_at_length(s * len_);
}

double dbgl_linear_curve_3d::curvature_at_length(double s) const
{
  vcl_cout << "dbgl_linear_curve_3d::curvature_at_length NOT IMPLEMENTED!" << vcl_endl;
  return 0;
}

double dbgl_linear_curve_3d::torsion_at(double s) const
{
  assert(s >= 0 && s <= 1);
  return torsion_at_length(s * len_);
}

double dbgl_linear_curve_3d::torsion_at_length(double s) const
{
  vcl_cout << "dbgl_linear_curve_3d::torsion_at_length NOT IMPLEMENTED!" << vcl_endl;
  return 0;
}
