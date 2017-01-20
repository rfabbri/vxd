// This is vcpld_distmap_optimize_camera.h
#ifndef vcpld_distmap_optimize_camera_
#define vcpld_distmap_optimize_camera_
//:
//\file
//\brief Optimizes camera given 3D points and 2D edgemaps
//\author Based on original code by  Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 06/07/10 20:01:59 EDT
//


#include <vcl_vector.h>
#include <vcl_limits.h>
#include <vnl/vnl_vector.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_homg_point_3d.h>
#include <vgl/algo/vgl_rotation_3d.h>
#include <vil/vil_image_view.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <sdet/sdetd_edgemap.h>
#include <sdet/sdetd_edgemap_sptr.h>

#include <vnl/vnl_least_squares_function.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_homg_point_3d.h>
#include <bmcsd/bmcsd_util.h>

//:
//
// This implementation is useful in practical cases where one wants to simply
// refine an initial camera setup as much as possible, but technically it may get
// stuck in non-differentiable regions of the cost function.  We recommend
// looking into the work of Irina Nurutdinova and Andrew Fitzgibbon:
//
// @inproceedings{nurutdinova2015towards,
//  title={Towards Pointless Structure from Motion: 3D reconstruction and camera
//  parameters from general 3D curves},
//  author={Nurutdinova, Irina and Fitzgibbon, Andrew},
//  booktitle={Proceedings of the IEEE International Conference on Computer Vision},
//  pages={2363--2371},
//  year={2015}
// }
//
class vcpld_distmap_optimize_camera_lsqr : public vnl_least_squares_function
{
  typedef vcl_vector<vgl_point_3d<double> > point_set;
  public:

  vcpld_distmap_optimize_camera_lsqr(
      const vpgl_calibration_matrix<double> &K,
      vcl_vector<  vgl_homg_point_3d<double> > &world_points,
      const vil_image_view<vxl_uint_32> &dt,
      const vil_image_view<unsigned> &label,
      const sdetd_edgemap_sptr &em);

  //: The main function.
  //  Given the parameter vector x, compute the vector of residuals fx.
  //  Fx has been sized appropriately before the call.
  //  The parameters in x are really two three component vectors {wx, wy, wz, tx, ty, tz}
  //  where w is the Rodrigues vector of the rotation and t is the translation.
  virtual void f(vnl_vector<double> const& x, vnl_vector<double>& fx);

  //: Evaluates the distance map at the point.
  // If point outside the distance map, set to infinity.
  // Maybe there's a smoother way to treat that case.
  // \param[in] pt the query point
  // \param[out] e: the error vector (dx,dy)
  void lookup_distance(const vgl_homg_point_2d<double> &pt, double e[]) const
  {
    double px = pt.x()/pt.w();
    double py = pt.y()/pt.w();
    unsigned p_i = static_cast<unsigned>(px+0.5);
    unsigned p_j = static_cast<unsigned>(py+0.5);

    if (!label_.in_range(p_i, p_j)) {
      e[0] = e[1] = vcl_numeric_limits<double>::infinity();
      return;
    }

    double min_d = vcl_numeric_limits<double>::infinity();
    unsigned l = label_(p_i, p_j);
    const vcl_vector<sdetd_edgel*> &ev = em_->edge_cells.begin()[l];
#ifndef NDEBUG
    if (ev.size() == 0) {
      vcl_cout << "label: " << l 
        << " px: " << px << " py: " << py << vcl_endl;

      vcl_cout << "label_: " << label_.top_left_ptr()[l] << vcl_endl;
      vcl_cout << "DT: "<< dt_(p_i,p_j) << vcl_endl;
      vcl_cout << "DT(label): "<< dt_.top_left_ptr()[l] << vcl_endl;
      // const vcl_vector<sdetd_edgel*> &ev = em_->edge_cells.begin()[l];
    }
    assert(ev.size());
#endif
    for (unsigned i=0; i < ev.size(); ++i) {
      // form a vector d betwen pt and ev
      // project this vcl_vector onto the normal; this is our residual
      double dx = px - ev[i]->pt.x();
      double dy = py - ev[i]->pt.y();

      if (dx*dx + dy*dy < min_d) {
        min_d = dx*dx + dy*dy;
        double nx = -vcl_sin(ev[i]->tangent);
        double ny = vcl_cos(ev[i]->tangent);
        double dt_dot_n = dx*nx + dy*ny;
        e[0] = dt_dot_n*nx;
        e[1] = dt_dot_n*ny;
      }
    }
  }

  // \returns average error.
  // also outputs max error, median error.
  double reproj_error(vnl_vector<double>& fx, double *max_err, double *med_err)
  {
    vcl_vector<double> ev((unsigned)(fx.size()*0.5));
    double e=0;
    for (unsigned i=0; i+1 < fx.size(); i+= 2) {
      ev[i/2] = vcl_sqrt(fx[i]*fx[i] + fx[i+1]*fx[i+1]);
      e += ev[i/2];
//      vcl_cout << "err: " << ei << vcl_endl;
    }

    unsigned id;
    bmcsd_util::max(ev, id);
    *max_err = ev[id];
    *med_err  =  bmcsd_util::median(ev);
   
    return  e /= (fx.size()*0.5);
  }


  private:
  //: this might be float in the future.
  typedef vil_image_view<vxl_uint_32> dt_type;

  //: The fixed internal camera calibration
  vpgl_calibration_matrix<double> K_;
  //: The known points in the world
  vcl_vector<vgl_homg_point_3d<double> > world_points_;

  //: The distance maps for the measured projected features in each view being
  // optimized. This is fixed throughout.
  dt_type dt_;
  //: The nearest label maps for the measured projected features in each
  // view being optimized. This is fixed throughout.
  vil_image_view<unsigned> label_;
  //: The subpixel edge maps for each view.
  const sdetd_edgemap_sptr em_;
};





class vcpld_distmap_optimize_camera 
{
  typedef vcl_vector<vgl_point_3d<double> > point_set;

 public:

  //: optimize orientation and position for a perspective camera
  static vpgl_perspective_camera<double>
    opt_orient_pos(const vpgl_perspective_camera<double>& camera,
                  vcl_vector< point_set > &world_objects,
                  const vil_image_view<vxl_uint_32> &dt,
                  const vil_image_view<unsigned> &label,
                  const sdetd_edgemap_sptr &em)
    {
      double dummy;
      return opt_orient_pos(camera, world_objects, dt, label, em, &dummy);
    }

  static vpgl_perspective_camera<double> 
  opt_orient_pos(const vpgl_perspective_camera<double>& camera,
                 vcl_vector< point_set > &world_objects,
                 const vil_image_view<vxl_uint_32> &dt,
                 const vil_image_view<unsigned> &label,
                 const sdetd_edgemap_sptr &em,
                 double *avg_err_ptr);

  static vpgl_perspective_camera<double> 
  opt_orient_pos_subset(const vpgl_perspective_camera<double>& camera,
                 vcl_vector< point_set > &world_objects,
                 const vil_image_view<vxl_uint_32> &dt,
                 const vil_image_view<unsigned> &label,
                 const sdetd_edgemap_sptr &em);
};


#endif // vcpld_distmap_optimize_camera_
