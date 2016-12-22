// This is bbasd/bgld/bgld_poly_curve_line.h
#ifndef bgld_poly_curve_line_h_
#define bgld_poly_curve_line_h_
//:
// \file
// \brief  Class that implements methods to create lines and 
//         find length as a special case of a polynomial curve
//         to be used in linear interpolation
// \author Based on original code by  Ozge Can Ozcanli (@lems.brown.edu)
// \date 02/11/05
//
// \verbatim
// Modifications
//   
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <bgld/bgld_poly_curve.h>

//: Each polynomial has 2 coefficients in linear case. 
//  For each interval C[i-1, i] on curve, compute px(s) = a1*s + a0
//  s takes values in (0,1] in each interval
//  find coefficients such that px(0) = start_x, px(1) = end_x   
//                          and py(0) = start_y, py(1) = end_y

// to interpolate the point at l arclength away from start point use:
// point_at(l/length()) ,i.e. set s = l/total_length 

class bgld_poly_curve_line : public bgld_poly_curve
{
 public:

  bgld_poly_curve_line() {}

  //: construct from vectors of coefs
  bgld_poly_curve_line(double len, vcl_vector<double> coefs_x, vcl_vector<double> coefs_y) : 
  bgld_poly_curve(1, len, coefs_x, coefs_y) {}  
  //order_(1), len_(len), coefs_x_(coefs_x), coefs_y_(coefs_y) {}

  //: copy_constructor
  bgld_poly_curve_line( const bgld_poly_curve_line& that ) :
    bgld_poly_curve(that) {} 
    //: order_(that.order_), len_(that.len_), coefs_x_(that.coefs_x_), coefs_y_(that.coefs_y_) {} 

  //: construct from two points 
  bgld_poly_curve_line(vgl_point_2d<double> start, vgl_point_2d<double> end);

  //:
  virtual ~bgld_poly_curve_line() {}

  static const vcl_type_info& type_id()
  { return typeid(bgld_poly_curve_line); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(bgld_poly_curve_line) == type)!=0 ||
            this->bgld_poly_curve::is_type(type);
  }

  bgld_param_curve *clone() const { return new bgld_poly_curve_line(*this); }
 
  // Property functions ----------------------------------------------
  
  //: length of the polynomial piece between start and end points
  virtual 
  double length() const;

  //: Get sample point at arclength s away from starting point
  //  The class deriving from this must implement this based on the convention used 
  //  to find the coefficients (i.e. whether it was a dummy variable, or it was normalized arclength, etc.)
  virtual vgl_point_2d<double> point_at_length(double l) const;

  //: Get tangent of the point at arclength s away from starting point
  virtual vgl_vector_2d<double> tangent_at_length(double l) const;

  //: Get curvature of the point at s arclength away from starting point.
  virtual double curvature_at_length(double l) const;

  //: Compute the angle using derivatives of the polynomials at given length
  virtual double tangent_angle_at_length(double l) const;

 private:
  // disabled
  bgld_poly_curve_line& operator=( bgld_poly_curve_line const& );
};

#endif // bgld_poly_curve_line_h_
