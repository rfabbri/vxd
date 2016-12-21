// This is basic/dbgl/dbgl_eno_curve_3d.h
#ifndef dbgl_eno_curve_3d_h_
#define dbgl_eno_curve_3d_h_
//:
// \file
// \brief Class that implements third-order polynomial parametric 
//        curves where the polynomial coefficients are obtained by ENO.
//        This class is not aware of how the coefficients are computed,
//        so it could be used for representing other kinds of fitting
//        polynomials of third order.
//        See "Differential Geometry of Curves and Surfaces" by DoCarmo
//        for curve property (curvature, torsion, tangent, normal, etc.)
//        computations
//
// \author H. Can Aras (can@lems.brown.edu)
// \date 2006-11-17
//
// \verbatim
// Modifications
//   
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_vector_3d.h>
#include <dbgl/dbgl_param_curve_3d.h>
#include <vcl_iostream.h>

// accuracy used for the computation of "t" (running parameter) from "s" (arclength)
#define S_T_ACCURACY 10e-6

//: polynomial is parametrized by t, not necessarily arclength and runs in (-infinity, +infinity).
//  x(t) = coef_x[0] + coef_x[1]*t + coef_x[2]*t^2 + coef_x[3]*t^3
//  y(t) = coef_y[0] + coef_y[1]*t + coef_y[2]*t^2 + coef_y[3]*t^3
//  z(t) = coef_z[0] + coef_z[1]*t + coef_z[2]*t^2 + coef_z[3]*t^3
//  IMPORTANT: t (curve parameter) values are not normalized between [0,1]
//  as in the 2D case because of the third order ENO implementation difference
//  (start_t_ and end_t_ holds true (not normalized) t values for the start and 
//   end points of this interval instead of being 0 and 1, respectively)

//: There is no sense in calling point_at(double s) and similar functions with s
//  parameter in [0,1]. Instead, the correct function to call would be
//  point_at_length(double s) which will in advance call point_at_t(double t), and
//  same as similar functions

class dbgl_eno_curve_3d : public dbgl_param_curve_3d
{
public:
  //: No empty constructors

  //: Constructor from coefficients, start point parameter value, and
  //  end point parameter value
  dbgl_eno_curve_3d(vcl_vector<double> coefs_x, 
                    vcl_vector<double> coefs_y, 
                    vcl_vector<double> coefs_z,
                    double start_t,
                    double end_t);

  virtual ~dbgl_eno_curve_3d();

  static const vcl_type_info& type_id()
  { return typeid(dbgl_eno_curve_3d); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(dbgl_eno_curve_3d) == type)!=0 ||
  this->dbgl_eno_curve_3d::is_type(type);
  }

  //: comparison operator.
  //  Comparison is on the curve, two parametric curves are identical if their
  //  equations are equivalent
  bool operator==(dbgl_eno_curve_3d const& c) const {return this == &c; } 

  //: access functions
  double start_t(){ return start_t_; };
  double end_t(){ return end_t_; };
  double coef_x(int index) { return coefs_x_[index]; }
  double coef_y(int index) { return coefs_y_[index]; }
  double coef_z(int index) { return coefs_z_[index]; }

  // Elementary geometric functions ----------------------------------

  //: given s (arclength), find the t value to some accuracy using binary search algorithm,
  //  s within [0, len_]
  double s_to_t(double s, double accuracy) const;

  //: compute the length of parametric curve for given t, t within [start_t, end_t]
  double length_at_t(double t) const;
  //: return length of parametric curve from start point to end point
  double length() const;

  //: Get sample point at value s of a parameter along the curve, s within [0,1] 
  vgl_point_3d<double> point_at(double s) const;
  //: Get sample point at arclength s away from starting point, s within [0,len_]
  vgl_point_3d<double> point_at_length(double s) const;
  //: Get sample point at value t along the curve, t within [start_t_, end_t_]
  vgl_point_3d<double> point_at_t(double t) const;

  //: Get tangent at value s of a parameter along the curve, s within [0,1]
  vgl_vector_3d<double> tangent_at(double s) const;
  //: Get tangent at arclength s away from starting point, s within [0,len_]
  vgl_vector_3d<double> tangent_at_length(double s) const;
  //: Get tangent at value t along the curve, t within [start_t_, end_t_]
  vgl_vector_3d<double> tangent_at_t(double t) const;

  //: Get normal at value s of a parameter along the curve, s within [0,1]
  vgl_vector_3d<double> normal_at(double s) const;
  //: Get normal at arclength s away from starting point, s within [0,len_]
  vgl_vector_3d<double> normal_at_length(double s) const;
  //: Get normal at value t along the curve, t within [start_t_, end_t_]
  vgl_vector_3d<double> normal_at_t(double t) const;

  //: Get tangent angles at value s of a parameter along the curve, s within [0,1]
  struct tangent_angles tangent_angles_at(double s) const;
  //: Get tangent angles in radians at arclength s away from starting point, s within [0,len_]
  struct tangent_angles tangent_angles_at_length(double s) const;
  //: Get tangent angles in radians at value t along the curve, t within [start_t_, end_t_]
  struct tangent_angles tangent_angles_at_t(double t) const;

  //: Get curvature at value s of a parameter along the curve, s within [0,1]
  double curvature_at(double s) const;
  //: Get curvature at arclength s away from starting point, s within [0,len_]
  double curvature_at_length(double s) const;
  //: Get curvature at value t along the curve, t within [start_t_, end_t_]
  double curvature_at_t(double t) const;

  //: Get torsion at value s of a parameter along the curve, s within [0,1]
  double torsion_at(double s) const;
  //: Get torsion at arclength s away from starting point, s within [0,len_]
  double torsion_at_length(double s) const;
  //: Get torsion at value t along the curve, t within [start_t_, end_t_]
  double torsion_at_t(double t) const;

  dbgl_eno_curve_3d& operator=( dbgl_eno_curve_3d const& ) { return *this; }

private:
  //: evaluate at t, t between [0,1]
  double evaluate_x(double t) const;
  double evaluate_y(double t) const;
  double evaluate_z(double t) const;
  double evaluate_first_derivative_x(double t) const;
  double evaluate_first_derivative_y(double t) const;
  double evaluate_first_derivative_z(double t) const;
  double evaluate_second_derivative_x(double t) const;
  double evaluate_second_derivative_y(double t) const;
  double evaluate_second_derivative_z(double t) const;
  double evaluate_third_derivative_x(double t) const;
  double evaluate_third_derivative_y(double t) const;
  double evaluate_third_derivative_z(double t) const;

protected:
  //: cache length 
  double len_;

  //: vector of coefficients
  vcl_vector<double> coefs_x_;
  vcl_vector<double> coefs_y_;
  vcl_vector<double> coefs_z_;

  //: start_t_/end_t_ gives the coordinates of the starting/ending 
  //  point of the curve when evaluated
  double start_t_, end_t_;

  //: tolerance for bounds check assertion on t and s which are input
  //  parameters to most of the functions in this class, necessary when
  //  we want to get values (almost) exactly on the original sampling points
  double tolerance_;
};

#endif // dbgl_eno_curve_3d_h_
