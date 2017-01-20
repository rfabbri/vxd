#include "dvcpl_distmap_optimize_camera.h"

#include <vnl/vnl_vector_ref.h>
#include <vnl/vnl_double_3.h>
#include <vnl/algo/vnl_sparse_lm.h>
#include <vgl/algo/vgl_rotation_3d.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>

#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vcl_algorithm.h>
#include <ext/algorithm>
#include <vcl_cassert.h>

#include <vnl/vnl_rotation_matrix.h>
#include <vnl/vnl_double_3.h>
#include <vnl/vnl_double_3x3.h>
#include <vnl/algo/vnl_levenberg_marquardt.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vgl/algo/vgl_rotation_3d.h>

using namespace std;


dvcpl_distmap_optimize_camera_lsqr::
dvcpl_distmap_optimize_camera_lsqr(
      const vpgl_calibration_matrix<double> & K,
      vcl_vector< vgl_homg_point_3d<double>  > &world_points,
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<unsigned> &label,
      const dbdet_edgemap_sptr &em)
:
  vnl_least_squares_function(6,2*world_points.size(),no_gradient),
  K_(K),
  world_points_(world_points),
  dt_(dt),
  label_(label),
  em_(em)
{
}

//: The main function.
//  Given the parameter vector x, compute the vector of residuals fx.
//  Fx has been sized appropriately before the call.
//  The parameters in x are really two three component vectors {wx, wy, wz, tx, ty, tz}
//  where w is the Rodrigues vector of the rotation and t is the translation.
void
dvcpl_distmap_optimize_camera_lsqr::
f(vnl_vector<double> const& x, vnl_vector<double>& fx)
{
  assert(x.size() == 6);
  vnl_double_3 w(x[0], x[1], x[2]);
  vgl_homg_point_3d<double> t(x[3], x[4], x[5]);
  vpgl_perspective_camera<double> cam(K_,t,vgl_rotation_3d<double>(w));
  for (unsigned int i=0; i<world_points_.size(); ++i)
  {
    vgl_homg_point_2d<double> proj = cam(world_points_[i]);

    lookup_distance(proj, fx.data_block() + 2*i);
  }
}


vpgl_perspective_camera<double> dvcpl_distmap_optimize_camera::
opt_orient_pos(const vpgl_perspective_camera<double>& camera,
               vcl_vector< point_set > &world_objects,
               const vil_image_view<vxl_uint_32> &dt,
               const vil_image_view<unsigned> &label,
               const dbdet_edgemap_sptr &em,
               double *avg_err_ptr)
{
  const vpgl_calibration_matrix<double>& K = camera.get_calibration();
  vgl_point_3d<double> c = camera.get_camera_center();
  const vgl_rotation_3d<double>& R = camera.get_rotation();
  vnl_double_3 w = R.as_rodrigues();

  vcl_vector< vgl_homg_point_3d<double> > world_points;

  unsigned npts = 0;
  for (unsigned c=0; c < world_objects.size(); ++c) 
    npts += world_objects[c].size();

  world_points.reserve(npts);
  for (unsigned c=0; c < world_objects.size(); ++c) 
    for (unsigned ip=0; ip < world_objects[c].size(); ++ip)
      world_points.push_back(vgl_homg_point_3d<double>(world_objects[c][ip]));

  dvcpl_distmap_optimize_camera_lsqr lsqr_func(K, world_points, dt, label, em);

  vnl_levenberg_marquardt lm(lsqr_func);
  vnl_vector<double> params(6);
  params[0]=w[0];  params[1]=w[1];  params[2]=w[2];
  params[3]=c.x();  params[4]=c.y();  params[5]=c.z();

  {
  vnl_vector<double> fx(lsqr_func.get_number_of_residuals(), -1);
  lsqr_func.f(params, fx);

  double max_err, med_err;
  double avg_err = lsqr_func.reproj_error(fx, &max_err, &med_err);
  vcl_cout << "Initial average, max, med error: " << avg_err << ", " <<
    max_err << ", " << med_err << vcl_endl;

  if (avg_err == vcl_numeric_limits<double>::infinity()) {
    *avg_err_ptr = avg_err;
    return camera;
  }
  }

  lm.set_trace(true);
  lm.minimize(params);
  vnl_double_3 w_min(params[0],params[1],params[2]);
  vgl_homg_point_3d<double> c_min(params[3], params[4], params[5]);

  // final err
  {
  vnl_vector<double> fx(lsqr_func.get_number_of_residuals(), -1);
  lsqr_func.f(params, fx);
  double max_err, med_err;
  double avg_err = lsqr_func.reproj_error(fx, &max_err, &med_err);
  vcl_cout << "Final average, max, med error: " << avg_err << ", " <<
    max_err << ", " << med_err << vcl_endl;
  *avg_err_ptr = avg_err;
  if (avg_err == vcl_numeric_limits<double>::infinity())
    return camera;
  }

  return vpgl_perspective_camera<double>(K, c_min, vgl_rotation_3d<double>(w_min) );
}

