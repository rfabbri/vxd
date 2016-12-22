// This is bbasd/bgld/bgld_fuzzy_boolean.h
#ifndef _bgld_fuzzy_boolean_h_
#define _bgld_fuzzy_boolean_h_
//:
// \file
// \brief  DBGL fuzzy boolean functions.
// \author Based on original code by  Ming-Ching Chang (@lems.brown.edu)
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
inline bool bgld_eq (const double& a, const double& b, const double& epsilon)
{
  return vcl_fabs (a-b) < epsilon;
}

//: the mid-epsilon equality test, use absolute test.
inline bool bgld_eq_m (const double& a, const double& b)
{
  return vcl_fabs (a-b) < DBGL_MID_EPSILON;
}

inline bool bgld_leq_m (const double& a, const double& b)
{
  return a < b || bgld_eq_m (a, b);
}

//: the coarse-epsilon equality test, use absolute test.
inline bool bgld_eq_c (const double& a, const double& b)
{
  return vcl_fabs (a-b) < DBGL_COARSE_EPSILON;
}

#endif
