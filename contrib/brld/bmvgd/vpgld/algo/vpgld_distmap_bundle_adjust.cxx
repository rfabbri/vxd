#include "dvpgl_distmap_bundle_adjust.h"

#include <vnl/vnl_vector_ref.h>
#include <vnl/vnl_double_3.h>
#include <vnl/algo/vnl_sparse_lm.h>
#include <vgl/algo/vgl_rotation_3d.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>

#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vcl_algorithm.h>
#include <vcl_cassert.h>

//: Constructor
dvpgl_distmap_bundle_adj_lsqr::
dvpgl_distmap_bundle_adj_lsqr(
  const vcl_vector<vpgl_calibration_matrix<double> >& K,
  const vcl_vector<vil_image_view<vxl_uint_32> > &dt,
  const vcl_vector<vcl_vector<bool> >& mask
  )
 : 
   // The base
   vnl_sparse_lst_sqr_function(
       // num_a (number of cameras as the number of supplied K matrices)
       K.size(),
       // num_params_per_a (rotation, translation)
       6,
       // num_b (number of 3D points being estimated).
       // This is because
       // mask[id_view][id_point] == true if view[id_view]
       // sees a projection of point[id_point].
       mask[0].size(),
       // num_params_per_b (X,Y,Z)
       3,
       // num_params_per_c
       0, 
       // mask indicating which residuals exist.
       // mask[id_view][id_point] == true if view[id_view]
       // sees a projection of point[id_point].
       mask,
       // num_residuals_per_e
       1,
       // we're using finite differences for the Jacobian.
       no_gradient
       ),
   K_(K),
   dt_(dt),
   iteration_count_(0)
{
  Km_.reserve(K_.size());
  for (unsigned int i=0; i<K_.size(); ++i)
    Km_.push_back(K_[i].get_matrix());
}

void dvpgl_distmap_bundle_adj_lsqr::
f(vnl_vector<double> const& a,
  vnl_vector<double> const& b,
  vnl_vector<double>& e)
{
  typedef vnl_crs_index::sparse_vector::iterator sv_itr;
  for (unsigned int i=0; i < number_of_a(); ++i) {
    //: Construct the ith camera
    vnl_double_3x4 Pi = param_to_cam_matrix(i,a);

    vnl_crs_index::sparse_vector row = residual_indices_.sparse_row(i);
    for (sv_itr r_itr=row.begin(); r_itr != row.end(); ++r_itr) {
      unsigned int j = r_itr->second;
      unsigned int k = r_itr->first;

      // Construct the jth point
      const double * bj = b.data_block()+index_b(j);
      vnl_vector_fixed<double,4> Xj(bj[0], bj[1], bj[2], 1.0);

      // Project jth point with the ith camera
      vnl_vector_fixed<double,3> xij = Pi*Xj;

      double* eij = e.data_block()+index_e(k);

      // Now just look up the distance in the image: 
      eij[0] = lookup_distance(dt_[i], xij);
    }
  }
}

void dvpgl_distmap_bundle_adj_lsqr::
fij(int i, int /*j*/, vnl_vector<double> const& ai,
    vnl_vector<double> const& bj, vnl_vector<double>& fij)
{
  //: Construct the ith camera. Don't want to do this all the time, which is why
  // we rewrote f itself.
  vnl_double_3x4 Pi = param_to_cam_matrix(i, ai.data_block());

  // Construct the jth point
  vnl_vector_fixed<double,4> Xj(bj[0], bj[1], bj[2], 1.0);

  // Project jth point with the ith camera
  vnl_vector_fixed<double,3> xij = Pi*Xj;

  // Now just look up the distance in the image:
  fij[0] = lookup_distance(dt_[i], xij);
}
