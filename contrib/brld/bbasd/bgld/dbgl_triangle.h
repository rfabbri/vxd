// This is basic/dbgl/dbgl_triangle.h
#ifndef _dbgl_triangle_h_
#define _dbgl_triangle_h_
//:
// \file
// \brief  Basic triangle geometric.
// \author Ming-Ching Chang (mcchang@lems.brown.edu)
// \date   May/31/2005
//
// \verbatim
//         Modify on Mar 07, 2007.
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vcl_cmath.h>
#include <vcl_algorithm.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_distance.h>
#include <dbgl/dbgl_dist.h>
#include <dbgl/dbgl_fuzzy_boolean.h>

//: return true for acute triangle or obtuse triangle.
//  the right triangle is treated as non-acute triangle.
inline bool dbgl_is_tri_non_acute (const vgl_point_3d<double>& v0,
                                   const vgl_point_3d<double>& v1,
                                   const vgl_point_3d<double>& v2)
{
  double dasq = dbgl_sqdist_3d (v0, v1);
  double dbsq = dbgl_sqdist_3d (v1, v2);
  double dcsq = dbgl_sqdist_3d (v0, v2);

  if (dbgl_leq_m (dasq + dbsq, dcsq) ||
      dbgl_leq_m (dbsq + dcsq, dasq) ||
      dbgl_leq_m (dasq + dcsq, dbsq))
    return true;
  else
    return false;
}

inline bool dbgl_footpt_on_line (const vgl_point_3d<double>& P, 
                                  const vgl_point_3d<double>& A, 
                                  const vgl_point_3d<double>& B)
{
  vgl_vector_3d<double> AP = P-A;
  vgl_vector_3d<double> AB = B-A;
  double l = vgl_distance (A, B);
  double t = dot_product (AP, AB) / l;
  
  if (0<=t && t<=l)
    return true;
  else
    return false;
}

inline bool dbgl_footpt_on_line (const vgl_point_3d<double>& P, 
                                  const vgl_point_3d<double>& A, 
                                  const vgl_point_3d<double>& B,
                                  double& t, double& l)
{
  vgl_vector_3d<double> AP = P-A;
  vgl_vector_3d<double> AB = B-A;
  l = vgl_distance (A, B);
  t = dot_product (AP, AB) / l;
  
  if (0<=t && t<=l)
    return true;
  else
    return false;
}

#endif
