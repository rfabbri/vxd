#include <testlib/testlib_test.h>
#include <dbnl/dbnl_angle.h>
#include <vnl/vnl_math.h>

// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date Oct 23, 2009


//: Test functions deadling with angles
MAIN( test_angle )
{
  START ("Test angle functions");
  
  TEST("angle_0to2pi - in ranage", dbnl_angle_0to2pi(1.7), 1.7);
  TEST_NEAR("angle_0to2pi - below", dbnl_angle_0to2pi(-7.1), -7.1 + 4*vnl_math::pi, 1e-8);
  TEST_NEAR("angle_0to2pi - above", dbnl_angle_0to2pi(7.1), 7.1 - 2*vnl_math::pi, 1e-8);

  double twopi = vnl_math::pi * 2;
  double y[] = {6.0, 6.01, 6.5, 6.8};
  double x[] = {y[2]-twopi, y[3] + 5*twopi, y[0], y[1]-4*twopi};
  unsigned num_pts = 4;
  dbnl_angle_sort_ascending(x, num_pts);

  TEST_NEAR("angle_sort_ascending", x[0], y[0], 1e-8);
  TEST_NEAR("angle_sort_ascending", x[1], y[1], 1e-8);
  TEST_NEAR("angle_sort_ascending", x[2], y[2], 1e-8);
  TEST_NEAR("angle_sort_ascending", x[3], y[3], 1e-8);
  
  SUMMARY();
}

