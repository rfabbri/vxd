# This is core/vidl1/tests/CMakeLists.txt

link_libraries( vidl1 )

add_executable( vidl1_test_all vidl1_test.cxx )
add_test( vidl1_test  ${EXECUTABLE_OUTPUT_PATH}/vidl1_test_all )

add_executable( vidl1_test_include test_include.cxx )
