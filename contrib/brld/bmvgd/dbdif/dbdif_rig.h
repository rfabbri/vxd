// This is dbdif_rig.h
#ifndef dbdif_rig_h
#define dbdif_rig_h
//:
//\file
//\brief Structure to aid programming based on global coordinates as in notation of the paper:
// "High-Order Differential geometry of Curves for Multiview Reconstruction 
// and Matching", Ricardo Fabbri & Benjamin Kimia, EMMCVPR'05
//
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 05/04/2005 06:26:54 PM EDT
//

#include <vgl/vgl_homg_point_3d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vgl/vgl_homg_line_2d.h>
#include <vgl/algo/vgl_homg_operators_2d.h>
#include <vgl/algo/vgl_homg_operators_3d.h>
#include <vnl/vnl_cross.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vpgl/vpgl_fundamental_matrix.h>
#include <dbdif/dbdif_util.h>
#include <dbdif/dbdif_frenet.h>
#include <dbdif/dbdif_camera.h>

//: Currently contains only 2 perspective cameras.  Basically contains methods 
// for reconstruction and correspondences using multiple dbdif_cameras.
//
// It caches camera-related data to facilitate work in global
// non-homogeneous coordinates. In this philosophy, we avoid converting back to 
// image pixel coordinates whenever we can. Also, the cache variable holds all compatible
// representations when possible, e.g. matrices and vectors
//
class dbdif_rig {
public:
  dbdif_rig() : cam(2) {}

  dbdif_rig(const vpgl_perspective_camera <double> &P1, const vpgl_perspective_camera <double> &P2);

  ~dbdif_rig() {}

  double reconstruct_point_lsqr(
     const dbdif_vector_3d &gama1, const dbdif_vector_3d &gama2,
     dbdif_vector_3d *Cpt_v) const;

  double reconstruct_point_lsqr(
     const vsol_point_2d_sptr &xi1, const vsol_point_2d_sptr &xi2,
     dbdif_vector_3d *Cpt_v) const;

  void reconstruct_3d_curve(
    vcl_vector<dbdif_vector_3d> *Crv3d,
    const vcl_vector<dbdif_vector_3d> &crv1, 
    const vcl_vector<dbdif_vector_3d> &crv2);

  void reconstruct_3d_curve(
    vcl_vector<dbdif_vector_3d> *C3d,
    const vcl_vector<vsol_point_2d_sptr> &c1, 
    const vcl_vector<vsol_point_2d_sptr> &c2);
   
  //: All input in world coordinates and normalized.
  inline bool reconstruct_tangent(
     const dbdif_vector_3d &gama1, const dbdif_vector_3d &gama2,
     const dbdif_vector_3d &t1, const dbdif_vector_3d &t2,
     dbdif_vector_3d *Cpt_v) const;

  //: Overload for tangents as vgl_vector_2d and in image coordinates. However,
  // \p gama_1 and \p gama_2 are in normalized world coordinates.
  void reconstruct_tangent(
        const dbdif_vector_3d &gama1, const dbdif_vector_3d &gama2,
        const vgl_vector_2d<double> &t1_img, const vgl_vector_2d<double> &t2_img,
        dbdif_vector_3d *T_rec) const;

  //: Reconstruct a 3D point tangent, given corresponding 2D point-tangents.
  // Inputs are in normalized world coordinates.
  //
  // \remark the normals at the 2D points are not used.
  void reconstruct_1st_order(const dbdif_1st_order_point_2d &frame1, 
        const dbdif_1st_order_point_2d &frame2,
        dbdif_1st_order_point_3d *Frame) const;

  //: reconstruct 3D curvature and normal, starting from 2D 2nd order measures
  void reconstruct_2nd_order(const dbdif_2nd_order_point_2d &frame1, 
        const dbdif_2nd_order_point_2d &frame2,
        dbdif_2nd_order_point_3d *Frame) const;

  void reconstruct_3rd_order( const dbdif_3rd_order_point_2d &fr1, 
      const dbdif_3rd_order_point_2d &fr2,
      dbdif_3rd_order_point_3d *Frame) const;

  //: returns angle of given vector to the normal to the epipolar plane
  // Angle value btween 0 and pi
  double angle_with_epipolar_plane(const dbdif_vector_3d &v_world, 
      const dbdif_vector_3d &gama) const;

  //: Angle of vector in left image (v_img) with an epipolar line in the left image through pos_img
  static double angle_with_epipolar_line(
      const dbdif_vector_3d &v_img, 
      const dbdif_vector_3d &pos_img, 
      const vpgl_fundamental_matrix<double> &F);
    
