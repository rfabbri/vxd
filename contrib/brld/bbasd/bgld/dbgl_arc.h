// This is basic/dbgl/dbgl_arc.h
#ifndef dbgl_arc_h_
#define dbgl_arc_h_
//:
// \file
// \brief An arc implementation via a parametric equation
//        for sampling and calculation of geometric properties.
// \author Ozge Can Ozcanli (ozge@lems.brown.edu)
// \date 02/09/05
//
// \verbatim
// Modifications
// 02/28/05  Ricardo Fabbri: constructor parameters now passed by reference
//
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_line_2d.h>
#include <dbgl/dbgl_arc_base.h>
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>
#include <vcl_iostream.h>

//: 
// An arc segment class. \sa dbgl_param_curve documentation, specially 
// regarding the conventions for parametrized access to the curve. Above all,
// whenever a parameter is assumed to be within [0,1] range, do NOT assume it to
// be a fraction of arclength parameter, even though it might actually be.
//
//\remarks
//  Internally, the arc is parametrized by s running in [0,1] 
//  x(s) = center_x + radius * cos(alpha(s))
//  y(s) = center_y + radius * sin(alpha(s))
//  angle is a linear function of s 
//  but depends on the direction of arc (ccw = 1 if counterclockwise, -1 otherwise)
//  alpha(s) = alpha0 + [ [ 2PI + ccw * (alpha1-alpha0) ] % 2PI ] * s)
class dbgl_arc : public dbgl_arc_base
{
 public:
    static const double near_zero_value
      VCL_STATIC_CONST_INIT_FLOAT_DECL(1.0e-8);
    static const double minimum_curvature
      VCL_STATIC_CONST_INIT_FLOAT_DECL(0.00260416);  //(1/384.0)



  //: default constructor 
   dbgl_arc() : center_(0,0), curv_(1.0), ccw_(1), alpha0_(0.0), alpha1_(PI) {}

  //: copy_constructor
  dbgl_arc( const dbgl_arc& that )
    : dbgl_arc_base(), center_(that.center_), curv_(that.curv_),
      ccw_(that.ccw_), alpha0_(that.alpha0_), alpha1_(that.alpha1_)
  {    }  

  //: construct from 3 points
  //  check curvature, should be nonzero for a valid arc
  dbgl_arc(const vgl_point_2d<double> &start, const vgl_point_2d<double> &end, const vgl_point_2d<double> &other);

  //:
  virtual ~dbgl_arc();

  static const vcl_type_info& type_id()
  { return typeid(dbgl_arc); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(dbgl_arc) == type)!=0 ||
            this->dbgl_arc_base::is_type(type);
  }

  dbgl_param_curve *clone() const { return new dbgl_arc(*this); }

  //: comparison operator.
  //  Comparison is on the curve, two parametric curves are identical if their
  //  equations are equivalent
  virtual
  bool operator==(const dbgl_arc &c) const;
  inline bool operator!=(const dbgl_arc &other) const {return !operator==(other);}

  //bool operator==(dbgl_param_curve const& obj) const; // virtual of dbgl_param_curve

  //: Write "<dbvgl_param_curve> to stream"
  // \relates dbvgl_param_curve
  //virtual
  //vcl_ostream&  operator<<(vcl_ostream& s);

  vcl_ostream& print(vcl_ostream & = vcl_cerr) const;


  // Property functions ----------------------------------------------

  int orientation() const { return ccw_; }
  //: retuns angle of starting point, in [0,2PI] range. 
  //If curvature is 0, the behaviour of this function is undefined
  double start_angle() const { return alpha0_; }
  //: retuns angle of endpoint, in [0,2PI] range
  //If curvature is 0, the behaviour of this function is undefined
  double end_angle() const { return alpha1_; }
  // If curvature is 0, retuns middlepoint of circle
  const vgl_point_2d<double> & center() const { return center_; }
  //: length of arc
  double length() const;

  // Elementary geometric functions ----------------------------------

  //: Get sample point at value s of a parameter along the curve, s within [0,1] 
  virtual vgl_point_2d<double> point_at(double s) const;

  //: Get sample point at arclength s away from starting point
  virtual vgl_point_2d<double> point_at_length(double s) const
    { return point_at(s/length()); }

  //: Get tangent of the point at parameter s within [0,1]
  virtual vgl_vector_2d<double> tangent_at(double s) const;

  //: Get tangent of the point at arclength s away from starting point
  virtual vgl_vector_2d<double> tangent_at_length(double s) const
    { return tangent_at(s/length()); }

  //: Gets tangent angle (in radian) in [0, 2PI) at parameter s within [0,1]
  virtual double tangent_angle_at(double s) const;
  
  //: Gets tangent angle (in radian) in [0, 2PI)  at arclength s away 
  // from the starting point
  virtual double tangent_angle_at_length(double s) const
    { return tangent_angle_at(s/length()); }

  //: Get curvature of the point at s within [0,1]
  virtual double curvature_at(double) const { return curv_; }

  //: Get curvature of the point at s arclength away from starting point.
  virtual double curvature_at_length(double) const {return curv_;}

  double curvature() const { return curv_; }

  //: make an euler spiral from itself and return an euler spiral 
  //dbgl_euler_spiral make_euler_spiral() const;

  //
  dbgl_arc& operator=( dbgl_arc const& );

  friend class dbgl_arc_algo;

 protected: // some routines that maybe useful for friends and derived classes
  
  static bool is_almost_zero(double x);
  static bool is_almost_equal(double val1, double val2, double tol);
  static bool is_colinear(const vgl_point_2d<double> &point1,
                         const vgl_point_2d<double> &point2,
                         const vgl_point_2d<double> &point3);

  //: line-line intersection
  static unsigned compute_inters_ll(
     const vgl_point_2d<double> &p11, const vgl_point_2d<double> &p12,
     const vgl_point_2d<double> &p21, const vgl_point_2d<double> &p22,
     vgl_point_2d<double> *inter);

 private:
  bool find_center( const vgl_point_2d<double>& p11, const vgl_point_2d<double>& p12,
                    const vgl_point_2d<double>& p21, const vgl_point_2d<double>& p22);
   //: center. For 0-curvature arcs, this is the middle point of the line segment
   vgl_point_2d<double> center_;
   //: curvature 1/radius, curvature is more useful if stored than radius
   double curv_;
   //: direction ccw_ = 1 if counterclockwise, -1 otherwise
   int ccw_;
   //: starting angle in [0,2PI]. But for 0-curvature arcs, this is x-coordinate of the start point
   double alpha0_;
   //: ending angle in [0,2PI]. But for 0-curvature arcs, this is y-coordinate of the start point
   double alpha1_;
};

// Inlined definitions -------------------------------------------
inline bool dbgl_arc::is_almost_zero(double x)
{
  return (vcl_fabs(x) < near_zero_value);
}

inline bool dbgl_arc::is_almost_equal(double val1, double val2, double tolerance)
{
  return (vcl_fabs(val1 - val2) < tolerance);
}

//: length of arc 
inline double dbgl_arc::length() const
{
   if (curv_ != 0.0) {
      double dif = vcl_fmod((TWOPI+ccw_*(alpha1_-alpha0_)),TWOPI);
      return dif/curv_;   // delta angle * radius
   } else
      return 2*vnl_math::hypot(center_.x()-alpha0_, center_.y()-alpha1_);
}

#endif // dbgl_arc_h_
