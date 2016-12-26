# basic/dbnl/tests/CMakeLists.txt
# Tests for dbnl library

add_executable( dbnl_test_all
  test_driver.cxx
  test_fresnel.cxx
  test_sinc.cxx
  test_newton_root.cxx
  test_angle.cxx
  test_solve_quadratic_equation.cxx
  test_solve_trigonometric_equation.cxx
)

target_link_libraries( dbnl_test_all dbnl testlib )

add_test( dbnl_test_fresnel              dbnl_test_all test_fresnel )
add_test( dbnl_test_sinc                 dbnl_test_all test_sinc )
add_test( dbnl_test_newton_root          dbnl_test_all test_newton_root )
add_test( dbnl_test_angle                dbnl_test_all test_angle )
add_test( dbnl_test_solve_quadratic_equation           dbnl_test_all    test_solve_quadratic_equation )
add_test( dbnl_test_solve_trigonometric_equation       dbnl_test_all    test_solve_trigonometric_equation )


add_executable( dbnl_test_include test_include.cxx )
target_link_libraries( dbnl_test_include dbnl )
