#include <testlib/testlib_test.h>
#include <dbnl/algo/dbnl_eno.h>
#include <dbnl/algo/dbnl_eno_zerox.h>
#include <dbnl/algo/dbnl_eno_zerox_label.h>
#include <vcl_iostream.h>
#include <vcl_limits.h>
#include <vcl_cmath.h>

MAIN( test_eno_zerox )
{
   START ("eno_zerox");

   double tol = vcl_numeric_limits<double>::epsilon()*1000;

   {

     vcl_cout << "\n Testing dbnl_eno_zero_crossing\n";

     dbnl_eno_poly p(2);

     //  This is just a test... the user should NOT access "data" members
     // directly

     vcl_cout << vcl_endl << "Part 1:" << vcl_endl;
     p[dbnl_eno_poly::zero_order_index]   = -4;
     p[dbnl_eno_poly::first_order_index]  =  0;
     p[dbnl_eno_poly::second_order_index] =  1;

     dbnl_eno_zero_crossing z1;
     z1.compute(p);
     z1.print();
     TEST("Quantity", z1.number(), 2);
     TEST_NEAR("Location", z1.location(0), -2, vcl_numeric_limits<double>::epsilon()*100);
     TEST_NEAR("Location", z1.location(1),  2, vcl_numeric_limits<double>::epsilon()*100);


     vcl_cout << vcl_endl << "Part 2:" << vcl_endl;

     dbnl_eno_zero_crossing z2;
     p[dbnl_eno_poly::zero_order_index]   =  0;
     p[dbnl_eno_poly::first_order_index]  =  0;
     p[dbnl_eno_poly::second_order_index] =  1000;

     // two roots equal to 0 represents one root of multiplicity 2
     z2.compute(p);
     z2.print();
     TEST("Quantity", z2.number(), 2);
     TEST_NEAR("Location", fabs(z2.location(0)), 0, vcl_numeric_limits<double>::epsilon()*100);
     TEST_NEAR("Location", fabs(z2.location(1)), 0, vcl_numeric_limits<double>::epsilon()*100);

     vcl_cout << "Part 3" << vcl_endl;
     z2.remove_duplicates();
     
     dbnl_eno_zero_crossing z;
     z.combine(&z1,&z2);
     TEST("Quantity after Combine",z.number(),3);
     z.print();
     z.sort();
     TEST("Sort order",z.location(0) <= z.location (1) && z.location(1) <= z.location(2),true);

     z.print();
   }

   {
     vcl_cout << "\n\n----------------------------------------------------------------------\n";
     vcl_cout << "Testing dbnl_eno_zerox_vector and dbnl_eno_zerox_label\n";


     // Define data
     //   - samples along two lines
     vnl_vector<double> data(7);

     // These are samples of:
     //   l(x) = line between points (0,0) and (3,5),  if x < 3
     //   l(x) = line between points (3,5) and (6,-4), if x >= 3
     //   roots: x = 0  and 14/3 ~ 4.667
     //
     data(0) = 0;
     double a=5.0/3.0;
     data(1) = a;
     data(2) = a*2;
     data(3) = a*3;

     a=-3.0;
     double b=14.0;
     data(4) = a*4 + b;
     data(5) = a*5 + b;
     data(6) = a*6 + b;
     
     // data = 0 1.66667 3.33333 5 2 -1 -4


     vcl_cout << "Data: " << vcl_endl;
     vcl_cout << data << vcl_endl;

     {
     dbnl_eno_1d e;
     e.interpolate(&data);
     
     vcl_cout << "Computing zeros" << vcl_endl;
     dbnl_eno_zerox_vector z(e);
     TEST_NEAR("Zero1, test 1",z[4].location(0),14.0/3.0,tol);
     TEST_NEAR("Zero2, test 1",z[0].location(0),0,tol);

     unsigned label[] = { 0, 11, 22, 33, 44, 55, 66 };
   
     dbnl_eno_zerox_label zl(data.data_block(), label, z);

     zl.print(e, z, vcl_cout);
     TEST("Label1",zl[0][0],11);
     TEST("Label2",zl[4][0],44);
     vcl_cout << "  zl[4][0] ==  " << zl[4][0] << vcl_endl;
     }


     
     // ------------------


     { // The following test is only to stress the behavior of the code for this
       // specific example. It doesn't mean it is the correct behaviour.

     vnl_vector<double> data2(7);
     data2 = data - 5;
     // now 2 intervals will signal zeros at 3
     
     dbnl_eno_1d e;
     e.interpolate(&data2);

     dbnl_eno_zerox_vector z(e);
     TEST_NEAR("Zero1, test 2",z[2].location(0),3,tol);
     TEST_NEAR("Zero2, test 2",z[3].location(0),3,tol);

     unsigned label[] = { 0, 11, 22, 33, 44, 55, 66 };
     dbnl_eno_zerox_label zl(data2.data_block(), label, z);

     zl.print(e, z, vcl_cout);
     TEST("Label1",zl[2][0],dbnl_eno_zerox_label::unlabeled);
     TEST("Label2",zl[3][0],dbnl_eno_zerox_label::unlabeled);
     }
     // TODO:
     //   - test cases where there are 2 zero crossings in an interval.

     // ------------------

     // OBS: other tests about zero crossings are being executed in test_eno_1d.cxx


   }

   SUMMARY();
}
