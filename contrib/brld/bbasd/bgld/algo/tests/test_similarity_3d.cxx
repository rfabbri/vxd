// Test dbgl_similarity_3d


#include <testlib/testlib_test.h>
#include <dbgl/algo/dbgl_similarity_3d.h>
#include <vgl/vgl_plane_3d.h>
#include <vgl/vgl_homg_line_3d_2_points.h>
#include <vgl/vgl_line_3d_2_points.h>
#include <vgl/vgl_line_segment_3d.h>
#include <vgl/vgl_distance.h>
#include <vnl/vnl_double_4.h>
#include <vnl/vnl_double_4x4.h>
#include <vnl/vnl_random.h>
#include <vnl/vnl_matrix_exp.h>

static const double epsilon = 1e-11;



// Test that the inverse similarity works as expected
static void test_inverse(const dbgl_similarity_3d<double>& sim)
{
  dbgl_similarity_3d<double> simI = sim * sim.inverse();
  vnl_matrix_fixed<double,4,4> I; I.set_identity();
  double diff = (simI.as_matrix() - I).frobenius_norm();
  TEST_NEAR("Inverse similarity", diff, 0.0, epsilon);
}


// Test application of similarity to other vgl objects
static void test_application(const dbgl_similarity_3d<double>& sim)
{
  vnl_double_4x4 S = sim.as_matrix();

  vgl_homg_point_3d<double> hpt(100,50,200,2);
  vnl_double_4 p(hpt.x(),hpt.y(),hpt.z(),hpt.w());

  vgl_homg_plane_3d<double> hpl(0,1,0,100);

  vgl_homg_point_3d<double> s_hpt = sim*hpt;
  vnl_double_4 s_p = S*p;

  vgl_homg_plane_3d<double> s_hpl = sim*hpl;

  TEST_NEAR("Transformed point-plane dist (homg)", sim.scale()*vgl_distance(hpt,hpl),
            vgl_distance(s_hpt, s_hpl), epsilon);

  double diff = (s_p - vnl_double_4(s_hpt.x(),s_hpt.y(),
                                    s_hpt.z(),s_hpt.w())).magnitude();
  TEST_NEAR("Transformed point (homg)", diff, 0.0, epsilon);

  vgl_point_3d<double> pt(-30, 0, 75);
  vgl_point_3d<double> s_pt = sim*pt;
  vnl_double_4 s_pt2h = S*vnl_double_4(pt.x(), pt.y(), pt.z(), 1);
  vnl_double_3 s_pt2(s_pt2h[0]/s_pt2h[3],s_pt2h[1]/s_pt2h[3],s_pt2h[2]/s_pt2h[3]);

  diff = (s_pt2 - vnl_double_3(s_pt.x(),s_pt.y(), s_pt.z())).magnitude();
  TEST_NEAR("Transformed point", diff, 0.0, epsilon);

  vgl_plane_3d<double> pl(0,0,1,50);
  vgl_plane_3d<double> s_pl = sim*pl;
  TEST_NEAR("Transfomed point-plane dist", sim.scale()*vgl_distance(pt,pl),
            vgl_distance(s_pt, s_pl), epsilon);

  vgl_vector_3d<double> v= vgl_point_3d<double>(hpt) - pt;
  vgl_vector_3d<double> s_v = sim*v;
  vgl_vector_3d<double> s_v2 = vgl_point_3d<double>(s_hpt) - s_pt;
  TEST_NEAR("Transformed vector", (s_v-s_v2).length(), 0.0, epsilon);

  vgl_homg_line_3d_2_points<double> hl(hpt, vgl_homg_point_3d<double>(v.x(),v.y(),v.z(),0));
  vgl_homg_line_3d_2_points<double> s_hl = sim*hl;
  vcl_cout << "Transformed hl = " << s_hl << vcl_endl;
  //FIXME  add test

  vgl_line_3d_2_points<double> l(pt, pt+v);
  vgl_line_3d_2_points<double> s_l = sim*l;
  vcl_cout << "Transformed l = " << s_l << vcl_endl;
  //FIXME  add test

  vgl_line_segment_3d<double> s(pt, pt+v);
  vgl_line_segment_3d<double> s_s = sim*s;
  vcl_cout << "Transformed s = " << s_s << vcl_endl;
  //FIXME  add test
}

