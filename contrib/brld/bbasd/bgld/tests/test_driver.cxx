#include <testlib/testlib_register.h>


DECLARE( test_arc );
DECLARE( test_poly_line );
DECLARE( test_minimal_container );
DECLARE( test_eno_curve );
DECLARE( test_eno_curve_3d );
DECLARE( test_linear_curve_3d );
DECLARE( test_area );

void
register_tests()
{
  REGISTER( test_arc );
  REGISTER( test_poly_line );
  REGISTER( test_minimal_container );
  //REGISTER( test_eno_curve );
  //REGISTER( test_eno_curve_3d );
  REGISTER( test_linear_curve_3d );
  REGISTER( test_area );
}

DEFINE_MAIN;


