#include <testlib/testlib_test.h>

#include <dbsol/algo/dbsol_curve_3d_algs.h>
#include <dbsol/dbsol_interp_curve_3d.h>
#include <vsol/vsol_point_3d.h>
#include <vgl/vgl_vector_3d.h>
#include <vnl/vnl_math.h>

#include <vcl_iostream.h> 
#include <vcl_cmath.h>
#include <vcl_limits.h>
#include <vcl_cstring.h>
#include <vcl_iostream.h>
#include <vcl_cstdio.h>

MAIN( test_interp_linear_3d )
{
   START ("3D Linear Interpolation");

   double tolerance = 10e-6;

   vcl_vector<vsol_point_3d_sptr> pts;

   pts.push_back(new vsol_point_3d(0, -1, 0));
   pts.push_back(new vsol_point_3d(0, 0, 0));
   pts.push_back(new vsol_point_3d(1, 0, 0));
   pts.push_back(new vsol_point_3d(2, 0, 1));
   pts.push_back(new vsol_point_3d(3, 1, 2));

   dbsol_interp_curve_3d c;
   dbsol_curve_3d_algs::interpolate_linear_3d(&c, pts);
   vcl_cout << c;

   TEST_NEAR("point sample test 1x: ", (*(c.point_at_sample(3))).x(), 2, tolerance);
   TEST_NEAR("point sample test 1y: ", (*(c.point_at_sample(3))).y(), 0, tolerance);
   TEST_NEAR("point sample test 1z: ", (*(c.point_at_sample(3))).z(), 1, tolerance);

   TEST_NEAR("length test: ", c.length_at(0), 0.0, tolerance);
   TEST_NEAR("length test: ", c.length_at(1), 1.0, tolerance);
   TEST_NEAR("length test: ", c.length_at(2), 2.0, tolerance);
   TEST_NEAR("length test: ", c.length_at(3), 3.41421356237310, tolerance);
   TEST_NEAR("length test: ", c.length_at(4), 5.14626436994197, tolerance);
   TEST_NEAR("length test: ", c.length(), 5.14626436994197, tolerance);

   SUMMARY();
}
