// This is basic/dbgl/algo/tests/test_distance.cxx

// \author Nhon Trinh
// \date Aug 11, 2005

#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>
#include <testlib/testlib_test.h>
#include <dbgl/algo/dbgl_distance.h>

////: Test dbgl_distance::point_point function
//void test_distance_point_point()
//{
//  vcl_cout << "In test_distance_point_point() \n";
//  vgl_point_2d<double > p1(1, 2);
//  vgl_point_2d<double > p2(3, 5);
//  double d = dbgl_distance::point_point(p1, p2);
//  double reald = vcl_sqrt(2.0*2.0 + 3.0*3.0);
//  TEST_NEAR("Test distance point-point", d, reald, 1e-8);
//  return;
//}


//: Test dbgl_distance::lineseg_lineseg function
void test_distance_lineseg_lineseg()
{
  vcl_cout << "In test_distance_lineseg_linesegn() \n";
  vgl_point_2d<double > p1(1, 1);
  vgl_point_2d<double > p2(3, 3);
  vgl_point_2d<double > p3(2, 3);
  vgl_point_2d<double > p4(3, 0);

  // case 1 - two line segments intersect
  double d = dbgl_distance::lineseg_lineseg(p1, p2, p3, p4);
  TEST("Distance of 2 intersecting lines", d==0, true);

  // case 2 - two line segments don't intersect
  // min-distance is between one endpoint and a mid-point
  p1.set(1, 1);
  p2.set(3, 3);
  p3.set(3, 2);
  p4.set(3, 0);


  d = dbgl_distance::lineseg_lineseg(p1, p2, p3, p4);
  double dreal = vcl_sqrt((double)2.0)/2;
  TEST_NEAR("Min-distance is b/w one endpoint and one midpoint", d, dreal, 1e-8);

  // case 3 - two line segments don't intersect
  // min-distance occurs between 2 endpoints
  p1.set(0, 0);
  p2.set(1, 2);
  p3.set(3, 3);
  p4.set(5, 3);
  
  d = dbgl_distance::lineseg_lineseg(p1, p2, p3, p4);
  dreal = vcl_sqrt((double)1.0*1.0+2*2);
  TEST_NEAR("Min-distance is b/w 2 endpoints", d, dreal, 1e-8);

  // case 4 - two lines barely meet
  p1.set(0, 0);
  p2.set(2, 1);
  p3.set(3, 0);
  p4.set(2, 1);
  d = dbgl_distance::lineseg_lineseg(p1, p2, p3, p4);
  TEST_NEAR("Min-distance is b/w 2 endpoints", d, 0, 1e-8);
}

