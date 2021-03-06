// This is bbasd/bgld/algo/bgld_biarc.cxx
//:
// \file

#include "bgld_biarc.h"
#include <vnl/vnl_math.h>
#include <vgl/vgl_vector_2d.h>
#include <vcl_cassert.h>

#define bgld_biarc_e_angle 0.0001   //Epsilon for angles
#define bgld_biarc_e_k 0.00001   //Epsilon for curvature  0.0001
#define bgld_biarc_k_large 100000 //large curvature value


// -------------- CONSTRUCTORS and DESTRUCTORS --------------
//: Constructor - from intrinsic parameters
bgld_biarc::
bgld_biarc(vgl_point_2d<double > start, double start_angle, double k1, double len1, double k2, double len2)
  : start_(start), k1_(k1), len1_(len1), k2_(k2), len2_(len2)
{
  this->set_start_angle(start_angle);
}
  





//: Constructor - from 2 point-tangents
bgld_biarc::
bgld_biarc( vgl_point_2d< double > start, double start_angle, 
           vgl_point_2d< double > end, double end_angle )
{
  this->set_start_params(start, start_angle);
  this->set_end_params(end, end_angle);
  this->compute_biarc_params();
}



//: Constructor - from 2 point-tangents
bgld_biarc::
bgld_biarc(vgl_point_2d< double > start, vgl_vector_2d<double > start_tangent, 
           vgl_point_2d< double > end, vgl_vector_2d<double > end_tangent )
{
  double start_angle = vcl_atan2(start_tangent.y(), start_tangent.x());
  double end_angle = vcl_atan2(end_tangent.y(), end_tangent.x());

  this->set_start_params(start, start_angle);
  this->set_end_params(end, end_angle);
  this->compute_biarc_params();
}




////: Copy Constructor 
//bgld_biarc::
//bgld_biarc(const bgld_biarc & biarc ) 
//  : start_(biarc.start_), start_angle_(biarc.start_angle_),
//  k1_(biarc.k1_), len1_(biarc.len1_),
//  k2_(biarc.k2_), len2_(biarc.len2_)
//{
//}


// ----------------- BASIC ------------------------------

//: Set the start angle, converted to the range [0, 2*pi)
void bgld_biarc::
set_start_angle( double start_angle ){
  double theta = vcl_fmod(start_angle, vnl_math::pi * 2);
  if (theta < 0.0)
    theta = theta + vnl_math::pi * 2;
  this->start_angle_ = theta;
}

//: Set end angle of the biarc, converted to the range [0, 2*pi)
void bgld_biarc::
set_end_angle( double end_angle ){
  double theta = vcl_fmod(end_angle, vnl_math::pi * 2);
  if (theta < 0.0)
    theta = theta + vnl_math::pi * 2;
  this->end_angle_ = theta;
}






// -----------------------------------------------------------------------------
//: Verify if indeed the biarc parameters and its start and end parameters match
bool bgld_biarc::
is_consistent() const
{
  // check end point
  double len = this->len();
  vgl_point_2d<double > end = this->point_at(len);
  if ((end-this->end()).sqr_length() > 1e-6)  // check (squared) distance between real and expected end points
    return false;

  // check end tangent
  vgl_vector_2d<double > t_end = this->tangent_at(len);
  vgl_vector_2d<double > expected_t_end(vcl_cos(this->end_angle()), vcl_sin(this->end_angle()));
  double diff = vcl_abs( signed_angle(t_end, expected_t_end) );
  
  return ( diff < 1e-3);
}









// ---------------- GEOMETRY PROPERTIES -----------------------
  
//: Return radius of the first arc
double bgld_biarc::
r1() const
{
  if (this->k1() == 0) return vnl_huge_val(this->k1());
  return 1.0/vcl_fabs(this->k1()); 
}
  
//: Return radius of the second arc
double bgld_biarc::
r2() const
{
  if (this->k2() == 0) return vnl_huge_val(this->k2());
  return 1.0/vcl_fabs(this->k2()); 
}


//: Return the point at which two arcs meet
vgl_point_2d< double > bgld_biarc::
mid_pt() const 
{
  // curvature of the first arc is non-zero
  if (vcl_abs(this->k1()) >= bgld_biarc_e_k){
    double dt = this->len1() * this->k1(); //params.L1*params.K1;
    double angle = this->start_angle() + vnl_math::pi_over_2 + dt;
    vgl_vector_2d < double > t(vcl_cos(angle), vcl_sin(angle));
    return this->center1() - t * (this->r1() * this->dir1());
  }
  // the first arc is a line segment
  else {
    vgl_vector_2d< double > t(vcl_cos(this->start_angle()), vcl_sin(this->start_angle()));
    return this->start() + t * this->len1();
  }
}