vpgl_perspective_camera<double> dvcpl_distmap_optimize_camera::
opt_orient_pos_subset(const vpgl_perspective_camera<double>& camera,
               vcl_vector< point_set > &world_objects,
               const vil_image_view<vxl_uint_32> &dt,
               const vil_image_view<unsigned> &label,
               const dbdet_edgemap_sptr &em)
{
  // do optimization on all curves
  double best_err;
  vpgl_perspective_camera<double> best_cam= 
  opt_orient_pos(camera, world_objects, dt, label, em, &best_err);

  vpgl_perspective_camera<double> cam_ini = best_cam;
  double err_ini = best_err;

  // do optimization on ns subsets of nc curves, and take the minimum.

  unsigned const ns = 100, nc = 3, minsize = 20;
  vpgl_perspective_camera<double> cam = best_cam;

  for (unsigned i=0; i < ns; ++i) {

    vcl_vector< point_set > samp(nc);
    bool nosizes=true;
    do {
      random_sample_n(world_objects.begin(), world_objects.end(),
          samp.begin(), nc);
      assert(samp.size() == nc);
      for (unsigned c=0; c < nc; ++c) {
        if (samp[c].size() < minsize) {
          nosizes = true;
          break;
        } else
          nosizes = false;
      }
    } while(nosizes);

    vcl_cout << "Sampled curves with sizes(id): ";
    for (unsigned c=0; c < nc; ++c) {
      unsigned id = 0;
      vcl_vector< point_set >::iterator iter =  
        vcl_find(world_objects.begin(), world_objects.end(), samp[c]);
      assert(iter != world_objects.end());
      id = iter - world_objects.begin();
      vcl_cout << samp[c].size() << "(" << id << ") ";
    }
    vcl_cout << vcl_endl;
      

    double err3;
    vpgl_perspective_camera<double> newcam = 
    opt_orient_pos(cam, samp, dt, label, em, &err3);
    vcl_cout << "error 3: " << err3 << vcl_endl;

    double err_all;
    vpgl_perspective_camera<double> newcam_all = 
    opt_orient_pos(newcam, world_objects, dt, label, em, &err_all);

    if (err_all < best_err) {
      best_err = err_all;
      best_cam = newcam_all;
      cam = newcam_all;
    }
  }

  // do optimization on all curves one last time
  double err_last;
  vpgl_perspective_camera<double> cam_last = 
  opt_orient_pos(best_cam, world_objects, dt, label, em, &err_last);

  if (err_last < err_ini) {
    best_err = err_last;
    best_cam = cam_last;
  } else {
    best_err = err_ini;
    best_cam = cam_ini;
  }

  vcl_cout << "Very final average error: " << best_err << vcl_endl;

  vcl_cout <<"---\n";

  return best_cam;
}
