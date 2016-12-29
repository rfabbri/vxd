// This is brcv/mvg/dbecl/dbecl_episeg.cxx
//:
// \file

#include <vcl_cmath.h>
#include <vcl_algorithm.h>
#include <vcl_limits.h>
#include "dbecl_episeg.h"
#include <vsol/vsol_digital_curve_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <vgl/vgl_distance.h>

//: Default Constructor 
dbecl_episeg::dbecl_episeg() :
  epipole_(NULL),
  curve_(NULL),
  next_seg_(NULL),
  prev_seg_(NULL),
  min_dist_(-1.0),
  max_dist_(-1.0),
  min_index_(-1),
  max_index_(-1)
{}



//: Constructor takes an epipole and a 2D digital curve
dbecl_episeg::dbecl_episeg( const dbecl_epipole_sptr& epipole,
                          const vsol_digital_curve_2d_sptr& curve,
                          double min_index, double max_index) :
  epipole_(epipole),
  curve_(curve),
  next_seg_(NULL),
  prev_seg_(NULL),
  min_index_(min_index),
  max_index_(max_index)
{
  assert(min_index_ >= 0);
  assert(max_index_ >= min_index_);
  dir_positive_ = (angle(max_index) > angle(min_index));
  compute_bounds();
}



//: Destructor cleans up next and previous pointers
dbecl_episeg::~dbecl_episeg()  {
  if(next_seg_ != NULL) {
    // The next one's "previous" field should be pointing to me.
    // Make sure first, then update it.
    assert(next_seg_->prev_seg_ == this);
    next_seg_->prev_seg_ = prev_seg_;
  }
  if(prev_seg_ != NULL) {
    // Similar to above
    assert(prev_seg_->next_seg_ == this);
    prev_seg_->next_seg_ = next_seg_;
  }
}


//: Epipolar distance
double dbecl_episeg::dist(double theta)  const {
  return epipole_->distance(this->point(theta));
}


//: Index of the point at the given angle
double dbecl_episeg::index(double theta)  const
{
  // Make sure that theta is actually in range
  assert(theta >= this->min_angle());
  assert(theta <= this->max_angle());

  // Binary search to find bounding sample points
  int lower = index_search(theta, (int)vcl_floor(min_index_), (int)vcl_ceil(max_index_));

  // Linearly interpolate the index as a linear function of angle
  // This uses the small angle approximation.
  // This should be close enough in practice since linear segments are
  // generally one pixel long and hundreds of pixels away from the epipole.
  double lowerAngle = this->angle(lower);
  double upperAngle = this->angle(lower+1);
  double ratio = (theta - lowerAngle)/(upperAngle - lowerAngle);
  assert( ratio<=1.0 && ratio>=0.0 );
  double index = double(lower) + ratio;
  
  assert( index >= min_index_ && index <= max_index_ );
  return index;
}


//: Binary search to find the index
int
dbecl_episeg::index_search(double theta, int lower, int upper) const
{
  assert( upper > lower );
  int mid = (upper-lower)/2 + lower;
  if(mid <= lower)
    return lower;
  double mid_angle = this->angle((double)mid);
  if((theta > mid_angle) ^ dir_positive_)
    return index_search(theta, lower, mid);
  else
    return index_search(theta, mid, upper);
}


//: The point (possibly interpolated) at the given angle
vgl_point_2d<double> dbecl_episeg::point(double theta)  const {
  return curve_->interp( index(theta) );
}

//: Get the angle of the point at the given index on the real curve
double dbecl_episeg::angle(double idx) const {
  assert( epipole_ );
  assert( curve_ );
  assert( idx >= min_index_ );
  assert( idx <= max_index_ );
  return epipole_->angle( curve_->interp(idx) );
}

// BOUNDS
double dbecl_episeg::min_angle()  const {
  return dir_positive_ ? angle(min_index_) : angle(max_index_); 
}
double dbecl_episeg::max_angle()  const {
  return dir_positive_ ? angle(max_index_) : angle(min_index_);
}
double dbecl_episeg::min_dist()  const {
  return min_dist_;
}
double dbecl_episeg::max_dist()  const {
  return max_dist_;
}
double dbecl_episeg::min_index()  const {
  return min_index_;
}
double dbecl_episeg::max_index()  const {
  return max_index_;
}


//: Compute the bounds on distance
void dbecl_episeg::compute_bounds() 
{
  min_dist_ = vcl_numeric_limits<double>::infinity();
  max_dist_ = -vcl_numeric_limits<double>::infinity();

  for(int i = (int) vcl_ceil(min_index_); i <= (int) vcl_floor(max_index_); i++) {
    double d = epipole_->distance(curve_->interp(i));
    if(d > max_dist_) {
      max_dist_ = d;
    }
    if(d < min_dist_) {
      min_dist_ = d;
    }
  }
}
