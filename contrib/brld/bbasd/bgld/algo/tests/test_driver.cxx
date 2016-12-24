// This is basic/dbgl/algo/tests/test_driver.cxx

#include <testlib/testlib_register.h>

DECLARE( test_circ_arc );
DECLARE( test_biarc );
DECLARE( test_eulerspiral );
DECLARE( test_conic_arc );
DECLARE( test_arc_intersect );
DECLARE( test_intersect );
DECLARE( test_distance );
DECLARE( test_closest_point );
DECLARE( test_pt_tri_dist_3d );
DECLARE( test_similarity_3d );
DECLARE( test_fit_circ_arc_spline );
DECLARE( test_compute_symmetry_point );
DECLARE( test_diffgeom );


void
register_tests()
{
  REGISTER( test_circ_arc )
  REGISTER( test_biarc );
  REGISTER( test_eulerspiral );
  REGISTER( test_conic_arc );
  REGISTER( test_arc_intersect );
  REGISTER( test_intersect );
  REGISTER( test_distance );
  REGISTER( test_closest_point );
  REGISTER( test_pt_tri_dist_3d );
  REGISTER( test_similarity_3d );
  REGISTER( test_fit_circ_arc_spline );
  REGISTER( test_compute_symmetry_point );
  REGISTER( test_diffgeom );
}

DEFINE_MAIN;



