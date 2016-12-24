// This is basic/dbgl/algo/dbgl_circ_arc.cxx
//:
// \file

#include "dbgl_circ_arc.h"

#include <vcl_cmath.h>
#include <vcl_limits.h>

const double dbgl_circ_arc_pi = 3.14159265358979323846;



//**************************************************************
// CONSTRUCTORS / DESTRUCTORS / INITIALIZERS
//**************************************************************

const double dbgl_circ_arc::taylor_4_bound = 
    vcl_sqrt(vcl_sqrt(vcl_numeric_limits<double >::epsilon()));
const double dbgl_circ_arc::epsilon = 1e-12;


//------------------------------------------------------------------------
//: Constructor - default
dbgl_circ_arc::
dbgl_circ_arc():
point1_(vgl_point_2d<double >(0, 0)),
point2_(vgl_point_2d<double >(1, 0)),
k_(1)
{
}

//------------------------------------------------------------------------
//: Constructor - from intrinsic parameters
dbgl_circ_arc::
dbgl_circ_arc(const vgl_point_2d<double >& point1, 
              const vgl_point_2d<double > & point2, 
              double curvature ):
point1_(point1), point2_(point2), k_(curvature)
{
}



//**************************************************************
// DATA ACCESS
//**************************************************************




//: Create a new circular arc from three points on the arc
// Return false if the creation fails
bool dbgl_circ_arc::
set_from(const vgl_point_2d<double >& start, 
         const vgl_point_2d<double >& middle,
         const vgl_point_2d<double >& end )
{
  // Return false if the middle point coincides with one of the end points
  if ((middle-start).length() < dbgl_circ_arc::epsilon ||
    (middle-end).length() < dbgl_circ_arc::epsilon )
  {
    vcl_cerr << "Error: middle point coincides with one of the end points.\n";
    return false;
  }

  if (dot_product(middle-start, middle-end) > 0)
  {
    vcl_cerr << "Error: Cannot create a less-than-half-a-circle arc from the given points\n";
    return false;
  }


  double len_start = (middle-start).length();
  double len_end = (middle-end).length();
  double chord_length = (end-start).length();
  double sin_a = cross_product(middle-start, middle-end) / (len_start * len_end);

  
  // compute curvature
  double k = - 2 * sin_a / chord_length;

  this->set(start, end, k);
  return true;
}


//: Create a new circular arc from starting point (P1), circular center (O),
// and a normal vector at the end point (pointing toward the center)
// Return false if the creation fails
bool dbgl_circ_arc::
set_from(const vgl_point_2d<double >& start, 
         const vgl_point_2d<double >& center,
         const vgl_vector_2d<double >& end_normal_vector )
{
  // premilinary check
  double radius;
  if ((radius = (center-start).length()) < dbgl_circ_arc::epsilon)
  {
    vcl_cerr << "Error: Circular center and starting point of the arc coincide.\n";
    return false;
  }

  if (end_normal_vector.length() < dbgl_circ_arc::epsilon)
  {
    vcl_cerr << "Error: non-zero end_normal_vector required.\n";
    return false;
  }

  // curvature
  double k = (cross_product(center-start, end_normal_vector) > 0) ? 1/radius : -1/radius;

  // ending point
  vgl_point_2d<double > end = center - radius * normalized(end_normal_vector);

  // set new parameters for the arc
  this->set(start, end, k);
  return true;

}






//: Create a circular arc from two end points and tangent at the starting point
// Return false if creation fails
bool dbgl_circ_arc::
set_from(const vgl_point_2d<double >& start,
         const vgl_vector_2d<double >& start_tangent,
         const vgl_point_2d<double >& end)
{
  vgl_vector_2d<double > chord_dir = end-start;
  double chord_len = chord_dir.length();

  double sin_angle = 
    cross_product(start_tangent, chord_dir)/ (start_tangent.length()*chord_len);

  // make sure this is sine of the smaller angle
  sin_angle = (dot_product(start_tangent, chord_dir) > 0) ? sin_angle : -sin_angle;
  
  // curvature
  double k = (chord_len < dbgl_circ_arc::epsilon)? 0 : (2*sin_angle / chord_len);
  
  // finalize
  this->set(start, end, k);
  return true;
}





