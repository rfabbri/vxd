//: This is dbgl/algo/tests/test_pt_tri_dist_3d.cxx
#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <testlib/testlib_test.h>

#include <dbgl/algo/dbgl_distance.h>
#include <dbgl/algo/dbgl_intersect.h>

MAIN( test_pt_tri_dist_3d )
{
  START ("Point-to-Triangle Distance in R3");

  vcl_cout << "Test point to triangle distance in R3:" << vcl_endl;
  vgl_point_3d<double> P (0.0, 0.0, 0.0);
  vgl_point_3d<double> A (1.0, 0.0, 0.0);   
  vgl_point_3d<double> B (0.0, 1.0, 0.0);   
  vgl_point_3d<double> C (0.0, 0.0, 1.0);  

  //A complete test of dbgl_pt_fpt_tri ()
  vgl_point_3d<double> F; 
  double t, u, v; 
  bool result = dbgl_pt_fpt_tri (P, A, B, C, F, t, u, v);
  TEST("Foot point inside ABC", result, true);
  vgl_point_3d<double> F0 (0.33333333333, 0.33333333333, 0.33333333333);
  TEST_NEAR("Foot point position", vgl_distance (F, F0), 0, 1E-5);
  
  P.set (1.0, 1.0, 0.0);
  result = dbgl_pt_fpt_tri (P, A, B, C, F, t, u, v);
  TEST("Foot point outside ABC", result==false, true);

  //A complete test of dbgl_pt_tri_dist_3d ()
  A.set (0.0, 0.0, 0.0);   
  B.set (1.0, 0.0, 0.0);   
  C.set (0.0, 1.0, 0.0);  
  vgl_point_3d<double> G; 
  
  //P inside ABC
  P.set (0.3, 0.5, 0.2);
  t = dbgl_pt_tri_dist_3d (P, A, B, C, G);
  vgl_point_3d<double> G0 (0.3, 0.5, 0.0);
  TEST_NEAR("Closest point position", vgl_distance (G, G0), 0, 1E-5);
  TEST_NEAR("Closest distance", t, vgl_distance (P, G0), 1E-5);

  //P inside barycentric region of A
  P.set (-0.1, -0.2, 0.2);
  t = dbgl_pt_tri_dist_3d (P, A, B, C, G);
  G0 = A;
  TEST_NEAR("Closest point position", vgl_distance (G, G0), 0, 1E-5);
  TEST_NEAR("Closest distance", t, vgl_distance (P, G0), 1E-5);

  //P inside barycentric region of B
  P.set (1.3, -0.02, 0.2);
  t = dbgl_pt_tri_dist_3d (P, A, B, C, G);
  G0 = B;
  TEST_NEAR("Closest point position", vgl_distance (G, G0), 0, 1E-5);
  TEST_NEAR("Closest distance", t, vgl_distance (P, G0), 1E-5);
  
  //P inside barycentric region of C
  P.set (-0.4, 1.3, 0.2);
  t = dbgl_pt_tri_dist_3d (P, A, B, C, G);
  G0 = C;
  TEST_NEAR("Closest point position", vgl_distance (G, G0), 0, 1E-5);
  TEST_NEAR("Closest distance", t, vgl_distance (P, G0), 1E-5);

  //P inside barycentric region of AB
  P.set (0.4, -0.5, 0.3);
  t = dbgl_pt_tri_dist_3d (P, A, B, C, G);
  G0.set (0.4, 0, 0);
  TEST_NEAR("Closest point position", vgl_distance (G, G0), 0, 1E-5);
  TEST_NEAR("Closest distance", t, vgl_distance (P, G0), 1E-5);

  //P inside barycentric region of AC
  P.set (-0.4, 0.8, 0.4);
  t = dbgl_pt_tri_dist_3d (P, A, B, C, G);
  G0.set (0, 0.8, 0);
  TEST_NEAR("Closest point position", vgl_distance (G, G0), 0, 1E-5);
  TEST_NEAR("Closest distance", t, vgl_distance (P, G0), 1E-5);

  //P inside barycentric region of BC
  P.set (0.8, 0.6, 0.2);
  t = dbgl_pt_tri_dist_3d (P, A, B, C, G);
  G0.set (0.43431457505076210, 0.56568542494923790, 0);
  TEST_NEAR("Closest point position", vgl_distance (G, G0), 0, 1E-5);
  TEST_NEAR("Closest distance", t, vgl_distance (P, G0), 1E-5);


  SUMMARY();
}
