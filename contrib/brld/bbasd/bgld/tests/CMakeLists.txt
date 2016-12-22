# basic/dbgl/tests/CMakeLists.txt


add_executable( dbgl_test_all
  test_driver.cxx
  test_arc.cxx
  test_poly_line.cxx
  test_eno_curve.cxx
  test_eno_curve_3d.cxx
  test_linear_curve_3d.cxx
  test_area.cxx
)

target_link_libraries( dbgl_test_all dbgl testlib )

add_test( dbgl_test_arc        dbgl_test_all test_arc )
add_test( dbgl_test_poly_line  dbgl_test_all test_poly_line )
add_test( dbgl_test_eno_curve  dbgl_test_all test_eno_curve )
add_test( dbgl_test_eno_curve_3d  dbgl_test_all test_eno_curve_3d )
add_test( dbgl_test_linear_curve_3d  dbgl_test_all test_linear_curve_3d )
add_test( dbgl_test_area  dbgl_test_all test_area )


add_executable( dbgl_test_include test_include.cxx )
target_link_libraries( dbgl_test_include dbgl vsl )
