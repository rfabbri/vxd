// This is basic/dbgl/dbgl_param_curve_3d.h
#ifndef dbgl_param_curve_3d_h_
#define dbgl_param_curve_3d_h_
//:
// \file
// \brief A parametric 3D curve that can be sampled via a parametric equation.
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
#include <vcl_typeinfo.h>
#include <vcl_utility.h>

//: A 3D parametric curve
//
//  Parametric curves are required to represent continuous curves
//  and they enable sampling and calculation of basic differential 
//  geometries.
//
//  dbgl_param_curve_3d is the base class, various parametric curves 
//  such as arcs, euler spirals, polynomials which shall derive from this base class.
//
//  A parametric curve has a start and an endpoint. It also enables parametric
//  access by at least the following methods:
//
//    point_at_length(s), with s in [0,length()], with 0 corresponding to starting
//    point, and length() to endpoint. This can be numerically demanding for certain
//    curves.
//
//    point_at(s), with s in [0,1],  with 0 corresponding to starting point, and 1 to
//    endpoint. Note that this is NOT normalized arclength, i.e., point_at(0.5)
//    may or MAY NOT be the point at arclength 0.5*length() from starting point.
//
//  Each derived curve could have a natural parameter which is not arclength. In
//  that case, e.g. for sake of efficiency, that class could _also_ define a
//  "point_at_xx" method where "xx" is a parameter name. 
//
//  The same convention holds for tangent_at(s), curvature_at(s), and all other
//  parametric functions.
//
//  For example, to recover the starting point, use point_at(0). To recover the
//  endpoint, use point_at(1) or point_at_length(length()).
//
//

struct tangent_angles
{
  //: The angle between the tangent and z-axis, value within [0, pi]
  double phi_;
  //: The angle between the projection of the tangent on xy-plane 
  //  and x-axis, value within [0, 2*pi]
  double theta_;
  // The tangent can be recovered from these angles by
  // T = [sin(phi).cos(theta), sin(phi).sin(theta), cos(phi)]
};

class dbgl_param_curve_3d
{
 public:

  dbgl_param_curve_3d() {}

  virtual ~dbgl_param_curve_3d() {}

  static const vcl_type_info& type_id()
  { return typeid(dbgl_param_curve_3d); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(dbgl_param_curve_3d) == type)!=0; }

  //: comparison operator.
  //  Comparison is on the curve, two parametric curves are identical if their
  //  equations are equivalent
  virtual
  bool operator==(dbgl_param_curve_3d const& c) const {return this == &c; } 

  // Elementary geometric functions ----------------------------------

  //: length of parametric curve from start point to end point
  virtual double length() const = 0;

  //: Get sample point at value s of a parameter along the curve, s within [0,1] 
  virtual vgl_point_3d<double> point_at(double s) const = 0;

  //: Get sample point at arclength s away from the starting point
  virtual vgl_point_3d<double> point_at_length(double s) const = 0;

  //: Get tangent vector at value s of a parameter along the curve, s within [0,1] 
  virtual vgl_vector_3d<double> tangent_at(double s) const = 0;

  //: Get tangent of the point at arclength s away from the starting point
  virtual vgl_vector_3d<double> tangent_at_length(double s) const = 0;

  //: Get normal vactor at value s of a parameter along the curve, s within [0,1] 
  virtual vgl_vector_3d<double> normal_at(double s) const = 0;

  //: Get normal of the point at arclength s away from the starting point
  virtual vgl_vector_3d<double> normal_at_length(double s) const = 0;

  //: Get tangent angles at value s of a parameter along the curve, s within [0,1] 
  virtual struct tangent_angles tangent_angles_at(double s) const = 0;
  
  //: Gets tangent angles (in radians) at arclength s away from the starting point
  virtual struct tangent_angles tangent_angles_at_length(double s) const = 0;

  //: Get curvature at value s of a parameter along the curve, s within [0,1] 
  virtual double curvature_at(double s) const = 0;

  //: Get curvature of the point at s arclength away from starting point.
  virtual double curvature_at_length(double s) const = 0;

  //: Get torsion at value s of a parameter along the curve, s within [0,1] 
  virtual double torsion_at(double s) const = 0;

  //: Get torsion of the point at s arclength away from starting point.
  virtual double torsion_at_length(double s) const = 0;

  virtual
    dbgl_param_curve_3d& operator=( dbgl_param_curve_3d const& ) { return *this; }
 private:
  
};

#endif // dbgl_param_curve_3d_h_
