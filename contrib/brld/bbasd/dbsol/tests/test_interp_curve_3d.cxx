#include <testlib/testlib_test.h>

#include <dbgl/dbgl_eno_curve_3d.h>
#include <dbsol/dbsol_interp_curve_3d.h>

#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>

MAIN( test_interp_curve_3d )
{
  START ("Interpolated Curve 3d ");

  // In parallel with test_eno_curve_3d test case
  // x(t) = 1 - 2t - 2t^2 + t^3
  // y(t) = -1 + t + t^2 - 2t^3
  // z(t) = 2 + 2t - t^2 + t^3

  double tolerance = 10e-3;

  vcl_vector<double> coefs_x;
  coefs_x.push_back(1);
  coefs_x.push_back(-2);
  coefs_x.push_back(-2);
  coefs_x.push_back(1);
  vcl_vector<double> coefs_y;
  coefs_y.push_back(-1);
  coefs_y.push_back(1);
  coefs_y.push_back(1);
  coefs_y.push_back(-2);
  vcl_vector<double> coefs_z;
  coefs_z.push_back(2);
  coefs_z.push_back(2);
  coefs_z.push_back(-1);
  coefs_z.push_back(1);

  vcl_vector<dbgl_param_curve_3d *> ints(2);
  ints[0] = new dbgl_eno_curve_3d(coefs_x, coefs_y, coefs_z, 0.0, 1.0);
  ints[1] = new dbgl_eno_curve_3d(coefs_x, coefs_y, coefs_z, 1.0, 2.0);

  dbsol_interp_curve_3d c(ints);

  TEST("size test: ", c.size(), 2);

  TEST_NEAR("length test: ", c.length(), 15.719, tolerance);
  TEST_NEAR("length test: ", c.length_at(0), 0, tolerance);
  TEST_NEAR("length test: ", c.length_at(1), 3.81355068983072, tolerance);
  TEST_NEAR("length test: ", c.length_at(2), 15.71920841667880, tolerance);

  TEST_NEAR("point test: ", c.point_at(0)->x(), 1, tolerance);
  TEST_NEAR("point test: ", c.point_at(0)->y(), -1, tolerance);
  TEST_NEAR("point test: ", c.point_at(0)->z(), 2, tolerance);

  TEST_NEAR("point test: ", c.point_at(3.81355068983072)->x(), -2, tolerance);
  TEST_NEAR("point test: ", c.point_at(3.81355068983072)->y(), -1, tolerance);
  TEST_NEAR("point test: ", c.point_at(3.81355068983072)->z(), 4, tolerance);

  TEST_NEAR("point test: ", c.point_at(15.71920841667880)->x(), -3, tolerance);
  TEST_NEAR("point test: ", c.point_at(15.71920841667880)->y(), -11, tolerance);
  TEST_NEAR("point test: ", c.point_at(15.71920841667880)->z(), 10, tolerance);

  TEST_NEAR("point at sample test: ", c.point_at_sample(0)->x(), 1, tolerance);
  TEST_NEAR("point at sample test: ", c.point_at_sample(0)->y(), -1, tolerance);
  TEST_NEAR("point at sample test: ", c.point_at_sample(0)->z(), 2, tolerance);

  TEST_NEAR("point at sample test: ", c.point_at_sample(1)->x(), -2, tolerance);
  TEST_NEAR("point at sample test: ", c.point_at_sample(1)->y(), -1, tolerance);
  TEST_NEAR("point at sample test: ", c.point_at_sample(1)->z(), 4, tolerance);

  TEST_NEAR("point at sample test: ", c.point_at_sample(2)->x(), -3, tolerance);
  TEST_NEAR("point at sample test: ", c.point_at_sample(2)->y(), -11, tolerance);
  TEST_NEAR("point at sample test: ", c.point_at_sample(2)->z(), 10, tolerance);

  TEST_NEAR("tangent test: ", c.tangent_at(3.81355068983072).x(), -0.57735, tolerance);
  TEST_NEAR("tangent test: ", c.tangent_at(3.81355068983072).y(), -0.57735, tolerance);
  TEST_NEAR("tangent test: ", c.tangent_at(3.81355068983072).z(), 0.57735, tolerance);

  TEST_NEAR("tangent angles test: ", c.tangent_angles_at(3.81355068983072).phi_, 0.95531661812451, tolerance);
  TEST_NEAR("tangent angles test: ", c.tangent_angles_at(3.81355068983072).theta_, 5*(vnl_math::pi)/4, tolerance);

  TEST_NEAR("tangent angles test: ", c.tangent_angles_at(c.length()).phi_, 1.08858566004430, tolerance);
  TEST_NEAR("tangent angles test: ", c.tangent_angles_at(c.length()).theta_, 4.81726591911492, tolerance);

  TEST_NEAR("curvature test: ", c.curvature_at(c.length_at(1)), 0.31426, tolerance);

  TEST_NEAR("torsion test: ", c.torsion_at(c.length_at(1)), 0.05555556, tolerance);
  
  SUMMARY();
}