//: Return the center of the first arc
vgl_point_2d< double > bgld_biarc::
center1() const
{
  double angle = this->start_angle()- vnl_math::pi_over_2;
  vgl_vector_2d< double > t(vcl_cos(angle), vcl_sin(angle));
  return this->start() - t * (this->r1() * this->dir1()) ;  
}

//: Return the center of the second arc
vgl_point_2d< double > bgld_biarc::
center2() const
{
  double angle = this->end_angle()-vnl_math::pi_over_2;
  vgl_vector_2d< double > t(vcl_cos(angle), vcl_sin(angle));
  return this->end() - t * (this->r2() * this->dir2()) ;
}


// --- TO BE CONSISTENT WITH CURVE CLASS -----
  
//: Return a point on the biarc with s arclength away from starting point
// assert s in range [0, L]
vgl_point_2d< double > bgld_biarc::
point_at( double s) const
{
  //assert(s >= 0 && s <= this->len());

  if ( s <= this->len1() ){
    // curvature of the first arc is non-zero
    if (vcl_abs(this->k1()) >= bgld_biarc_e_k){
      double dt = s * this->k1();
      double angle = this->start_angle() + vnl_math::pi_over_2 + dt;
      vgl_vector_2d < double > t(vcl_cos(angle), vcl_sin(angle));
      return this->center1() - t * (this->r1() * this->dir1());
    }
    // the first arc is a line segment
    else {
      vgl_vector_2d< double > t(vcl_cos(this->start_angle()), vcl_sin(this->start_angle()));
      return this->start() + t * s;
    }
  }
  else{ //if ( s <= this->len() ){
    // if curvature of the second arc is non-zero
    if (vcl_abs(this->k2()) >= bgld_biarc_e_k){
      double angle = this->start_angle() + this->len1()*this->k1() + (s-this->len1())*this->k2()
        + vnl_math::pi_over_2;
      vgl_vector_2d < double > t(vcl_cos(angle), vcl_sin(angle));
      return this->center2() - t * (this->r2() * this->dir2());
    }
    // the second arc is a line segment
    else {
      double angle = this->start_angle() + this->len1()*this->k1();
      vgl_vector_2d< double > t(vcl_cos(angle), vcl_sin(angle));
      return this->mid_pt() + t * (s-this->len1());
    }
  }
}
  
//: Return tangent of a point on the biarc with s arclength away from starting point
vgl_vector_2d< double > bgld_biarc::
tangent_at( double s) const
{
  //assert(s >= 0 && s <= this->len());
  
  double angle = 0;
  if ( s <= this->len1() )
    angle = this->start_angle() + s * this->k1();
  else //if ( s <= this->len() )
    angle = this->start_angle() + this->len1() * this->k1() + (s - this->len1()) * this->k2();
  return vgl_vector_2d< double >(vcl_cos(angle), vcl_sin(angle));
}

//: Return curvature of a point on the biarc with s arclength away from starting point
double bgld_biarc::
curvature_at( double s) const
{
  assert(s >= 0 && s<= this->len());
  if (s <= this->len1())
    return this->k1();
  else
    return this->k2();
}


// ------------------ UTILITIES ------------------------

//: set parameters at starting point
void bgld_biarc::
set_start_params( vgl_point_2d< double > start, double start_angle ){
  this->set_start(start);
  this->set_start_angle(start_angle);
}

//: set parameters at ending point
void bgld_biarc::
set_end_params( vgl_point_2d< double > end, double end_angle ){
  this->set_end(end);
  this->set_end_angle(end_angle);
}



