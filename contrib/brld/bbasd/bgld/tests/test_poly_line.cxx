#include <testlib/testlib_test.h>
#include <dbgl/dbgl_poly_curve_line.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>

#define NearZeroValue    (1.0e-8)

bool almost_equal2(vgl_point_2d<double> a, vgl_point_2d<double> b) {
  return (vcl_fabs(a.x()-b.x()) < NearZeroValue &&
          vcl_fabs(a.y()-b.y()) < NearZeroValue);
}

MAIN( test_poly_line )
{
  START ("Poly Line ");

  vcl_cout << "construct a line along x axis:" << vcl_endl;
  vgl_point_2d<double> start(-1.0, 0.0);  // 0
  vgl_point_2d<double> end(1.0, 0.0);   
  vgl_point_2d<double> center(0.0, 0.0);
  
  dbgl_poly_curve_line a(start, end);
  TEST("length() ", a.length(), 2.0);
  
  TEST("point_at() start ", almost_equal2(a.point_at(0),start), true);
  TEST("point_at() end ", a.point_at(1) == end, true);
  TEST("point_at() middle ", a.point_at(0.5) == center, true);

  TEST("point_at_length() start ", almost_equal2(a.point_at_length(0),start), true);
  TEST("point_at_length() middle ", a.point_at_length(1.0) == center, true);

  TEST("tangent_angle_at() start", vcl_fabs(a.tangent_angle_at(0.0)-0.0)<NearZeroValue, true);
  TEST("tangent_angle_at() start", vcl_fabs(a.tangent_angle_at(1)-0.0)<NearZeroValue, true);

  vgl_vector_2d<double> midtan = a.tangent_at(0.5);

  TEST("tangent_at() mid", vcl_fabs(atan2(midtan.y(),midtan.x())-0.0)<NearZeroValue, true);

  midtan = a.tangent_at_length(1.0);
  TEST("tangent_at_length() mid", vcl_fabs(atan2(midtan.y(),midtan.x())-0.0)<NearZeroValue, true);

  dbgl_poly_curve_line b(start, end);
  TEST("Operator == ", a == b, true);

  dbgl_poly_curve_line c(a);
  TEST("Copy Constructor ", a == c, true);

  //: construct a line in reverse orientation
  vcl_cout << "Testing line in reverse direction\n";
  dbgl_poly_curve_line d(end, start);
  TEST("line vs reverse line equality ", c == d, false);
  TEST("reverse line tangent_angle_at() ", vcl_fabs(d.tangent_angle_at(0.0)-PI)<NearZeroValue, true);
  
  midtan = d.tangent_at(1.0/4.0);
  TEST("reverse line tangent_at() ", vcl_fabs(atan2(midtan.y(),midtan.x())-PI)<NearZeroValue*100, true);

  //: construct line at center (1,1) 
  vcl_cout << "Testing third line direction\n";
  start.set(-2.0,0.0);
  end.set(0.0,2.0);
  center.set(-1.0,1.0);
  dbgl_poly_curve_line f(start, end);
  
  TEST("Operator == ", a == f, false);

  TEST("third line point_at() middle ", f.point_at(0.5) == center, true);
  TEST("third line length() ", vcl_fabs(f.length()-2*vcl_sqrt(2.0))<NearZeroValue, true); 

  midtan = f.tangent_at(1.0/4.0);
  TEST("third line tangent_at() ", vcl_fabs(atan2(midtan.y(),midtan.x())-PI/4)<NearZeroValue, true);
  
  SUMMARY();
}
