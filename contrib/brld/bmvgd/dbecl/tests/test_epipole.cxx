#include <testlib/testlib_test.h>
#include <dbecl/dbecl_epipole.h>
#include <dbecl/dbecl_epipole_sptr.h>
#include <vnl/vnl_math.h>
#include <vcl_iostream.h>


static bool approx_equal(double a, double b)
{
  return vcl_abs(b-a) < 1e-14;
}

MAIN( test_epipole )
{
  START ("epipole");

  // make an epipole
  dbecl_epipole_sptr epipole = new dbecl_epipole(-10.0,20.0);
  vcl_cout << "Epipole at (-10.0, 20.0)" << vcl_endl;
  

  // test the distance function
  double s = epipole->distance(6.0,32.0);
  vcl_cout << "distance to (6.0, 32.0) = " << s << vcl_endl;
  TEST("Distance", s, 20.0);


  // test the angle function
  double a = epipole->angle(10.0,20.0);
  vcl_cout << "angle at (10.0,20.0) = " << a << vcl_endl;
  TEST("Angle (0 deg)",a, 0.0);

  a = epipole->angle(-5.0,25.0);
  vcl_cout << "angle at (-5.0,25.0) = " << a << vcl_endl;
  TEST("Angle (+45 deg)",approx_equal(a, vnl_math::pi_over_4), true);

  a = epipole->angle(0.0,10.0);
  vcl_cout << "angle at (0.0,10.0) = " << a << vcl_endl;
  TEST("Angle (-45 deg)",approx_equal(a, -vnl_math::pi_over_4), true);

  a = epipole->angle(-10.0 - 1.0/vcl_sqrt(3.0), 21.0);
  vcl_cout << "angle at (-10.0 - 1.0/sqrt(3.0), 21.0) = " << a << vcl_endl;
  TEST("Angle (150 deg)",approx_equal(a, (vnl_math::pi)*2.0/3.0), true);

  SUMMARY();
}
