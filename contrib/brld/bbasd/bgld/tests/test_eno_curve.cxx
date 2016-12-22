#include <testlib/testlib_test.h>
#include <dbgl/dbgl_eno_curve.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>

MAIN( test_eno_curve )
{
  START ("ENO Curve");

  double tolerance = 10e-3;

  vcl_vector<double> coefs_x;
  coefs_x.push_back(3.0);
  coefs_x.push_back(2.0);
  coefs_x.push_back(1.0);
  vcl_vector<double> coefs_y;
  coefs_y.push_back(-1.0);
  coefs_y.push_back(-1.0);
  coefs_y.push_back(2.0);

  dbgl_eno_curve curve(coefs_x, coefs_y, 0, 1);
  
  TEST_NEAR("length test: ", curve.length(), 3.307, tolerance);
  
  TEST_NEAR("length test: ", curve.length_at(0), 0.0, tolerance);
  TEST_NEAR("length test: ", curve.length_at(0.5), 1.28325905230702, tolerance);
  TEST_NEAR("length test: ", curve.length_at(0.25), 0.5815055878 , tolerance);
  TEST_NEAR("length test: ", curve.length_at(0.75), 2.179566269, tolerance);

  TEST_NEAR("point test: ", curve.point_at(0).x(), 3, tolerance);
  TEST_NEAR("point test: ", curve.point_at(0).y(), -1, tolerance);

  TEST_NEAR("point test: ", curve.point_at(1).x(), 6, tolerance);
  TEST_NEAR("point test: ", curve.point_at(1).y(), 0, tolerance);

  TEST_NEAR("point at length test: ", curve.point_at_length(0).x(), 3, tolerance);
  TEST_NEAR("point at length test: ", curve.point_at_length(0).y(), -1, tolerance);

  TEST_NEAR("point at length test: ", curve.point_at_length(curve.length()).x(), 6, tolerance);
  TEST_NEAR("point at length test: ", curve.point_at_length(curve.length()).y(), 0, tolerance);

  TEST_NEAR("point at length test: ", curve.point_at_length(1.283259).x(), 4.25, tolerance);
  TEST_NEAR("point at length test: ", curve.point_at_length(1.283259).y(), -1, tolerance);

  TEST_NEAR("curvature test: ", curve.curvature_at(0), 0.8944, tolerance);
  TEST_NEAR("curvature test: ", curve.curvature_at(0.5), 0.3162, tolerance);
  TEST_NEAR("curvature test: ", curve.curvature_at(1), 0.08, tolerance);

  TEST_NEAR("curvature at length test: ", curve.curvature_at_length(0), 0.8944, tolerance);
  TEST_NEAR("curvature at length test: ", curve.curvature_at_length(1.283259), 0.3162, tolerance);
  TEST_NEAR("curvature at length test: ", curve.curvature_at_length(curve.length()), 0.08, tolerance);

  TEST_NEAR("tangent angle test: ", curve.tangent_angle_at(0), 5.8195, tolerance);
  TEST_NEAR("tangent angle test: ", curve.tangent_angle_at(0.5), 0.3218, tolerance);
  TEST_NEAR("tangent angle test: ", curve.tangent_angle_at(1), 0.6435, tolerance);

  TEST_NEAR("tangent angle at length test: ", curve.tangent_angle_at_length(0), 5.8195, tolerance);
  TEST_NEAR("tangent angle at length test: ", curve.tangent_angle_at_length(1.283259), 0.3218, tolerance);
  TEST_NEAR("tangent angle at length test: ", curve.tangent_angle_at_length(curve.length()), 0.6435, tolerance);

  vcl_cout << "*******************" << vcl_endl;

  coefs_x.clear();
  coefs_x.push_back(3.0);
  coefs_x.push_back(-1.0);
  coefs_x.push_back(2.0);
  coefs_y.clear();
  coefs_y.push_back(1.0);
  coefs_y.push_back(3.0);
  coefs_y.push_back(-1.0);

  dbgl_eno_curve curve2(coefs_x, coefs_y, 3, 4);

  TEST("start point test: ", curve2.point_at(0).x(), 18);
  TEST("start point test: ", curve2.point_at(0).y(), 1);

  TEST("end point test: ", curve2.point_at(1).x(), 31);
  TEST("end point test: ", curve2.point_at(1).y(), -3);

  TEST_NEAR("length test: ", curve2.length(), 13.60315284, tolerance);

  TEST_NEAR("length test: ", curve2.length_at(0), 0.0, tolerance);
  TEST_NEAR("length test: ", curve2.length_at(0.25), 2.987642796 , tolerance);
  TEST_NEAR("length test: ", curve2.length_at(0.5), 6.250267902, tolerance);

  TEST_NEAR("point at length test: ", curve2.point_at_length(0).x(), 18, tolerance);
  TEST_NEAR("point at length test: ", curve2.point_at_length(0).y(), 1, tolerance);

  TEST_NEAR("point at length test: ", curve2.point_at_length(curve2.length()).x(), 31, tolerance);
  TEST_NEAR("point at length test: ", curve2.point_at_length(curve2.length()).y(), -3, tolerance);

  TEST_NEAR("point at length test: ", curve2.point_at_length(2.987642796).x(), 20.8750, tolerance);
  TEST_NEAR("point at length test: ", curve2.point_at_length(2.987642796).y(), 0.1875, tolerance);

  TEST_NEAR("curvature test: ", curve2.curvature_at(0), -0.006746600149, tolerance);
  TEST_NEAR("curvature test: ", curve2.curvature_at(0.5), -0.003974133092, tolerance);
  TEST_NEAR("curvature test: ", curve2.curvature_at(1), -0.00252982128, tolerance);

  TEST_NEAR("curvature at length test: ", curve2.curvature_at_length(0), -0.006746600149, tolerance);
  TEST_NEAR("curvature at length test: ", curve2.curvature_at_length(6.250267902), -0.003974133092, tolerance);
  TEST_NEAR("curvature at length test: ", curve2.curvature_at_length(curve2.length()), -0.00252982128, tolerance);

  TEST_NEAR("tangent angle test: ", curve2.tangent_angle_at(0), 6.0169, tolerance);
  TEST_NEAR("tangent angle test: ", curve2.tangent_angle_at(0.5), 5.9847, tolerance);
  TEST_NEAR("tangent angle test: ", curve2.tangent_angle_at(1), 5.9614, tolerance);

  TEST_NEAR("tangent angle at length test: ", curve2.tangent_angle_at_length(0), 6.0169, tolerance);
  TEST_NEAR("tangent angle at length test: ", curve2.tangent_angle_at_length(6.250267902), 5.9847, tolerance);
  TEST_NEAR("tangent angle at length test: ", curve2.tangent_angle_at_length(curve2.length()), 5.9614, tolerance);
  
  SUMMARY();
}
