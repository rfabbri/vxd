# This is basic/dbspi/tests

add_executable ( dbspi_test_all
  test_driver.cxx
  test_curve_distance.cxx
)

target_link_libraries( dbspi_test_all dbspi  dbil_algo testlib )

add_test( dbspi_test_curve_distance dbspi_test_all test_curve_distance )

add_executable( dbspi_test_include test_include.cxx )
target_link_libraries( dbspi_test_include dbspi)
