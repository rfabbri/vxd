#include <testlib/testlib_test.h>
#include <dbnl/algo/dbnl_eno.h>
#include <dbnl/algo/dbnl_eno_shock.h>
#include <vcl_iostream.h>
#include <vcl_limits.h>
MAIN( test_eno_shock_1d )
{
   START ("eno_shock_1d");

   {
   vcl_cout << "Testing samples of a polynomial with known zeros" << vcl_endl;
   dbnl_eno_shock_1d e;

   vnl_vector<double> data(4);

   // these are samples of  p(x) = (x-1.2)(x-1.8)
   data(0) = 2.16;
   data(1) = 0.16;
   data(2) = 0.16;
   data(3) = 2.99;

   e.interpolate(&data);
   e.place_shocks(); 
//   e.place_zerox();
   e.print(vcl_cout);
//   TEST_NEAR("Zero1",e[1].zerox().location(0),1.2,vcl_numeric_limits<double>::epsilon()*10);
//   TEST_NEAR("Zero2",e[1].zerox().location(1),1.8,vcl_numeric_limits<double>::epsilon()*10);
   }


   SUMMARY();
}

