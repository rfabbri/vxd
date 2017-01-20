// This is dvcpl_distmap_bundle_adjust.h
#ifndef dvcpl_distmap_bundle_adjust_h
#define dvcpl_distmap_bundle_adjust_h
//:
//\file
//\brief Bundle adjustment from curves and edge maps.
//\author Based on original code by  Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 11/05/2009 09:52:56 PM PST
//


#include <vcl_vector.h>
#include <vcl_limits.h>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_sparse_lst_sqr_function.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_homg_point_3d.h>
#include <vgl/algo/vgl_rotation_3d.h>
#include <vil/vil_image_view.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <dbdet/edge/dbdet_edgemap.h>
#include <dbdet/edge/dbdet_edgemap_sptr.h>


//: Computes the residuals for bundle adjustment similarly to vpgl_distmap_bundle_adj.
//
// This implementation is useful in practical cases where one wants to simply
// refine an initial camera setup as much as possible, but technically it may get
// stuck in non-differentiable regions of the cost function. We recommend
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
// To achive this subpixel precision, we take in the original subpixel
// feature map and a closest label map indexing into it so that the closest
// feature can be looked up and the actual distance computed. In this class, we
// use a subpixel edge map, this can use orientation to measure only distance
// along the normal direction to the projected 3D points.
//
// The 3D points are not oriented in this formulation. During optimization, only
// their position is updated.
//
// \remarks Specifying covariance for each point is not yet supported.
class dvcpl_distmap_bundle_adj_lsqr : public vnl_sparse_lst_sqr_function
{
public:
  //: Constructor
  //
  // \remarks We may want to smooth the distance map a little and convert to a
  // floating point version, so that the Jacobian can make more sense near
  // Voronoi boundaries.
  dvcpl_distmap_bundle_adj_lsqr(
      const vcl_vector<vpgl_calibration_matrix<double> >& K,
      const vcl_vector<vil_image_view<vxl_uint_32> > &dt,
      const vcl_vector<vil_image_view<unsigned> > &label,
      const vcl_vector<dbdet_edgemap_sptr> &em,
      const vcl_vector<vcl_vector<bool> >& mask);

  //: Compute the residuals from the ith component of a and the jth component of b.
  //  This is not normally used because f() has a self-contained efficient implementation
  //  It is used for finite-differencing.
  //
  //  \todo copy over the finite differencing functions and modify them so that
  //  we don't need to reconstitute the camera matrix every time they call this.
  virtual void fij(int i, int j, vnl_vector<double> const& ai,
      vnl_vector<double> const& bj, vnl_vector<double>& fij);

  //: See base class documentation. Why do we override f? This is almost exactly
  // the same function as the base class, but here we can construct the camera
  // matrix from the parameters on the outer loop.
  void f(vnl_vector<double> const& a, vnl_vector<double> const& b, vnl_vector<double>& e);

  //: construct the ith perspective camera matrix from parameter vector \param a
  //  Bypass vpgl for efficiency
  vnl_double_3x4 param_to_cam_matrix(int i, const vnl_vector<double>& a) const {
    return param_to_cam_matrix(i, a.data_block()+index_a(i));
  }

  //: Fast conversion of rotation from Rodrigues vector to matrix
  static vnl_matrix_fixed<double,3,3> rod_to_matrix(const double* r)
  {
    double x2 = r[0]*r[0], y2 = r[1]*r[1], z2 = r[2]*r[2];
    double m = x2 + y2 + z2;
    double theta = vcl_sqrt(m);
    double s = vcl_sin(theta) / theta;
    double c = (1.0 - vcl_cos(theta)) / m;

    vnl_matrix_fixed<double,3,3> R(0.0);
    R(0,0) = R(1,1) = R(2,2) = 1.0;
    if (m == 0.0)
      return R;

    R(0,0) -= (y2 + z2) * c;
    R(1,1) -= (x2 + z2) * c;
    R(2,2) -= (x2 + y2) * c;
    R(0,1) = R(1,0) = r[0]*r[1]*c;
    R(0,2) = R(2,0) = r[0]*r[2]*c;
    R(1,2) = R(2,1) = r[1]*r[2]*c;

    double t = r[0]*s;
    R(1,2) -= t;
    R(2,1) += t;
    t = r[1]*s;
    R(0,2) += t;
    R(2,0) -= t;
    t = r[2]*s;
    R(0,1) -= t;
    R(1,0) += t;

    return R;
  }
  
  vnl_double_3x4 param_to_cam_matrix(int i, const double* d) const {
    vnl_double_3x3 M = Km_[i]*rod_to_matrix(d);
    vnl_double_3x4 P;
    P.update(M);
    const vnl_vector_ref<double> c(3,const_cast<double*>(d+3));
    P.set_column(3,-(M*c));
    return P;
  }

