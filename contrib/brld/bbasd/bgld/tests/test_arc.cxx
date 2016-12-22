#include <testlib/testlib_test.h>
#include <dbgl/dbgl_arc.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>

bool almost_equal(vgl_point_2d<double> a, vgl_point_2d<double> b) {
  return (vcl_fabs(a.x()-b.x()) < dbgl_arc::near_zero_value &&
          vcl_fabs(a.y()-b.y()) < dbgl_arc::near_zero_value);
}

MAIN( test_arc )
{
  START ("ARC ");

  double tolerance=dbgl_arc::near_zero_value;

  vcl_cout << "construct upper hemisphere of unit circle:" << vcl_endl;
  vgl_point_2d<double> start(1.0, 0.0);  // 0
  vgl_point_2d<double> end(-1.0, 0.0);   // pi
  vgl_point_2d<double> other(0.0, -1.0); // 3*pi/2
  vgl_point_2d<double> center(0.0, 0.0);  // 0
  dbgl_arc a(start, end, other);
  TEST("curvature() ", a.curvature(), 1.0);
  TEST("center() ", a.center() == center, true);
  TEST("start_angle() ", vcl_fabs(a.start_angle()-0.0)<tolerance, true);
  TEST("end_angle() ", vcl_fabs(a.end_angle()-PI)<tolerance, true);
  TEST("orientation() ", a.orientation(), 1);  // counterclockwise
  double len = a.length();
  TEST("length() ", len, PI); 

  TEST("point_at() start", almost_equal(a.point_at(0.0), start), true);
  TEST("point_at() end", almost_equal(a.point_at(1), end), true);

  TEST("tangent_angle_at() start", vcl_fabs(a.tangent_angle_at(0.0)-PIOVERTWO)<tolerance, true);
  TEST("tangent_angle_at() end", vcl_fabs(a.tangent_angle_at(1)-3*PIOVERTWO)<tolerance, true);

  //: check mid point 
  vgl_point_2d<double> mid(0.0, 1.0);  // pi/2

  TEST("point_at_length() mid", almost_equal(a.point_at_length(0.5*a.length()), mid), true);  
  
  // Pedagogical remark: the following is not necessarily right, as explained in doc:
  //
  // TEST("point_at() mid", almost_equal(a.point_at(0.5), mid), true);  

  //: mid point of the arc is pi/2, check for its angle
  TEST("tangent_angle_at() mid", 
      vcl_fabs(a.tangent_angle_at_length(0.5*a.length())-PI)<tolerance, true);

  vgl_vector_2d<double> midtan = a.tangent_at_length(0.5*a.length());

  //vcl_cout << "tangent vector at mid point: " << midtan << vcl_endl;
  TEST("tangent_at_length() mid", vcl_fabs(atan2(midtan.y(),midtan.x())-PI)<tolerance, true);
  //vcl_cout << "tangent vector at mid point angle: " << atan2(midtan.y(),midtan.x()) << vcl_endl;
  
  dbgl_arc b(start, end, other);
  TEST("Operator == ", a == b, true);

  dbgl_arc c(a);
  TEST("Copy Constructor ", a == c, true);

  //: construct upper hemisphere in reverse orientation
  vcl_cout << "Testing arc in reverse direction\n";
  dbgl_arc d(end, start, other);
  TEST("reverse arc curvature() ", d.curvature(), 1.0);
  TEST("reverse arc center() ", d.center() == center, true);
  TEST("reverse arc start_angle() ", vcl_fabs(d.start_angle()-PI)<tolerance, true);
  TEST("reverse arc end_angle() ", vcl_fabs(d.end_angle()-0.0)<tolerance, true);
  TEST("reverse arc orientation() ", d.orientation(), -1);  // clockwise
  TEST("arc vs reverse arc equality ", c == d, false);
  TEST("reverse arc tangent_angle_at() ", 
      vcl_fabs(a.tangent_angle_at_length((1.0/4.0)*a.length())-3.0*PI/4.0)<tolerance, true);

  midtan = a.tangent_at_length((1.0/4.0)*a.length());
  TEST("reverse arc tangent_at() ", vcl_fabs(atan2(midtan.y(),midtan.x())-3.0*PI/4.0)<tolerance, true);

  vcl_cout << "try to construct an arc from linear points: " << vcl_endl;
  dbgl_arc e(start, end, center);
  TEST("collinearity ", e.curvature()==0.0, true);

  //: construct arc at center (1,1) 
  start.set(1.0,-1.0);
  end.set(-1.0,1.0);
  other.set(1.0,3.0);
  center.set(1.0,1.0);
  dbgl_arc f(start, end, other);
  //vcl_cout << "f center " << f.center() << vcl_endl;
  //vcl_cout << "f curv " << f.curvature() << vcl_endl;
  TEST("third arc curvature() ", vcl_fabs(f.curvature()-1.0/2.0) < tolerance, true);
  TEST("third arc center() ", almost_equal(f.center(),center), true);
  TEST("third arc orientation() ", f.orientation(), -1); //cw
  TEST("third arc length() ", f.length(), (PI/2.0)*2.0); 


  //: Zero-curvature arc (line segment)

  // zero-curvature (line)
  start.set(-4,-4);
  end.set(4,4);
  other.set(-5,-5);
  dbgl_arc a2(start,end,other);
  a2.print();
  //: center has to be midpoint of segment in the convention of the class
  TEST_NEAR("CenterX",a2.center().x(),0, tolerance);
  TEST_NEAR("CenterY",a2.center().y(),0, tolerance);
  TEST_NEAR("Length", a2.length(), 8*vcl_sqrt(2.0), tolerance);
  
  SUMMARY();
}
