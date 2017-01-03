# This is rec/dbcsi/tests

add_executable ( dbcsi_test_all
  test_driver.cxx
  test_curve_distance.cxx
)

target_link_libraries( dbcsi_test_all dbcsi dbdet_algo vil_algo testlib )

add_test( dbcsi_test_curve_distance dbcsi_test_all test_curve_distance )

add_executable( dbcsi_test_include test_include.cxx )
target_link_libraries( dbcsi_test_include dbcsi)
