# algo/becld/tests/CMakeLists.txt


add_executable( becld_test_all
  test_driver.cxx
  test_epipole.cxx
  test_episeg.cxx
  test_epiband.cxx
)

target_link_libraries( becld_test_all becld testlib )

add_test( becld_test_epipole           becld_test_all test_epipole )
add_test( becld_test_episeg            becld_test_all test_episeg )
add_test( becld_test_epiband           becld_test_all test_epiband )

add_executable( becld_test_include test_include.cxx )
target_link_libraries( becld_test_include becld )