//: Tests documented by Ricardo in his jan 18 2007 notes
void test_edge_direction_distance()
{
  const double tol=1e-7;

  //: Edges that are parallel
  //: Rememeber: edge distance is squared.
  vcl_cout << "\nTestcase: Degeneracy 1 (Parallel edges)\n";
  {
  double x1,y1,x2,y2,theta1,theta2, dn12,dn21, dt12, de12, dt21, de21;

  x1 = 1; y1 = 0; theta1 = 0; // E1
  x2 = 1; y2 = 1; theta2 = 0; // E2

  dbgl_distance::projected_distance(x1,y1, x2,y2,theta2, &de12, &dn12, &dt12);

  vcl_cout << "Testing d(E1,E2)" << vcl_endl;
  TEST_NEAR("Edge distance",de12,1,tol);
  TEST_NEAR("Normal distance",dn12,1,tol);
  TEST_NEAR("Tangential distance",dt12,0,tol);

  dbgl_distance::projected_distance(x2,y2,x1,y1,theta1, &de21, &dn21, &dt21);
  vcl_cout << "Testing d(E2,E1)" << vcl_endl;
  TEST_NEAR("Edge distance",de21,1,tol);
  TEST_NEAR("Normal distance",dn21,1,tol);
  TEST_NEAR("Tangential distance",dt12,0,tol);
  }

  vcl_cout << "\nTestcase A\n";
  {
  double x1,y1,x2,y2,theta1,theta2, dn12,dn21, dt12, de12, dt21, de21;

  x1 = 0; y1 = 0; theta1 = 0; // E1
  x2 = 1; y2 = 1; theta2 = 3.0*vnl_math::pi/4.0; // E2

  dbgl_distance::projected_distance(x1,y1, x2,y2,theta2, &de12, &dn12, &dt12);

  vcl_cout << "Testing d(E1,E2)" << vcl_endl;
  TEST_NEAR("Edge distance",de12,vcl_sqrt(2.0),tol);
  TEST_NEAR("Normal distance",dn12,vcl_sqrt(2.0),tol);
  TEST_NEAR("Tangential distance",0,0,tol);

  dbgl_distance::projected_distance(x2,y2, x1,y1,theta1, &de21, &dn21, &dt21);
  vcl_cout << "Testing d(E2,E1)" << vcl_endl;
  TEST_NEAR("Edge distance",de21,vcl_sqrt(2.0),tol);
  TEST_NEAR("Normal distance",dn21,1,tol);
  TEST_NEAR("Tangential distance",dt21,1,tol);
  }

  vcl_cout << "\nTestcase A rotated\n";
  {
  double x1,y1,x2,y2,theta1,theta2, dn12,dn21, dt12, de12, dt21, de21;

  x1 = 0; y1 = 0; theta1 = 0 + vnl_math::pi/3.0; // E1
  x2 = 1; y2 = 1; theta2 = 3.0*vnl_math::pi/4.0 + vnl_math::pi/3.0; // E2

  x2 = vcl_cos(vnl_math::pi/3.0) - vcl_sin(vnl_math::pi/3);
  y2 = vcl_sin(vnl_math::pi/3.0) + vcl_cos(vnl_math::pi/3);

  dbgl_distance::projected_distance(x1,y1, x2,y2,theta2, &de12, &dn12, &dt12);

  vcl_cout << "Testing d(E1,E2)" << vcl_endl;
  TEST_NEAR("Edge distance",de12,vcl_sqrt(2.0),tol);
  TEST_NEAR("Normal distance",dn12,vcl_sqrt(2.0),tol);
  TEST_NEAR("Tangential distance",dt12,0,tol);

  dbgl_distance::projected_distance(x2,y2, x1,y1, theta1, &de21, &dn21, &dt21);
  vcl_cout << "Testing d(E2,E1)" << vcl_endl;
  TEST_NEAR("Edge distance",de21,vcl_sqrt(2.0),tol);
  TEST_NEAR("Normal distance",dn21,1,tol);
  TEST_NEAR("Tangential distance",dt21,1,tol);
  }

  vcl_cout << "\nTestcase: Degeneracy 3 (edge 2 direction in line of projection)\n";
  {
  double x1,y1,x2,y2,theta1,theta2, dn12,dn21, dt12, de12, dt21, de21;

  x1 = 0; y1 = 0; theta1 = 0; // E1
  x2 = 1; y2 = 1; theta2 = vnl_math::pi/4.0; // E2

  dbgl_distance::projected_distance(x1,y1, x2,y2,theta2, &de12, &dn12, &dt12);

  vcl_cout << "Testing d(E1,E2)" << vcl_endl;
  TEST_NEAR("Edge distance",de12,vcl_sqrt(2.0),tol);
  TEST_NEAR("Normal distance",dn12,0,tol);
  TEST_NEAR("Tangential distance",dt12,vcl_sqrt(2.0),tol);

  dbgl_distance::projected_distance(x2,y2, x1,y1, theta1, &de21, &dn21, &dt21);
  vcl_cout << "Testing d(E2,E1)" << vcl_endl;
  TEST_NEAR("Edge distance",de21,vcl_sqrt(2.0),tol);
  TEST_NEAR("Normal distance",dn21,1,tol);
  TEST_NEAR("Tangential distance",dt21,1,tol);
  }

  vcl_cout << "\nTestcase: Degeneracy 3 (edge 2 direction in line of projection - rotated)\n";
  {
  double x1,y1,x2,y2,theta1,theta2, dn12,dn21, dt12, de12, dt21, de21;

  x1 = 0; y1 = 0; theta1 = 0 + vnl_math::pi/3.0; // E1
  x2 = 1; y2 = 1; theta2 = vnl_math::pi/4.0 + vnl_math::pi/3.0; // E2

  x2 = vcl_cos(vnl_math::pi/3.0) - vcl_sin(vnl_math::pi/3);
  y2 = vcl_sin(vnl_math::pi/3.0) + vcl_cos(vnl_math::pi/3);

  dbgl_distance::projected_distance(x1,y1, x2,y2,theta2, &de12, &dn12, &dt12);

  vcl_cout << "Testing d(E1,E2)" << vcl_endl;
  TEST_NEAR("Edge distance",de12,vcl_sqrt(2.0),tol);
  TEST_NEAR("Normal distance",dn12,0,tol);
  TEST_NEAR("Tangential distance",dt12,vcl_sqrt(2.0),tol);

  dbgl_distance::projected_distance(x2,y2, x1,y1, theta1, &de21, &dn21, &dt21);
  vcl_cout << "Testing d(E2,E1)" << vcl_endl;
  TEST_NEAR("Edge distance",de21,vcl_sqrt(2.0),tol);
  TEST_NEAR("Normal distance",dn21,1,tol);
  TEST_NEAR("Tangential distance",dt21,1,tol);
  }

}



//: Test functions of biarc class
MAIN( test_distance )
{
  START (" Test dbgl_distance class functions");
  //test_distance_point_point();
  test_distance_lineseg_lineseg();
  test_edge_direction_distance();
  SUMMARY();
}
