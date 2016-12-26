// This is basic/dbnl/dbnl_math.h

#ifndef dbnl_math_h_
#define dbnl_math_h_

#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif


//:
// \file
// \brief Various commonly used and simple math functions
// 
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date Oct 14, 2008
//
// \verbatim
//  Modifications:
// \endverbatim



//: Compute exponential with an integer power: f(x) = x^n
template<class T>
inline T dbnl_math_pow(T x, int n)
{
  T fx = 1;
  int abs_n = (n < 0) ? (-n) : n;
  for (int i =0; i < abs_n; ++i)
  {
    fx *= x;
  }

  return (n < 0) ? (T(1) / fx) : fx;
}


//: Return true if |x-y| <= tol
template<class T>
inline bool dbnl_math_near(T x, T y, T tol)
{
  T abs_diff = (x > y) ? (x-y) : (y-x);
  return abs_diff <= tol;
}

#endif // basic/dbnl/dbnl_math.h
