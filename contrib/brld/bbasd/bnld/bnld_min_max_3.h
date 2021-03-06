// This is bbasd/bnld/bnld_min_max_3.h

#ifndef bnld_min_max_3_h_
#define bnld_min_max_3_h_

//:
// \file
// \brief Min and Max of 3 inputs.
// 
// \author Based on original code by  Ming-Ching Chang ( @lems.brown.edu)
// \date   Mar/07/2007
//
// \verbatim
//  Modifications:
// \endverbatim

#include <vcl_algorithm.h>

inline double bnld_min3 (const double& a, const double& b, const double& c)
{
  return vcl_min (vcl_min(a,b), vcl_min(b,c));
}

inline double bnld_max3 (const double& a, const double& b, const double& c)
{
  return vcl_max (vcl_max(a,b), vcl_max(b,c));
}

inline int bnld_max3 (const int& a, const int& b, const int& c)
{
  if (a>b) {
    if (a>c)
      return a;
    else
      return c;
  }
  else {
    if (b>c)
      return b;
    else
      return c;
  }
}

#endif
