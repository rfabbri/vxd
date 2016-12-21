// This is basic/dbgl/dbgl_fuzzy_boolean.h
#ifndef _dbgl_fuzzy_boolean_h_
#define _dbgl_fuzzy_boolean_h_
//:
// \file
// \brief  DBGL fuzzy boolean functions.
// \author Ming-Ching Chang (mcchang@lems.brown.edu)
// \date   May/07/2007
//
// \verbatim
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vcl_cmath.h>
#include <vcl_algorithm.h>

#define DBGL_FINE_EPSILON     5E-15
#define DBGL_MID_EPSILON      5E-10
#define DBGL_COARSE_EPSILON   5E-5

//: the mid-epsilon equality test, use absolute test.
inline bool dbgl_eq (const double& a, const double& b, const double& epsilon)
{
  return vcl_fabs (a-b) < epsilon;
}

//: the mid-epsilon equality test, use absolute test.
inline bool dbgl_eq_m (const double& a, const double& b)
{
  return vcl_fabs (a-b) < DBGL_MID_EPSILON;
}

inline bool dbgl_leq_m (const double& a, const double& b)
{
  return a < b || dbgl_eq_m (a, b);
}

//: the coarse-epsilon equality test, use absolute test.
inline bool dbgl_eq_c (const double& a, const double& b)
{
  return vcl_fabs (a-b) < DBGL_COARSE_EPSILON;
}

#endif
