// This is bdifd_analytic.h
#ifndef bdifd_analytic_h
#define bdifd_analytic_h
//:
//\file Analytic objects useful for testing algorithms
//
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 02/24/2006 11:31:09 AM EST
//

#include <bdifd/bdifd_util.h>
#include <bdifd/bdifd_frenet.h>

class bdifd_analytic {
  public:
  static void sphere_occluding_contour(
      double rho,
      const bdifd_vector_3d &s0,
      const bdifd_vector_3d &c,
      vcl_vector<bdifd_vector_3d> &Gamma,
      bdifd_vector_3d &Gamma_center,
      double &Gamma_radius
      );

  static void sphere_occluding_contour(
      double rho,
      const bdifd_vector_3d &s0,
      const bdifd_vector_3d &c,
      vcl_vector<bdifd_3rd_order_point_3d> &crv3d,
      bdifd_vector_3d &Gamma_center,
      double &Gamma_radius
      );

  static void circle_curve( double radius, vcl_vector<bdifd_3rd_order_point_3d> &C, vcl_vector<double> &t,
      double t_initial, double step, double range);

  static void circle_curve( 
      double radius, 
      const bdifd_vector_3d &translation,
      vcl_vector<bdifd_3rd_order_point_3d> &C, vcl_vector<double> &t,
      double t_initial, double step, double range);

  static void circle_curve(
      double radius,
      const bdifd_vector_2d &translation,
      vcl_vector<bdifd_3rd_order_point_2d> &C, vcl_vector<double> &t,
      double t_initial, double step, double range
      );

  static void 
  ellipse(
      double a,
      double b,
      const bdifd_vector_2d &translation,
      vcl_vector<bdifd_3rd_order_point_2d> &C, 
      vcl_vector<double> &t,
      double t_initial, double step, double range
      );

  static void 
  ellipse(
      double a,
      double b,
      const bdifd_vector_3d &translation,
      vcl_vector<bdifd_3rd_order_point_3d> &C, 
      vcl_vector<double> &t,
      double t_initial, double step, double range
      );

  static void helix_curve( 
      double radius, double alpha, 
      bdifd_vector_3d &translation,
      vcl_vector<bdifd_3rd_order_point_3d> &C, 
      vcl_vector<double> &t, double t_initial, double step, double range );

  static void
  space_curve1(
      double radius, 
      bdifd_vector_3d &translation,
      vcl_vector<bdifd_3rd_order_point_3d> &C, 
      vcl_vector<double> &t,
      double t_initial, double step, double range
      );

  static void 
  line(
      bdifd_vector_3d &translation,
      bdifd_vector_3d &direction,
      vcl_vector<bdifd_3rd_order_point_3d> &C, 
      vcl_vector<double> &t,
      double t_final, double step
      );

  static bool
  limit_distance(
      const vcl_vector<bdifd_3rd_order_point_2d> &C, 
      vcl_vector<bdifd_3rd_order_point_2d> &C_limited);

  static double
  perturb( double coord, double dp);

  static void 
  perturb( vgl_vector_2d<double> &t, double dt);

  static void 
  rotate( vcl_vector<bdifd_3rd_order_point_3d> &c, const bdifd_vector_3d &axis);

  static void 
  translate( vcl_vector<bdifd_3rd_order_point_3d> &c, const bdifd_vector_3d &transl);
};


#endif // bdifd_analytic_h
