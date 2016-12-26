# This is /contrib/rfabbri/mw/tests

add_executable ( bmcsd_test_all
   # Driver source and utilities
   test_driver.cxx

   # The tests
   test_bmcsd_util.cxx
   test_bmcsd_discrete_corresp_io.cxx
   #   test_bmcsd_discrete_corresp_n_io.cxx
   test_bmcsd_curve_3d_sketch_io.cxx
)

target_link_libraries( bmcsd_test_all mw testlib )

add_test( bmcsd_test_bmcsd_util bmcsd_test_all test_bmcsd_util )
add_test( bmcsd_test_discrete_corresp_io bmcsd_test_all test_discrete_corresp_io )
#add_test( bmcsd_test_discrete_corresp_n_io bmcsd_test_all test_discrete_corresp_n_io )
add_test( bmcsd_curve_3d_sketch_io bmcsd_test_all test_bmcsd_curve_3d_sketch_io)

add_executable( bmcsd_test_include test_include.cxx )

target_link_libraries(bmcsd_test_include bmcsd)
