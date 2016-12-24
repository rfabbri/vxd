// This is basic/dbgl/algo/tests/test_closest_point.cxx

// \author Nhon Trinh
// \date Aug 11, 2005

#include <testlib/testlib_test.h>

#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>
#include <vgl/vgl_lineseg_test.h>
#include <vgl/vgl_triangle_test.h>

#include <dbgl/algo/dbgl_closest_point.h>
#include <dbgl/algo/dbgl_circ_arc.h>

//: Test dbgl_distance::lineseg_lineseg function
void test_closest_point_lineseg_lineseg()
{
  vcl_cout << "In test_closest_point_lineseg_lineseg() \n";
  vgl_point_2d<double > p1(1, 5);
  vgl_point_2d<double > p2(9, 1);
  vgl_point_2d<double > p3(6, 0);
  vgl_point_2d<double > p4(9, 6);

  double d, ratio1, ratio2;
  double real_d, real_ratio1, real_ratio2;
  //unused bool success = false;

  // case 1 - two line segments intersect at(7,2)
  d = dbgl_closest_point::lineseg_lineseg(p1, p2, p3, p4,ratio1, ratio2);
  
  real_d = 0;
  real_ratio1 = 0.75;
  real_ratio2 = 1.0/3;
  
  TEST("Distance of 2 intersecting lines", d==0, true);
  TEST_NEAR("Closest points of intersecting lines (1)", ratio1, real_ratio1, 1e-8);
  TEST_NEAR("Closest points of intersecting lines (2)", ratio2, real_ratio2, 1e-8);

  // case 2 - two line segments don't intersect
  // min-distance is between one endpoint and a mid-point
  p1.set(1, 5);
  p2.set(9, 1);
  p3.set(8, 4);
  p4.set(9, 6);

  // case 2.1
  d = dbgl_closest_point::lineseg_lineseg(p1, p2, p3, p4,ratio1, ratio2);
  real_d = vcl_sqrt((double)5.0);
  real_ratio1 = 0.75;
  real_ratio2 = 0;
  TEST_NEAR("Distance between 2 non-intersecting lines", d, real_d, 1e-8);
  TEST_NEAR("Case 1 - line 1", ratio1, real_ratio1, 1e-8);
  TEST_NEAR("Case 1 - line 2", ratio2, real_ratio2, 1e-8);

  // case 2.2
  d = dbgl_closest_point::lineseg_lineseg(p1, p2, p4, p3, ratio1, ratio2);
  real_d = vcl_sqrt((double)5.0);
  real_ratio1 = 0.75;
  real_ratio2 = 1;
  TEST_NEAR("Distance between 2 non-intersecting lines", d, real_d, 1e-8);
  TEST_NEAR("Case 1 - line 1", ratio1, real_ratio1, 1e-8);
  TEST_NEAR("Case 1 - line 2", ratio2, real_ratio2, 1e-8);

  // case 2.3
  d = dbgl_closest_point::lineseg_lineseg(p3, p4, p1, p2, ratio1, ratio2);
  real_d = vcl_sqrt((double)5.0);
  real_ratio1 = 0;
  real_ratio2 = 0.75;
  TEST_NEAR("Distance between 2 non-intersecting lines", d, real_d, 1e-8);
  TEST_NEAR("Case 1 - line 1", ratio1, real_ratio1, 1e-8);
  TEST_NEAR("Case 1 - line 2", ratio2, real_ratio2, 1e-8);


  // case 2.4
  d = dbgl_closest_point::lineseg_lineseg(p4, p3, p2, p1, ratio1, ratio2);
  real_d = vcl_sqrt((double)5.0);
  real_ratio1 = 1;
  real_ratio2 = 0.25;
  TEST_NEAR("Distance between 2 non-intersecting lines", d, real_d, 1e-8);
  TEST_NEAR("Case 1 - line 1", ratio1, real_ratio1, 1e-8);
  TEST_NEAR("Case 1 - line 2", ratio2, real_ratio2, 1e-8);

  //// failed case in shock code
  //p1.set(328.99996948242187, 127.99999237060547);
  //p2.set(271.98755895963365, 178.20118501723579);
  //p3.set(181.43250686733217, 257.93771209135798);
  //p4.set(102.99999237060547, 326.99996948242187);

  //double x1 = p1.x();
  //double y1 = p1.y();
  //double x2 = p2.x();
  //double y2 = p2.y();
  //double x3 = p3.x();
  //double y3 = p3.y();
  //double x4 = p4.x();
  //double y4 = p4.y();
  //
  //if (!vgl_lineseg_test_lineseg<double>(p1.x(), p1.y(), p2.x(), p2.y(),
  //      p3.x(), p3.y(), p4.x(), p4.y())) 
  //    
  //{
  //  vcl_cout << "Two lines do not intersect\n";
  //}
  //else
  //{
  //  vcl_cout << "Two lines DO intersect\n";
  //}


  //double a = vgl_triangle_test_discriminant<double >(x1, y1, x2, y2, x3, y3);
  //double b = vgl_triangle_test_discriminant<double >(x1, y1, x2, y2, x4, y4);
  //double c = vgl_triangle_test_discriminant<double >(x3, y3, x4, y4, x1, y1);
  //d = vgl_triangle_test_discriminant<double >(x3, y3, x4, y4, x2, y2);


  //vcl_cout << "a = " << a << vcl_endl;
  //vcl_cout << "b = " << b << vcl_endl;
  //vcl_cout << "c = " << c << vcl_endl;
  //vcl_cout << "d = " << d << vcl_endl;

  //// when the two lines intersect, find the intersection 
  //// and modify the edges
  //d = dbgl_closest_point::lineseg_lineseg(p1, p2, p3, p4, ratio1, ratio2);

  //vcl_cout << "d = " << d << vcl_endl;
  //vcl_cout << "ratio1 = " << ratio1 << vcl_endl;
  //vcl_cout << "ratio2 = " << ratio2 << vcl_endl;

  return;
}

