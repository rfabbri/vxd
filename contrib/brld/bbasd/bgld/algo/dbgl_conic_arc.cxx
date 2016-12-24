// This is basic/dbgl/algo/dbgl_conic_arc.cxx
//:
// \file

#include "dbgl_conic_arc.h"
#include <vcl_cmath.h>
#include <vgl/algo/vgl_homg_operators_2d.h>


const double dbgl_conic_arc::epsilon = 1e-10;

bool dbgl_conic_arc::
set_from(const vgl_conic_segment_2d<double> &c)
{

  // handles the case where the conic doesn't pass exactly through the endpoints

  vgl_point_2d<double> pt1(
    vgl_homg_operators_2d<double>::closest_point(c.conic(), c.point1()));

  vgl_point_2d<double> pt2(
      vgl_homg_operators_2d<double>::closest_point(c.conic(), c.point2()));

  vgl_vector_2d<double> chord = pt2 - pt1;
  double chord_length = chord.length();

  //  Need to generate a consistent orientation for the arc.
  //  In order to consider all possible arcs, all we need is a clockwise or
  //  counterclockwise parameter. For now, we just get the smallest arc.

  vgl_vector_2d<double> start_tangent = 
    c.conic().tangent_at(vgl_homg_point_2d<double>(pt1)).direction();

  vgl_vector_2d<double> end_tangent = 
    c.conic().tangent_at(vgl_homg_point_2d<double>(pt2)).direction();

  if (cos_angle(start_tangent, chord) < 0)
    start_tangent = -start_tangent;

  if (cos_angle(end_tangent, chord) < 0)
    end_tangent = -end_tangent;

  // Compute curvature
  double start_curvature = c.conic().curvature_at(pt1);

  return set_from(
      pt1, chord, chord_length,
      start_tangent, end_tangent, 
      start_curvature);
}

//: Create a new circular arc from 2 point-tangents and one curvature
bool dbgl_conic_arc::
set_from(const vgl_point_2d<double >& start,
         const vgl_vector_2d<double >& chord_direction,
         double chord_length,
         const vgl_vector_2d<double >& start_tangent,
         const vgl_vector_2d<double >& end_tangent,
         double start_curvature)
{
  // degerate case when the whole curve is just a point
  // only extrinsic parameters matters in this case
  if (chord_length < dbgl_conic_arc::epsilon)
  {
    double theta = vcl_atan2(chord_direction.y(), chord_direction.x());
    this->set_extrinsic(start, theta);
    this->set_intrinsic(0, 0, 0, 1); 
    
    // return false if the angles are not consistent
    if ((angle(chord_direction, start_tangent) > dbgl_conic_arc::epsilon) ||
      (angle(chord_direction, end_tangent) > dbgl_conic_arc::epsilon) )
    {
      return false;
    }
    else
    {
      return true;
    }
  }


  // extrinsic params
  double theta = vcl_atan2(chord_direction.y(), chord_direction.x());
  
  // intrinsic params
  // Transform all info into canonical coordinate system
  // curvature is invariant w.r.t 
  double x2 = chord_length;
  vgl_vector_2d<double > t0 = normalized(rotated(start_tangent, -theta));
  vgl_vector_2d<double > t2 = normalized(rotated(end_tangent, -theta));
  double k0 = start_curvature;

  // -----------------------------------------------------
  // degenerate case : a straight line
  if (vcl_abs(k0) < dbgl_conic_arc::epsilon)
  {
    // t0 and t2 need to be horizontal to make a straight line
    if (vcl_abs(vcl_atan2(t0.y(), t0.x())) < 100*dbgl_conic_arc::epsilon &&
      vcl_abs(vcl_atan2(t0.y(), t0.x())) < 100*dbgl_conic_arc::epsilon)
    {
      // the straight line is parameterized wrt arclength
      this->set_extrinsic(start, theta);
      this->set_intrinsic(x2, x2/2, 0, 1); 
      return true;
    }
    return false;
  }
  // -----------------------------------------------------

  // regular case

  // a^2 = -sin(psi_0) * x_2 / (2 * k0)
  double a_sqr = -t0.y() * x2 / (2*k0);
  
  // required: a^2 >=0
  if (a_sqr < 0) return false;

  double a = vcl_sqrt(a_sqr);

  // w1 = a * sin(psi2 - psi0) / (x2 * sin(psi2))
  double w1 = a * (t2.y()*t0.x() - t2.x()*t0.y()) / (x2 * t2.y());

  // required: w1 >=0
  if (w1 < 0) return false;

  // all computation is done, now assign the member variables
  this->set_extrinsic(start, theta);
  this->set_intrinsic(x2, a*t0.x(), a*t0.y(), w1);

  return true;
  
}



// ----------------------------------------------------------------------------
//: Create a new conic arc from 2 point-tangents and one curvature
bool dbgl_conic_arc::
set_from(const vgl_point_2d<double >& start,
         const vgl_point_2d<double >& end,
         const vgl_vector_2d<double >& start_tangent,
         const vgl_vector_2d<double >& end_tangent,
         double start_curvature)
{
  if ((end-start).length() < dbgl_conic_arc::epsilon)
  {
    return this->set_from(start, start_tangent, 0, start_tangent, 
    end_tangent, start_curvature);
  }
  return this->set_from(start, end-start, (end-start).length(), start_tangent,
    end_tangent, start_curvature);
}


