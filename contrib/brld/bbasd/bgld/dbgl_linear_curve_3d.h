// This is basic/dbgl/dbgl_linear_curve_3d.h
#ifndef dbgl_linear_curve_3d_h_
#define dbgl_linear_curve_3d_h_
//:
// \file
// \brief Class that implements linear three dimensional curve 
//
// \author H. Can Aras (can@lems.brown.edu)
// \date 2007-01-15
//
// TO DO: Since tangent, tangent angles, etc. have the same value at every point
// of the segment, these can be cached so that they are not computed at each call.
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

//: polynomial is parametrized by arclength, which is between [0, len_]
//  x(s) = coef_x[0] + coef_x[1]*s
//  y(s) = coef_y[0] + coef_y[1]*s
//  z(s) = coef_z[0] + coef_z[1]*s
//  IMPORTANT: s (curve parameter) values are not normalized between [0,1]

class dbgl_linear_curve_3d : public dbgl_param_curve_3d
{
public:
  //: No empty constructors

  //: Constructor from start and end points
  dbgl_linear_curve_3d(vgl_vector_3d<double> start, vgl_vector_3d<double> end);

  virtual ~dbgl_linear_curve_3d();

  static const vcl_type_info& type_id()
  { return typeid(dbgl_linear_curve_3d); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(dbgl_linear_curve_3d) == type)!=0 ||
  this->dbgl_linear_curve_3d::is_type(type);
  }

  //: comparison operator.
  //  Comparison is on the curve, two parametric curves are identical if their
  //  equations are equivalent
  bool operator==(dbgl_linear_curve_3d const& c) const {return this == &c; } 

  //: access functions
  vgl_vector_3d<double> start(){ return start_; };
  vgl_vector_3d<double> end(){ return end_; };

  // Elementary geometric functions ----------------------------------

  //: return length of parametric curve from start point to end point
  double length() const;

  //: Get sample point at value s of a parameter along the curve, s within [0,1]
  vgl_point_3d<double> point_at(double s) const;
  //: Get sample point at arclength s away from starting point, s within [0,len_]
  vgl_point_3d<double> point_at_length(double s) const;

  //: Get tangent at value s of a parameter along the curve, s within [0,1]
  vgl_vector_3d<double> tangent_at(double s) const;
  //: Get tangent at arclength s away from starting point, s within [0,len_]
  vgl_vector_3d<double> tangent_at_length(double s) const;

  //: Get normal at value s of a parameter along the curve, s within [0,1]
  vgl_vector_3d<double> normal_at(double s) const;
  //: Get normal at arclength s away from starting point, s within [0,len_]
  vgl_vector_3d<double> normal_at_length(double s) const;

  //: Get tangent angles at value s of a parameter along the curve, s within [0,1]
  struct tangent_angles tangent_angles_at(double s) const;
  //: Get tangent angles in radians at arclength s away from starting point, s within [0,len_]
  struct tangent_angles tangent_angles_at_length(double s) const;

  //: Get curvature at value s of a parameter along the curve, s within [0,1]
  double curvature_at(double s) const;
  //: Get curvature at arclength s away from starting point, s within [0,len_]
  double curvature_at_length(double s) const;

  //: Get torsion at value s of a parameter along the curve, s within [0,1]
  double torsion_at(double s) const;
  //: Get torsion at arclength s away from starting point, s within [0,len_]
  double torsion_at_length(double s) const;

  dbgl_linear_curve_3d& operator=( dbgl_linear_curve_3d const& ) { return *this; }

private:

protected:
  //: cache length 
  double len_;

  //: start and end points
  vgl_vector_3d<double> start_, end_;

  //: slope and intercept values for the linear fits, in order x-y-z
  double slopes_[3];
  double intercepts_[3];

  //: tolerance for bounds check assertion on t and s which are input
  //  parameters to most of the functions in this class
  double tolerance_;
};

#endif // dbgl_linear_curve_3d_h_
