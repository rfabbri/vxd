# bbasd/bsold/tests/CMakeLists.txt


add_executable( bsold_test_all
  test_driver.cxx
  test_interp_curve_2d.cxx
  test_interp_curve_3d.cxx
  test_file_io.cxx
)

target_link_libraries( bsold_test_all bsold testlib )

add_test( bsold_test_interp_curve_2d  bsold_test_all test_interp_curve_2d )
add_test( bsold_test_interp_curve_3d  bsold_test_all test_interp_curve_3d )
add_test( bsold_test_file_io bsold_test_all test_file_io )

add_executable( bsold_test_include test_include.cxx )
target_link_libraries( bsold_test_include bsold vsl )
