// This is dbdif_frenet.h
#ifndef dbdif_frenet_h
#define dbdif_frenet_h
//:
//\file
//\brief Simple Frenet frame datastructures
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 05/22/2005 02:21:06 PM EDT
//
#include <dbdif/dbdif_util.h>
#include <vnl/vnl_cross.h>
#include <vnl/vnl_matrix_fixed.h>

//-- 3D --------------------------------------------------------------------

//: represents the frenet frame at a given point.
struct dbdif_1st_order_point_3d {
  dbdif_vector_3d Gama, T;
  bool valid; //:< false if differential geometry is invalid due to degeneracies
  dbdif_1st_order_point_3d() : valid(true){}
  bool operator==(const dbdif_1st_order_point_3d &o) const 
  { return Gama == o.Gama && T == o.T && valid == o.valid; }
};

struct dbdif_2nd_order_point_3d : public dbdif_1st_order_point_3d {
  dbdif_vector_3d N,B;  //:< These are only valid when K != 0. User must test this! 
//  bool valid_normal;
//  dbdif_2nd_order_point_3d() : valid_normal(true){}
  //: sets normal and binormal
  void set_normal(const dbdif_vector_3d &Normal)
  { N = Normal; N.normalize(); B = vnl_cross_3d(T,N); }
  double K;
};

struct dbdif_3rd_order_point_3d : public dbdif_2nd_order_point_3d {
  double Kdot,Tau;
  //: absolute value of third derivative with respect to arc length
  double Gamma_3dot_abs() const { double k2 = K*K;  return k2*k2 + Kdot*Kdot + k2*Tau*Tau; }
};

typedef vcl_vector<dbdif_1st_order_point_3d> dbdif_1st_order_curve_3d;
typedef vcl_vector<dbdif_2nd_order_point_3d> dbdif_2nd_order_curve_3d;
typedef vcl_vector<dbdif_3rd_order_point_3d> dbdif_3rd_order_curve_3d;

//-- 2D --------------------------------------------------------------------

struct dbdif_1st_order_point_2d {
  dbdif_vector_3d gama, t, n;
//  dbdif_1st_order_extras cache1;
  bool valid; //:< false if differential geometry is invalid due to degeneracies
  dbdif_1st_order_point_2d() : valid(true){}
};

struct dbdif_2nd_order_point_2d : public dbdif_1st_order_point_2d {
  dbdif_2nd_order_point_2d() {};
  dbdif_2nd_order_point_2d(const dbdif_1st_order_point_2d &p)
    :
    dbdif_1st_order_point_2d(p),
    k(0)
  {}
  double k;
//  dbdif_2nd_order_extras cache2;
};

struct dbdif_3rd_order_point_2d : public dbdif_2nd_order_point_2d {
  dbdif_3rd_order_point_2d() {};
  dbdif_3rd_order_point_2d(const dbdif_1st_order_point_2d &p)
    :
    dbdif_2nd_order_point_2d(p),
    kdot(0)
  {}
  double kdot;
//  dbdif_3rd_order_extras cache3;
};

typedef vcl_vector<dbdif_1st_order_point_2d> dbdif_1st_order_curve_2d;
typedef vcl_vector<dbdif_2nd_order_point_2d> dbdif_2nd_order_curve_2d;
typedef vcl_vector<dbdif_3rd_order_point_2d> dbdif_3rd_order_curve_2d;

//: Various utilities for manipulating frenet points.
class dbdif_frenet {
public:

  inline static bool
  linear_transform(
      const dbdif_vector_2d &normal_p,
      const dbdif_vector_2d &tangent_p,
      double k_p,
      double kdot_p,
      double *k_q,
      double *kdot_q,
      dbdif_vector_2d *t_q_ptr,
      dbdif_vector_2d *n_q_ptr,
      vnl_matrix_fixed<double,2,2> &L
        );

//  bool
//  has_2d_convention(dbdif_3rd_order_point_3d &p);
};

//: Given a point p in a curve c1, and given that points q of c2 are a general linear
// transformation of c1's points (q = Lp), return the 3rd order frenet frame at
// Lp, given the geometry at p. Returns the signed curvature.
inline bool dbdif_frenet::
linear_transform(
    const dbdif_vector_2d &n_p,
    const dbdif_vector_2d &t_p,
    double k_p,
    double kdot_p,
    double *k_q,
    double *kdot_q,
    dbdif_vector_2d *t_q_ptr,
    dbdif_vector_2d *n_q_ptr,
    vnl_matrix_fixed<double,2,2> &L
      )
{
  bool retval = true;
  dbdif_vector_2d &t_q = *t_q_ptr;
  dbdif_vector_2d &n_q = *n_q_ptr;

  t_q = L*t_p;
  double g  = t_q.two_norm();
  t_q.normalize();

  n_q[0] = -t_q[1];
  n_q[1] =  t_q[0];

  double g2 = g*g;

  dbdif_vector_2d L_kp_np = L*(k_p*n_p); //:< \dddot gama_img
  *k_q = dot_product(L_kp_np, n_q) / (g2);


  double gprime = dot_product(L_kp_np, t_q);

//  if (dbdif_util::near_zero(g*g*g)) {
  /*
    vcl_cout << "near zero!\n";
    if (dbdif_util::near_zero(t_p.two_norm()))
      vcl_cout << "AND TP TOO\n";
  */
//    retval = false;
//  }

  dbdif_vector_2d gama_p_3dot = -(k_p*k_p)*t_p + kdot_p*n_p;

  *kdot_q = (1.0/g2) * ( dot_product(L*gama_p_3dot, n_q) / g  - 3*gprime* (*k_q));


//  *kdot_q = (dot_product((L*(-k_p*k_p*t_p + kdot_p*n_p)), n_q) - 3*g*gprime* (*k_q) ) / (g*g*g);
  return retval;
}

//struct dbdif_1st_order_extras {
//  double lambda, //:< \Gama \cdot F
//         speed;  //:< speed ratio
//
//  dbdif_1st_order_extras() : iscached_(false);
//  bool iscached_;
//};

//struct dbdif_2nd_order_extras {
//  double lambda_dot, //:< \T \cdot F
//         accel; //:< acceleration in image when unit speed in 3D
//};


#endif // dbdif_frenet_h
