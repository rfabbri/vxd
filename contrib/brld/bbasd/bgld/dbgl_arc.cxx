// This is basic/dbgl/dbgl_arc.cxx
//:
//\file
// A considerable part of this implementation is "inspired" from old GENO C code
// by Kwun Han.

#include "dbgl_arc.h"
#include <vcl_cmath.h>
#include <vcl_iostream.h>



#if !VCL_STATIC_CONST_INIT_FLOAT_NO_DEFN
const double  dbgl_arc::near_zero_value
      VCL_STATIC_CONST_INIT_FLOAT_DEFN(1.0e-8);
const double dbgl_arc::minimum_curvature
      VCL_STATIC_CONST_INIT_FLOAT_DEFN(0.00260416);  //(1/384.0)
#endif





bool dbgl_arc::is_colinear(const vgl_point_2d<double> &point1,
                           const vgl_point_2d<double> &point2,
                           const vgl_point_2d<double> &point3)
{
    double x1, y1, x2, y2, x3, y3;
    double epsilon;

    x1 = point1.x();
    y1 = point1.y();
    x2 = point2.x();
    y2 = point2.y();
    x3 = point3.x();
    y3 = point3.y();

    /* Added check to see if it is within some epsilon of zero */
    epsilon = (y2 - y1)*(x3 - x2) - (y3 - y2)*(x2 - x1);
    return is_almost_zero(epsilon);
}

//: finds intersection of two disecting lines and sets the center of the circle
// \return false if there is no intersection 
bool dbgl_arc::
find_center(
      const vgl_point_2d<double>& p11, const vgl_point_2d<double>& p12,
      const vgl_point_2d<double>& p21, const vgl_point_2d<double>& p22)
{

   if (!compute_inters_ll(p11,p12,p21,p22,&center_))
      return false;
   return true;
}


