#include <testlib/testlib_register.h>


DECLARE( test_process );
DECLARE( test_process_factory );
DECLARE( test_delay_filter );
DECLARE( test_storage );


void
register_tests()
{
  REGISTER( test_process );
  REGISTER( test_process_factory );
  REGISTER( test_delay_filter );
  REGISTER( test_storage );
}

DEFINE_MAIN;


