// This is basic/dbgl/algo/dbgl_conic_arc.h

#ifndef dbgl_conic_arc_h_
#define dbgl_conic_arc_h_

//:
// \file
// \brief Conic arc - a quadratic curve segments represented with
// two end points and three (real number) parameters
// 
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date Oct 28, 2006
// \verbatim
//  Modifications:
// \endverbatim


#include <vcl_iostream.h>
//#include <vcl_cmath.h>
#include <vgl/vgl_conic_segment_2d.h>


//: Conic arc class - a quadratic curve segment
// This class can represent all three conic types: ellipse, hyperbola, parabola
// and its degenerate case: a straight line
// It does not use the traditional way to parameterize a conic (using a center
// of some type) to avoid large numerical error when the center is very far
// away from the segment of interest.
// The model is based on the formulation in 
// Gerald Farin, "Curves and Surfaces for CAGD" with modifications documented
// in Nhon's notes (links will be provided later).
class dbgl_conic_arc
{
public:
  //static const double taylor_4_bound;
  static const double epsilon;


protected:
  // Extrinsic info
  double x0_;
  double y0_;
  double theta_;

  // Intrinsic info
  // x-coordinate of second end point in the canonical coordinate system
  // i.e.
  // starting point : (x0, y0) = (0,0)
  // ending point:    (x2, y2) = (x2, 0)
  double x2_;

  // Geometric parameters
  // (ax/w1, ay/w1) is the third vertex of the triangle tangent to 
  // the conic segment. This point can be at infinity (when w1==0) but each
  // number is finite. Therefore the three numbers are represented explicitly
  // The value of w1 also determines the type of this conic
  // w1 < 1: ellipse
  // w1 = 1: parabola
  // w1 > 1: hyperbola
  double ax_;
  double ay_;
  double w1_;

public:

  //**************************************************************
  // CONSTRUCTORS / DESTRUCTORS / INITIALIZERS
  //**************************************************************

  //: Constructor - default
  dbgl_conic_arc(){};

  //: Constructor - member variables
  dbgl_conic_arc(double x0, double y0, double theta, 
    double x2,
    double ax, double ay,
    double w1) : 
  x0_(x0), y0_(y0), theta_(theta), x2_(x2), ax_(ax), ay_(ay), w1_(w1)
  {}


  //: Destructor
  virtual ~dbgl_conic_arc(){};


  //**************************************************************
  // DATA ACCESS
  //**************************************************************
  double x0() const {return this->x0_; }
  double y0() const {return this->y0_; }
  double theta() const {return this->theta_; }
  double x2() const {return this->x2_; }
  double ax() const {return this->ax_; }
  double ay() const {return this->ay_; }
  double w1() const {return this->w1_; }



  //: Set internal variables
  void set(double start_x, double start_y, double theta, 
    double x2,
    double ax, double ay,
    double w1)
  {
    this->x0_ = (start_x);
    this->y0_ = (start_y);
    this->theta_ = (theta);
    this->x2_ = (x2);
    this->ax_ = (ax);
    this->ay_ = (ay);
    this->w1_ = (w1);
  }

  //: Set all intrinsic parameters of an arc
  void set_extrinsic(const vgl_point_2d<double >& origin, double rot_angle)
  { this->x0_ = origin.x(); y0_ = origin.y(); theta_ = rot_angle;  }

  void set_intrinsic(double x2, double ax, double ay, double w1)
  { this->x2_ = x2; this->ax_ = ax; this->ay_ = ay; this->w1_ = w1; }

  //: Return chord length of the conic
  double chord_length() const;

  // --------------------------------------------------------------------------
  // GEOMETRY PROPERTIES
  // --------------------------------------------------------------------------

  //: Create a new conic from 2 point-tangents and one curvature
  bool set_from(const vgl_point_2d<double >& start,
    const vgl_point_2d<double >& end,
    const vgl_vector_2d<double >& start_tangent,
    const vgl_vector_2d<double >& end_tangent,
    double start_curvature);

  //: Create a new circular arc from 2 point-tangents and one curvature
  bool set_from(const vgl_point_2d<double >& start,
    const vgl_vector_2d<double >& chord_direction,
    double chord_length,
    const vgl_vector_2d<double >& start_tangent,
    const vgl_vector_2d<double >& end_tangent,
    double start_curvature);


  // Create a new conic from vgl_conic_segment. It takes the smallest arc.
  bool set_from(const vgl_conic_segment_2d<double> &c);


  //   ------- TO BE INHERITED from parametric curve -----------


  //: Get sample point of the conic with parameter t, t \in [0, 1] with
  // t = 0 : starting point
  // t = 1 : ending point
  virtual vgl_point_2d<double > point_at(double t) const;

  //: Get tangent of the point at parameter t within [0,1]
  virtual vgl_vector_2d<double > tangent_at(double t) const;

  //: Get curvature of the point at parameter t, t \in [0, 1]
  virtual double curvature_at(double t) const;

  
  //**************************************************************
  // BINARY I/O
  //**************************************************************
  
  //: Print parameters of the circular arc
  virtual void print(vcl_ostream &os = vcl_cerr );


protected:


};

#endif // basic/dbgl/algo/dbgl_conic_arc.h