//: construct from 3 points star,end,other.
//  Fit a circular arc so that x(0) = start.x
//                             y(0) = start.y
//                             x(1) = end.x
//                             y(1) = end.y
// Assume The arc does not pass through "other" point.
//
// Arcs of curvature 0 (lines) are supported. The center will be the medial
// point of the line segment. And the angles indicate the direction of the line
// traversal. Arcs of radius 0 (points) are NOT supported by this class (e.g. 
// methods such as point_at will have undefined behavior).
//
dbgl_arc::dbgl_arc(
      const vgl_point_2d<double> &start, 
      const vgl_point_2d<double> &end, 
      const vgl_point_2d<double> &other)
{
  //: check for degenerate case
  if (is_colinear(start, end, other)) {
    curv_ = 0.0;
    center_.x() = (end.x() + start.x()) / 2.0;
    center_.y() = (end.y() + start.y()) / 2.0;
    alpha0_ = start.x();
    alpha1_ = start.y();
    return;
  } else {

     //: Equation of line disecting two points.
     //  A x + B y = C
     //  A = 2 ( x2 - x1 )
     //  B = 2 ( y2 - y1 )
     //  C = (x2 + x1) (x2 - x1) + (y2 + y1) (y2 - y1)
     //  find two of these lines and check for parallelness, if not parallel, then
     //  intersection of them will be the center of the circle
     //  A, B and C are the parameter for the first line
     //  D, E and F are the parameter for the second line 

     double x1 = start.x();
     double y1 = start.y();
     double x2 = end.x();
     double y2 = end.y();
     double x3 = other.x();
     double y3 = other.y();

     double A = 2 * ( x2 - x1 ); 
     double B = 2 * ( y2 - y1 );
     // not used:  double C = (x2 + x1) * (x2 - x1) + (y2 + y1) * (y2 - y1);

     double D = 2 * ( x3 - x2 ); 
     double E = 2 * ( y3 - y2 );
     // not used: double F = (x3 + x2) * (x3 - x2) + (y3 + y2) * (y3 - y2);

     //: check for the linearity of the disecting lines
     if ( is_almost_equal((A * E),(B * D),near_zero_value)) {
        // this should't happen; maybe be radical and abort/assert?
       vcl_cerr << "Found a set of parallel lines. But previous computation ";
       vcl_cerr << "says these are not parallel: " << A*E << " " << B*D << "\n";
       vcl_cerr << "p1: " << start << " p2: " << end << " p3: " << other << vcl_endl;
       curv_ = 0.0;
       return;
     }

     vgl_point_2d<double> mid12( (x1 + x2) / 2.0, (y1 + y2) / 2.0);
     vgl_point_2d<double> mid23( (x2 + x3) / 2.0, (y2 + y3) / 2.0);
     vgl_point_2d<double> v1( mid12.x() + B, mid12.y() - A);
     vgl_point_2d<double> v2( mid23.x() + E, mid23.y() - D);

     bool found = find_center(mid12, v1, mid23, v2);
     double r;
     if (found) 
       r  = (center_-start).length();
     else {
#ifndef NDEBUG
         vcl_cerr << "dbgl_arc: no center found..\n";
#endif
       r = 0.0;
     }

     if (is_almost_zero(r)) {
        vcl_cerr << "dbgl_arc: Radius 0 (not supported!)\n";
        curv_ = 0.0;
     } else
        curv_ = 1.0/r;


     if (curv_ < minimum_curvature){
//         vcl_cerr << "dbgl_arc: clipping curvature " << curv_ << " --> 0\n";
         curv_ = 0.0;
         center_.x() = (end.x() + start.x()) / 2.0;
         center_.y() = (end.y() + start.y()) / 2.0;
         alpha0_ = start.x();
         alpha1_ = start.y();
         return;
     }
  }

  // ---- calculate angles and find direction ----
  alpha0_ = vcl_atan2(start.y()-center_.y(), start.x()-center_.x());
  // this is in [-pi,pi] sic, inclusive both -pi and pi
  
  if (alpha0_ < 0) alpha0_ = TWOPI+alpha0_; //:< now its in [0,2pi]

  alpha1_ = vcl_atan2(end.y()-center_.y(), end.x()-center_.x());
  if (alpha1_ < 0) alpha1_ = TWOPI+alpha1_;
  
  double alpha2 = vcl_atan2(other.y()-center_.y(), other.x()-center_.x());
  if (alpha2 < 0) alpha2 = TWOPI+alpha2;

  //: find the location of the "other" point to determine direction of the arc
  //  assuming that "other" point is not between start and end 
  //  i.e., arc is running either [other start end] or [start end other]
  if (alpha0_ < alpha1_) {
    if (alpha0_ < alpha2 && alpha2 < alpha1_) 
      ccw_ = -1; // cw
    else 
      ccw_ =  1; // ccw
  } else {
    if (alpha1_ < alpha2 && alpha2 < alpha0_) 
      ccw_ =  1; // ccw
    else 
      ccw_ = -1; // cw
  }

  
  return;
}

//:
dbgl_arc::~dbgl_arc() {}

//: comparison operator.
//  Comparison is on the curve, two parametric curves are identical if their
//  equations are equivalent
bool dbgl_arc::operator==(dbgl_arc const& c) const 
{
  return (curv_== c.curvature() && 
          center_== c.center() && 
          ccw_ == c.orientation() &&
          alpha0_ == c.start_angle() && 
          alpha1_ == c.end_angle()); 
}

dbgl_arc& dbgl_arc::operator=( dbgl_arc const& c)
{
  curv_ = c.curvature();
  center_ = c.center(); 
  ccw_  = c.orientation();
  alpha0_ = c.start_angle();
  alpha1_ = c.end_angle(); 
  return *this;
}

// Elementary geometric functions ----------------------------------

//: Get sample point at value s of a parameter along the curve, s within [0,1] 
vgl_point_2d<double> 
dbgl_arc::point_at(double s) const
{
  if (curv_ == 0.0) {
     double x = alpha0_ + s*2*(center_.x() - alpha0_);
     double y = alpha1_ + s*2*(center_.y() - alpha1_);
     vgl_point_2d<double> pt(x,y);
     return pt;
  } else {
     //: check for arc's validity
     assert(curv_ > 0);

     // alpha = alpha0-(alpha0-alpha1)*s;  
     double dif = vcl_fmod((TWOPI+ccw_*(alpha1_-alpha0_)),TWOPI);
#if 0
//this variable is not used in the code.  PLEASE FIX!  -MM
     double len = dif/curv_;   // delta angle * radius
#endif
     if (s < 0)
       s = 0;
     else if (s > 1)
       s = 1;

     double alpha = alpha0_+ccw_*dif*(s);
     double x = center_.x() + (1.0/curv_)*cos(alpha);
     double y = center_.y() + (1.0/curv_)*sin(alpha);
     vgl_point_2d<double> pt(x,y);
     return pt;
  }
}


