#include <testlib/testlib_test.h>
#include <dbnl/dbnl_solve_quadratic_equation.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>


//: Compare the values of Fresnel integral (cosin and sin) computed
// against those computed using Matlab.
MAIN( test_solve_quadratic_equation )
{
  START ("Test solving quadratic equation");

  // zero root
  double a = 2;
  double b = 2;
  double c = 1;
  vcl_vector<double > roots;
  dbnl_solve_quadratic_equation(a, b, c, roots);
  TEST("Equation with no root", roots.empty(), true);

  // one root - x = 3
  a = 3;
  b = 18;
  c = 27;
  dbnl_solve_quadratic_equation(a, b, c, roots);
  TEST("Equation with one root - test # of root", roots.size(), 1);
  if (roots.size() == 1)
  {
    TEST_NEAR("Equation with one root - test root value", roots[0], -3, 1e-12);
  }

  // two roots x = 1 and x = 4;
  a = 1;
  b = -5;
  c = 4;
  dbnl_solve_quadratic_equation(a, b, c, roots);
  TEST("Equation with two roots - test # of root", roots.size(), 2);
  if (roots.size() == 2)
  {
    TEST_NEAR("Equation with two roots - value of root 1", roots[0], 1, 1e-12);
    TEST_NEAR("Equation with two roots - value of root 1", roots[1], 4, 1e-12);
  }


  
  

 
  
  SUMMARY();
}
