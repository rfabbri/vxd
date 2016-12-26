#include <testlib/testlib_register.h>

DECLARE( test_interp_linear );
DECLARE( test_interp_linear_3d );
DECLARE( test_interp_eno );
DECLARE( test_interp_eno_3d );
DECLARE( test_geno );
DECLARE( test_geno_shock );

void
register_tests()
{
   REGISTER( test_interp_linear );
   REGISTER( test_interp_linear_3d );
   REGISTER( test_interp_eno );
   REGISTER( test_interp_eno_3d );
   REGISTER( test_geno );
   REGISTER( test_geno_shock );
}

DEFINE_MAIN;