//: compute biar parameters using the currently saved start and end parameters
bool bgld_biarc::
compute_biarc_params(){

  double k1, k2, k3, k4;
  double d1, d2, d3, d4;

  double t0 = this->start_angle(); 
  double t2 = this->end_angle(); 
  double tjoin;
  
  vgl_vector_2d< double > v = this->end() - this->start();
  double d = v.length(); 

  //degenerate case
  if ( d < bgld_biarc_e_angle ){
    this->set_k1( vnl_huge_val( this->k1() ));
    this->set_k2( vnl_huge_val( this->k2() )); 
    this->energy_ = vnl_huge_val( this->energy_ ); 
    return false;
  }
  
  double psi = vcl_atan2(v.y(), v.x());
  
  if ( psi < 0 ) 
    psi += 2 * vnl_math::pi; 

  this->flag_ = 0;

  d1 = -10.0;
  d2 = -10.0;

  // due to the 0-2*pi discontinuity even for perfect arcs,
  // (psi-(t2+t0)/2)~{-pi,0, pi} for cocircular solutions
  // if (abs(mod(psi -(t2+t0)/2, pi))<bgld_biarc_e_angle) % this condition is not correct
  
  double tdiff = psi-(t2+t0)/2;
  tdiff = vcl_fmod(vcl_fabs(tdiff), vnl_math::pi*2);

  if ( (vcl_fabs(tdiff) < bgld_biarc_e_angle) || 
    (vcl_fabs(tdiff - vnl_math::pi) < bgld_biarc_e_angle) || 
    (vcl_fabs(tdiff - 2*vnl_math::pi) < bgld_biarc_e_angle) )
  {
    // straight line (still need to check mod 2*pi)
    if (vcl_fabs(vcl_fmod(psi-t0,2*vnl_math::pi)) < bgld_biarc_e_angle){ 
      k1 = 0;
      k2 = 0;
      d1 = d;
      d2 = 0;
    } 
    // single arc
    else {
      k1 = -4*vcl_sin((3*t0+t2)/4 - psi)* vcl_cos((t2-t0)/4)/d;
      // special case when start_angle == end_angle = psi-pi : no solution
      if (vcl_fabs(k1) < bgld_biarc_e_k){
        k1 = 0;
        this->set_len1(vnl_huge_val((double) 1));
        return false;
      }
      k2 = 0;
      d1 = this->compute_arclength(t0,t2,k1);
      d2 = 0;
    }
    //record the solutions the parameters
    this->set_len1(d1);
    this->set_len2(d2);
    this->set_k1(k1);
    this->set_k2(k2);
    return true;
  }
  else {
    this->flag_ = 1;   //truly a biarc
  
    k1 = -4*vcl_sin((3*t0+t2)/4 - psi)*vcl_cos((t2-t0)/4)/d;
    k2 = 4*vcl_sin((t0+3*t2)/4 - psi)*vcl_cos((t2-t0)/4)/d;
  
    if (vcl_fabs(k1)<bgld_biarc_e_k){
      k1 = 0;
      d1 = d * vcl_sin((t2+t0)/2-psi) / vcl_sin((t2-t0)/2);
      d2 = this->compute_arclength(t0,t2,k2);
    }
  
    else{
      if ( vcl_fabs(k2)<bgld_biarc_e_k ){
        k2 = 0;
        d2 = d*vcl_sin((t2+t0)/2-psi)/vcl_sin((t0-t2)/2);
        d1 = this->compute_arclength(t0,t2,k1);
      }
      else {
        // tjoin will be incorrect if k1~0 or k2~0
        tjoin = this->compute_join_theta(k1,k2);
        d1 = this->compute_arclength(t0,tjoin,k1);
        d2 = this->compute_arclength(tjoin,t2,k2);
      }
    }
  
    // the other possible biarc
    d3 = -10;
    d4 = -10;

    k3 = 4 * vcl_cos((3*t0+t2)/4 - psi) * vcl_sin((t2-t0)/4)/d;
    k4 = 4 * vcl_cos((t0+3*t2)/4 - psi)* vcl_sin((t2-t0)/4)/d;
  
    // since this solution picks the biarc with the bigger turn
    // the curvature solutions can still be close to zero
    if ( ((vcl_fabs(k3) > bgld_biarc_e_k) || (vcl_fabs(k4)>bgld_biarc_e_k)) && vcl_fabs(k4-k3) > bgld_biarc_e_k ){
      if (vcl_fabs(k3)<bgld_biarc_e_k){
        k3 = 0;
        d3 = d * vcl_sin((t2+t0)/2-psi)/ vcl_sin((t2-t0)/2);
        d4 = this->compute_arclength(t0,t2,k4);
      }
      else {
        if (vcl_fabs(k4) < bgld_biarc_e_k){
          k4 = 0;
          d4 = d*vcl_sin((t2+t0)/2-psi)/vcl_sin((t0-t2)/2);
          d3 = this->compute_arclength(t0,t2,k3);
        }
        else {
          tjoin = this->compute_join_theta(k3,k4);
          d3 = this->compute_arclength(t0,tjoin,k3);
          d4 = this->compute_arclength(tjoin,t2,k4);
        }
      }
    }
  
    // choose the smaller one
    // but due to the epsilon settings (bgld_biarc_e_angle and bgld_biarc_e_k) we could still get an incorrect solution
    // this could be caught by looking at the signs of Ls.
    if ( (d1>0 && d2>0) && ((d3<0 || d4<0) || (d1+d2)<(d3+d4)) ){
      //k1 and k2 are the correct solutions
      this->set_len1(d1); 
      this->set_len2(d2); 
      this->set_k1(k1); 
      this->set_k2(k2); 
      this->energy_ = (k2-k1)*(k2-k1); 
      return true;
    }
    else { 
      if (d3>0 && d4>0){
        this->set_len1(d3); 
        this->set_len2(d4); 
        this->set_k1(k3); 
        this->set_k2(k4); 
        this->energy_ = (k3-k4)*(k3-k4); 
        return true;
      }
      else {
        //this should never happen
        //assert(false);
        this->flag_ = -1;
        return false;
      }
    }
  }
}

