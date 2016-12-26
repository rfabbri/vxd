# bbasd/buld/tests/CMakeLists.txt
# Tests for buld library

add_executable( buld_test_all
  test_driver.cxx
  test_solar_position.cxx
  test_parse_simple_file.cxx
)

target_link_libraries( buld_test_all buld vul testlib  )


add_test( buld_test_solar_position         buld_test_all test_solar_position )
add_test( buld_test_parse_simple_file buld_test_all test_parse_simple_file)

add_executable( buld_test_include test_include.cxx )

target_link_libraries( buld_test_include buld )
