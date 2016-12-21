//: dbgl/dbgl_rigid_align.h
//  Jul 21, 2008  MingChing Chang

#ifndef dbgl_rigid_align_h_
#define dbgl_rigid_align_h_

#include <vcl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector_fixed.h>
#include <vgl/algo/vgl_h_matrix_3d.h>

void dbgl_get_rigid_alignment (vcl_vector<vgl_point_3d<double> >& cor_fixPS,
                               vcl_vector<vgl_point_3d<double> >& cor_movPS,
                               vnl_matrix_fixed<double,3,3>& R, 
                               vnl_vector_fixed<double,3>& fixC, 
                               vnl_vector_fixed<double,3>& movC);

void print_R_C1_C2 (const vnl_matrix_fixed<double,3,3>& R, 
                    const vnl_vector_fixed<double,3>& fixC, 
                    const vnl_vector_fixed<double,3>& movC);

void transform_point_3d (const vnl_matrix_fixed<double,3,3>& R, 
                         const vnl_vector_fixed<double,3>& fixC, 
                         const vnl_vector_fixed<double,3>& movC,
                         double& px, double& py, double& pz);

void transform_point_3d (const vnl_matrix_fixed<double,3,3>& R, 
                         const vnl_vector_fixed<double,3>& fixC, 
                         const vnl_vector_fixed<double,3>& movC,
                         vgl_point_3d<double>& Pt);

#endif

