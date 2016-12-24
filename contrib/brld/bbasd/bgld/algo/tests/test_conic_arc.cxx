// This is basic/dbgl/algo/tests/test_circ_arc.cxx

// \author Nhon Trinh
// \date Aug 18, 2005

#include <testlib/testlib_test.h>
#include <dbgl/algo/dbgl_conic_arc.h>
#include <vcl_cmath.h>


//: test functions of dbgl_circ_arc class
void test_conic_arc_functions()
{
  vcl_cout << "In test_conic_arc_functions()\n";
  const double epsilon = 1e-10;

  // constructing a parabola
  // y = 3 x^2
  // y' = 6 x
  // y'' = 6;
  // k = y'' / (1+y'^2)3/2
  {
    dbgl_conic_arc conic;
    conic.set_from(vgl_point_2d<double >(0, 0), vgl_point_2d<double >(2, 12),
      vgl_vector_2d<double >(1, 0), vgl_vector_2d<double >(1, 12),
      6);

    bool success = true;

    for (double t=0; t<=1; t = t+0.1)
    {
      vgl_point_2d<double > pt = conic.point_at(t);
      //vcl_cout << "point_at ( " << t << " )= " << pt << vcl_endl;
      success = success && (vcl_abs(pt.y() - 3*pt.x()*pt.x()) < epsilon);

      vgl_vector_2d<double > tangent = conic.tangent_at(t);
      success = success && 
        (vcl_abs(vcl_atan2(tangent.y(), tangent.x())-vcl_atan2(6.0*pt.x(), 1.0)) < epsilon);
    }
    TEST("Test parabola y=3*x^2", success, true);
  }


  // Same, but constructing from a vgl_conic_segment_2d.

  {
    // Parabola y = 3x^2
    dbgl_conic_arc conic;
    vgl_conic<double> vc(3, 0, 0, 0, -1, 0);

    vgl_conic_segment_2d<double> 
      vc_segment(vgl_point_2d<double >(0, 0), vgl_point_2d<double >(2, 12), vc);

    bool retval = conic.set_from(vc_segment);

    Assert("Construct from vgl successful", retval);

    assert(retval);


    // Endpoints
    {
    vgl_point_2d<double > pt = conic.point_at(0);
    TEST_NEAR("Construct from vgl, point_at", pt.x(), 0, epsilon);
    TEST_NEAR("Construct from vgl, point_at", pt.y(), 0, epsilon);
    }

    {
    vgl_point_2d<double > pt = conic.point_at(1);
    TEST_NEAR("Construct from vgl, point_at", pt.x(), 2, epsilon);
    TEST_NEAR("Construct from vgl, point_at", pt.y(), 12, epsilon);
    }

    bool success = true;
    for (double t=0.1; t<1; t = t+0.1)
    {
      vgl_point_2d<double > pt = conic.point_at(t);
      TEST_NEAR("Construct from vgl - parabola y=3*x^2, point_at", 
          pt.y(), 3*pt.x()*pt.x(), epsilon);

      vgl_vector_2d<double > tangent = conic.tangent_at(t);
      success = success && 
        (vcl_abs(vcl_atan2(tangent.y(), tangent.x())-vcl_atan2(6.0*pt.x(), 1.0)) < epsilon);
    }
    TEST("Construct from vgl - parabola y=3*x^2", success, true);
  }


  // constructing ellipse
  // (x-10)^2 + (y-20)^2/4 = 1;
  // x = cos u + 10
  // y = 2 sin u + 20
  // dx/du = -sin u;
  // dy / du = 2 * cos u
  // k = (x' * y'' - y' * x'') / (x' ^2 + y' ^2)^(3/2)

  // u1 = pi / 6
  // x(u1) = sqrt(3)/2 + 10
  // y(u1) = 21
  // x'(u1) = -1/2
  // y'(u1) = sqrt(3)
  // k(u1) = 16 * sqrt(13) / 169

  // u2 = 3 * pi/4
  // x(u2) = -sqrt(2)/2 + 10;
  // y(u2) = sqrt(2) + 20;
  // x'(u2) = -sqrt(2)/2
  // y'(u2) = -sqrt(2)
  // k(u2) = 4 * sqrt(10) / 25

  vgl_point_2d<double > start(vcl_sqrt(3.0)/2 + 10, 21);
  vgl_point_2d<double > end (-vcl_sqrt(2.0)/2 + 10, vcl_sqrt(2.0) + 20);
  vgl_vector_2d<double > start_tangent(-1.0/2, vcl_sqrt(3.0));
  vgl_vector_2d<double > end_tangent(-vcl_sqrt(2.0)/2, -vcl_sqrt(2.0));
  double start_k = 16 * vcl_sqrt(13.0) / 169;

  dbgl_conic_arc conic;
  conic.set_from(start, end, start_tangent, end_tangent, start_k);

  
  vcl_cout << "\nstart_k = " << start_k
    << "\nconic.curvature_at(0) = " << conic.curvature_at(0) << vcl_endl;
  TEST_NEAR("conic curvature", conic.curvature_at(0), start_k, 1e-8);


  bool success = true;

  for (double t=0; t<=1; t = t+0.1)
  {
    vgl_point_2d<double > pt = conic.point_at(t);
    //vcl_cout << "point_at ( " << t << " )= " << pt << vcl_endl;
    double error = (pt.x()-10)*(pt.x()-10) + (pt.y()-20)*(pt.y()-20)/4-1;
    //vcl_cout << "error = " << error << vcl_endl;
    success = success && (error < epsilon);
  }
  TEST("Test ellipse (x-10)^2 + (y-20)^2/4 = 1", success, true);

  // test super generate case for conic arc: when the arc is just a point
  start.set(100, 100);
  end.set(100, 100);
  start_tangent.set(2, 2);
  end_tangent.set(2, 2);
  double a = 1;
  double b = 0;
  start_k = a/b;
  
  conic.set_from(start, end, start_tangent, end_tangent, start_k);
  vcl_cout << "Tangent at t=0.7 is " << conic.tangent_at(0.7) << vcl_endl;
  vcl_cout << "Point at t=0.7 is " << conic.point_at(0.7) << vcl_endl;
  
  success = ((conic.point_at(0.7)-start).length() < 1e-8) &&
    (signed_angle(conic.tangent_at(0.7), start_tangent) < 1e-8);

  TEST("Test super degenerate conic arc (a point)", success, true);
  

  vcl_cout << "End of test_conic_arc\n";
  
  
}

//: Test functions of biarc class
MAIN( test_conic_arc )
{
  START ("Test functions of dbgl_circ_arc");
  test_conic_arc_functions();
  SUMMARY();
}

