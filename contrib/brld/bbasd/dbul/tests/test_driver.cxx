#include <testlib/testlib_register.h>


DECLARE( test_solar_position );
DECLARE( test_parse_simple_file );

void
register_tests()
{
  REGISTER( test_solar_position );
  REGISTER( test_parse_simple_file );
}

DEFINE_MAIN;



