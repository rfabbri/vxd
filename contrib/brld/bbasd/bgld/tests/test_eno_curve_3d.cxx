#include <testlib/testlib_test.h>
#include <dbgl/dbgl_eno_curve_3d.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>

MAIN( test_eno_curve_3d )
{
  START ("ENO Curve 3D");

  double tolerance = 10e-3;

  // x(t) = 1 - 2t - 2t^2 + t^3
  // y(t) = -1 + t + t^2 - 2t^3
  // z(t) = 2 + 2t - t^2 + t^3

  vcl_vector<double> coefs_x;
  coefs_x.push_back(1);
  coefs_x.push_back(-2);
  coefs_x.push_back(-2);
  coefs_x.push_back(1);
  vcl_vector<double> coefs_y;
  coefs_y.push_back(-1);
  coefs_y.push_back(1);
  coefs_y.push_back(1);
  coefs_y.push_back(-2);
  vcl_vector<double> coefs_z;
  coefs_z.push_back(2);
  coefs_z.push_back(2);
  coefs_z.push_back(-1);
  coefs_z.push_back(1);

  dbgl_eno_curve_3d curve(coefs_x, coefs_y, coefs_z, 1.0, 2.0);

  TEST_NEAR("test start point: ", curve.start_t(), 1.0, tolerance);
  TEST_NEAR("test end point: ", curve.end_t(), 2.0, tolerance);

  TEST_NEAR("length test 1: ", curve.length(), 11.90565772684809, tolerance);
  
  TEST_NEAR("length at t test 2: ", curve.length_at_t(1.0), 0.0, tolerance);
  TEST_NEAR("length at t test 3: ", curve.length_at_t(1.3), 1.96404378658562, tolerance);

  TEST_NEAR("s to t conversion test 1: ", curve.s_to_t(1.96404378658562, 10e-6), 1.3, tolerance);
  TEST_NEAR("s to t conversion test 2: ", curve.s_to_t(0, 10e-6), curve.start_t(), tolerance);
  TEST_NEAR("s to t conversion test 3: ", curve.s_to_t(curve.length(), 10e-6), curve.end_t(), tolerance);

  TEST_NEAR("point at t test 1: ", curve.point_at_t(curve.start_t()).x(), -2, tolerance);
  TEST_NEAR("point at t test 2: ", curve.point_at_t(curve.start_t()).y(), -1, tolerance);
  TEST_NEAR("point at t test 3: ", curve.point_at_t(curve.start_t()).z(), 4, tolerance);

  TEST_NEAR("point at t test 4: ", curve.point_at_t(curve.end_t()).x(), -3, tolerance);
  TEST_NEAR("point at t test 5: ", curve.point_at_t(curve.end_t()).y(), -11, tolerance);
  TEST_NEAR("point at t test 6: ", curve.point_at_t(curve.end_t()).z(), 10, tolerance);

  TEST_NEAR("point at t test 7: ", curve.point_at_t(1.3).x(), -2.783, tolerance);
  TEST_NEAR("point at t test 8: ", curve.point_at_t(1.3).y(), -2.404, tolerance);
  TEST_NEAR("point at t test 9: ", curve.point_at_t(1.3).z(), 5.107, tolerance);

  TEST_NEAR("point at length test 1: ", curve.point_at_length(1.96404378658562).x(), -2.783, tolerance);
  TEST_NEAR("point at length test 2: ", curve.point_at_length(1.96404378658562).y(), -2.404, tolerance);
  TEST_NEAR("point at length test 3: ", curve.point_at_length(1.96404378658562).z(), 5.107, tolerance);

  TEST_NEAR("curvature at t test 1: ", curve.curvature_at_t(curve.start_t()), 0.31426, tolerance);

  TEST_NEAR("curvature at length test 1: ", curve.curvature_at_length(0.0), 0.31426, tolerance);

  TEST_NEAR("torsion at t test 1: ", curve.torsion_at_t(curve.start_t()), 0.05555556, tolerance);

  TEST_NEAR("torsion at length test 1: ", curve.torsion_at_length(0.0), 0.05555556, tolerance);

  TEST_NEAR("tangent angles at t test 1: ", curve.tangent_angles_at_t(curve.start_t()).phi_, 0.95531661812451, tolerance);
  TEST_NEAR("tangent angles at t test 2: ", curve.tangent_angles_at_t(curve.start_t()).theta_, 5*(vnl_math::pi)/4, tolerance);

  TEST_NEAR("tangent angles at t test 3: ", curve.tangent_angles_at_t(curve.end_t()).phi_, 1.08858566004430, tolerance);
  TEST_NEAR("tangent angles at t test 4: ", curve.tangent_angles_at_t(curve.end_t()).theta_, 4.81726591911492, tolerance);

  TEST_NEAR("tangent angles at length test 1: ", curve.tangent_angles_at_length(0.0).phi_, 0.95531661812451, tolerance);
  TEST_NEAR("tangent angles at length test 2: ", curve.tangent_angles_at_length(0.0).theta_, 5*(vnl_math::pi)/4, tolerance);

  TEST_NEAR("tangent angles at length test 3: ", curve.tangent_angles_at_length(curve.length()).phi_, 1.08858566004430, tolerance);
  TEST_NEAR("tangent angles at length test 4: ", curve.tangent_angles_at_length(curve.length()).theta_, 4.81726591911492, tolerance);

  TEST_NEAR("tangent at t test 1: ", curve.tangent_at_t(curve.start_t()).x(), -0.57735, tolerance);
  TEST_NEAR("tangent at t test 2: ", curve.tangent_at_t(curve.start_t()).y(), -0.57735, tolerance);
  TEST_NEAR("tangent at t test 3: ", curve.tangent_at_t(curve.start_t()).z(), 0.57735, tolerance);

  TEST_NEAR("tangent at length test 1: ", curve.tangent_at_length(0.0).x(), -0.57735, tolerance);
  TEST_NEAR("tangent at length test 2: ", curve.tangent_at_length(0.0).y(), -0.57735, tolerance);
  TEST_NEAR("tangent at length test 3: ", curve.tangent_at_length(0.0).z(), 0.57735, tolerance);
  
  SUMMARY();
}
