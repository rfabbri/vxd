// This is dbdif_analytic.h
#ifndef dbdif_analytic_h
#define dbdif_analytic_h
//:
//\file Analytic objects useful for testing algorithms
//
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 02/24/2006 11:31:09 AM EST
//

#include <dbdif/dbdif_util.h>
#include <dbdif/dbdif_frenet.h>

class dbdif_analytic {
  public:
  static void sphere_occluding_contour(
      double rho,
      const dbdif_vector_3d &s0,
      const dbdif_vector_3d &c,
      vcl_vector<dbdif_vector_3d> &Gamma,
      dbdif_vector_3d &Gamma_center,
      double &Gamma_radius
      );

  static void sphere_occluding_contour(
      double rho,
      const dbdif_vector_3d &s0,
      const dbdif_vector_3d &c,
      vcl_vector<dbdif_3rd_order_point_3d> &crv3d,
      dbdif_vector_3d &Gamma_center,
      double &Gamma_radius
      );

  static void circle_curve( double radius, vcl_vector<dbdif_3rd_order_point_3d> &C, vcl_vector<double> &t,
      double t_initial, double step, double range);

  static void circle_curve( 
      double radius, 
      const dbdif_vector_3d &translation,
      vcl_vector<dbdif_3rd_order_point_3d> &C, vcl_vector<double> &t,
      double t_initial, double step, double range);

  static void circle_curve(
      double radius,
      const dbdif_vector_2d &translation,
      vcl_vector<dbdif_3rd_order_point_2d> &C, vcl_vector<double> &t,
      double t_initial, double step, double range
      );

  static void 
  ellipse(
      double a,
      double b,
      const dbdif_vector_2d &translation,
      vcl_vector<dbdif_3rd_order_point_2d> &C, 
      vcl_vector<double> &t,
      double t_initial, double step, double range
      );

  static void 
  ellipse(
      double a,
      double b,
      const dbdif_vector_3d &translation,
      vcl_vector<dbdif_3rd_order_point_3d> &C, 
      vcl_vector<double> &t,
      double t_initial, double step, double range
      );

  static void helix_curve( 
      double radius, double alpha, 
      dbdif_vector_3d &translation,
      vcl_vector<dbdif_3rd_order_point_3d> &C, 
      vcl_vector<double> &t, double t_initial, double step, double range );

  static void
  space_curve1(
      double radius, 
      dbdif_vector_3d &translation,
      vcl_vector<dbdif_3rd_order_point_3d> &C, 
      vcl_vector<double> &t,
      double t_initial, double step, double range
      );

  static void 
  line(
      dbdif_vector_3d &translation,
      dbdif_vector_3d &direction,
      vcl_vector<dbdif_3rd_order_point_3d> &C, 
      vcl_vector<double> &t,
      double t_final, double step
      );

  static bool
  limit_distance(
      const vcl_vector<dbdif_3rd_order_point_2d> &C, 
      vcl_vector<dbdif_3rd_order_point_2d> &C_limited);

  static double
  perturb( double coord, double dp);

  static void 
  perturb( vgl_vector_2d<double> &t, double dt);

  static void 
  rotate( vcl_vector<dbdif_3rd_order_point_3d> &c, const dbdif_vector_3d &axis);

  static void 
  translate( vcl_vector<dbdif_3rd_order_point_3d> &c, const dbdif_vector_3d &transl);
};


#endif // dbdif_analytic_h
