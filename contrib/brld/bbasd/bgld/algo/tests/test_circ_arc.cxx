// This is basic/dbgl/algo/tests/test_circ_arc.cxx

// \author Nhon Trinh
// \date Aug 18, 2005

#include <testlib/testlib_test.h>
#include <dbgl/algo/dbgl_circ_arc.h>
#include <vcl_cmath.h>
#include <vcl_limits.h>
#include <vgl/vgl_distance.h>
//////#include <vnl/vnl_math.h>

//: test functions of dbgl_circ_arc class
void test_dbgl_circ_arc_functions()
{
  vcl_cout << "In test_dbgl_circ_arc_functions()\n";
  vgl_point_2d<double > circ_center(2, 1);
  vgl_point_2d<double > point1(0, 1);
  vgl_point_2d<double > point2(3, 1+vcl_sqrt(3.0));
  double k = -0.5;

  // construct the circular arc
  dbgl_circ_arc arc1(point1, point2, k);
  

  // constant
  const double pi = 3.14159265358979323846;

  TEST("point1()", arc1.point1(), point1);
  TEST("point2()", arc1.point2(), point2);
  TEST("k()", arc1.k(), k);

  vgl_vector_2d<double > chord_dir = arc1.chord_dir();
  TEST_NEAR("chord_dir()", atan2(chord_dir.y(), chord_dir.x()), pi/6, 1e-8);
  TEST_NEAR("chord_length()", arc1.chord_len(), vcl_sqrt(12.0), 1e-8);

  double dist = (arc1.chord_midpoint()-
    vgl_point_2d<double >(1.5, 1+vcl_sqrt(3.0)/2)).length();
  TEST("chord_midpoint()", dist < 1e-8, true);

  TEST_NEAR("central_angle()", arc1.central_angle(), -2*pi/3, 1e-8);
  TEST_NEAR("height()", arc1.height(), 1, 1e-8);
  TEST_NEAR("len()", arc1.len(), 2*pi*2/3, 1e-8);

  double true_area = 0.5 * 2*2* (pi * 2.0/3) - 0.5 * 2*2 * vcl_sin(pi * 2.0/3);

  TEST_NEAR("area()", arc1.area(), true_area, 1e-8);
  TEST_NEAR("area_approx()", arc1.area_approx(), true_area, 0.01*true_area);


  //vcl_cout << "true area = " << true_area << vcl_endl;
  //vcl_cout << "area = " << arc1.area() << vcl_endl;
  //vcl_cout << "area approximation = " << arc1.area_approx() << vcl_endl;

  vgl_vector_2d<double > tangent1 = arc1.tangent_at_start();
  TEST_NEAR("tangent_at_start()", vcl_atan2(tangent1.y(), 
    tangent1.x()), pi/2, 1e-8);

  vgl_vector_2d<double > tangent2 = arc1.tangent_at_end();
  TEST_NEAR("tangent_at_end()", vcl_atan2(tangent2.y(), 
    tangent2.x()), -pi/6, 1e-8);

  vgl_vector_2d<double > normal1 = arc1.normal_at_start();
  TEST_NEAR("normal_at_start()", vcl_atan2(normal1.y(), normal1.x()), 0, 1e-8);


  vgl_vector_2d<double > normal2 = arc1.normal_at_end();
  TEST_NEAR("normal_at_end()", vcl_atan2(normal2.y(), normal2.x()), 
    -2*pi/3, 1e-8);

  //vcl_cout << "\n\nCircle center " << arc1.center() << vcl_endl;
  dist = (circ_center-arc1.center()).length();
  TEST("center()", dist < 1e-8, true);

  vgl_point_2d<double > pt = arc1.point_at_length(2*pi/3);
  //vcl_cout << "\n\nPt at 2*pi/3" << pt << vcl_endl;
  dist = (pt-vgl_point_2d<double >(1, 1+vcl_sqrt(3.0))).length();
  TEST("point_at_length(s)", dist < 1e-8, true);

  vgl_vector_2d<double > t = arc1.tangent_at_length(pi);
  TEST_NEAR("tangent_at_length(s)", vcl_atan2(t.y(), t.x()), 0, 1e-8);

  TEST_NEAR("curvature_at_length(s)", arc1.curvature_at_length(1.0112), 
    k, 1e-8);

  // Test utility function
  vgl_point_2d<double > inside_pt(1, 2);
  TEST("circle region() - inside", arc1.circ_region(inside_pt), -1);

  vgl_point_2d<double > outside_pt(2, 3.000000000001);
  TEST("circle region() - outside", arc1.circ_region(outside_pt), 1);

  vgl_point_2d<double > oncircle_pt(2, -1);
  TEST("circle_region() - on circle", arc1.circ_region(oncircle_pt), 0);
}

void test_dbgl_circ_arc_creation()
{
  vcl_cout << "In test_dbgl_circ_arc_creation()\n";

  // test creating circular arcs from three points
  // k = 0.5
  vgl_point_2d<double > start (0, 0);
  vgl_point_2d<double > end (2*vcl_sqrt((double)3.0), 0);
  
   vgl_point_2d<double > middle(vcl_sqrt(3.0) - 1, vcl_sqrt(3.0)-1);
  //vgl_point_2d<double > middle(vcl_sqrt(3.0), 1);
  dbgl_circ_arc arc;
  arc.set_from(start, middle, end);
  double true_k = -0.5;
  TEST_NEAR("Circular arc from 3 points - k", arc.k(), true_k, 1e-8);

  middle.set(0, 1);
  TEST("Circular arc from 3 points - input validity", arc.set_from(start, middle, end),false);


  // test creating a circular arc from center, starting point, and normal at ending point
  vgl_point_2d<double > center(vcl_sqrt(3.0), -1);
  vgl_vector_2d<double > end_normal(4, 0);
  arc.set_from(start, center, end_normal);
  
  TEST_NEAR("Circular arc from 1 point, 1 center, 1 normal - curvature", arc.k(), 0.5, 1e-8);

  vgl_point_2d<double > true_end_pt(vcl_sqrt(3.0)-2, -1);
  double error = (true_end_pt-arc.point2()).length();
  TEST_NEAR("Circular arc from 1 point, 1 center, 1 normal - end point", error, 0, 1e-8);


  // test creating circular arc from start, end, and tangent at starting point
  start.set(0, 0);
  end.set(2, 2);
  vgl_vector_2d<double > start_tangent(0, 1);
  arc.set_from(start, start_tangent, end);
  TEST_NEAR("Circular arc from 2 points and 1 tangent - test 1", arc.k(), -0.5, 1e-8);

  
  vgl_vector_2d<double > end_tangent(1, 0);
  arc.set_from(end, end_tangent, start);
  TEST_NEAR("Circular arc from 2 points and 1 tangent - test 2", arc.k(), 0.5, 1e-8);

  // test creating circular arc from start, start tangent, curvature, and length
  start.set(3, 3);
  start_tangent.set(0, 2);
  double k = -0.5;
  const double pi = 3.14159265358979323846;
  double length = pi;
  vgl_point_2d<double > correct_p2 (5, 5);
  arc.set_from(start, start_tangent, k, length);

  TEST_NEAR("Circular arc from 1 point, 1 tangent, 1 curvature, and length", 
    vgl_distance(arc.point2(), correct_p2), 0, 1e-8);


  
}

//: Test functions of biarc class
MAIN( test_circ_arc )
{
  START ("Test functions of dbgl_circ_arc");
  test_dbgl_circ_arc_functions();
  test_dbgl_circ_arc_creation();
  SUMMARY();
}

