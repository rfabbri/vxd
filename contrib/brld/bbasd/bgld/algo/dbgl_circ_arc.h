// This is basic/dbgl/algo/dbgl_circ_arc.h

#ifndef dbgl_circ_arc_h_
#define dbgl_circ_arc_h_

//:
// \file
// \brief Circular arc - a curve segment with constant curvature, 
// internally represented by two end points and a signed curvature
// 
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date 08/17/2005
//
// \verbatim
//  Modifications:
//       Nhon Trinh   08/17/2005    Initial version
// \endverbatim


#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vgl/vgl_point_2d.h>


//: Circular arc class - a curve segment with constant curvature, 
// internally represented by two end points and a signed curvature
// A circular arc is assumed to be less than half a circle
// This class is temporary and will be merged with dbgl_arc later on
// The reason it is separate now is because of the difference in 
// internal representation
// This circular arc class avoids using the circle center as much as possible
// because for low curvature, the center is very far away, hence causes
// large numerical error.
class dbgl_circ_arc
{
public:
  static const double taylor_4_bound;
  static const double epsilon;


protected:
  vgl_point_2d<double > point1_;
  vgl_point_2d<double > point2_;
  double k_;

public:

  // ================================================================================
  //// ***********************************************************
  //// NEW FUNCTIONS - to merge with dbgl_arc
  //// ***********************************************************

  //dbgl_arc( const dbgl_arc& that );

  ////: construct from 3 points
  ////  check curvature, should be nonzero for a valid arc
  //dbgl_arc(const vgl_point_2d<double> &start, const vgl_point_2d<double> &end, const vgl_point_2d<double> &other);

  //static const vcl_type_info& type_id()
  //{ return typeid(dbgl_arc); }

  //virtual bool is_type( const vcl_type_info& type ) const
  //{ return (typeid(dbgl_arc) == type)!=0 ||
  //          this->dbgl_arc_base::is_type(type);
  //}

  ////: comparison operator.
  ////  Comparison is on the curve, two parametric curves are identical if their
  ////  equations are equivalent
  //virtual
  //bool operator==(const dbgl_arc &c) const;
  //inline bool operator!=(const dbgl_arc &other) const {return !operator==(other);}


  //vcl_ostream& print(vcl_ostream & = vcl_cerr) const;

  //int orientation() const { return ccw_; }

  ////: retuns angle of starting point, in [0,2PI] range. 
  ////If curvature is 0, the behaviour of this function is undefined
  //double start_angle() const { return alpha0_; }
  ////: retuns angle of endpoint, in [0,2PI] range
  ////If curvature is 0, the behaviour of this function is undefined
  //double end_angle() const { return alpha1_; }

  ////: Gets tangent angle (in radian) in [0, 2PI) at parameter s within [0,1]
  //virtual double tangent_angle_at(double s) const;
  //
  ////: Gets tangent angle (in radian) in [0, 2PI)  at arclength s away 
  //// from the starting point
  //virtual double tangent_angle_at_length(double s) const
  //  { return tangent_angle_at(s/length()); }


  // //: Get curvature of the point at s within [0,1]
  //virtual double curvature_at(double) const { return curv_; }

// ================================================================================

  //**************************************************************
  // CONSTRUCTORS / DESTRUCTORS / INITIALIZERS
  //**************************************************************

  //: Constructor - default
  dbgl_circ_arc();

  //: Constructor - from intrinsic parameters
  dbgl_circ_arc(const vgl_point_2d<double >& point1, 
    const vgl_point_2d<double > & point2, double curvature );

  //: Destructor
  virtual ~dbgl_circ_arc(){};
  
  // DATA ACCESS --------------------------------------------------------------

  //: Return endpoint point1
  inline vgl_point_2d<double > point1() const {return this->point1_; }
  vgl_point_2d<double > start() const {return this->point1_; }

  //: Return endpoint point2
  inline vgl_point_2d<double > point2() const {return this->point2_; }
  vgl_point_2d<double > end() const {return this->point2_; }

  //: Return curvature of the arc
  inline double k() const { return this->k_; }

  //: Set all intrinsic parameters of an arc
  inline void set(const vgl_point_2d<double >& new_p1, 
    const vgl_point_2d<double >& new_p2, double new_k)
  { this->point1_ = new_p1; this->point2_ = new_p2; this->k_ = new_k; }

  //: Create a new circular arc from three points on the arc
  // Return false if the creation fails
  bool set_from(const vgl_point_2d<double >& start, 
    const vgl_point_2d<double >& middle,
    const vgl_point_2d<double >& end );


  //: Create a new circular arc from starting point (P1), circular center (O),
  // and a normal vector at the end point (pointing toward the center)
  // Return false if the creation fails
  bool set_from(const vgl_point_2d<double >& start, 
    const vgl_point_2d<double >& center,
    const vgl_vector_2d<double >& end_normal_vector );

