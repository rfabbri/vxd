# This is /contrib/rfabbri/mw/tests

add_executable ( mw_test_all
   # Driver source and utilities
   test_driver.cxx

   # The tests
   test_mw_util.cxx
   test_mw_discrete_corresp_io.cxx
   test_mw_discrete_corresp_n_io.cxx
   test_dbmcs_curve_3d_sketch_io.cxx
)

target_link_libraries( mw_test_all mw testlib )

add_test( mw_test_mw_util mw_test_all test_mw_util )
add_test( mw_test_discrete_corresp_io mw_test_all test_discrete_corresp_io )
add_test( mw_test_discrete_corresp_n_io mw_test_all test_discrete_corresp_n_io )
add_test( dbmcs_curve_3d_sketch_io mw_test_all test_dbmcs_curve_3d_sketch_io)

add_executable( mw_test_include test_include.cxx )

target_link_libraries( mw_test_include mw )
