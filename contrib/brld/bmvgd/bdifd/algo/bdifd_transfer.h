// This is dbdif_transfer.h
#ifndef dbdif_transfer_h
#define dbdif_transfer_h
//:
//\file
//\brief Methods for image-based reprojection/transfer
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 05/15/2008 07:22:40 AM EDT
//

#include <dbdif/dbdif_rig.h>

// Methods supporting image-based reprojection/transfer of differential-geometric invariants
class dbdif_transfer {
public:
  
  //: 
  // \param[in] p1: point in image 1 in image coordinates // \param[in] p2: point in image 2 in image coordinates
  // \param[out] pt_img3: reprojected point in image coords
  // \param[out] Prec: reconstructed point from triangulating p1 and p2
  static bool 
    transfer_by_reconstruct_and_reproject ( 
    const dbdif_3rd_order_point_2d &p1, 
    const dbdif_3rd_order_point_2d &p2, 
    dbdif_3rd_order_point_2d &p3_reproj, 
    dbdif_3rd_order_point_3d &Prec,
    const dbdif_camera &cam3,
    const dbdif_rig &rig )
  {
    dbdif_3rd_order_point_2d p1_w, p2_w;

    // get hold of Prec

    rig.cam[0].img_to_world(&p1,&p1_w);
    rig.cam[1].img_to_world(&p2,&p2_w);

    rig.reconstruct_3rd_order(p1_w, p2_w, &Prec);

    // theta1 = theta(view3) 
    bool valid;
    p3_reproj = cam3.project_to_image(Prec,&valid);
    return valid && Prec.valid;
  }

  static bool 
  transfer_by_reconstruct_and_reproject ( 
    const vsol_point_2d_sptr &pt_img1, 
    const vsol_point_2d_sptr &pt_img2, 
    vgl_point_2d<double> &pt_img3, 
    const dbdif_camera &cam3, 
    const dbdif_rig &rig )
  {
    dbdif_vector_3d P; 

    rig.reconstruct_point_lsqr(pt_img1,pt_img2,&P);

    dbdif_vector_2d p_aux;
    p_aux = cam3.project_to_image(P);
    pt_img3.set(p_aux[0],p_aux[1]);
    return true;
  }


  static bool 
  transfer_tangent_band(
      const dbdif_1st_order_point_2d &p0, 
      const dbdif_1st_order_point_2d &p1, 
      double t_err,
      double *theta_min_reproj,
      double *theta_max_reproj,
      const dbdif_camera &cam, 
      const dbdif_rig &rig);
  
  //: only for sectors within [0, vnl_math::pi]
  static bool 
  has_intersect_pi(
      double theta_min_a, double theta_max_a,
      double theta_min_b, double theta_max_b)
  {
    double dc = theta_max_a + theta_min_a - theta_max_b - theta_min_b;

    double ra_plus_rb = theta_max_b - theta_min_b + theta_max_a - theta_min_a;

    return dc < ra_plus_rb && -dc < ra_plus_rb;
  }

  static bool 
  has_tangent_band_intersection(
      double tx_a, double ty_a, double t_err_a, 
      double theta_min_b, double theta_max_b)
  {

    double theta_mid = atan(ty_a/tx_a);

    double theta_min_a = theta_mid - t_err_a;
    double theta_max_a = theta_mid + t_err_a;

    // put within conventioned range
    if (theta_min_a < 0) {
      theta_min_a += vnl_math::pi;
      theta_max_a += vnl_math::pi;
    }

    if (theta_max_a > vnl_math::pi) {
      if (theta_max_b <= vnl_math::pi)
        return has_intersect_pi( 0, theta_max_a-vnl_math::pi, theta_min_b, theta_max_b) 
            || has_intersect_pi( theta_min_a, vnl_math::pi, theta_min_b, theta_max_b);
      else
        return true;
    } else {
      if (theta_max_b <= vnl_math::pi)
        return has_intersect_pi( theta_min_a, theta_max_a, theta_min_b, theta_max_b); 
      else
        return has_intersect_pi( theta_min_a, theta_max_a, 0, theta_max_b-vnl_math::pi) 
            || has_intersect_pi( theta_min_a, theta_max_a, theta_min_b,vnl_math::pi);
    }
  }


};

#endif // dbdif_transfer_h

