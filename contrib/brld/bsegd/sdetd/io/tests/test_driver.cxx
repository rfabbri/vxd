#include <testlib/testlib_register.h>

DECLARE( test_load_edg );
DECLARE( test_sel );
DECLARE( test_edgemap );
DECLARE( test_compute_cues );
DECLARE( test_curve_fragment_postprocess );
DECLARE( test_contour_breaker );
DECLARE( test_graphical_model_contour_merge );

void
register_tests()
{
  REGISTER( test_curve_fragment_postprocess );
  REGISTER( test_compute_cues );
  REGISTER( test_load_edg );
  REGISTER( test_sel );
  REGISTER( test_edgemap );
  REGISTER( test_contour_breaker );
  REGISTER( test_graphical_model_contour_merge );
}

DEFINE_MAIN;
