#include <testlib/testlib_test.h>

#include <dbnl/dbnl_solve_trigonometric_equation.h>
#include <vcl_iostream.h>
#include <vnl/vnl_math.h>
#include <vcl_cmath.h>


//: Compare the values of Fresnel integral (cosin and sin) computed
// against those computed using Matlab.
MAIN( test_solve_trigonometric_equation )
{
  START ("Solving trigonometric equation");

  // solve 1st order equation: 
  // a sin(x) + b cos(x) + c = 0;

  // No root
  double a = 3;
  double b = 4;
  double c = 6;
  vcl_vector<double > cos_x;
  vcl_vector<double > sin_x;
  dbnl_solve_1st_order_trig_equation(a, b, c, sin_x, cos_x);
  TEST("Solve 1st-order trigonometric equation - no root", cos_x.empty() && sin_x.empty(), true);

  // one root
  a = 3;
  b = 5;
  c = vcl_sqrt(a*a + b*b);
  dbnl_solve_1st_order_trig_equation(a, b, c, sin_x, cos_x);

  TEST("Solve 1st-order trig. equation - one root - test #roots", cos_x.size() == 1 && sin_x.size()==1, true);
  if (cos_x.size() ==1)
  {
    double fx = a*sin_x[0] + b*cos_x[0] + c;
    TEST_NEAR("Solve 1st-order trig. equation - one root - test root value", fx, 0, 1e-12);
  }


  // two roots
  a = 3;
  b = 5;
  c = vcl_sqrt(a*a + b*b - 2);
  dbnl_solve_1st_order_trig_equation(a, b, c, sin_x, cos_x);

  TEST("Solve 1st-order trig. equation - two roots - test #roots", cos_x.size() == 2 && sin_x.size()==2, true);
  if (cos_x.size() ==2)
  {
    double fx0 = a*sin_x[0] + b*cos_x[0] + c;
    double fx1 = a*sin_x[1] + b*cos_x[1] + c;
    TEST_NEAR("Solve 1st-order trig. equation - two roots - value of root1", fx0, 0, 1e-12);
    TEST_NEAR("Solve 1st-order trig. equation - two roots - value of root2", fx1, 0, 1e-12);
  }

  
  SUMMARY();
}