  static vgl_homg_line_2d<double> 
  l_epipolar_line(
      const vpgl_fundamental_matrix<double> &F, 
      const vgl_homg_point_2d<double>& pl);

  static vgl_homg_line_2d<double>
  l_epipolar_line(
      const vpgl_fundamental_matrix<double> &F, 
      const vgl_homg_line_2d<double> &ep_r);

  // -------------------------------------------------------------------------------
  // Methods for differential imagery and occluding contours 
  // WARNING: code may be obsolete and not working
  // -------------------------------------------------------------------------------

  bool reconstruct_occluding_contour_point(
        const dbdif_vector_3d &gamma,
        const dbdif_vector_3d &gamma_s,
        dbdif_vector_3d delta_gama_t,
        dbdif_vector_3d *Cpt_v);

  bool reconstruct_occluding_contour_point(
        const dbdif_vector_3d &gamma,
        const dbdif_vector_3d &gamma_s,
        dbdif_vector_3d delta_gamma_t,
        dbdif_vector_3d *Cpt_v,
        dbdif_vector_3d delta_c);

  void reconstruct_3d_occluding_contour(
    vcl_vector<dbdif_vector_3d> *Crv3d,
    vcl_vector<bool> &valid,
    const vcl_vector<vsol_point_2d_sptr> &crv1, 
    const vcl_vector<vsol_point_2d_sptr> &crv2);

  void reconstruct_3d_occluding_contour(
    vcl_vector<dbdif_vector_3d> *Crv3d,
    vcl_vector<bool> &valid,
    const vcl_vector<vsol_point_2d_sptr> &crv1, 
    const vcl_vector<vsol_point_2d_sptr> &crv2,
    const dbdif_vector_3d &dC);

  void reconstruct_3d_occluding_contour(
    vcl_vector<dbdif_vector_3d> *Crv3d,
    vcl_vector<bool> &valid,
    const vcl_vector<dbdif_vector_3d> &crv1, 
    const vcl_vector<dbdif_vector_3d> &crv2);

  void reconstruct_3d_occluding_contour(
    vcl_vector<dbdif_vector_3d> *Crv3d,
    vcl_vector<bool> &valid,
    const vcl_vector<dbdif_vector_3d> &crv1, 
    const vcl_vector<dbdif_vector_3d> &crv2,
    const dbdif_vector_3d &dC);

public: // DATA

  vcl_vector<dbdif_camera> cam;

  vpgl_fundamental_matrix <double> f12;

  // ================= Marginal variables: ===================
  //: stores [gama1 -gama2] after a call to reconstruct_point_lsqr
  mutable vnl_matrix_fixed<double,3,2> A_;
  //: stores 2 depths after a call to reconstruct_point_lsqr
  mutable vnl_vector<double> lambda_;
  
  // ------------ Bellow: cached data after a run to reconstruct_curvature
  mutable dbdif_vector_3d U1_;
  mutable dbdif_vector_3d U2_;
  mutable dbdif_vector_3d Gama1_;
  mutable dbdif_vector_3d Gama2_;
  mutable double g1_, g1g1_;
  mutable double g2_, g2g2_;
  mutable double n_dot_U1_;
  mutable double n_dot_U2_;
  mutable vnl_matrix_fixed<double,3,3> Coefs_; //:< coefficient matrix for curvature reconstruction
  // ================= END Marginal variables. =================== *
  
  //: use this whenever cam1 or cam2 change
  void compute_extra_info();
};


bool dbdif_rig::
reconstruct_tangent(
      const dbdif_vector_3d &gama1,
      const dbdif_vector_3d &gama2,
      const dbdif_vector_3d &t1,
      const dbdif_vector_3d &t2,
      dbdif_vector_3d *T_rec) const
{
  bool valid=true;
   // assert dot_product(t,F) near zero

  assert (dbdif_util::near_zero(dot_product(t1,cam[0].F)));
  assert (dbdif_util::near_zero(dot_product(t2,cam[1].F)));
 
  dbdif_vector_3d T= vnl_cross_3d( vnl_cross_3d(t1,gama1), vnl_cross_3d(t2,gama2) );
 
  if (dbdif_util::near_zero(T.two_norm()))
    valid = false;
 
  // reproject to check sign:
  if ( dot_product(T - dot_product(T,cam[0].F)*gama1,t1) > 0 && 
       dot_product(T - dot_product(T,cam[1].F)*gama2,t2) > 0
      )
    *T_rec  = T;
  else
    *T_rec = -T;
 
  return valid;
}

#endif // dbdif_rig_h

