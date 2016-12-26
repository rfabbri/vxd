# This is lemsvxlsrc/basic/dbnl/algo/tests

add_executable ( dbnl_algo_test_all
   # Driver source and utilities
   test_driver.cxx

   # The tests
   test_eno_poly.cxx
   test_eno_zerox.cxx
   test_eno_image.cxx
   test_eno_interp.cxx
   test_eno_1d.cxx
   test_eno_shock_1d.cxx
   test_eno_third_order.cxx
   test_brent_root.cxx
)

target_link_libraries( dbnl_algo_test_all dbnl_algo testlib )

add_test( dbnl_algo_test_eno_poly         dbnl_algo_test_all test_eno_poly )
add_test( dbnl_algo_test_eno_interp       dbnl_algo_test_all test_eno_interp)
add_test( dbnl_algo_test_eno_zerox        dbnl_algo_test_all test_eno_zerox )
add_test( dbnl_algo_test_eno_1d           dbnl_algo_test_all test_eno_1d)
add_test( dbnl_algo_test_eno_image        dbnl_algo_test_all test_eno_image)
add_test( dbnl_algo_test_eno_shock_1d     dbnl_algo_test_all test_eno_shock_1d)
add_test( dbnl_algo_test_eno_third_order  dbnl_algo_test_all test_eno_third_order)


add_executable( dbnl_algo_test_include test_include.cxx )
target_link_libraries( dbnl_algo_test_include dbnl_algo )

