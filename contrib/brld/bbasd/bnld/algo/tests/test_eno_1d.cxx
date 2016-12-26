#include <testlib/testlib_test.h>
#include <dbnl/algo/dbnl_eno.h>
#include <dbnl/algo/dbnl_eno_zerox.h>
#include <vcl_iostream.h>
#include <vcl_limits.h>

static double p1(double x);
static void test_interval(double x, double x_ref, dbnl_eno_1d &e,double tol);

MAIN( test_eno_1d )
{
   START ("eno_1d");

   double tol = vcl_numeric_limits<double>::epsilon()*1000;

   {
   vcl_cout << "Testing samples of a polynomial with known zeros" << vcl_endl;
   dbnl_eno_1d e;

   vnl_vector<double> data(4);

   // these are samples of  p(x) = (x-1.2)(x-1.8)
   data(0) = 2.16;
   data(1) = 0.16;
   data(2) = 0.16;
   data(3) = 2.99;

   e.interpolate(&data);
   e.print(vcl_cout);
   
   vcl_cout << "Computing zeros" << vcl_endl;
   dbnl_eno_zerox_vector z(e);
   z.print(e,vcl_cout);
   TEST_NEAR("Zero1",z[1].location(0),1.2,tol);
   TEST_NEAR("Zero2",z[1].location(1),1.8,tol);
   }

   vcl_cout << "Testing samples of a polynomial with known zeros" << vcl_endl;
   dbnl_eno_1d e;

   vnl_vector<double> data(4), x(4);

   // these are samples of  p(x) = (x-9.1)(x-10.8)
   x(0) = -13;
   x(1) = -8;
   x(2) = 11;
   x(3) = 21.33;
   data(0) = p1(x(0));
   data(1) = p1(x(1));
   data(2) = p1(x(2));
   data(3) = p1(x(3));

   e.interpolate(&data,&x);
   e.print(vcl_cout);
   
   vcl_cout << "Computing zeros" << vcl_endl;
   dbnl_eno_zerox_vector z(e);
   z.print(e,vcl_cout);
   TEST_NEAR("Zero1",z[1].location(0),9.1,tol);
   TEST_NEAR("Zero2",z[1].location(1),10.8,tol);

   // samples

   test_interval(x(0),data(0),e,tol);
   double xx = (x(0)+x(1))/2;
   test_interval(xx,p1(xx),e,tol);
   test_interval(x(1),data(1),e,tol);
   xx = (x(1)+x(2))/2;
   test_interval(xx,p1(xx),e,tol);
   test_interval(x(2),data(2),e,tol);
   xx = (x(2)+x(3))/2;
   test_interval(xx,p1(xx),e,tol);
   test_interval(x(3),data(3),e,tol);

   SUMMARY();
}

void test_interval(double x, double x_ref, dbnl_eno_1d &e, double tol)
{
   vcl_cout << "Abscissa " << x << " in interval #" << e.interval_index(x) << vcl_endl;
   TEST_NEAR("Sample",e.sample(x),x_ref,tol);
}

double p1(double x)
{
   return (x-9.1)*(x-10.8);
}