//: Test computing closest point from a point to a circular arc
void test_closest_point_to_circular_arc()
{
  vcl_cout << "In test_closest_point_to_circular_arc() \n";
  
  // Parameters of circular arc
  vgl_point_2d<double > arc_p1(2, -1);
  vgl_point_2d<double > arc_p2(5, vcl_sqrt(3.0)-1);
  double arc_k = -0.5;

  vgl_point_2d<double > query_pt(0,0);
  double real_d = vcl_sqrt(17.0)-2;
  double real_ratio = vcl_atan2(1.0, 4.0)/(2*vnl_math::pi/3);
  double ret_ratio = -1;
  double ret_d = dbgl_closest_point::point_to_circular_arc(query_pt, arc_p1,
    arc_p2, arc_k, ret_ratio);
  TEST_NEAR("point to arc (inside arc) - distance", ret_d, real_d, 1e-12);
  TEST_NEAR("point to arc (inside arc) - ratio", ret_ratio, real_ratio, 
    1e-12);

  query_pt.set(5, -2);
  real_d = vcl_sqrt(3.0) + 1;
  real_ratio = 1;
  ret_d = dbgl_closest_point::point_to_circular_arc(query_pt, arc_p1,
    arc_p2, arc_k, ret_ratio);
  TEST_NEAR("point to arc (from inside circle) - distance", ret_d, real_d, 1e-12);
  TEST_NEAR("point to arc (from inside circle) - ratio", ret_ratio, real_ratio, 
    1e-12);

  query_pt.set(1, -2);
  real_d = vcl_sqrt((double)2.0);
  real_ratio = 0;
  ret_d = dbgl_closest_point::point_to_circular_arc(query_pt, arc_p1,
    arc_p2, arc_k, ret_ratio);
  TEST_NEAR("point to arc (at 1st endpoint) - distance", ret_d, real_d, 1e-12);
  TEST_NEAR("point to arc (at 1st endpoint) - ratio", ret_ratio, real_ratio, 
    1e-12);

  query_pt.set(6, 0);
  real_d = vcl_sqrt(1+vnl_math::sqr(vcl_sqrt(3.0)-1));
  real_ratio = 1;
  ret_d = dbgl_closest_point::point_to_circular_arc(query_pt, arc_p1,
    arc_p2, arc_k, ret_ratio);
  TEST_NEAR("point to arc (at 2nd endpoint) - distance", ret_d, real_d, 1e-12);
  TEST_NEAR("point to arc (at 2nd endpoint) - ratio", ret_ratio, real_ratio, 
    1e-12);

  // arc with very small curvature
  arc_p1.set(2, 2);
  arc_p2.set(2, -2);
  arc_k = -1e-11;
  query_pt.set(3, 0);
  //real_d =  1-1e11*(1-cos(2e-11));
  dbgl_circ_arc arc2(arc_p1, arc_p2, arc_k);
  real_d = 3-arc2.point_at_length(arc2.len()/2).x();
  
  real_ratio = 0.5;
  ret_d = dbgl_closest_point::point_to_circular_arc(query_pt, arc_p1,
    arc_p2, arc_k, ret_ratio);
  TEST_NEAR("point to arc (small k) - distance", ret_d, real_d, 1e-12);
  TEST_NEAR("point to arc (small k) - ratio", ret_ratio, real_ratio, 
    1e-12);
}