//------------------------------------------------------------------------------
//: Creat a circular arc from starting point, starting tangent, curvature,
// and length of the arc
bool dbgl_circ_arc::
set_from(const vgl_point_2d<double >& start,
         const vgl_vector_2d<double >& start_tangent,
         double curvature,
         double arc_length)
{
  // limit the arc to half a circle
  if ( vcl_abs(curvature*arc_length) > dbgl_circ_arc_pi )
    return false;

  // The only missing info is the position of the end point
  // Formula:
  // x(s) = x0 + sinc(ks/2) cos(psi0 + ks/2) s
  // y(s) = y0 - sinc(ks/2) sin(psi0 + ks/2) s

  double t = curvature * arc_length / 2;
  double psi0 = vcl_atan2(start_tangent.y(), start_tangent.x());
  vgl_vector_2d<double > v(vcl_cos(psi0+t), vcl_sin(psi0+t));
  vgl_point_2d<double > end = start+ this->sinc(t)*arc_length*v;

  this->set(start, end, curvature);

  return true;
}


//------------------------------------------------------------------------------
//: Create a circular arc from mid-point (on the arc), tangent at mid-point
// chord length and height of arc (negative for curvature < 0)
bool dbgl_circ_arc::
set_from(double chord_length, double height, 
         const vgl_point_2d<double >& mid_pt, 
         const vgl_vector_2d<double >& mid_tangent)
{
  // height of an arc is at most half the chord length
  // "=" occurs when half-circles
  if (vcl_abs(height) > chord_length/2)
    return false;

  // start and end points are corners of the bounding box
  vgl_vector_2d<double > t = normalized(mid_tangent);

  // t rotated 90 degree
  vgl_vector_2d<double > n(-t.y(), t.x());

  vgl_point_2d<double > start = mid_pt - (chord_length/2) * t + height * n;
  vgl_point_2d<double > end = mid_pt + (chord_length/2) * t + height * n;
  return this->set_from(start, mid_pt, end);
}


// GEOMETRY PROPERTIES --------------------------------------------------------
//-----------------------------------------------------------------------------
//: Return height of the arc
double dbgl_circ_arc::
height() const
{
  // formula
  //             k c^2
  // h = -------------------- -----------
  //      4 (1 + sqrt(1 - k^2 * c^2 / 4))

  double kc2 = this->k()*this->chord_len()*this->chord_len();

  return vcl_abs(kc2/(4*(1+vcl_sqrt(1-this->k()*kc2/4))));
}




//------------------------------------------------------------------------
//: Return length of `this' arc
double dbgl_circ_arc::
len() const
{
  double t = this->k()*this->chord_len()/2;
  double asin_t_over_t;

  // compute arcsin(t)/t
  // for large t, use normal formule
  if (vcl_abs(t) > dbgl_circ_arc::taylor_4_bound)
  {
    asin_t_over_t = vcl_asin(t)/t;
  }
  // for small t, approximate the function using 4th order Taylor expansion
  // arcsin(t)/t = 1 + 1/6 t^2 + 3/40 * t^4 + 5/12 * t^6 + O(7);
  else
  {
    asin_t_over_t = 1 + t*t*((double)1.0/6 + 3*t*t/40);
  }
  return (this->chord_len() * asin_t_over_t);

}




//: Return area of the region limited by the circular arc and its chord
double dbgl_circ_arc::
area() const
{
  double s = this->length();
  double a = vcl_abs(s * this->k());

  // area = (1/2) * s^2 * (a - sin(a)) / (a^2)
  
  // compute w = (a - sin(a)) / (a^2)
  // Problematic when a --> 0. Use Taylor expansion
  // for large a, use normal formula
  double w = 0;
  if (vcl_abs(a) > dbgl_circ_arc::taylor_4_bound)
  {
    w = (a - vcl_sin(a)) / (a*a);
  }
  // for small a, approximate w using Taylor expansion
  // w = a / 6 - a^3 / 120 
  else
  {
    w = a/6 - a*a*a / 120;
  }
  double area = (1.0/2) * s*s * w; 
  return area;
}

//: Approximatio of the cicular arc segment area
// Method by (Harris and Stocker 1998)
// accurate to within 0.1% for 0< central_angle < 150 o
// and 0.8% for  150 o < central_angle < 180 o
// See http://mathworld.wolfram.com/CircularSegment.html
double dbgl_circ_arc::
area_approx() const
{
  double c = this->chord_len();
  double h = this->height();
  double area = (2.0/3)* c*h + h*h*h / (2*c);
  return area;
}