#if 0

// ##############################################################
// Ming: old code temp here.
//In order to use bsol_intrinsic_curve_3d,
//we need vbl (smart pointer), vul (time stamp), vsol, bsol

#include "bsol/bsol_intrinsic_curve_3d.h"
//#include "bsol/bsol_intrinsic_curve_2d.h"

void test_3d_curve_old ()
{
  char s[1024];

  bsol_intrinsic_curve_3d curve3d;
  curve3d.add_vertex (27, 9.6433520000000001, 0);
  curve3d.add_vertex (29, 9.5220929999999999, 0);
  curve3d.add_vertex (33, 9.9859190000000009, 0);
  curve3d.add_vertex (32.824621999999998, 11, 0);
  curve3d.add_vertex (35, 11.429949000000001, 0);
  curve3d.add_vertex (37, 10.702358, 0);
  curve3d.add_vertex (39, 10.5, 0);
  curve3d.add_vertex (45, 10.780542, 0);
  curve3d.add_vertex (42.463413, 13, 0);
  //curve3d.computeProperties_old ();

  vcl_cout<< "\ntest 3d polyline old:\n";

  //number of vertices: size()
  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "v[%d]= (%lf, %lf, %lf), s[%d]=%lf, arcLength[%d]= %lf\n", i, curve3d.x(i), curve3d.y(i), curve3d.z(i),
          i, curve3d.s(i), i, curve3d.arcLength(i)); vcl_cout<< s;
  }

  //first point:
  sprintf (s, "p0= (%lf, %lf, %lf)\n", curve3d.p0()->x(), curve3d.p0()->y(), curve3d.p0()->z()); vcl_cout<< s;
  //last point:
  sprintf (s, "p1= (%lf, %lf, %lf)\n", curve3d.p1()->x(), curve3d.p1()->y(), curve3d.p1()->z()); vcl_cout<< s;

  //arc length:
  sprintf (s, "length= %lf\n", curve3d.length()); vcl_cout<< s;
  sprintf (s, "totalCurvature= %lf\n", curve3d.totalCurvature()); vcl_cout<< s;
  sprintf (s, "totalAngleChange= %lf\n", curve3d.totalAngleChange()); vcl_cout<< s;

  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "k[%d]= %lf, theta[%d]= %lf, norm. arcLength[%d]= %lf\n", 
          i, curve3d.curvature(i), i, curve3d.theta(i), i, curve3d.normArcLength(i)); vcl_cout<< s;
  }

  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "T[%d]= (%lf, %lf, %lf), N[%d]= (%lf, %lf, %lf)\n", 
          i, curve3d.Tangent(i)->x(), curve3d.Tangent(i)->y(), curve3d.Tangent(i)->z(),
          i, curve3d.Normal(i)->x(), curve3d.Normal(i)->y(), curve3d.Normal(i)->z()); vcl_cout<< s;
  }
  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "B[%d]= (%lf, %lf, %lf)\n", 
          i, curve3d.Binormal(i)->x(), curve3d.Binormal(i)->y(), curve3d.Binormal(i)->z()); vcl_cout<< s;
  }

  curve3d.SaveCON3File ("qqq.con3");
  vcl_cout<<"\n\n";
}