void test_closest_point_to_circle()
{
  // line case
  vgl_point_2d<double > start ( 1, 1);
  vgl_vector_2d<double > start_tangent (5, 0);
  double k = 0;
  vgl_point_2d<double > query_pt(0, 3);
  double arc_length = 1e100;
  double min_dist = 
    dbgl_closest_point::point_to_circle(query_pt, start, start_tangent, k, arc_length);

  TEST_NEAR("point to circle - line case - min_dist", min_dist, 2, 1e-12);
  TEST_NEAR("point to circle - line case -arc length of closest pt", arc_length, -1, 1e-10);


  // proper arc
  start.set(1, 1);
  start_tangent.set(100, 0);
  k = 0.5;
  query_pt.set(-1, 1);
  min_dist = dbgl_closest_point::point_to_circle(query_pt, start, start_tangent, k, arc_length);
  
  TEST_NEAR("point to circle - arc case - min_dist", min_dist, 2*vcl_sqrt(2.0)-2, 1e-12);
  TEST_NEAR("point to circle-arc case-arc length of closest pt", arc_length, -vnl_math::pi_over_2, 1e-10);



}

//: Test computing closest point between a linesegment and circular arc
void test_closest_point_lineseg_to_circular_arc()
{
  vcl_cout << "In test_closest_point_lineseg_to_circular_arc() \n";

  // Case 1 - two intersections

  // Parameters of circular arc
  vgl_point_2d<double > arc_p1(2, -1);
  vgl_point_2d<double > arc_p2(5, vcl_sqrt(3.0)-1);
  double arc_k = -0.5;


  // Parameter of line segment
  // line (x, y) = (4, 1) + t (sqrt(3), 1)
  vgl_point_2d<double > line_p1(4 + vcl_sqrt(3.0)*(-2), 1+ 1*(-2));
  vgl_point_2d<double > line_p2(4 + vcl_sqrt(3.0)*(0.5), 1+ 1*(0.5));


  double real_d = 0;
  double real_line_ratios[2];
  real_line_ratios[0] = 0.4;
  real_line_ratios[1] = 0.8;

  double real_arc_ratios[2];
  real_arc_ratios[0] = 0.25;
  real_arc_ratios[1] = 0.75;

  vcl_vector<double > line_ratios;
  vcl_vector<double > arc_ratios;
  double d = dbgl_closest_point::lineseg_to_circular_arc(line_p1, line_p2, 
    arc_p1, arc_p2, arc_k, line_ratios, arc_ratios);

  TEST("lineseg to arc (2 intersections) - distance", d, real_d);
  TEST("lineseg to arc (2 intersections) - num. intersect", line_ratios.size(), 2);

  bool check_ratios = true;
  for (unsigned int i=0; i<line_ratios.size(); ++i)
  {
    check_ratios = check_ratios && (vnl_math::abs(line_ratios[i]-real_line_ratios[i]) < 1e-12);
    check_ratios = check_ratios && (vnl_math::abs(arc_ratios[i]-real_arc_ratios[i]) < 1e-12);
  }
  TEST("lineseg to arc (2 intersections) - ratios", check_ratios, true);


  // Case 2: one intesection
  // Parameter of line segment
  // line (x, y) = (4, 1) + t (sqrt(3), 1)
  line_p1.set(4 + vcl_sqrt(3.0)*(-2), 1+ 1*(-2));
  line_p2.set(4 + vcl_sqrt(3.0)*(-0.5), 1+ 1*(-0.5));

  real_d = 0;
  real_line_ratios[0] = 2.0/3;
  real_arc_ratios[0] = 0.25;
  d = dbgl_closest_point::lineseg_to_circular_arc(line_p1, line_p2, 
    arc_p1, arc_p2, arc_k, line_ratios, arc_ratios);
  TEST("lineseg to arc (1 intersections) - distance", d, real_d);
  TEST("lineseg to arc (1 intersections) - num. intersect", line_ratios.size(), 1);

  check_ratios = true;
  for (unsigned int i=0; i<line_ratios.size(); ++i)
  {
    check_ratios = check_ratios && (vnl_math::abs(line_ratios[i]-real_line_ratios[i]) < 1e-12);
    check_ratios = check_ratios && (vnl_math::abs(arc_ratios[i]-real_arc_ratios[i]) < 1e-12);
  }
  TEST("lineseg to arc (1 intersections) - ratios", check_ratios, true);

  // Case 3: No intesection, local minima
  // Parameter of line segment
  line_p1.set(0, 2);
  line_p2.set(5, 2);

  real_d = 1;
  real_line_ratios[0] = 0.8;
  real_arc_ratios[0] = 0.75;
  d = dbgl_closest_point::lineseg_to_circular_arc(line_p1, line_p2, 
    arc_p1, arc_p2, arc_k, line_ratios, arc_ratios);
  TEST("lineseg to arc (0 intersections, local minimum) - distance", vnl_math::abs(d-real_d)<1e-12, true);
  TEST("lineseg to arc (0 intersections, local minimum) - num. ratios returned", line_ratios.size(), 1 );

  check_ratios = true;
  for (unsigned int i=0; i<line_ratios.size(); ++i)
  {
    check_ratios = check_ratios && (vnl_math::abs(line_ratios[i]-real_line_ratios[i]) < 1e-12);
    check_ratios = check_ratios && (vnl_math::abs(arc_ratios[i]-real_arc_ratios[i]) < 1e-12);
  }
  TEST("lineseg to arc (0 intersections, local minimum) - ratios", check_ratios, true);

  // Case 4: No intesection, line_p1 and arc
  // Parameter of line segment
  line_p1.set(4, 2);
  line_p2.set(1, 3);

  real_d = 1;
  real_line_ratios[0] = 0;
  real_arc_ratios[0] = 0.75;
  d = dbgl_closest_point::lineseg_to_circular_arc(line_p1, line_p2, 
    arc_p1, arc_p2, arc_k, line_ratios, arc_ratios);
  TEST("lineseg to arc (0 intersect, line_p1 and arc) - distance", vnl_math::abs(d-real_d)<1e-12, true);
  TEST("lineseg to arc (0 intersect, line_p1 and arc) - num. ratios returned", line_ratios.size(), 1 );

  check_ratios = (line_ratios[0] == 0) &&
    (vnl_math::abs(arc_ratios[0]-real_arc_ratios[0]) < 1e-12);
  TEST("lineseg to arc (0 intersect, line_p1 and arc) - ratios", check_ratios, true);

  // Case 4: No intesection, arc_p1 and line
  // Parameter of line segment
  line_p1.set(4.5, 0);
  line_p2.set(6.5, 0);

  real_d = vcl_sqrt(3.0)-1;
  real_line_ratios[0] = 0.25;
  real_arc_ratios[0] = 1;
  d = dbgl_closest_point::lineseg_to_circular_arc(line_p1, line_p2, 
    arc_p1, arc_p2, arc_k, line_ratios, arc_ratios);
  TEST("lineseg to arc (0 intersect, arc_p2 and line) - distance", vnl_math::abs(d-real_d)<1e-12, true);
  TEST("lineseg to arc (0 intersect, arc_p2 and line) - num. ratios returned", line_ratios.size(), 1 );

  check_ratios = (arc_ratios[0] == 1) &&
    (vnl_math::abs(line_ratios[0]-real_line_ratios[0]) < 1e-12);
  TEST("lineseg to arc (0 intersect, arc_p2 and line) - ratios", check_ratios, true);

}