//: compute biar parameters, given start and end parameters
bool bgld_biarc::
compute_biarc_params( vgl_point_2d< double > start, double start_angle,
    vgl_point_2d< double > end, double end_angle )
{
  this->set_start_params(start, start_angle);
  this->set_end_params(end, end_angle);
  return this->compute_biarc_params();
}





//: compute biar parameters, given start and end parameters
bool bgld_biarc::
compute_biarc_params(const vgl_point_2d< double >& start, 
                     const vgl_vector_2d<double >& start_tangent,
                     const vgl_point_2d< double >& end, 
                     const vgl_vector_2d<double >& end_tangent )
{
  double start_angle = vcl_atan2(start_tangent.y(), start_tangent.x());
  double end_angle = vcl_atan2(end_tangent.y(), end_tangent.x());

  this->set_start_params(start, start_angle);
  this->set_end_params(end, end_angle);

  return this->compute_biarc_params();
}







//: Compute the angle at which the two arcs meet
double bgld_biarc::
compute_join_theta (double k1, double k2){
  // compute the theta at which the two arcs meet
  
  double x0 = this->start().x(); 
  double y0 = this->start().y(); 
  double x2 = this->end().x(); 
  double y2 = this->end().y(); 
  double t0 = this->start_angle(); 
  double t2 = this->end_angle(); 

  double sin_join_theta = ( k1*k2*(x2-x0) + k2*vcl_sin(t0) - k1*vcl_sin(t2) )/(k2-k1);
  double cos_join_theta = ( -k1*k2*(y2-y0) + k2*vcl_cos(t0) - k1*vcl_cos(t2))/(k2-k1);

  double join_theta = vcl_atan2(sin_join_theta, cos_join_theta);
  if (join_theta<0) 
    join_theta += 2*vnl_math::pi;

  return join_theta;
}


//: compute arc length of a circle with curvature k, turning from
// angle t0 to t1
double bgld_biarc::
compute_arclength(double t0, double t1, double k){
  double num = (t1-t0);
  if (k == 0)
    return vnl_huge_val((double)1.0);
  if (k<0 && (t1-t0)>0)
    num = num - 2*vnl_math::pi;
  else if (k>0 && (t1-t0)<0)
    num = num + 2*vnl_math::pi;
  return num / k;
}


// ---------------- MISCELLANEOUS ----------------------
//: Print parameters of the biarc
void bgld_biarc::
print(vcl_ostream &os ) const
{
  os << vcl_endl << "Start parameters" << vcl_endl;
  os << "Start point : ( "<< this->start().x() << " , " << this->start().y() << " )" << vcl_endl;
  os << "Start angle : " << this->start_angle() << vcl_endl;
  os << "End parameters" << vcl_endl;
  os << "End point : ( "<< this->end().x() << " , " << this->end().y() << " )" << vcl_endl;
  os << "End angle : " << this->end_angle() << vcl_endl;
  os << "k1 = " << this->k1() << vcl_endl;
  os << "len1 = " << this->len1() << vcl_endl;
  os << "k2 = " << this->k2() << vcl_endl;
  os << "len2 = " << this->len2() << vcl_endl;
}

