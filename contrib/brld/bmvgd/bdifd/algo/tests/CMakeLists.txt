# This is brl/bmvgd/bdifd/algo/tests/CMakeLists.txt

add_executable (bdifd_algo_test_all
   # Driver source and utilities
   test_driver.cxx

   # The tests
   test_diff_geometry.cxx
)

target_link_libraries( bdifd_algo_test_all bdifd_algo testlib )

add_test( bdifd_algo_test_diff_geometry bdifd_algo_test_all test_diff_geometry )

add_executable( bdifd_algo_test_include test_include.cxx )

target_link_libraries( bdifd_algo_test_include bdifd_algo )
