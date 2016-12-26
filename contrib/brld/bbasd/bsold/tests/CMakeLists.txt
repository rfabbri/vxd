# basic/dbsol/tests/CMakeLists.txt


add_executable( dbsol_test_all
  test_driver.cxx
  test_interp_curve_2d.cxx
  test_interp_curve_3d.cxx
  test_file_io.cxx
)

target_link_libraries( dbsol_test_all dbsol testlib )

add_test( dbsol_test_interp_curve_2d  dbsol_test_all test_interp_curve_2d )
add_test( dbsol_test_interp_curve_3d  dbsol_test_all test_interp_curve_3d )
add_test( dbsol_test_file_io dbsol_test_all test_file_io )

add_executable( dbsol_test_include test_include.cxx )
target_link_libraries( dbsol_test_include dbsol vsl )
