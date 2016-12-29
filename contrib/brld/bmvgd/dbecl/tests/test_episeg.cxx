#include <testlib/testlib_test.h>
#include <dbecl/dbecl_episeg.h>
#include <dbecl/dbecl_episeg_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_digital_curve_2d.h>
#include <vnl/vnl_math.h>
#include <vcl_iostream.h>


static bool approx_equal(double a, double b, double thresh=1e-12)
{
  return vcl_abs(b-a) < thresh;
}

MAIN( test_episeg )
{
  START ("episeg");

  // make an epipole
  dbecl_epipole_sptr epipole = new dbecl_epipole(-100.0,50.0);

  // make a curve
  vsol_digital_curve_2d_sptr dc1 = new vsol_digital_curve_2d;
  dc1->add_vertex(new vsol_point_2d(20.0,0.0));
  dc1->add_vertex(new vsol_point_2d(19.0,1.0));
  dc1->add_vertex(new vsol_point_2d(20.0,2.0));
  dc1->add_vertex(new vsol_point_2d(20.5,3.0));
  dc1->add_vertex(new vsol_point_2d(20.5,4.0));
  dc1->add_vertex(new vsol_point_2d(21.0,5.0));
  dc1->add_vertex(new vsol_point_2d(22.0,5.0));
  dc1->add_vertex(new vsol_point_2d(21.0,6.0));

  // make an episeg
  dbecl_episeg_sptr episeg1 = new dbecl_episeg(epipole, dc1, 0.0, double(dc1->size()-1));

  // make a second curve - with decreasing angle
  vsol_digital_curve_2d_sptr dc2 = new vsol_digital_curve_2d;
  dc2->add_vertex(new vsol_point_2d(50.0,30.0));
  dc2->add_vertex(new vsol_point_2d(51.0,29.0));
  dc2->add_vertex(new vsol_point_2d(51.5,28.0));
  dc2->add_vertex(new vsol_point_2d(50.5,27.0));

  // make a second episeg
  dbecl_episeg_sptr episeg2 = new dbecl_episeg(epipole, dc2, 0.0, double(dc2->size()-1));

  TEST("Epipole", episeg1->epipole(), epipole);
  TEST("Curve", episeg1->curve(), dc1);

  // Test the bounds 
  TEST("Min Index", episeg1->min_index(), 0.0);
  TEST("Max Index", episeg1->max_index(), double(dc1->size()-1));

  double min_a = episeg1->min_angle();
  vcl_cout << "Minimum Angle = " << min_a << vcl_endl;
  TEST("Min Angle", approx_equal(min_a, epipole->angle(dc1->p0()->get_p())), true);
  double max_a = episeg1->max_angle();
  vcl_cout << "Maximum Angle = " << max_a << vcl_endl;
  TEST("Max Angle", approx_equal(max_a, epipole->angle(dc1->p1()->get_p())), true);

  min_a = episeg2->min_angle();
  vcl_cout << "Minimum Angle (Reverse) = " << min_a << vcl_endl;
  TEST("Min Angle (Reverse)", approx_equal(min_a, epipole->angle(dc2->p1()->get_p())), true);
  max_a = episeg2->max_angle();
  vcl_cout << "Maximum Angle (Reverse) = " << max_a << vcl_endl;
  TEST("Max Angle (Reverse)", approx_equal(max_a, epipole->angle(dc2->p0()->get_p())), true);
  
  double min_s = epipole->distance(dc1->point(1)->get_p());
  vcl_cout << "Minimum Distance = " << min_s << vcl_endl;
  TEST("Min Distance", approx_equal(min_s, episeg1->min_dist()), true);
  double max_s = epipole->distance(dc1->point(6)->get_p());
  vcl_cout << "Maximum Distance = " << max_s << vcl_endl;
  TEST("Max Distance", approx_equal(max_s, episeg1->max_dist()), true);

  // Test an arbitrary angle
  double a = episeg1->angle(2.5);
  vcl_cout << "Test Angle = " << a << vcl_endl;
  TEST("Angle (2.5)", approx_equal(a, epipole->angle(dc1->interp(2.5))), true);

  // Test the binary search through index
  double index = episeg1->index(episeg1->angle(5.4));
  vcl_cout << " Index Search = "<< index << vcl_endl;
  TEST("Index", approx_equal(index, 5.4, 1e-2), true); // less accurate due to small angle approx.

  index = episeg1->index(episeg1->angle(2.1));
  vcl_cout << " Index Search (Reverse) = "<< index << vcl_endl;
  TEST("Index (Reverse)", approx_equal(index, 2.1, 1e-2), true); // less accurate due to small angle approx.

  SUMMARY();
}
