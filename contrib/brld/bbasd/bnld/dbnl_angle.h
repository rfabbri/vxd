// This is basic/dbnl/dbnl_angle.h

#ifndef dbnl_angle_h_
#define dbnl_angle_h_

#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif


//:
// \file
// \brief Various simple functions for dealing with angles
// 
// \author Nhon Trinh ( ntrinh@lems.brown.edu )
// \date Oct 23, 2009
//
// \verbatim
//  Modifications:
// \endverbatim

#include <vcl_cmath.h>
#include <vcl_algorithm.h>

//: Convert an angle to [0, 2pi) range
template <class T>
inline T dbnl_angle_0to2pi(T angle_in_radian)
{
  const T twopi = static_cast<T>(6.28318530717958647692); //2*3.14159265358979323846;
  T x = vcl_fmod(angle_in_radian, twopi);
  return (x < 0) ? (x + twopi) : x;
};



//------------------------------------------------------------------------------
//: Sort a list of angle values in ascending order.
// The resulting angle array will be all positive with the smallest angle in [0, 2pi)
// Since angles are circular, the smallest angle is chosen to minimize the overall 
// range of the ange list. As a result, some angles may become larger than 2pi.
template <class T >
inline void dbnl_angle_sort_ascending(T* angle_array_in_radian, unsigned num_pts)
{
  // START HERE
  if (num_pts == 0) return;

  const T twopi = static_cast<T>(6.28318530717958647692); //2*3.14159265358979323846;

  // since angles are circular, we define min and as angles such that the range
  // between them covers all "angles" and is minimum.

  // Simplify the notation
  T* x = angle_array_in_radian;

  // convert to 0 to 2pi
  for (unsigned i =0; i < num_pts; ++i)
  {
    x[i] = dbnl_angle_0to2pi(x[i]);
  }

  // sort the angles
  vcl_sort(x, x+num_pts);

  // find the maximum difference between adjacent elements
  T* diff = new T[num_pts];
  for (unsigned i = 1; i < num_pts; ++i)
  {
    diff[i] = x[i] - x[i-1];
  }
  diff[0] = x[0] - (x[num_pts-1] - twopi);

  // position of maximum difference
  unsigned max_idx = 0;
  T max_diff = diff[0];
  for (unsigned i =1; i < num_pts; ++i)
  {
    if (diff[i] > max_diff)
    {
      max_diff = diff[i];
      max_idx = i;
    }
  }

  
  // min_angle in the angle list
  T min_angle = x[max_idx];

  // re-compute x so that it starts with min_angle and keeps increasing
  // Previous: x[0] x[1] ... x[max_idx] x[max_idx+1] ... x[num_pts-1]
  // Now -->                 x[0]       x[1] ....
  
  x[0] = min_angle;
  for (unsigned i = 1; i < num_pts; ++i)
  {
    unsigned pos = (i + max_idx) % (num_pts);
    x[i] = x[i-1] + diff[pos];
  }
  delete[] diff;
  return;
}



#endif // basic/dbnl/dbnl_angle.h
