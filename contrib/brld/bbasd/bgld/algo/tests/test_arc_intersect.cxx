// This is basic/dbgl/algo/tests/test_arc_algo.cxx

#include <testlib/testlib_test.h>
#include <dbgl/algo/dbgl_arc_algo.h>
#include <vgl/vgl_point_2d.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vcl_limits.h>
#include <vnl/vnl_math.h>

//: Test intersections between arcs
MAIN( test_arc_intersect )
{
   const double tolerance=vcl_numeric_limits<double>::epsilon() *1e5;

   vgl_point_2d<double> p1, p2, p3, i1,i2;

   // arc of circle with radius 3 centered at (0,0)
   p1.set(3*vcl_sqrt(2.0)/2,3*vcl_sqrt(2.0)/2);
   p2.set(3*vcl_sqrt(2.0)/2,-3*vcl_sqrt(2.0)/2);
   p3.set(3,0);
   dbgl_arc a1(p1,p2,p3);
   a1.print();


   {
      // arc of circle with radius 5 centered at (0,4)
      p1.set(4,5);
      p2.set(4,-5);
      p3.set(9,0);
      dbgl_arc a2(p1,p2,p3);

      unsigned n=dbgl_arc_algo::compute_intersections(a1,a2,&i1,&i2);
      vcl_cout << "Number: " << n << vcl_endl;
      TEST("Number",n,2);
      vcl_cout << "Intesections: (" << i1.x() << ", "<< i1.y()<<")  "  <<
                                "(" << i2.x() << ", "<< i2.y()<<")" << vcl_endl;
      TEST_NEAR("Intersection1.x",i1.x(),0,tolerance);
      TEST_NEAR("Intersection2.x",i2.x(),0,tolerance);
      TEST_NEAR("Intersection1.y",vnl_math::max(i1.y(),i2.y()), 3,tolerance);
      TEST_NEAR("Intersection2.y",vnl_math::min(i1.y(),i2.y()),-3,tolerance);
   }

   {

   // center (-4,0), radius 1, tangential to the arc a1
   p1.set(-4,1);
   p2.set(-4,-1);
   p3.set(-5,0);
   dbgl_arc a2(p1,p2,p3);
   a2.print();

   unsigned n=dbgl_arc_algo::compute_intersections(a1,a2,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,1);
   vcl_cout << "Intesection: (" << i1.x() << ", "<< i1.y()<<")  " << vcl_endl;
   TEST_NEAR("Intersection1.x",i1.x(),-3,tolerance);
   TEST_NEAR("Intersection1.y",i1.y(),0,tolerance);
   }

   {
   // center (0,4), radius 1, tangential to the arc a1
   p1.set(-1,4);
   p2.set(1,4);
   p3.set(0,5);
   dbgl_arc a2(p1,p2,p3);
   a2.print();

   unsigned n=dbgl_arc_algo::compute_intersections(a1,a2,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,1);
   vcl_cout << "Intesection: (" << i1.x() << ", "<< i1.y()<<")  " << vcl_endl;
   TEST_NEAR("Intersection1.x",i1.x(),0,tolerance);
   TEST_NEAR("Intersection1.y",i1.y(),3,tolerance);
   }

   {
   // center (0,2), radius 1, internally tangential to the arc a1
   p1.set(-1,2);
   p2.set(1,2);
   p3.set(0,1);
   dbgl_arc a2(p1,p2,p3);
   a2.print();

   i1.set(-10,-10); //< clear it
   unsigned n=dbgl_arc_algo::compute_intersections(a1,a2,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,1);
   vcl_cout << "Intesection: (" << i1.x() << ", "<< i1.y()<<")  " << vcl_endl;
   TEST_NEAR("Intersection1.x",i1.x(),0,tolerance);
   TEST_NEAR("Intersection1.y",i1.y(),3,tolerance);
   }

   {
   // coinciding circles
   i1.set(-10,-10); //< clear it
   unsigned n=dbgl_arc_algo::compute_intersections(a1,a1,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,3);
   }

   {
   // zero-curvature (line), two intersections
   p1.set(-4,-4);
   p2.set(4,4);
   p3.set(-5,-5);
   dbgl_arc a2(p1,p2,p3);
   a2.print();

   i1.set(-10,-10); //< clear it
   unsigned n=dbgl_arc_algo::compute_intersections(a1,a2,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,2);
   vcl_cout << "Intesections: (" << i1.x() << ", "<< i1.y()<<")  "  <<
                             "(" << i2.x() << ", "<< i2.y()<<")" << vcl_endl;
   TEST_NEAR("Intersection1.x",i1.x(),a1.point_at(0).x(),tolerance);
   TEST_NEAR("Intersection1.y",i1.y(),a1.point_at(0).y(),tolerance);
   TEST_NEAR("Intersection2.x",i2.x(),a1.point_at_length((2.0/3.0)*a1.length()).x(),tolerance);
   TEST_NEAR("Intersection2.y",i2.y(),a1.point_at_length((2.0/3.0)*a1.length()).y(),tolerance);
   }

   {
   // zero-curvature (line), tangent to arc
   double delta = 3/sqrt(2.0);
   p1.set(-4-delta,-4+delta);
   p2.set(4-delta,4+delta);
   p3.set(-5-delta,-5+delta);
   dbgl_arc a2(p1,p2,p3);
   a2.print();

   i1.set(-10,-10); //< clear it
   unsigned n=dbgl_arc_algo::compute_intersections(a1,a2,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,1);
   vcl_cout << "Intesection: (" << i1.x() << ", "<< i1.y()<<")  " << vcl_endl;
   TEST_NEAR("Intersection1.x",i1.x(),-delta,tolerance);
   TEST_NEAR("Intersection1.y",i1.y(),delta,tolerance);
   }

   {
   // zero-curvature (line), non-intersecting
   double delta = 100;
   p1.set(-4-delta,-4+delta);
   p2.set(4-delta,4+delta);
   p3.set(-5-delta,-5+delta);
   dbgl_arc a2(p1,p2,p3);

   unsigned n=dbgl_arc_algo::compute_intersections(a1,a2,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,0);
   }

   // ------------ vertical line cases --------------
   //

   {
   // arc of circle with radius 3 centered at (-10,200)

   p1.set(3*vcl_sqrt(2.0)/2-10,3*vcl_sqrt(2.0)/2 +200);
   p2.set(3*vcl_sqrt(2.0)/2-10,-3*vcl_sqrt(2.0)/2 +200);
   p3.set(3-10,0+200);
   dbgl_arc a3(p1,p2,p3);

   // zero-curvature (line), tangent to arc
   p1.set(-3 -10,-4 + 200);
   p2.set(-3 -10, 4 + 200);
   p3.set(-3 -10, 5 + 200);
   dbgl_arc a2(p1,p2,p3);
   a2.print();

   i1.set(-10,-10); //< clear it
   unsigned n=dbgl_arc_algo::compute_intersections(a2,a3,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,1);
   vcl_cout << "Intesection: (" << i1.x() << ", "<< i1.y()<<")  " << vcl_endl;
   TEST_NEAR("Intersection1.x",i1.x(),-3 -10,tolerance);
   TEST_NEAR("Intersection1.y",i1.y(),0  +200,tolerance);
   }

   {
   // arc of circle with radius 3 centered at (-10,200)

   p1.set(3*vcl_sqrt(2.0)/2-10,3*vcl_sqrt(2.0)/2 +200);
   p2.set(3*vcl_sqrt(2.0)/2-10,-3*vcl_sqrt(2.0)/2 +200);
   p3.set(3-10,0+200);
   dbgl_arc a3(p1,p2,p3);

   // zero-curvature (line), secant to arc
   p1.set(0 -10,-4 +200);
   p2.set(0 -10, 4 +200);
   p3.set(0 -10, 5 +200);
   dbgl_arc a2(p1,p2,p3);
   a2.print();

   i1.set(-10,-10); //< clear it
   unsigned n=dbgl_arc_algo::compute_intersections(a2,a3,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,2);
   vcl_cout << "Intesections: (" << i1.x() << ", "<< i1.y()<<")  "  <<
                             "(" << i2.x() << ", "<< i2.y()<<")" << vcl_endl;
   TEST_NEAR("Intersection1.x",i1.x(),0-10,tolerance);
   TEST_NEAR("Intersection1.y",vnl_math::max(i1.y(),i2.y()),3+200,tolerance);
   TEST_NEAR("Intersection2.x",i2.x(),0-10,tolerance);
   TEST_NEAR("Intersection2.y",vnl_math::min(i1.y(),i2.y()),-3+200,tolerance);
   }

   {
   // arc of circle with radius 3 centered at (-10,200)

   p1.set(-3*vcl_sqrt(2.0)/2-10,+3*vcl_sqrt(2.0)/2 +200);
   p2.set(-3*vcl_sqrt(2.0)/2-10,-3*vcl_sqrt(2.0)/2 +200);
   p3.set(-3-10,0+200);
   dbgl_arc a3(p1,p2,p3);

   // zero-curvature (line), tangent to arc
   p1.set(3 -10,-4 + 200);
   p2.set(3 -10, 4 + 200);
   p3.set(3 -10, 5 + 200);
   dbgl_arc a2(p1,p2,p3);
   a2.print();

   i1.set(-10,-10); //< clear it
   unsigned n=dbgl_arc_algo::compute_intersections(a2,a3,&i1,&i2);
   vcl_cout << "Number: " << n << vcl_endl;
   TEST("Number",n,1);
   vcl_cout << "Intesection: (" << i1.x() << ", "<< i1.y()<<")  " << vcl_endl;
   TEST_NEAR("Intersection1.x",i1.x(),3 -10,tolerance);
   TEST_NEAR("Intersection1.y",i1.y(),0  +200,tolerance);

   }


   SUMMARY();
}
