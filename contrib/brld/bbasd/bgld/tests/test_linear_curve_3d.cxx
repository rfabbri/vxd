#include <testlib/testlib_test.h>
#include <dbgl/dbgl_linear_curve_3d.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>

MAIN( test_linear_curve_3d )
{
  START ("Linear Curve 3D");

  double tolerance = 10e-6;

  vgl_vector_3d<double> start(1, 1, 1);
  vgl_vector_3d<double> end(2, 3, 4);

  dbgl_linear_curve_3d curve(start, end);

  TEST_NEAR("curve length: ", curve.length(), 3.74165738677394, tolerance);
  /*******/
  TEST_NEAR("curve point at length test 1x: ", curve.point_at_length(0).x(), 1, tolerance);
  TEST_NEAR("curve point at length test 1y: ", curve.point_at_length(0).y(), 1, tolerance);
  TEST_NEAR("curve point at length test 1z: ", curve.point_at_length(0).z(), 1, tolerance);

  TEST_NEAR("curve point at length test 2x: ", curve.point_at_length(curve.length()/3).x(), 1 + 1/3.0, tolerance);
  TEST_NEAR("curve point at length test 2y: ", curve.point_at_length(curve.length()/3).y(), 1 + 2/3.0, tolerance);
  TEST_NEAR("curve point at length test 2z: ", curve.point_at_length(curve.length()/3).z(), 2, tolerance);
  
  TEST_NEAR("curve point at length test 3x: ", curve.point_at_length(curve.length()).x(), 2, tolerance);
  TEST_NEAR("curve point at length test 3y: ", curve.point_at_length(curve.length()).y(), 3, tolerance);
  TEST_NEAR("curve point at length test 3z: ", curve.point_at_length(curve.length()).z(), 4, tolerance);
  /*******/
  TEST_NEAR("curve point at s test 1x: ", curve.point_at(0).x(), 1, tolerance);
  TEST_NEAR("curve point at s test 1y: ", curve.point_at(0).y(), 1, tolerance);
  TEST_NEAR("curve point at s test 1z: ", curve.point_at(0).z(), 1, tolerance);

  TEST_NEAR("curve point at s test 2x: ", curve.point_at(1/3.0).x(), 1 + 1/3.0, tolerance);
  TEST_NEAR("curve point at s test 2y: ", curve.point_at(1/3.0).y(), 1 + 2/3.0, tolerance);
  TEST_NEAR("curve point at s test 2z: ", curve.point_at(1/3.0).z(), 2, tolerance);
  
  TEST_NEAR("curve point at s test 3x: ", curve.point_at(1).x(), 2, tolerance);
  TEST_NEAR("curve point at s test 3y: ", curve.point_at(1).y(), 3, tolerance);
  TEST_NEAR("curve point at s test 3z: ", curve.point_at(1).z(), 4, tolerance);
  /*******/
  TEST_NEAR("tangent angles at length phi test 1: ", curve.tangent_angles_at_length(0).phi_, 0.64052231267942, tolerance);
  TEST_NEAR("tangent angles at length phi test 2: ", curve.tangent_angles_at_length(curve.length()/3).phi_, 0.64052231267942, tolerance);
  TEST_NEAR("tangent angles at length phi test 3: ", curve.tangent_angles_at_length(curve.length()).phi_, 0.64052231267942, tolerance);

  TEST_NEAR("tangent angles at length theta test 1: ", curve.tangent_angles_at_length(0).theta_, 1.10714871779409, tolerance);
  TEST_NEAR("tangent angles at length theta test 2: ", curve.tangent_angles_at_length(curve.length()/3).theta_, 1.10714871779409, tolerance);
  TEST_NEAR("tangent angles at length theta test 3: ", curve.tangent_angles_at_length(curve.length()).theta_, 1.10714871779409, tolerance);
  /*******/
  TEST_NEAR("tangent angles at s phi test 1: ", curve.tangent_angles_at_length(0).phi_, 0.64052231267942, tolerance);
  TEST_NEAR("tangent angles at s phi test 2: ", curve.tangent_angles_at_length(1/3.0).phi_, 0.64052231267942, tolerance);
  TEST_NEAR("tangent angles at s phi test 3: ", curve.tangent_angles_at_length(1).phi_, 0.64052231267942, tolerance);

  TEST_NEAR("tangent angles at s theta test 1: ", curve.tangent_angles_at_length(0).theta_, 1.10714871779409, tolerance);
  TEST_NEAR("tangent angles at s theta test 2: ", curve.tangent_angles_at_length(1/3.0).theta_, 1.10714871779409, tolerance);
  TEST_NEAR("tangent angles at s theta test 3: ", curve.tangent_angles_at_length(1).theta_, 1.10714871779409, tolerance);
  /*******/
  TEST_NEAR("tangent at length test 1x: ", curve.tangent_at_length(0).x(), 0.26726124191242, tolerance);
  TEST_NEAR("tangent at length test 1y: ", curve.tangent_at_length(0).y(), 0.53452248382485, tolerance);
  TEST_NEAR("tangent at length test 1z: ", curve.tangent_at_length(0).z(), 0.80178372573727, tolerance);

  TEST_NEAR("tangent at s test 1x: ", curve.tangent_at(0).x(), 0.26726124191242, tolerance);
  TEST_NEAR("tangent at s test 1y: ", curve.tangent_at(0).y(), 0.53452248382485, tolerance);
  TEST_NEAR("tangent at s test 1z: ", curve.tangent_at(0).z(), 0.80178372573727, tolerance);

  SUMMARY();
}
