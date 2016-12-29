#include <testlib/testlib_register.h>


DECLARE( test_epipole );
DECLARE( test_episeg );
DECLARE( test_epiband );

void
register_tests()
{
  REGISTER( test_epipole );
  REGISTER( test_episeg );
  REGISTER( test_epiband );
}

DEFINE_MAIN;


