#include <testlib/testlib_test.h>
#include <dbnl/algo/dbnl_eno.h>
#include <vcl_iostream.h>

MAIN( test_eno_poly )
{
   START ("eno_poly");

   const unsigned order=2;
   dbnl_eno_poly poly(order);

   poly[0] = 1.28;
   poly[1] = 69;
   poly[2] = 77.3;

   poly.print();
   TEST("Order",order,poly.order());

   TEST("Coefficient 0", poly[0], 1.28);
   TEST("Coefficient 1", poly[1], 69);
   TEST("Coefficient 2", poly[2], 77.3);

   dbnl_eno_poly poly2(2);

   poly2[0] = 1;
   poly2[1] = -40;
   poly2[2] = 100;

   dbnl_eno_poly dif = poly - poly2;
   TEST("Difference Coefficient 0", dif[0], 0.28);
   TEST("Difference Coefficient 1", dif[1], 109);
   TEST_NEAR("Difference Coefficient 2", dif[2], -22.7,1e-10);
   dif.print();

   SUMMARY();
}
