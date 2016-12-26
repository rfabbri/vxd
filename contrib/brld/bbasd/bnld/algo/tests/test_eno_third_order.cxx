#include <testlib/testlib_test.h>
#include <dbnl/algo/dbnl_eno_third_order.h>
#include <vcl_iostream.h>

MAIN( test_eno_third_order )
{
   START ("eno_third_order");

   dbnl_eno_third_order eno;
   
   vcl_vector<double> xi;
   vcl_vector<double> fxi;
   // these are samples of  p(x) = -4 + x - 3*x^2 + 2*x^3
   xi.push_back(0);
   xi.push_back(1);
   xi.push_back(2);
   xi.push_back(3);

   fxi.push_back(-4);
   fxi.push_back(-4);
   fxi.push_back(2);
   fxi.push_back(26);

   eno.interpolate(xi, fxi);
   
   TEST_NEAR("Coefficient test", eno.coefficient(0, 0), -4, 10e-6);
   TEST_NEAR("Coefficient test", eno.coefficient(0, 1), 1, 10e-6);
   TEST_NEAR("Coefficient test", eno.coefficient(0, 2), -3, 10e-6);
   TEST_NEAR("Coefficient test", eno.coefficient(0, 3), 2, 10e-6);

   TEST_NEAR("Coefficient test", eno.coefficient(1, 0), -4, 10e-6);
   TEST_NEAR("Coefficient test", eno.coefficient(1, 1), 1, 10e-6);
   TEST_NEAR("Coefficient test", eno.coefficient(1, 2), -3, 10e-6);
   TEST_NEAR("Coefficient test", eno.coefficient(1, 3), 2, 10e-6);

   TEST_NEAR("Coefficient test", eno.coefficient(2, 0), -4, 10e-6);
   TEST_NEAR("Coefficient test", eno.coefficient(2, 1), 1, 10e-6);
   TEST_NEAR("Coefficient test", eno.coefficient(2, 2), -3, 10e-6);
   TEST_NEAR("Coefficient test", eno.coefficient(2, 3), 2, 10e-6);

   TEST("Start point test", eno.polynomial_start_index(0), 0);
   TEST("Start point test", eno.polynomial_start_index(1), 0);
   TEST("Start point test", eno.polynomial_start_index(2), 0);

   SUMMARY();
}
