# vpgld/io/tests/CMakeLists.txt
# Tests for vpgld_io library

add_executable( vpgld_io_test_all
  # Driver source and utilities
  test_driver.cxx
  # The tests
#  test_misc.cxx
)

target_link_libraries( vpgld_io_test_all vpgld_algo vsl vbl_io testlib )
#add_test( vpgld_io_test_triangulation vpgld_io_test_all test_triangulation)

add_executable( vpgld_io_test_include test_include.cxx )
target_link_libraries( vpgld_io_test_include vpgld_io )
#add_executable( vpgld_io_test_template_include test_template_include.cxx )
#target_link_libraries( vpgld_io_test_template_include vpgld_io)
