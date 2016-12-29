// This is vpgld_distmap_bundle_adjust.h
#ifndef vpgld_distmap_bundle_adjust_h
#define vpgld_distmap_bundle_adjust_h
//:
//\file
//\brief Bundle adjustment using distance tranforms.
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
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


//: Computes the residuals for bundle adjustment just like vpgl_bundle_adj,
// but where the residuals of reprojected points are computed using a distance
// map for fast look up of distances to nearest image feature sets.
//
// \remarks Specifying covariance for each point is not yet supported.
//
// \todo to achive subpixel precision, write a variant of this class that
// takes in the original subpixel feature map and a closest label map indexing
// into it so that the closest feature can be looked up and the actual distance
// computed. In the case of edgels, this can use orientation to measure only
// distance along the normal direction.
class vpgld_distmap_bundle_adj_lsqr : public vnl_sparse_lst_sqr_function
{
  //: Constructor
  //
  // \remarks We may want to smooth the distance map a little and convert to a
  // floating point version, so that the Jacobian can make more sense near
  // Voronoi boundaries.
  vpgld_distmap_bundle_adj_lsqr(
      const vcl_vector<vpgl_calibration_matrix<double> >& K,
      const vcl_vector<vil_image_view<vxl_uint_32> > &dt,
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
  unsigned iteration_count_;

  //: Evaluates the distance map at the point.
  // If point outside the distance map, set to infinity.
  // Maybe there's a smoother way to treat that case.
  static double lookup_distance(const dt_type &dt, const vnl_vector_fixed<double,3> &pt)
  {
    unsigned x = static_cast<unsigned>((pt[0]/pt[2])+0.5);
    unsigned y = static_cast<unsigned>((pt[1]/pt[2])+0.5);
    if (dt.in_range(x,y))
      return dt(x,y);
    else
      return vcl_numeric_limits<double>::infinity();
  }
};

#endif // vpgld_distmap_bundle_adjust_h
