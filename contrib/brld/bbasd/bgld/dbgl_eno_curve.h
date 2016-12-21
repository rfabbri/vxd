// This is basic/dbgl/dbgl_eno_curve.h
#ifndef dbgl_eno_curve_h_
#define dbgl_eno_curve_h_
//:
// \file
// \brief Class that implements second-order polynomial 
//        parametric curves where the polynomial coefficients
//        are obtained by ENO
//        This class is not aware of how the coefficients are computed,
//        so it could be used for representing other kinds of fitting
//        polynomials of second order where the parameter along x and y
//        is not necessarily arclength
// \author H. Can Aras (can@lems.brown.edu)
// \date 2006-11-15
//
// \verbatim
// Modifications
//   
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <dbgl/dbgl_param_curve.h>
#include <vcl_iostream.h>

#define S_T_ACCURACY 10e-6

//: polynomial is parametrized by t (runs in [-infinity,+infinity]). 
//  \code
//  x(t) = coef_x[0] + coef_x[1]*t + coef_x[2]*t^2
//  y(t) = coef_y[0] + coef_y[1]*t + coef_y[2]*t^2
//  \endcode

class dbgl_eno_curve : public dbgl_param_curve
{
public:
  //: No empty constructors

  //: Constructor from ENO coefficients
  dbgl_eno_curve(vcl_vector<double> coefs_x, vcl_vector<double> coefs_y, double start_t, double end_t);

  virtual ~dbgl_eno_curve();

  static const vcl_type_info& type_id()
  { return typeid(dbgl_eno_curve); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(dbgl_eno_curve) == type)!=0 ||
  this->dbgl_eno_curve::is_type(type);
  }

  //: comparison operator.
  //  Comparison is on the curve, two parametric curves are identical if their
  //  equations are equivalent
  bool operator==(dbgl_eno_curve const& c) const {return this == &c; } 

  //: access functions
  double start_t(){ return start_t_; };
  double end_t(){ return end_t_; };
  double coef_x(int index) { return coefs_x_[index]; }
  double coef_y(int index) { return coefs_y_[index]; }

  // Elementary geometric functions ----------------------------------

  //: given s (arclength), find the t value to some accuracy
  //  using binary search algorithm
  double s_to_t(double s, double accuracy) const;

  //: compute the length of parametric curve for given t, t within [0,1]
  double length_at(double t) const;

  //: return length of parametric curve from start point to end point
  double length() const;

  //: Get sample point at value t along the curve, t within [0,1] 
  vgl_point_2d<double> point_at(double t) const;

  //: Get sample point at arclength s away from starting point, s within [0,len_]
  vgl_point_2d<double> point_at_length(double s) const;

  //: Get tangent at value t along the curve, t within [0,1] 
  vgl_vector_2d<double> tangent_at(double t) const;

  //: Get tangent at arclength s away from starting point, s within [0,len_]
  vgl_vector_2d<double> tangent_at_length(double s) const;

  //: Get tangent angle in radians in [0,2*pi] at value t along the curve, t within [0,1] 
  double tangent_angle_at(double t) const;

  //: Get tangent angle in radians in [0,2*pi] at arclength s away from starting point, s within [0,len_]
  double tangent_angle_at_length(double s) const;

  //: Get curvature at value t along the curve, t within [0,1]
  double curvature_at(double t) const;

  //: Get curvature at arclength s away from starting point, s within [0,len_]
  double curvature_at_length(double s) const;

  dbgl_eno_curve& operator=( dbgl_eno_curve const& ) { return *this; }

  dbgl_param_curve *clone() const { return new dbgl_eno_curve(*this); }

private:
  //: evaluate at t, t between [0,1]
  double evaluate_x(double t) const;
  double evaluate_y(double t) const;
  double evaluate_first_derivative_x(double t) const;
  double evaluate_first_derivative_y(double t) const;
  double evaluate_second_derivative_x(double t) const;
  double evaluate_second_derivative_y(double t) const;

  inline bool my_isnan(double x) const
  {
    return x != x;
  }

protected:
  //: cache length 
  double len_;

  //: vector of coefficients, someone would like to update this class
  //  in order to make it work 
  vcl_vector<double> coefs_x_;
  vcl_vector<double> coefs_y_;

  //: global starting and ending t parameters 
  //  (necessary when a collection of parametric curves are used)
  double start_t_, end_t_;
  //: 
  double len_until_start_;

  //: tolerance for bounds check assertion on t and s which are input
  //  parameters to most of the functions in this class, necessary when
  //  we want to get values (almost) exactly on the original sampling points
  double tolerance_;
};

#endif // dbgl_eno_curve_h_
