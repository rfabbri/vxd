# algo/dbecl/tests/CMakeLists.txt


add_executable( dbecl_test_all
  test_driver.cxx
  test_epipole.cxx
  test_episeg.cxx
  test_epiband.cxx
)

target_link_libraries( dbecl_test_all dbecl testlib )

add_test( dbecl_test_epipole           dbecl_test_all test_epipole )
add_test( dbecl_test_episeg            dbecl_test_all test_episeg )
add_test( dbecl_test_epiband           dbecl_test_all test_epiband )

add_executable( dbecl_test_include test_include.cxx )
target_link_libraries( dbecl_test_include dbecl )