  //: Create a circular arc from two end points and tangent at the starting point
  // Return false if creation fails
  bool set_from(const vgl_point_2d<double >& start,
    const vgl_vector_2d<double >& start_tangent,
    const vgl_point_2d<double >& end);

  //: Create a circular arc from mid-point (on the arc), tangent at mid-point
  // chord length and height of arc (negative for curvature < 0)
  bool set_from(double chord_length, double height, 
    const vgl_point_2d<double >& mid_pt, 
    const vgl_vector_2d<double >& mid_tangent);


  //: Create a circular arc from starting point-tangent, curvature, and arc-length
  bool set_from(const vgl_point_2d<double >& start, 
    const vgl_vector_2d<double >& start_tangent,
    double curvature, double arc_length);


  
  // GEOMETRY PROPERTIES ------------------------------------------------------

  //: Return chord vector (connecting two end points)
  inline vgl_vector_2d<double > chord_dir() const
  { return normalized(this->point2()-this->point1()); }

  //: Return length of chord
  // \TODO Cache this number for fast computation
  inline double chord_len() const
  { return (this->point2()-this->point1()).length(); }

  //: Return midpoint of the chord
  inline vgl_point_2d<double > chord_midpoint() const
  { return centre<double >(this->point1(), this->point2()); }

  //: Return the angle a
  inline double central_angle() const
  { return 2*vcl_asin(this->k()*this->chord_len()/2); }


  //: Return height of the arc, always non-negative
  double height() const;

  //: Return length of `this' arc
  double len() const;

  //: length of arc
  double length() const {return this->len(); }


  //: Return area of the region limited by the circular arc and its chord
  double area() const;

  //: Approximatio of the cicular arc segment area
  // Method by (Harris and Stocker 1998)
  // accurate to within 0.1% for 0< central_angle < 150 o
  // and 0.8% for  150 o < central_angle < 180 o
  // See http://mathworld.wolfram.com/CircularSegment.html
  double area_approx() const;

  //: Return unit tangent vector at starting point (point1)
  vgl_vector_2d<double > tangent_at_start() const;

  //: Return unit tangent at ending point (point2)
  vgl_vector_2d<double > tangent_at_end() const;

  //: Return unit normal vector at starting point (point1)
  // Normal vector points TOWARD the circle center
  vgl_vector_2d<double > normal_at_start() const;

  //: Return unit normal vector at ending point (point2)
  // Normal vector points TOWARD the circle center
  vgl_vector_2d<double > normal_at_end() const;

  //: Return center of the circle containing the arc
  vgl_point_2d<double > center() const;

  //: Get sample point at value s of a parameter along the curve, s within [0,1] 
  vgl_point_2d<double> point_at(double t) const;

  //: Get sample point at arclength s away from starting point
  vgl_point_2d<double> point_at_length(double s) const;

  //: Sample the arc with a given sampling rate (length)
  vcl_vector<vgl_point_2d<double > > compute_samples(double ds) const;

  //: Sample the arc with a given sampling rate (length)
  void compute_samples(double ds, 
    vcl_vector<vgl_point_2d<double > >& pts,
    vcl_vector<vgl_vector_2d<double > >& tangents) const;



  //: Get tangent of the point at parameter t within [0,1]
  virtual vgl_vector_2d<double> tangent_at(double t) const;

  //: Get tangent of the point at arclength s away from starting point
  vgl_vector_2d<double> tangent_at_length(double s) const;

  //: Get normal vector of the point at parameter t within [0,1]
  // Normal vector points TOWARD the circle center
  virtual vgl_vector_2d<double> normal_at(double t) const;

  //: Get normal vector of the point at arclength s
  // Normal vector points TOWARD the circle center
  virtual vgl_vector_2d<double> normal_at_length(double s) const;





  //: Get curvature of the point at s arclength away from starting point.
  double curvature_at_length(double) const {return this->k_;}

  //: Return the exact arc but with reversed end-point order
  dbgl_circ_arc reversed_arc() const;

  //**************************************************************
  // UTILITIES
  //**************************************************************

  //: Return 1 if `pt' is OUTSIDE the circle
  //         0 if `pt' is ON the circle
  //        -1 if `pt' is INSIDE the circle
  int circ_region(const vgl_point_2d<double >& pt) const;

  
  //**************************************************************
  // BINARY I/O
  //**************************************************************
  
  //: Print parameters of the circular arc
  void print(vcl_ostream &os = vcl_cerr ) const;


protected:
  //: Compute sinc(t) = sin(t) / x
  double sinc(double t) const
  {
    // for large t, use normal formula
    if (vcl_abs(t) > dbgl_circ_arc::taylor_4_bound)
    {
      return vcl_sin(t)/t;
    }

    //for small t, approximate the function using 4th order Taylor expansion
    // sinc(t) = sin(t)/t = 1 - 1/2 t^2 + 1/120 x^4 + O(6)
    else
    {
      return (1 + t*t*(-(double)1.0/6 + t*t/120));
    }
  }
  

private:

};

#endif // basic/dbgl/algo/dbgl_circ_arc.h