//: Test computing closest point between a linesegment and circular arc
void test_closest_point_circular_arc_to_circular_arc()
{
  vcl_cout << "In test_closest_point_circular_arc_to_circular_arc() \n";

  // Case 1 - two intersections

  // Parameters of arc1
  // formular (x,y) = (5+r2 * cos(t), 0+ r2*sin(t));
  double r1 = 2;
  double t = (2.0/3) * vnl_math::pi;
  vgl_point_2d<double > arc1_p1(3+vcl_sqrt(3.0) + r1 * vcl_cos(t), 1+ r1*vcl_sin(t));
  t = -(2.0/3)*vnl_math::pi;
  vgl_point_2d<double > arc1_p2(3+vcl_sqrt(3.0) + r1 * vcl_cos(t), 1+ r1*vcl_sin(t));
  double arc1_k = 1/r1;

  // Parameters of arc2
  // formular (x,y) = (2+sqrt(2) * cos(t), 1+ sqrt(2)*sin(t));
  double r2 = vcl_sqrt(2.0);
  t = -(3.0/8) * vnl_math::pi;
  vgl_point_2d<double > arc2_p1(2 + r2 * vcl_cos(t), 1+ r2*vcl_sin(t));
  t = (3.0/8)*vnl_math::pi;
  vgl_point_2d<double > arc2_p2(2 + r2 * vcl_cos(t), 1+ r2*vcl_sin(t));
  double arc2_k = 1/r2;


  // The 2 circles intersect at (3, 0) and (3, 2)
  double real_d = 0;
  double real_arc1_ratios[2];
  real_arc1_ratios[0] = 1.0/4;
  real_arc1_ratios[1] = 3.0/4;

  //// For debugging purpose
  //vcl_cout << "ARC 1: \n";
  //dbgl_circ_arc arc1(arc1_p1, arc1_p2, arc1_k);
  //for (int i=0; i<2; ++i)
  //{
  //  vcl_cout << "Point at(" << real_arc1_ratios[i]*arc1.len() << ")= " << 
  //    arc1.point_at_length(real_arc1_ratios[i]*arc1.len())<< vcl_endl; 
  //  vcl_cout << "Tangent at(" << real_arc1_ratios[i]*arc1.len() << ")= " << 
  //    arc1.tangent_at_length(real_arc1_ratios[i]*arc1.len())<< vcl_endl; 
  //}

  double real_arc2_ratios[2];
  real_arc2_ratios[0] = 5.0/6;
  real_arc2_ratios[1] = 1.0/6;

  //// For debugging purpose
  //vcl_cout << "ARC 2: \n";
  //dbgl_circ_arc arc2(arc2_p1, arc2_p2, arc2_k);
  //for (int i=0; i<2; ++i)
  //{
  //  vcl_cout << "Point at(" << real_arc2_ratios[i]*arc2.len() << ")= " << 
  //    arc2.point_at_length(real_arc2_ratios[i]*arc2.len())<< vcl_endl; 
  //  vcl_cout << "Tangent at(" << real_arc2_ratios[i]*arc2.len() << ")= " << 
  //    arc2.tangent_at_length(real_arc2_ratios[i]*arc2.len())<< vcl_endl; 
  //}

  vcl_vector<double > arc1_ratios;
  vcl_vector<double > arc2_ratios;

  double d = dbgl_closest_point::circular_arc_to_circular_arc(arc1_p1, arc1_p2, arc1_k, 
    arc2_p1, arc2_p2, arc2_k, arc1_ratios, arc2_ratios);

  TEST("arc to arc (2 intersections) - distance", d, real_d);
  TEST("arc to arc (2 intersections) - num. intersect", arc1_ratios.size(), 2);
  bool check_ratios = true;
  for (unsigned int i=0; i<arc1_ratios.size(); ++i)
  {
    check_ratios = check_ratios && (vnl_math::abs(arc1_ratios[i]-real_arc1_ratios[i]) < 1e-12);
    check_ratios = check_ratios && (vnl_math::abs(arc2_ratios[i]-real_arc2_ratios[i]) < 1e-12);
  }
  TEST("arc to arc (2 intersections) - ratios", check_ratios, true);


  // Case 2: one intesection
  // Parameters of arc2
  // formular (x,y) = (2+sqrt(2) * cos(t), 1+ sqrt(2)*sin(t));
  r2 = vcl_sqrt(2.0);
  t = -(3.0/8) * vnl_math::pi;
  arc2_p1.set(2 + r2 * vcl_cos(t), 1+ r2*vcl_sin(t));
  t = 0; //(3.0/8)*vnl_math::pi;
  arc2_p2.set(2 + r2 * vcl_cos(t), 1+ r2*vcl_sin(t));
  arc2_k = 1/r2;

  // The 2 circles intersect at (3, 0) and (3, 2)
  real_d = 0;
  real_arc1_ratios[0] = 3.0/4;
  real_arc2_ratios[0] = 1.0/3;

  d = dbgl_closest_point::circular_arc_to_circular_arc(arc1_p1, arc1_p2, arc1_k, 
    arc2_p1, arc2_p2, arc2_k, arc1_ratios, arc2_ratios);

  TEST("arc to arc (1 intersections) - distance", d, real_d);
  TEST("arc to arc (1 intersections) - num. intersect", arc1_ratios.size(), 1);
  check_ratios = true;
  for (unsigned int i=0; i<arc1_ratios.size(); ++i)
  {
    check_ratios = check_ratios && (vnl_math::abs(arc1_ratios[i]-real_arc1_ratios[i]) < 1e-12);
    check_ratios = check_ratios && (vnl_math::abs(arc2_ratios[i]-real_arc2_ratios[i]) < 1e-12);
  }
  TEST("arc to arc (1 intersections) - ratios", check_ratios, true);


  // Case 3: No intesection - local minimum
  // Parameters of arc2
  // formular (x,y) = (0+sqrt(2) * cos(t), 1+ sqrt(2)*sin(t));
  r2 = vcl_sqrt(2.0);
  t = -(1.0/2) * vnl_math::pi;
  arc2_p1.set(0 + r2 * vcl_cos(t), 1+ r2*vcl_sin(t));
  t = (1.0/4)*vnl_math::pi;
  arc2_p2.set(0 + r2 * vcl_cos(t), 1+ r2*vcl_sin(t));
  arc2_k = 1/r2;

  // The 2 circles intersect at (3, 0) and (3, 2)
  real_d = 3+vcl_sqrt(3.0) - vcl_sqrt(2.0)-2;
  real_arc1_ratios[0] = 1.0/2;
  real_arc2_ratios[0] = 2.0/3;

  d = dbgl_closest_point::circular_arc_to_circular_arc(arc1_p1, arc1_p2, arc1_k, 
    arc2_p1, arc2_p2, arc2_k, arc1_ratios, arc2_ratios);

  TEST_NEAR("arc to arc (0 intersect-local minimum) - distance", d, real_d, 1e-12);
  TEST("arc to arc (0 intersect-local minimum) - num. pts returned", arc1_ratios.size(), 1);
  check_ratios = true;
  for (unsigned int i=0; i<arc1_ratios.size(); ++i)
  {
    check_ratios = check_ratios && (vnl_math::abs(arc1_ratios[i]-real_arc1_ratios[i]) < 1e-12);
    check_ratios = check_ratios && (vnl_math::abs(arc2_ratios[i]-real_arc2_ratios[i]) < 1e-12);
  }
  TEST("arc to arc (0 intersect-local minimum) - ratios", check_ratios, true);


  // Case 4: No intesection - end points
  // Parameters of arc2
  // formular (x,y) = (0+sqrt(2) * cos(t), 1+ sqrt(2)*sin(t));
  r2 = 4;
  arc2_p1.set(0, 1);
  arc2_p2.set(-4, 5);
  arc2_k = -1/r2;

  // The 2 circles intersect at (3, 0) and (3, 2)
  real_d = 3+vcl_sqrt(3.0)-2;
  real_arc1_ratios[0] = 1.0/2;
  real_arc2_ratios[0] = 0;

  d = dbgl_closest_point::circular_arc_to_circular_arc(arc1_p1, arc1_p2, arc1_k, 
    arc2_p1, arc2_p2, arc2_k, arc1_ratios, arc2_ratios);

  TEST_NEAR("arc to arc (0 intersect-arc2_p1 and arc1) - distance", d, real_d, 1e-12);
  TEST("arc to arc (0 intersect-arc2_p1 and arc1) - num. pts returned", arc1_ratios.size(), 1);
  check_ratios = true;
  for (unsigned int i=0; i<arc1_ratios.size(); ++i)
  {
    check_ratios = check_ratios && (vnl_math::abs(arc1_ratios[i]-real_arc1_ratios[i]) < 1e-12);
    check_ratios = check_ratios && (vnl_math::abs(arc2_ratios[i]-real_arc2_ratios[i]) < 1e-12);
  }
  TEST("arc to arc (0 intersect-arc2_p1 and arc1) - ratios", check_ratios, true);


  // Case 5: Two arcs are on the same circle - no overlapping
  arc1_p1.set(0, -5);
  arc1_p2.set(5, 0);
  arc1_k = 1.0/5;

  arc2_p1.set(5*vcl_sqrt(2.0)/2, 5*vcl_sqrt(2.0)/2);
  arc2_p2.set(0, 5);
  arc2_k = 1.0 / 5;

  d = dbgl_closest_point::circular_arc_to_circular_arc(arc1_p1, arc1_p2, arc1_k, 
    arc2_p1, arc2_p2, arc2_k, arc1_ratios, arc2_ratios);

  // closest point is at end point 
  real_d = (arc1_p2-arc2_p1).length();
  
  real_arc1_ratios[0] = 1;
  real_arc2_ratios[0] = 0;


  TEST_NEAR("arc to arc (2 arcs on same circle, no overlapping)-distance", d, real_d, 1e-12);
  TEST("arc to arc (2 arcs on same circle, no overlapping)-num. pts returned", arc1_ratios.size(), 1);
  check_ratios = true;
  for (unsigned int i=0; i<arc1_ratios.size(); ++i)
  {
    check_ratios = check_ratios && (vnl_math::abs(arc1_ratios[i]-real_arc1_ratios[i]) < 1e-12);
    check_ratios = check_ratios && (vnl_math::abs(arc2_ratios[i]-real_arc2_ratios[i]) < 1e-12);
  }
  TEST("arc to arc (2 arcs on same circle, no overlapping) - ratios", check_ratios, true);

  
  // Case 6: Two arcs are on the same circle - overlapping
  arc1_p1.set(0, -5);
  arc1_p2.set(5, 0);
  arc1_k = 1.0/5;

  arc2_p1.set(5*vcl_sqrt(2.0)/2, -5*vcl_sqrt(2.0)/2);
  arc2_p2.set(0, 5);
  arc2_k = 1.0 / 5;

  d = dbgl_closest_point::circular_arc_to_circular_arc(arc1_p1, arc1_p2, arc1_k, 
    arc2_p1, arc2_p2, arc2_k, arc1_ratios, arc2_ratios);

  // closest point is at end point 
  real_d = 0;

  TEST_NEAR("arc to arc (2 arcs on same circle, overlapping)-distance", d, real_d, 1e-12);
  TEST("arc to arc (2 arcs on same circle, overlapping)-num. pts returned", 
    arc1_ratios.size() > 0 && arc2_ratios.size() > 0, true);
}


//: Test closest point functions
MAIN( test_closest_point )
{
  START (" Test dbgl_distance class functions");
  test_closest_point_lineseg_lineseg();
  test_closest_point_to_circular_arc();
  test_closest_point_to_circle();
  test_closest_point_lineseg_to_circular_arc();
  test_closest_point_circular_arc_to_circular_arc();
  SUMMARY();
}
