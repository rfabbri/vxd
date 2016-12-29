# dvpgl/io/tests/CMakeLists.txt
# Tests for dvpgl_io library

add_executable( dvpgl_io_test_all
  # Driver source and utilities
  test_driver.cxx
  # The tests
#  test_misc.cxx
)

target_link_libraries( dvpgl_io_test_all dvpgl_algo vsl vbl_io testlib )
#add_test( dvpgl_io_test_triangulation dvpgl_io_test_all test_triangulation)

add_executable( dvpgl_io_test_include test_include.cxx )
target_link_libraries( dvpgl_io_test_include dvpgl_io )
#add_executable( dvpgl_io_test_template_include test_template_include.cxx )
#target_link_libraries( dvpgl_io_test_template_include dvpgl_io)
