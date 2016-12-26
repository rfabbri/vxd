#include <testlib/testlib_register.h>


DECLARE( test_fresnel );
DECLARE( test_sinc );
DECLARE( test_newton_root );
DECLARE( test_angle );
DECLARE( test_solve_quadratic_equation );
DECLARE( test_solve_trigonometric_equation );

void
register_tests()
{
  REGISTER( test_fresnel );
  REGISTER( test_sinc );
  REGISTER( test_newton_root );
  REGISTER( test_angle );
  REGISTER( test_solve_quadratic_equation );
  REGISTER( test_solve_trigonometric_equation );
}

DEFINE_MAIN;



