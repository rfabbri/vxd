// This is dbdif_camera.h
#ifndef dbdif_camera_h
#define dbdif_camera_h
//:
//\file
//\brief Explicit camera as in my paper
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 05/23/2005 06:25:57 PM EDT
//

#include <vpgl/vpgl_perspective_camera.h>
#include <dbdif/dbdif_util.h>
#include <dbdif/dbdif_frenet.h>

//: Explicit camera elements, obtained from a projection matrix. This class has
// methods for projecting 1st, 2nd, and 3rd order differential measures, and
// most of the other geometrical tools relating to a single view. 
//
// \remarks For reconstruction using more than one camera, cf. dbdif_rig class.
//
//
class dbdif_camera {
public:
  dbdif_camera()
     : unit_x(1,0,0), unit_y(0,1,0), unit_z(0,0,1) {}

  void set_p(const vpgl_perspective_camera<double> &Prj)
  { Pr_ = Prj; compute_explicit_params(); }

  //: Take care: Gama is relative vector, ie, Curve - c
  dbdif_vector_3d project(const dbdif_vector_3d &Gama) const
  { return Gama/dot_product(Gama,F); }

  dbdif_vector_2d project_to_image(const dbdif_vector_3d &Gama) const;

  dbdif_3rd_order_point_2d project_to_image(const dbdif_3rd_order_point_3d &p3d, bool *stat) const;

  //: \todo consistent interfaces for each order.
  bool project_1st_order_to_image(
      const dbdif_1st_order_point_3d &p3d, 
      dbdif_1st_order_point_2d *pimg_ptr) const;

  //: converts from image u,v coordinates to world coordinates
  inline void get_gama(double u, double v, dbdif_vector_3d *gama) const;

  //: ratio of speeds of space and image curve; can be interpreted as the 
  // speed of image curve wrt. arclenght of space curve
  double speed(const dbdif_1st_order_point_3d &Frm) const
  { return speed(Frm,dot_product(Frm.Gama - c,F)); }

  //: ratio of speeds of space and image curve; can be interpreted as the 
  // speed of image curve wrt. arclenght of space curve.
  double speed(const dbdif_1st_order_point_3d &Frm, double lambda) const
  { return ( Frm.T - dot_product(Frm.T,F)*project(Frm.Gama - c) ).two_norm()/lambda; }

  //: Derivative of speed of image curve wrt. arclenght of space curve
  double tangential_accel(const dbdif_2nd_order_point_3d *Frm, 
      double lambda, double lambda_dot, double g, const dbdif_vector_3d &t) const;

  //: project 3D T, given gama. User must normalize result to get unit tangent.
  void project_t(const dbdif_vector_3d &T, const dbdif_vector_3d &gama, dbdif_vector_3d *t) const
  { *t = T - dot_product(T,F)*gama; }

  //: project 2nd order measures
  bool project_k(const dbdif_2nd_order_point_3d &Frame, dbdif_2nd_order_point_2d *frame1) const;

  //: project 2nd order measures
  bool project_k(const dbdif_2nd_order_point_3d &Frame, dbdif_2nd_order_point_2d *frame1, double *plambda) const;
  
  bool project_3rd(const dbdif_3rd_order_point_3d &Frame, dbdif_3rd_order_point_2d *frame1) const;

  //: for unit vectors
  inline void 
  img_to_world_vector(double img_x, double img_y, vnl_vector_fixed<double, 3> *t_world) const;

  //: for unit vectors
  inline void
  world_to_img_vector(const vnl_vector_fixed<double, 3> &v_world, double *img_x, double *img_y) const;

  //: world 3D to camera 3D
  inline void 
  world_to_cam_vector(const dbdif_vector_3d &v_world, dbdif_vector_3d *v_cam) const;

  inline void
  world_to_img(const dbdif_1st_order_point_2d *p_w, dbdif_1st_order_point_2d *p_img) const;

  inline void
  img_to_world(const dbdif_3rd_order_point_2d *p_img, dbdif_3rd_order_point_2d *p_w) const;

  inline void 
  img_to_world( vgl_point_2d<double> &pt, double theta_img, double k_img, 
      double kdot_img, dbdif_3rd_order_point_2d *p_w) const;

  // DATA ----------------------------------------------------------------------


//protected:
  vpgl_perspective_camera <double> Pr_;
  vnl_matrix_fixed<double,3,3> K_;

  //: inverse elements of K_ (scaling/focal length + skew)
  double k_inv_00_;
  double k_inv_01_;
  double k_inv_11_;

  vnl_matrix_fixed<double,3,3> Rot, Rot_t;

  dbdif_vector_3d
    c,
    b1_base_e,b2_base_e,b3_base_e,
    F;

  //: Entry (0,0) in K calib. matrix
  // It is focal_length/(length units per pixel in x direction)
  double k_a_;

  //: Entry (1,1) in K calib. matrix. 
  // It is focal_length/(length units per pixel in y direction)
  double k_b_;

  double focal_length;

  // Utility:
  dbdif_vector_3d unit_x, unit_y, unit_z;

  //: use this whenever Pr change
  void compute_explicit_params();

  void print(vcl_ostream &os=vcl_cout) const;
};

