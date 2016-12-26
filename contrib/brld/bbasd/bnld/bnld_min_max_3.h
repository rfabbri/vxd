// This is basic/dbnl/dbnl_min_max_3.h

#ifndef dbnl_min_max_3_h_
#define dbnl_min_max_3_h_

//:
// \file
// \brief Min and Max of 3 inputs.
// 
// \author Ming-Ching Chang ( mcchang@lems.brown.edu)
// \date   Mar/07/2007
//
// \verbatim
//  Modifications:
// \endverbatim

#include <vcl_algorithm.h>

inline double dbnl_min3 (const double& a, const double& b, const double& c)
{
  return vcl_min (vcl_min(a,b), vcl_min(b,c));
}

inline double dbnl_max3 (const double& a, const double& b, const double& c)
{
  return vcl_max (vcl_max(a,b), vcl_max(b,c));
}

inline int dbnl_max3 (const int& a, const int& b, const int& c)
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
