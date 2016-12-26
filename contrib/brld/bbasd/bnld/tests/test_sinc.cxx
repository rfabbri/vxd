#include <testlib/testlib_test.h>
#include <dbnl/dbnl_sinc.h>


// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date 08/17/2005


//: Compared values of sinc function against results from Matlab
MAIN( test_sinc )
{
  START ("Sinc function sinc(x) = sin(x)/x");
  
  TEST_NEAR("sinc(0)", dbnl_sinc(0), 1, 1e-12);
  TEST_NEAR("sinc(1)", dbnl_sinc(1), 0.84147098480790, 1e-12);
  TEST_NEAR("sinc(pi)", dbnl_sinc(vnl_math::pi), 0, 1e-12);
  TEST_NEAR("sinc(-1)", dbnl_sinc(-1), 0.84147098480790, 1e-12);
  TEST_NEAR("sinc(1e-6)", dbnl_sinc(1e-6), 0.99999999999983, 1e-12);
  
  SUMMARY();
}
