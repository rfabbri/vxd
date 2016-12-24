// This is bgld/algo/bgld_compute_symmetry_point.h
#ifndef bgld_compute_symmetry_point_h_
#define bgld_compute_symmetry_point_h_

//:
// \file
// \brief Functions to fit a circular arc spline to a curve
//        
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date Nov 19, 2009
//
// \verbatim
//  Modifications
// \endverbatim


#include <bgld/algo/bgld_circ_arc.h>
#include <vcl_vector.h>



//: Compute symmetry point on a right boundary line, given a left boundary point-tangent
// Tangent orientation follows Giblin and Kimia (PAMI'03) convention, i.e.,
// right tangent points forward
// left tangent points backward
void bgld_compute_symmetry_point_on_line(const vgl_point_2d<double >& right_line_start,
                                         const vgl_point_2d<double >& right_line_end,
                                         const vgl_point_2d<double >& left_bnd_pt,
                                         const vgl_vector_2d<double >& left_bnd_tangent,
                                         vcl_vector<double >& signed_distance_from_line_start);


//: Compute symmetry point on a right boundary arc given a point-tangent on the left boundary
// Tangent orientation follow Giblin and Kimia (PAMI'03) convention, i.e.
// right tangent points forward
// left tangent points backward
void bgld_compute_symmetry_point_on_circ_arc(const bgld_circ_arc& right_bnd_arc,
                                 const vgl_point_2d<double >& left_bnd_pt,
                                 const vgl_vector_2d<double >& left_bnd_tangent,
                                 vcl_vector<double >& s_along_right_bnd_arc);



#endif // bgld/algo/bgld_compute_symmetry_point.h