//------------------------------------------------------------------------
//: Return unit tangent vector at starting point (point1)
// \TODO write it
vgl_vector_2d<double > dbgl_circ_arc::
tangent_at_start() const
{
  double sin_alpha = this->k()*this->chord_len() / 2;
  double cos_alpha = vcl_sqrt(1-sin_alpha*sin_alpha);

  vgl_vector_2d<double > v = this->chord_dir();

  // rotate v by angle (-alpha)
  //      [ cos(-alpha)  -sin(-alpha) ]
  // R =  [ sin(-alpha)   cos(-alpha) ]
  sin_alpha = -sin_alpha;
  v.set(cos_alpha*v.x()-sin_alpha*v.y(), sin_alpha*v.x()+ cos_alpha*v.y());
  return v;
}



//------------------------------------------------------------------------
//: Return unit tangent at ending point (point2)
vgl_vector_2d<double > dbgl_circ_arc::
tangent_at_end() const
{
  double sin_alpha = this->k()*this->chord_len() / 2;
  double cos_alpha = vcl_sqrt(1 - sin_alpha*sin_alpha);

  vgl_vector_2d<double > v = this->chord_dir();

  // rotate v by angle (alpha)
  //      [ cos(alpha)  -sin(alpha) ]
  // R =  [ sin(alpha)   cos(alpha) ]
  v.set(cos_alpha*v.x()-sin_alpha*v.y(), sin_alpha*v.x()+ cos_alpha*v.y());
  return v;
}

//------------------------------------------------------------------------
//: Return unit normal vector at starting point (point1)
// Normal vector points TOWARD the circle center
vgl_vector_2d<double > dbgl_circ_arc::
normal_at_start() const
{
  // compute normal vector using tangent vector and sign of curvature
  vgl_vector_2d<double > t1 = this->tangent_at_start();
  return (this->k()>=0) ? 
    vgl_vector_2d<double >(-t1.y(),  t1.x()) : 
    vgl_vector_2d<double >( t1.y(), -t1.x());
}


//------------------------------------------------------------------------
//: Return unit normal vector at ending point (point2)
// Normal vector points TOWARD the circle center
vgl_vector_2d<double > dbgl_circ_arc::
normal_at_end() const
{
  // compute normal vector using tangent vector and sign of curvature
  vgl_vector_2d<double > t2 = this->tangent_at_end();
  return (this->k()>=0) ? 
    vgl_vector_2d<double >(-t2.y(),  t2.x()) : 
    vgl_vector_2d<double >( t2.y(), -t2.x());
}

//------------------------------------------------------------------------
//: Return center of the circle containing the arc
vgl_point_2d<double > dbgl_circ_arc::
center() const
{
  // center is at infinity when k == 0
 if (this->k()==0)
 {
   return (vgl_point_2d<double >(0,0) + 
   vcl_numeric_limits<double >::max()*this->normal_at_start());
 }
 else
 {
   return this->point1() + vcl_abs(1.0/this->k()) * this->normal_at_start();
 }
}



//: Get sample point at value s of a parameter along the curve, s within [0,1]
vgl_point_2d<double> dbgl_circ_arc::
point_at(double t) const
{
  return this->point_at_length(t*this->len());
}

// ------------------------------------------------------------------------
//: Get sample point at arclength s away from starting point
vgl_point_2d<double> dbgl_circ_arc::
point_at_length(double s) const
{

  // formulae
  // x(s) = x0 + sinc(ks/2) cos(psi0 + ks/2) s
  // y(s) = y0 - sinc(ks/2) sin(psi0 + ks/2) s

  double t = this->k()*s / 2;

  // compute sinc(t)
  double sinc_t = this->sinc(t);

  // for large t, use normal formula
  if (vcl_abs(t) > dbgl_circ_arc::taylor_4_bound)
  {
    sinc_t = vcl_sin(t)/t;
  }
   
  //for small t, approximate the function using 4th order Taylor expansion
  // sinc(t) = sin(t)/t = 1 - 1/2 t^2 + 1/120 x^4 + O(6)
  else
  {
    sinc_t = 1 + t*t*(-(double)1.0/6 + t*t/120);
  }

  vgl_vector_2d<double > t1 = this->tangent_at_start();
  double psi0 = vcl_atan2(t1.y(), t1.x());
  vgl_vector_2d<double > v(vcl_cos(psi0+t), vcl_sin(psi0+t));

  return (this->point1()+sinc_t*s*v);
}



