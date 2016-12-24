// This is db3p/ray_tri/ray_tri.h
#ifndef ray_tri_h_
#define ray_tri_h_
//:
// \file
// \brief  test if a ray intersect a triangle or not.
//         Original code by Tomas Moller and Ben Trumbore's ray-triangle intersection
//         http://jgt.akpeters.com/papers/MollerTrumbore97/
//         Modified and Added by Ming-Ching Chang (mcchang@lems.brown.edu)
// \date   May/31/2005
//
// \verbatim
// \endverbatim
//
//-----------------------------------------------------------------------------

//: Test if a ray intersect a triangle or not.
//  Assume all triangles are oriented so a culling is used to improve performance.
//  Early jump-out: for the case of no intersection, 
//  the barycentric coord. (u, v) and the distance t are not computed.
//  return 0: not intersecting, 1: ray intersects the triangle.
//  t: dist(OT), (1-u-v, u, v): barycentric coord of T on triangle V0V1V2.
int intersect_triangle_cull_ejo (double orig[3], double dir[3],
                                 double vert0[3], double vert1[3], double vert2[3],
                                 double *t, double *u, double *v);


//: Test if a ray intersect a triangle or not.
//  Assume all triangles are not oriented.
//  Early jump-out: for the case of no intersection, 
//  the barycentric coord. (u, v) and the distance t are not computed.
//  return 0: not intersecting, 1: ray intersects the triangle.
//  t: dist(OT), (1-u-v, u, v): barycentric coord of T on triangle V0V1V2.
int intersect_triangle_ejo (double orig[3], double dir[3],
                            double vert0[3], double vert1[3], double vert2[3],
                            double *t, double *u, double *v);

//: Test if a ray intersect a triangle or not.
//  Assume all triangles are not oriented and no early-jump out.
//  return 0: not intersecting, 1: ray intersects the triangle.
//  t: dist(OT), (1-u-v, u, v): barycentric coord of T on triangle V0V1V2.
int intersect_triangle (double orig[3], double dir[3],
                        double vert0[3], double vert1[3], double vert2[3],
                        double *t, double *u, double *v);

///int intersect_triangle_fuzzy (double orig[3], double dir[3],
///                              double vert0[3], double vert1[3], double vert2[3],
///                              double *t, double *u, double *v);

#endif