//: Return chord length of the conic
double dbgl_conic_arc::
chord_length() const
{
  return this->x2();
}


//**************************************************************
// GEOMETRY PROPERTIES
//**************************************************************

// ----------------------------------------------------------------------------
//: Get sample point of the conic with parameter t, t \in [0, 1] with
// t = 0 : starting point
// t = 1 : ending point
vgl_point_2d<double > dbgl_conic_arc::
point_at(double t) const
{
  // coordinate in canonical coordinate system
  double denom = (1-t)*(1-t) + 2 * this->w1_ * t* (1-t) + t*t;
  double xc = (2 * this->ax_ * t * (1-t) + this->x2_ * t*t)/denom;
  double yc = (2 * this->ay_ * t * (1-t))/denom;

  // rigid transform to get the correct coordinates
  vgl_vector_2d<double > v = rotated(vgl_vector_2d<double >(xc, yc), this->theta_);
  return vgl_point_2d<double >(this->x0_, this->y0_) + v;
}


// ----------------------------------------------------------------------------
//: Get tangent of the point at parameter t within [0,1]
vgl_vector_2d<double > dbgl_conic_arc::
tangent_at(double t) const
{
  // for super degenerate case, when the chord length is zero
  // (the whole curve is just a point)
  if (this->x2()<dbgl_conic_arc::epsilon) 
    return vgl_vector_2d<double >(vcl_cos(this->theta()), vcl_sin(this->theta()));

  // for regular case, including straight line
  // check Farin for the formulation

  // w(t) = w0 * B_0^2(t) + w1 * B_1^2(t) + w2 * B_2^2(t)
  double w_t = (1-t)*(1-t) + this->w1() * 2*t*(1-t) + t*t;

  // w'(t) = w0 * d/dt(B_0^2(t)) + w1 * d/dt(B_1^2(t)) + w2 * d/dt(B_2^2(t))
  double wp_t = (2*t-2) + this->w1()*(2-4*t) + 2*t;

  // p(t) = w0*P0 * B_0^2(t) + w1*P1 * B_1^2(t) + w2*P2 * B_2^2(t)
  // p'(t) = w0*P0 * d/dt(B_0^2(t)) + w1*P1 * d/dt(B_1^2(t)) + w2*P2 * d/dt(B_2^2(t))
  double xp_t = this->ax()*(2-4*t) + this->x2() *(2*t);
  double yp_t = this->ay()*(2-4*t);

  // c(t)
  // coordinate in canonical coordinate system
  double denom = (1-t)*(1-t) + 2 * this->w1_ * t* (1-t) + t*t;
  double x_t = (2 * this->ax_ * t * (1-t) + this->x2_ * t*t)/denom;
  double y_t = (2 * this->ay_ * t * (1-t))/denom;
  vgl_point_2d<double > c_t(x_t, y_t);

  // c'(t) = (1/w(t)) * (p'(t) - c(t)*w'(t))
  double cxp_t = (xp_t - x_t * wp_t) / w_t;
  double cyp_t = (yp_t - y_t * wp_t) / w_t;
  return normalized(rotated(vgl_vector_2d<double >(cxp_t, cyp_t), this->theta()));
}




// ----------------------------------------------------------------------------
//: Get curvature of the point at parameter t, t \in [0, 1]
double dbgl_conic_arc::
curvature_at(double t) const
{
  double w0 = 1;
  double w2 = 1;
  double w1 = this->w1();

  vgl_vector_2d<double > P0(0, 0);
  vgl_vector_2d<double > P2(this->x2(), 0);
  vgl_vector_2d<double > w1P1(this->ax(), this->ay());

  // P(t), w(t), and c(t)
  vgl_vector_2d<double > P_t = w0*P0*(1-t)*(1-t) + 2*w1P1*t*(1-t) + w2*P2*t*t;
  double w_t = w0*(1-t)*(1-t) + 2*w1*t*(1-t) + w2*t*t;
  vgl_vector_2d<double > c_t = P_t / w_t;

  // P'(t), w'(t), and c'(t)
  vgl_vector_2d<double > P_t_p =  w0*P0*(2*t-2) + w1P1*(2-4*t) + w2*P2*(2*t);
  double w_t_p = w0*(2*t-2) + w1*(2-4*t) + w2*(2*t);
  vgl_vector_2d<double > c_t_p = (P_t_p - w_t_p * c_t) / w_t;

  // P''(t), w''(t), and c''(t)
  vgl_vector_2d<double > P_t_pp = 2*w0*P0 -4*w1P1 + 2*w2*P2;
  double w_t_pp = 2*w0 -4*w1 + 2*w2;

  vgl_vector_2d<double > c_t_pp = (P_t_pp - 2*w_t_p * c_t_p - w_t_pp * c_t) / w_t;

  // Finally we are ready to compute curvature
  double len = c_t_p.length();
  double k_t = cross_product(c_t_p, c_t_pp) /  (len*len*len);
  
  return k_t;
}




//**************************************************************
// BINARY I/O
//**************************************************************

// ------------------------------------------------------------------------
//: Print parameters of the circular arc
void dbgl_conic_arc::
print(vcl_ostream &os )
{
  os << "<dbgl_conic_arc> \n";
}
