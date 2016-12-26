#include <testlib/testlib_register.h>


DECLARE( test_interp_curve_2d );
DECLARE( test_interp_curve_3d );
DECLARE( test_file_io );

void
register_tests()
{
  REGISTER( test_interp_curve_2d );
  REGISTER( test_interp_curve_3d );
  REGISTER( test_file_io );
}

DEFINE_MAIN;


