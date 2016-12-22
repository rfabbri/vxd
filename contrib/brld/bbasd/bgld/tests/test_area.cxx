#include <testlib/testlib_test.h>
#include <dbgl/dbgl_area.h>
#include <vcl_iostream.h>
#include <vgl/vgl_point_3d.h>

void test_area_triangle()
{
  vcl_cout << "Compute area of a triangle\n";
  vgl_point_3d<double > p1(1, 1, 1);
  vgl_point_3d<double > p2(3, 5, 1);
  vgl_point_3d<double > p3(2, 3, 6);
  double true_area = vcl_sqrt(2.0 * 2.0  + 4*4) * 5 / 2;
  TEST_NEAR("Triangle area - 3D", dbgl_area_triangle(p1, p2, p3), true_area, 1e-8);

  return;
}



//: Test functions of biarc class
MAIN( test_area )
{
  START ("Test computing areas of geometric objects");
  test_area_triangle();
  SUMMARY();
}

