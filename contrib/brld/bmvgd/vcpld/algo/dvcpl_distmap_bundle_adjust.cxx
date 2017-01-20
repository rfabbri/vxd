#include "dvcpl_distmap_bundle_adjust.h"

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
dvcpl_distmap_bundle_adj_lsqr::
dvcpl_distmap_bundle_adj_lsqr(
  const vcl_vector<vpgl_calibration_matrix<double> > &K,
  const vcl_vector<vil_image_view<vxl_uint_32> > &dt,
  const vcl_vector<vil_image_view<unsigned> > &label,
  const vcl_vector<dbdet_edgemap_sptr> &em,
  const vcl_vector<vcl_vector<bool> > &mask
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
       // num_residuals_per_e. d_normal_x d_normal_y
       2,
       // we're using finite differences for the Jacobian.
       no_gradient
       ),
   K_(K),
   dt_(dt),
   label_(label),
   em_(em),
   iteration_count_(0)
{
  Km_.reserve(K_.size());
  for (unsigned int i=0; i<K_.size(); ++i)
    Km_.push_back(K_[i].get_matrix());
}

void dvcpl_distmap_bundle_adj_lsqr::
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
      lookup_distance(i, xij, eij);
    }
  }
}

void dvcpl_distmap_bundle_adj_lsqr::
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
  lookup_distance(i, xij, fij.data_block());
}


dvcpl_distmap_bundle_adjust::
dvcpl_distmap_bundle_adjust()
  : 
    ba_func_(NULL),
    start_error_(0.0),
    end_error_(0.0)
{
}

dvcpl_distmap_bundle_adjust::
~dvcpl_distmap_bundle_adjust()
{
  if (!ba_func_)
    delete ba_func_;
}

//: Bundle Adjust
bool dvcpl_distmap_bundle_adjust::
optimize(vcl_vector<vpgl_perspective_camera<double> > &cameras,
         vcl_vector< point_set > &world_objects,
         const vcl_vector<vil_image_view<vxl_uint_32> > &dt,
         const vcl_vector<vil_image_view<unsigned> > &label,
         const vcl_vector<dbdet_edgemap_sptr> &em,
         const vcl_vector<vcl_vector<bool> > &mask)
{
  // Extract the camera and point parameters
  vcl_vector<vpgl_calibration_matrix<double> > K;
  a_ = dvcpl_distmap_bundle_adj_lsqr::create_param_vector(cameras);

  vcl_vector< vgl_point_3d<double> > world_points;

  unsigned npts = 0;
  for (unsigned c=0; c < world_objects.size(); ++c) 
    npts += world_objects[c].size();

  world_points.reserve(npts);
  for (unsigned c=0; c < world_objects.size(); ++c) 
    for (unsigned ip=0; ip < world_objects[c].size(); ++ip)
      world_points.push_back(world_objects[c][ip]);

  b_ = dvcpl_distmap_bundle_adj_lsqr::create_param_vector(world_points);
  for (unsigned int i=0; i < cameras.size(); ++i) {
    K.push_back(cameras[i].get_calibration());
  }

  // do the bundle adjustment
  delete ba_func_;
  ba_func_ = new dvcpl_distmap_bundle_adj_lsqr(K, dt, label, em, mask);
  vnl_sparse_lm lm(*ba_func_);
  lm.set_max_function_evals(1000);
  lm.set_trace(true);
  lm.set_verbose(true);
  vnl_vector<double> c_dummy;
  if (!lm.minimize(a_,b_, c_dummy))
    vcl_cerr << "Warning: bundle adjustment did not converge properly" << vcl_endl;

  start_error_ = lm.get_start_error();
  end_error_ = lm.get_end_error();
  num_iterations_ = lm.get_num_iterations();

  // Store camera parameters
  for (unsigned int i=0; i<cameras.size(); ++i)
    cameras[i] = ba_func_->param_to_cam(i,a_);
  // Update the point locations
  for (unsigned int j=0; j<world_points.size(); ++j)
    world_points[j] = ba_func_->param_to_point(j,b_);

  unsigned j=0;
  for (unsigned c=0; c < world_objects.size(); ++c) 
    for (unsigned ip=0; ip < world_objects[c].size(); ++ip, ++j) {
      world_objects[c][ip] = ba_func_->param_to_point(j,b_);
    }

  return true;
}