void test_3d_curve ()
{
  char s[1024];

  bsol_intrinsic_curve_3d curve3d;
  curve3d.LoadCON3File ("qqq.con3");

  vcl_cout<< "\ntest 3d polyline:\n";

  //number of vertices: size()
  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "v[%d]= (%lf, %lf, %lf), s[%d]=%lf, arcLength[%d]= %lf\n", i, curve3d.x(i), curve3d.y(i), curve3d.z(i),
          i, curve3d.s(i), i, curve3d.arcLength(i)); vcl_cout<< s;
  }

  //first point:
  sprintf (s, "p0= (%lf, %lf, %lf)\n", curve3d.p0()->x(), curve3d.p0()->y(), curve3d.p0()->z()); vcl_cout<< s;
  //last point:
  sprintf (s, "p1= (%lf, %lf, %lf)\n", curve3d.p1()->x(), curve3d.p1()->y(), curve3d.p1()->z()); vcl_cout<< s;

  //Global properties:
  sprintf (s, "length= %lf\n", curve3d.length()); vcl_cout<< s;
  sprintf (s, "totalCurvature= %lf\n", curve3d.totalCurvature()); vcl_cout<< s;
  sprintf (s, "totalAngleChange= %lf\n", curve3d.totalAngleChange()); vcl_cout<< s;

  //arc length
  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "s[%d]= %lf, arcLength[%d]= %lf, normalized arcLength[%d]= %lf\n", 
          i, curve3d.s(i), 
          i, curve3d.arcLength(i), 
          i, curve3d.normArcLength(i)); vcl_cout<< s;
  }

  //phi and derivatives
  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "phi[%d]= %lf, phi_s[%d]= %lf, phi_ss[%d]= %lf\n", 
          i, curve3d.phi(i), 
          i, curve3d.phis(i), 
          i, curve3d.phiss(i)); vcl_cout<< s;
  }

  //theta and derivatives
  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "theta[%d]= %lf, theta_s[%d]= %lf, theta_ss[%d]= %lf\n", 
          i, curve3d.theta(i), 
          i, curve3d.thetas(i), 
          i, curve3d.thetass(i)); vcl_cout<< s;
  }

  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "T[%d]= (%lf, %lf, %lf)\n", 
          i, curve3d.Tangent(i)->x(), curve3d.Tangent(i)->y(), curve3d.Tangent(i)->z()); vcl_cout<< s;
  }
  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "N[%d]= (%lf, %lf, %lf)\n", 
          i, curve3d.Normal(i)->x(), curve3d.Normal(i)->y(), curve3d.Normal(i)->z()); vcl_cout<< s;
  }
  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "B[%d]= (%lf, %lf, %lf)\n", 
          i, curve3d.Binormal(i)->x(), curve3d.Binormal(i)->y(), curve3d.Binormal(i)->z()); vcl_cout<< s;
  }

  //curvature and torsion
  for (int i=0; i<curve3d.size(); i++) {
    sprintf (s, "curvature[%d]= %lf, torsion[%d]= %lf\n", 
          i, curve3d.curvature(i), 
          i, curve3d.torsion(i)); vcl_cout<< s;
  }

  curve3d.SaveCON3File ("qqq.con3");
}

void test_2d_curve ()
{/*
  char s[1024];

  bsol_intrinsic_curve_2d curve2d;
  //curve2d.add_vertex (1, 2);
  //curve2d.add_vertex (3, 2);
  //curve2d.add_vertex (3, 3);
  //curve2d.add_vertex (4, 4);
  //curve2d.add_vertex (5, 4);
  curve2d.add_vertex (27, 9.6433520000000001);
  curve2d.add_vertex (29, 9.5220929999999999);
  curve2d.add_vertex (33, 9.9859190000000009);
  curve2d.add_vertex (32.824621999999998, 11);
  curve2d.add_vertex (35, 11.429949000000001);
  curve2d.add_vertex (37, 10.702358);
  curve2d.add_vertex (39, 10.5);
  curve2d.add_vertex (45, 10.780542);
  curve2d.add_vertex (42.463413, 13);

  curve2d.computeProperties ();

  vcl_cout<< "\ntest 2d polyline:\n";

  //number of vertices: size()
  for (int i=0; i<curve2d.size(); i++) {
    sprintf (s, "v[%d]= (%lf, %lf), s[%d]=%lf, arcLength[%d]= %lf\n", i, curve2d.x(i), curve2d.y(i),
          i, curve2d.s(i), i, curve2d.arcLength(i)); vcl_cout<< s;
  }

  //first point:
  //sprintf (s, "p0= (%lf, %lf)\n", curve2d.p0()->x(), curve2d.p0()->y()); vcl_cout<< s;
  //last point:
  //sprintf (s, "p1= (%lf, %lf)\n", curve2d.p1()->x(), curve2d.p1()->y()); vcl_cout<< s;

  //arc length:
  sprintf (s, "length= %lf\n", curve2d.length()); vcl_cout<< s;
  sprintf (s, "totalCurvature= %lf\n", curve2d.totalCurvature()); vcl_cout<< s;
  sprintf (s, "totalAngleChange= %lf\n", curve2d.totalAngleChange()); vcl_cout<< s;

  //number ofdx
  for (int i=0; i<curve2d.size(); i++) {
    sprintf (s, "k[%d]= %lf, angle[%d]= %lf, norm. arcLength[%d]= %lf\n", 
          i, curve2d.curvature(i), i, curve2d.angle(i), i, curve2d.normArcLength(i)); vcl_cout<< s;
  }
*/
}

#endif


