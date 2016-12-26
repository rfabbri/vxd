#include <testlib/testlib_register.h>

DECLARE( test_mw_util );
DECLARE( test_discrete_corresp_io );
DECLARE( test_discrete_corresp_n_io );
DECLARE( test_dbmcs_curve_3d_sketch_io );

void
register_tests()
{
   REGISTER( test_mw_util );
   REGISTER( test_discrete_corresp_io );
   REGISTER( test_discrete_corresp_n_io );
   REGISTER( test_dbmcs_curve_3d_sketch_io );
}

DEFINE_MAIN;
