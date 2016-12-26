#include <testlib/testlib_test.h>

#include <dbgl/dbgl_poly_curve_line.h>
#include <dbsol/dbsol_interp_curve_2d.h>

#include <vcl_iostream.h>
#include <vcl_cmath.h>

#define NearZeroValue    (1.0e-8)

bool almost_equal3(vgl_point_2d<double> a, vgl_point_2d<double> b) {
  return (vcl_fabs(a.x()-b.x()) < NearZeroValue &&
          vcl_fabs(a.y()-b.y()) < NearZeroValue);
}

MAIN( test_interp_curve_2d )
{
  START ("Interpolated Curve 2d ");

  vcl_cout << "construct a line along x axis:" << vcl_endl;
  vgl_point_2d<double> p0(-1.0, -1.0);  // 0
  vgl_point_2d<double> p1(0.0, 0.0);   
  vgl_point_2d<double> p2(1.0, 0.0);
  double s = 0.74563;
  vgl_point_2d<double> test_p(s, 0.0);
  
  vcl_vector<dbgl_param_curve *> ints(2);
/*
  ints[0] = new dbgl_poly_curve_line(p0, p1);
  ints[1] = new dbgl_poly_curve_line(p1, p2);

  dbsol_interp_curve_2d c(ints);
  
  TEST("length() ", vcl_fabs(c.length()-(sqrt((double)2)+1))<NearZeroValue, true);
  TEST("point_at() start ", almost_equal3((c.point_at(0))->get_p(), p0), true);
  TEST("point_at() middle ", almost_equal3((c.point_at(sqrt((double)2)))->get_p(), p1), true);
  TEST("point_at() end ", almost_equal3((c.point_at(c.length()))->get_p(), p2), true);

  TEST("point_at() test_p ", almost_equal3((c.point_at(sqrt((double)2)+s))->get_p(), test_p), true);

  TEST("tangent_angle_at() start ", vcl_fabs(c.tangent_angle_at(0.0)-PI/4)<NearZeroValue, true);
  TEST("tangent_angle_at() 0.5 ", vcl_fabs(c.tangent_angle_at(0.5)-PI/4)<NearZeroValue, true);
  TEST("tangent_angle_at() 0.99999 ", vcl_fabs(c.tangent_angle_at(0.9999999999)-PI/4)<NearZeroValue, true);
  TEST("tangent_angle_at() middle ", vcl_fabs(c.tangent_angle_at(1)-PI/4)<NearZeroValue, true);
  TEST("tangent_angle_at() 1.5 ", vcl_fabs(c.tangent_angle_at(1.5)-0.0)<NearZeroValue, true);
  TEST("tangent_angle_at() end ", vcl_fabs(c.tangent_angle_at(2)-0.0)<NearZeroValue, true);

//  dbgl_poly_curve_line b(start, end);
//  TEST("Operator == ", a == b, true);

//  dbgl_poly_curve_line c(a);
//  TEST("Copy Constructor ", a == c, true);

  //: construct a line in reverse orientation
//  vcl_cout << "Testing line in reverse direction\n";
//  dbgl_poly_curve_line d(end, start);
//  TEST("line vs reverse line equality ", c == d, false);
//  TEST("reverse line tangent_angle_at() ", vcl_fabs(d.tangent_angle_at(0.0)-PI)<NearZeroValue, true);
  
//  midtan = d.tangent_at(1.0/4.0);
//  TEST("reverse line tangent_at() ", vcl_fabs(atan2(midtan.y(),midtan.x())-PI)<NearZeroValue*100, true);

  //: construct line at center (1,1) 
//  vcl_cout << "Testing third line direction\n";
//  start.set(-2.0,0.0);
//  end.set(0.0,2.0);
//  center.set(-1.0,1.0);
//  dbgl_poly_curve_line f(start, end);
  
//  TEST("Operator == ", a == f, false);

//  TEST("third line point_at() middle ", f.point_at(0.5) == center, true);
//  TEST("third line length() ", vcl_fabs(f.length()-2*vcl_sqrt(2.0))<NearZeroValue, true); 

//  midtan = f.tangent_at(1.0/4.0);
 // TEST("third line tangent_at() ", vcl_fabs(atan2(midtan.y(),midtan.x())-PI/4)<NearZeroValue, true);
  */
  SUMMARY();
}