inline void dbdif_camera::
img_to_world_vector(double img_x, double img_y, vnl_vector_fixed<double, 3> *t_world) const
{
   dbdif_vector_3d t_cam;

   t_cam[0] = img_x/k_a_ + k_inv_01_*img_y;   //:< take off streching;
   t_cam[1] = img_y/k_b_;
   t_cam[2] = 0;
   *t_world= Rot_t*t_cam;
   t_world->normalize();
}

inline void dbdif_camera::
world_to_img_vector(const vnl_vector_fixed<double, 3> &v_world, double *img_x, double *img_y) const
{
   dbdif_vector_3d v_cam;

   v_cam = Rot * v_world;

   assert(dbdif_util::near_zero(v_cam[2],1e-5));

   *img_x = v_cam[0]*k_a_ + K_(0,1)*v_cam[1];
   *img_y = v_cam[1]*k_b_;
   double len = hypot(*img_x,*img_y);
   *img_x /= len;
   *img_y /= len;
}

inline void dbdif_camera::
world_to_cam_vector(const dbdif_vector_3d &v_world, dbdif_vector_3d *v_cam) const
{
   *v_cam = Rot * v_world;

   assert(dbdif_util::near_zero((*v_cam)[2],1e-5));
}


//: from image 2D internal coordinates to 3D camera coordinates
inline void dbdif_camera::
get_gama(double u, double v,dbdif_vector_3d *gama) const
{
  double yn = (v - K_(1,2))/k_b_;
  double xn = (u - K_(0,2) - K_(0,1)*yn)/k_a_;

   *gama = xn*b1_base_e + yn*b2_base_e  + b3_base_e;
}

//: Given measures in image units and coordinates, return the same measures in
// world units and coordinates.
//
//\param[in] theta : angle of the tangent vector (as returned by atan2). From 0
//to 2pi is what I use but -pi to pi should work too
//
inline void dbdif_camera::
img_to_world( 
    vgl_point_2d<double> &pt, double theta_img, double k_img, 
    double kdot_img, dbdif_3rd_order_point_2d *p_w) const
{
  dbdif_3rd_order_point_2d p_img;

  p_img.gama[0] = pt.x();
  p_img.gama[1] = pt.y();
  p_img.gama[2] = 0;
  p_img.t[0] = vcl_cos(theta_img);
  p_img.t[1] = vcl_sin(theta_img);
  p_img.t[2] = 0;

  if (!vnl_math::isfinite(k_img))
    p_img.valid = false;

  p_img.k    = k_img;
  p_img.kdot = kdot_img;

  img_to_world(&p_img, p_w);
}

inline void dbdif_camera::
img_to_world(const dbdif_3rd_order_point_2d *p_img, dbdif_3rd_order_point_2d *p_w) const
{
  get_gama(p_img->gama[0], p_img->gama[1], &(p_w->gama));

  dbdif_vector_2d t_img(p_img->t[0], p_img->t[1]);

  dbdif_vector_2d n_img(p_img->n[0], p_img->n[1]);


  vnl_matrix_fixed<double,2,2> Kinv;

  Kinv(0,0) = k_inv_00_;
  Kinv(0,1) = k_inv_01_;
  Kinv(1,1) = k_inv_11_;
  Kinv(1,0) = 0;

  // - formulas for getting k,kdot in world units, i.e. normalized image (unit focal
  // length plus removing effect of skew)

  dbdif_vector_2d tcam_2, ncam_2; //:< 

  bool stat = dbdif_frenet::linear_transform(n_img, t_img, p_img->k, p_img->kdot, &(p_w->k), &(p_w->kdot),
      &tcam_2, &ncam_2, Kinv);
//  if (!dbdif_util::near_zero((Rot_t*tcam_2 - p_w->t).two_norm(), 1e-8))
//    vcl_cout << "Linear transform invalid in img to world\n";

  dbdif_vector_3d tcam_3;
  tcam_3[0] = tcam_2[0];
  tcam_3[1] = tcam_2[1];
  tcam_3[2] = 0;

  dbdif_vector_3d ncam_3;
  ncam_3[0] = ncam_2[0];
  ncam_3[1] = ncam_2[1];
  ncam_3[2] = 0;

  p_w->t = Rot_t*tcam_3;
  p_w->n = Rot_t*ncam_3;
//  p_w->n[0] = -p_w->t[1];
//  p_w->n[1] = p_w->t[0];
//  p_w->n[2] =  0;
  //not the final value; its sign is to be determined bellow from the curvature

  // We now went to 3-space so positive curvature is the convention
  if (p_w->k < 0) {
    p_w->k = -p_w->k;
    p_w->kdot = -p_w->kdot;
    p_w->n = -p_w->n;
  }

  p_w->valid = p_w->valid && p_img->valid && stat;

#ifndef NDEBUG
  if (!dbdif_util::near_zero(dot_product(p_w->n, p_w->t),1e-6)) {
    if (dbdif_util::near_zero(dot_product(p_img->t, p_img->n),1e-6)) {
      vcl_cout << "PAU EM img_to_world_vector! " << dot_product(p_w->n, p_w->t) <<"\n";
    } else {
      vcl_cout << "Spurious input in img_to_world_vector\n";
    }
  }
#endif

}


//: Convert differential measures from 2D normalized image plane coordinates to 2D 
// final image coordinates
//inline void
//world_to_img(const dbdif_1st_order_point_2d *pw, dbdif_1st_order_point_2d *p_img)
//{
//  p_img->gama = pw;
//}




#endif // dbdif_camera_h

