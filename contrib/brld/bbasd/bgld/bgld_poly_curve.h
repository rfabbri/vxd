// This is bbasd/bgld/bgld_poly_curve.h
#ifndef bgld_poly_curve_h_
#define bgld_poly_curve_h_
//:
// \file
// \brief Abstract base class for a polynomial curve that can be sampled via a polynomial equation.
// \author Based on original code by  Ozge Can Ozcanli (@lems.brown.edu)
// \date 02/11/05
//
// \verbatim
// Modifications
//   
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <bgld/bgld_param_curve.h>
#include <vcl_iostream.h>

//: polynomial is parametrized by s (runs in [-infinity,+infinity]). 
//  n --> order
//  \code
//  x(s) = coef_x[0] + coef_x[1]*s + coef_x[2]*s^2 + ... + coef_x[n]*s^n 
//  y(s) = coef_y[0] + coef_y[1]*s + coef_y[2]*s^2 + ... + coef_y[n]*s^n 
//  \endcode

class bgld_poly_curve : public bgld_param_curve
{
 public:

  bgld_poly_curve() {}

  //: construct from vectors of coefs
  bgld_poly_curve(int order, double len, vcl_vector<double> coefs_x, 
                                         vcl_vector<double> coefs_y) 
    : order_(order), len_(len), coefs_x_(coefs_x), coefs_y_(coefs_y)  {}

  //: copy_constructor
  bgld_poly_curve( const bgld_poly_curve& that )
    : bgld_param_curve(), order_(that.order_), len_(that.len_), 
      coefs_x_(that.coefs_x_), coefs_y_(that.coefs_y_)  {} 

  virtual ~bgld_poly_curve() {}

  static const vcl_type_info& type_id()
  { return typeid(bgld_poly_curve); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(bgld_poly_curve) == type)!=0 ||
            this->bgld_param_curve::is_type(type);
  }
 
  // Property functions ----------------------------------------------
  
  virtual
  bool operator==(const bgld_poly_curve &c) const;
  inline bool operator!=(const bgld_poly_curve &other) const {return !operator==(other);}

  //: return order of the polynomial
  int order() const { return order_; }

  //: return vector of coefficients
  vcl_vector<double> coefficients_x() const { return coefs_x_; }
  vcl_vector<double> coefficients_y() const { return coefs_y_; }

  //: length of the polynomial piece between start and end points
  virtual double length() const = 0;

  //: Compute the point at s using polynomials
  vgl_point_2d<double> point_at(double s) const;

  //: Get sample point at arclength s away from starting point
  //  The class deriving from this must implement this based on the convention used 
  //  to find the coefficients (i.e. whether it was a dummy variable, or it was normalized arclength, etc.)
  virtual vgl_point_2d<double> point_at_length(double s) const = 0;

  //: Compute the tangent using derivatives of the polynomials at s
  vgl_vector_2d<double> tangent_at(double s) const;

  //: Get tangent of the point at arclength s away from starting point
  virtual vgl_vector_2d<double> tangent_at_length(double s) const = 0;

  //: TODO
  virtual
  double curvature_at(double) const 
    { vcl_cerr << "curvature function not implemented\n"; return 0;}

  //: Get curvature of the point at s arclength away from starting point.
  virtual double curvature_at_length(double s) const = 0;
  
  //: Compute the angle using derivatives of the polynomials at given s
  double tangent_angle_at(double s) const;

  //: Compute the angle using derivatives of the polynomials at given length
  virtual double tangent_angle_at_length(double l) const = 0;

  // 
  bgld_poly_curve& operator=( bgld_poly_curve const& );

  virtual bgld_param_curve *clone() const = 0;

 private:

  //: evaluate at s
  double evaluate_x(double s) const;
  double evaluate_y(double s) const;
  double evaluate_derivative_x(double s) const;
  double evaluate_derivative_y(double s) const;

  // protected members

 protected:
  //: order of the polynomial
  int order_;  
  
  //: cache length 
  double len_;

  //: vector of coefficients
  vcl_vector<double> coefs_x_;
  vcl_vector<double> coefs_y_;


};

#endif // bgld_poly_curve_h_