//: Get tangent of the point at parameter s within [0,1]
vgl_vector_2d<double> dbgl_arc::tangent_at(double s) const
{
  double alpha = tangent_angle_at(s);
  return vgl_vector_2d<double>(cos(alpha), sin(alpha));
}

//: Gets tangent angle (in radian) in [0, 2PI) at parameter s within [0,1]
double dbgl_arc::tangent_angle_at(double s) const
{
  if (curv_ == 0) { 
     // atan2: [-pi,pi]
     double alpha = vcl_atan2(center_.y()-alpha1_, center_.x()-alpha0_);
     if (alpha < 0) alpha = TWOPI+alpha; // now [0,2pi]
     return alpha;

  } else {

    double alpha, alpha1_normal=alpha1_; 

    if (ccw_==1) {
      if (alpha1_ < alpha0_)   // normalize angle to the real line
        alpha1_normal = alpha1_ + TWOPI;

      // angle corresp to s
      alpha = alpha0_ + (alpha1_normal - alpha0_)*s;

      // ccw orthogonal
      alpha = alpha + PIOVERTWO;
    } else {
      if (alpha1_ > alpha0_)   // normalize angle to the real line
        alpha1_normal = alpha1_ - TWOPI;

      // angle corresp to s
      alpha = alpha0_ + (alpha1_normal - alpha0_)*s;

      // clockwise orthogonal
      alpha = alpha - PIOVERTWO;
    }

    // get rid of negatives:
    if (alpha < 0)
      alpha = alpha + 6*PI;    // I proved that, after the above, alpha is > -6PI

    alpha = vcl_fmod(alpha,TWOPI);
    assert (alpha >=0);
    return alpha;
  }
}

//: intersection of two lines
unsigned dbgl_arc::
compute_inters_ll(
      const vgl_point_2d<double> &p11, const vgl_point_2d<double> &p12,
      const vgl_point_2d<double> &p21, const vgl_point_2d<double> &p22,
      vgl_point_2d<double> *inter)
{
    double det;
    double m11, m12, m21, m22;
    double x1, x2, x3, x4, y1, y2, y3, y4;
    double c1, c2;
    double a, b;
    double epsilon;

    x1 = p11.x();       y1 = p11.y();
    x2 = p12.x() - x1;  y2 = p12.y() - y1;
    x3 = p21.x();       y3 = p21.y();
    x4 = p22.x() - x3;  y4 = p22.y() - y3;

    //: check to see if lines are parallel 
    epsilon = y2*x4-y4*x2;
    // orig: epsilon = ((y2 - y1) * (x3 - x2) - (y3 - y2) * (x2 - x1));

    //: if so, then return no intercepts found
    if(is_almost_zero(epsilon))
        return 0;

    det = y2*x4 - x2*y4;

    m11 = -y4;
    m12 = x4;
    m21 = -y2;
    m22 = x2;

    c1 = x3-x1;
    c2 = y3-y1;

    m11 /= det;
    m12 /= det;
    m21 /= det;
    m22 /= det;

    a = c1 * m11 + c2 * m12;
    b = c1 * m21 + c2 * m22;

    inter->set(x1 + a*x2,  y1 + a*y2);
    return 1;
}

vcl_ostream& 
dbgl_arc::
print(vcl_ostream &os) const
{
   
   
   os << "Center: " << center_.x() << ", " << center_.y() <<  "   "
      << "Curvature: " << curv_ << "   CCW: " << ccw_ << vcl_endl;

   if (curvature() == 0.0) {
      os << "Other point: (" << alpha0_ << ", " << alpha1_ << ")\n";
   } else {
      os << "Angles: " << 180*alpha0_/PI << ", " << 180*alpha1_/PI << " degrees\n";
   }

   os << "Length: " << length()
      << vcl_endl;

   return os;
}