  //: Create the parameter vector \p a from a vector of cameras
  static vnl_vector<double>
  create_param_vector(const vcl_vector<vpgl_perspective_camera<double> >& cameras)
  {
    vnl_vector<double> a(6*cameras.size(),0.0);
    for (unsigned int i=0; i<cameras.size(); ++i)
    {
      const vpgl_perspective_camera<double>& cam = cameras[i];
      const vgl_point_3d<double>& c = cam.get_camera_center();
      const vgl_rotation_3d<double>& R = cam.get_rotation();

      // compute the Rodrigues vector from the rotation
      vnl_vector<double> w = R.as_rodrigues();

      double* ai = a.data_block() + i*6;
      ai[0]=w[0];   ai[1]=w[1];   ai[2]=w[2];
      ai[3]=c.x();  ai[4]=c.y();  ai[5]=c.z();
    }
    return a;
  }

  //: Create the parameter vector \p b from a vector of 3D points
  static vnl_vector<double>
  create_param_vector(const vcl_vector<vgl_point_3d<double> >& world_points)
  {
    vnl_vector<double> b(3*world_points.size(),0.0);
    for (unsigned int j=0; j<world_points.size(); ++j){
      const vgl_point_3d<double>& point = world_points[j];
      double* bj = b.data_block() + j*3;
      bj[0]=point.x();  bj[1]=point.y();  bj[2]=point.z();
    }
    return b;
  }

  //: construct the ith perspective camera from parameter vector a
  vpgl_perspective_camera<double> param_to_cam(int i, const vnl_vector<double>& a) const
  {
    return param_to_cam(i, a.data_block()+index_a(i));
  }

  vpgl_perspective_camera<double> param_to_cam(int i, const double* d) const
  {
    vnl_vector<double> w(d,3);
    vgl_homg_point_3d<double> t(d[3], d[4], d[5]);
    return vpgl_perspective_camera<double>(K_[i],t,vgl_rotation_3d<double>(w));
  }
  
  //: construct the jth 3D point from parameter vector b
  vgl_homg_point_3d<double> param_to_point(int j, const vnl_vector<double>& b) const
  {
    const double* d = b.data_block() + index_b(j);
    return vgl_homg_point_3d<double>(d[0], d[1], d[2]);
  }

private:
  //: this might be float in the future.
  typedef vil_image_view<vxl_uint_32> dt_type;

  //: The fixed internal camera calibration
  vcl_vector<vpgl_calibration_matrix<double> > K_;
  //: The fixed internal camera calibration in matrix form
  vcl_vector<vnl_double_3x3> Km_;

  //: The distance maps for the measured projected features in each view being
  // optimized. This is fixed throughout.
  vcl_vector<dt_type> dt_;
  //: The nearest label maps for the measured projected features in each
  // view being optimized. This is fixed throughout.
  vcl_vector<vil_image_view<unsigned> > label_;

  //: The subpixel edge maps for each view.
  const vcl_vector<dbdet_edgemap_sptr> em_;

  unsigned iteration_count_;

  //: Evaluates the distance map at the point.
  // If point outside the distance map, set to infinity.
  // Maybe there's a smoother way to treat that case.
  // \param[in] v view
  // \param[in] pt the query point
  // \param[out] e: the error vector (dx,dy)
  void lookup_distance(unsigned v, const vnl_vector_fixed<double,3> &pt, double e[]) const
  {
    double px = pt[0]/pt[2];
    double py = pt[1]/pt[2];
    unsigned p_i = static_cast<unsigned>(px+0.5);
    unsigned p_j = static_cast<unsigned>(py+0.5);

    if (!label_[v].in_range(p_i, p_j)) {
      e[0] = e[1] = vcl_numeric_limits<double>::infinity();
      return;
    }

    double min_d = vcl_numeric_limits<double>::infinity();
    unsigned l = label_[v](p_i, p_j);
    const vcl_vector<dbdet_edgel*> &ev = em_[v]->edge_cells.begin()[l];
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
};

//: Static functions for bundle adjustment
class dvcpl_distmap_bundle_adjust
{
  typedef vcl_vector<vgl_point_3d<double> > point_set;

 public:
  //: Constructor
  dvcpl_distmap_bundle_adjust();
  //: Destructor
  ~dvcpl_distmap_bundle_adjust();

  //: Return the ending error
  double end_error() const { return end_error_; }
  //: Return the starting error
  double start_error() const { return start_error_; }
  //: Return the number of iterations
  int num_iterations() const { return num_iterations_; }

  //: Return the raw camera parameters
  const vnl_vector<double>& cam_params() const { return a_; }
  //: Return the raw world point parameters
  const vnl_vector<double>& point_params() const { return b_; }

  //: Bundle Adjust
  bool optimize(vcl_vector<vpgl_perspective_camera<double> > &cameras,
                vcl_vector< point_set > &world_objects,
                const vcl_vector<vil_image_view<vxl_uint_32> > &dt,
                const vcl_vector<vil_image_view<unsigned> > &label,
                const vcl_vector<dbdet_edgemap_sptr> &em,
                const vcl_vector<vcl_vector<bool> > &mask);

 private:
  //: The bundle adjustment error function
  dvcpl_distmap_bundle_adj_lsqr* ba_func_;
  //: The last camera parameters
  vnl_vector<double> a_;
  //: The last point parameters
  vnl_vector<double> b_;

  double start_error_;
  double end_error_;
  int num_iterations_;
};

#endif // dvcpl_distmap_bundle_adjust_h
