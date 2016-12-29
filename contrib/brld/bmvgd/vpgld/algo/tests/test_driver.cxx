#include <testlib/testlib_register.h>

DECLARE(test_fm_estimation);
DECLARE(test_triangulation);

void
register_tests()
{
  REGISTER(test_fm_estimation);
  REGISTER(test_triangulation);
}

DEFINE_MAIN;
