#include <testlib/testlib_test.h>
#include <dbnl/algo/dbnl_eno.h>
#include <dbnl/algo/dbnl_eno_measures.h>
#include <vcl_iostream.h>
#include <vcl_limits.h>

MAIN( test_eno_interp)
{
   START ("eno_interp");

   double tol = vcl_numeric_limits<double>::epsilon()*100;

   {
   dbnl_eno_interp e(1,2); // (start, end) coordinates of interval to interpolate
   double nhood[4];

   vcl_cout << "Test 1: plain interpolation" << vcl_endl;
   nhood[0] = 1;
   nhood[1] = 2;
   nhood[2] = 1;
   nhood[3] = 100; // so forward should be false

   e.interpolate(nhood);
   e.print();
   TEST("Forward",e.forward(),false);
   TEST("Start",e.start(),1);
   TEST("End",e.end(),2);

   nhood[0] = 100; // now forward should be true 
   nhood[1] = 1; nhood[2] = 2;
   nhood[3] = 1;

   e.interpolate(nhood);
   vcl_cerr << vcl_endl;
   e.print();
   TEST("Forward",e.forward(),true);

   // TODO: print the interpolated values 
   

   dbnl_eno_measures m;

   m.compute(e);
   vcl_cerr.precision(3);
   m.print();
   double k1 = m.total_curvature();

   nhood[0] = 1;
   nhood[1] = 2;
   nhood[2] = 1;
   nhood[3] = 100;

   e.interpolate(nhood);
   vcl_cerr << vcl_endl;
   e.print();  

   m.compute(e);
   m.print();

   TEST_NEAR("Curvature is invariant",k1, m.total_curvature(),tol);
   }

   /* now using non-integral abcissas */
   {
   dbnl_eno_interp e(0,0.5); // (start, end) coordinates of interval to interpolate
   double nhood[4];
   double x[4] = {-1, 0, 0.5, 2};

   vcl_cout << "Test: p(x) = 7 + 2*x + x^2\n";
   nhood[0] = 1    +7+2*x[0];
   nhood[1] = 0    +7+2*x[1];
   nhood[2] = 0.25 +7+2*x[2];
   nhood[3] = 4    +7+2*x[3];
   e.interpolate(nhood,x);
   e.print();
   TEST_NEAR("2nd order coeff",e[e.second_order_index], 1 ,tol);
   TEST_NEAR("1st order coeff",e[e.first_order_index], 2 ,tol);
   TEST_NEAR("0   order coeff",e[e.zero_order_index], 7 ,tol);
   }

   SUMMARY();
}