// ----------------------------------------------------------------------------
//: Sample the arc with a given sampling rate
vcl_vector<vgl_point_2d<double > > dbgl_circ_arc::
compute_samples(double ds) const
{
  vcl_vector<vgl_point_2d<double > > pts;
  double len = this->length();
  for (double s=0; s<= len; s = s+ds)
  {
    pts.push_back(this->point_at_length(s));
  }
  return pts;
}



//: Sample the arc with a given sampling rate (length)
void dbgl_circ_arc::
compute_samples(double ds, 
                vcl_vector<vgl_point_2d<double > >& pts,
                vcl_vector<vgl_vector_2d<double > >& tangents) const
{
  double len = this->length();
  int num_pts = int (len / ds) + 1;
  pts.reserve(num_pts);
  tangents.reserve(num_pts);
  
  for (double s=0; s<= len; s = s+ds)
  {
    pts.push_back(this->point_at_length(s));
    tangents.push_back(this->tangent_at_length(s));
  }
  return;
}






// ------------------------------------------------------------------------
//: Get tangent of the point at parameter t within [0,1]
vgl_vector_2d<double> dbgl_circ_arc::
tangent_at(double t) const
{
  return this->tangent_at_length(t*this->len());
}



// ------------------------------------------------------------------------
//: Get tangent of the point at arclength s away from starting point
vgl_vector_2d<double> dbgl_circ_arc::
tangent_at_length(double s) const
{
  // formula
  // psi(s) = psi0 + k * s
  vgl_vector_2d<double > t = this->tangent_at_start();
  double cos_a = vcl_cos(this->k()*s);
  double sin_a = vcl_sin(this->k()*s);
  return vgl_vector_2d<double >(cos_a*t.x()-sin_a*t.y(), 
    sin_a*t.x()+cos_a*t.y());
}




//: Get unit normal vector of the point at parameter t within [0,1]
// Normal vector points TOWARD the circle center
vgl_vector_2d<double> dbgl_circ_arc::
normal_at(double t) const
{
  return this->normal_at_length(t*this->len());
}

//: Get unit normal vector of the point at arclength s
vgl_vector_2d<double> dbgl_circ_arc::
normal_at_length(double s) const
{
  // compute normal vector using tangent vector and sign of curvature
  vgl_vector_2d<double > t = this->tangent_at_length(s);
  return vgl_vector_2d<double >(-t.y(),  t.x());
}




// ---------------------------------------------------------------------------
//: Return the exact arc but with reversed end-point order
dbgl_circ_arc dbgl_circ_arc::
reversed_arc() const
{
  return dbgl_circ_arc(this->point2(), this->point1(), -this->k()); 
}








// **************************************************************
// UTILITIES
// **************************************************************
  
// ------------------------------------------------------------------------
//: Return 1 if `pt' is OUTSIDE the circle
//         0 if `pt' is ON the circle
//        -1 if `pt' is INSIDE the circle
int dbgl_circ_arc::
circ_region(const vgl_point_2d<double >& pt) const
{
  double sin_alpha = this->k()*this->chord_len() / 2;
  double cos_alpha = vcl_sqrt(1 - sin_alpha*sin_alpha);

  // (p, q) ~ (cos(b), sin(b)) where b is view angle from `pt' to 
  // two endpoints of arc
  double p = dot_product<double >(pt-this->point1(), pt-this->point2());
  double q = cross_product<double >(pt-this->point1(), pt-this->point2());

  double test = q*cos_alpha - p*sin_alpha;
  
  if (vcl_abs(test) < dbgl_circ_arc::epsilon) return 0;
  else if (test > 0) return 1;
  else return -1;
}



//**************************************************************
// BINARY I/O
//**************************************************************

// ------------------------------------------------------------------------
//: Print parameters of the circular arc
void dbgl_circ_arc::
print(vcl_ostream &os ) const
{
  os << "<dbgl_circ_arc> p1=(" << 
    this->point1().x() << "," << this->point1().y() << ")  p2=(" <<
    this->point2().x() << "," << this->point2().y() << ") k=" <<
    this->k();
}
