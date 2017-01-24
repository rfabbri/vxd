# This is core/vil1/io/tests/CMakeLists.txt

add_executable( vil1_io_test_all
  test_driver.cxx

  golden_test_vil1_io.cxx
  test_memory_image_format_io.cxx
  test_memory_image_impl_io.cxx
  test_memory_image_of_io.cxx
  test_rgb_io.cxx
  test_rgba_io.cxx
)
TARGET_link_libraries( vil1_io_test_all vil1_io testlib vpl )

add_test( vil1_io_test_memory_image_format_io  ${EXECUTABLE_OUTPUT_PATH}/vil1_io_test_all test_memory_image_format_io )
add_test( vil1_io_test_memory_image_impl_io    ${EXECUTABLE_OUTPUT_PATH}/vil1_io_test_all test_memory_image_impl_io )
add_test( vil1_io_test_memory_image_of_io      ${EXECUTABLE_OUTPUT_PATH}/vil1_io_test_all test_memory_image_of_io )
add_test( vil1_io_test_rgb_io                  ${EXECUTABLE_OUTPUT_PATH}/vil1_io_test_all test_rgb_io )
add_test( vil1_io_test_rgba_io                 ${EXECUTABLE_OUTPUT_PATH}/vil1_io_test_all test_rgba_io )
add_test( vil1_io_test_golden                  ${EXECUTABLE_OUTPUT_PATH}/vil1_io_test_all golden_test_vil1_io )

add_executable( vil1_io_test_include test_include.cxx )
TARGET_link_libraries( vil1_io_test_include vil1_io )
add_executable( vil1_io_test_template_include test_template_include.cxx )
TARGET_link_libraries( vil1_io_test_template_include vil1_io )