// Test Lie algebra operations
static void test_lie(const dbgl_similarity_3d<double>& sim)
{
  vnl_vector_fixed<double,7> a = sim.lie_algebra_basis();
  // construct from lie algebra basis
  dbgl_similarity_3d<double> sim2(a);
  TEST_NEAR("Construct from Lie algebra basis",
            (sim.as_matrix() - sim2.as_matrix()).frobenius_norm(), 0.0, epsilon);
  vcl_cout << "basis <"<<sim.lie_algebra_basis()<<">\n" << vcl_endl;
  vcl_cout << "reformed "<<sim2.as_matrix()<<"\n" << vcl_endl;

  TEST_NEAR("Exponental of Lie algebra basis",
            (sim.as_matrix() - vnl_matrix_exp(sim.lie_algebra())).frobenius_norm(),
             0.0, epsilon);

  vnl_double_3x3 A = sim.lie_exp_t_matrix();
  vnl_double_3 t = A * sim.lie_algebra_basis().extract(3,4);
  vgl_vector_3d<double> v = sim.translation();
  TEST_NEAR("Exponental Matrix for translation",
            (t - vnl_double_3(v.x(),v.y(),v.z())).magnitude(),
             0.0, epsilon);

  vnl_double_3x3 Inv = sim.lie_exp_t_matrix()*sim.lie_log_t_matrix();
  vnl_double_3x3 I; I.set_identity();
  TEST_NEAR("Logarithm Matrix for translation",
            (Inv-I).frobenius_norm(),
             0.0, epsilon);
}

void test_similarity_3d()
{
  vcl_cout << "***************************\n"
           << " Testing dbgl_similarity_3d\n"
           << "***************************\n";


  {
    vcl_cout << "\n1. 2x scale, rotation about the x axis over 90 degrees, translation (1,-2,3).\n";
    vgl_rotation_3d<double> rot(1.57079632679489661923, 0.0, 0.0);
    vgl_vector_3d<double> t(1.0, -2.0, 3.0);
    dbgl_similarity_3d<double> sim(2.0,rot,t);
    //test_conversions(sim);
    test_inverse(sim);
    test_application(sim);
    test_lie(sim);
  }

  {
    vcl_cout << "\n2. no scale, rotation about the x axis over 90 degrees, translation (1,-2,3).\n";
    vgl_rotation_3d<double> rot(1.57079632679489661923, 0.0, 0.0);
    vgl_vector_3d<double> t(1.0, -2.0, 3.0);
    dbgl_similarity_3d<double> sim(1.0,rot,t);
    //test_conversions(sim);
    test_inverse(sim);
    test_application(sim);
    test_lie(sim);
  }

  {
    vcl_cout << "\n3. 2x scale, no rotation, translation (1,-2,3).\n";
    vgl_rotation_3d<double> rot;
    vgl_vector_3d<double> t(1.0, -2.0, 3.0);
    dbgl_similarity_3d<double> sim(2.0,rot,t);
    //test_conversions(sim);
    test_inverse(sim);
    test_application(sim);
    test_lie(sim);
  }

  {
    vcl_cout << "\n4. no scale, no rotation, translation (1,-2,3).\n";
    vgl_rotation_3d<double> rot;
    vgl_vector_3d<double> t(1.0, -2.0, 3.0);
    dbgl_similarity_3d<double> sim(1.0,rot,t);
    //test_conversions(sim);
    test_inverse(sim);
    test_application(sim);
    test_lie(sim);
  }

  {
    vnl_random rnd;
    vgl_rotation_3d<double> rot(rnd.normal(), rnd.normal(), rnd.normal());
    vgl_vector_3d<double> t(rnd.normal(), rnd.normal(), rnd.normal());
    dbgl_similarity_3d<double> sim(rnd.drand32()*10,rot,t);
    vcl_cout << "\n5. Random Similarity: " << sim << vcl_endl;
    //test_conversions(sim);
    test_inverse(sim);
    test_application(sim);
    test_lie(sim);
  }

}

TESTMAIN(test_similarity_3d);
