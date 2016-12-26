# basic/dbul/tests/CMakeLists.txt
# Tests for dbul library

add_executable( dbul_test_all
  test_driver.cxx
  test_solar_position.cxx
  test_parse_simple_file.cxx
)

target_link_libraries( dbul_test_all dbul vul testlib  )


add_test( dbul_test_solar_position         dbul_test_all test_solar_position )
add_test( dbul_test_parse_simple_file dbul_test_all test_parse_simple_file)

add_executable( dbul_test_include test_include.cxx )

target_link_libraries( dbul_test_include dbul )
