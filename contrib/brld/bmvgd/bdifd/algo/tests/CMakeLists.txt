# This is brcv/mvg/dbdif/algo/tests/CMakeLists.txt

add_executable (dbdif_algo_test_all
   # Driver source and utilities
   test_driver.cxx

   # The tests
   test_diff_geometry.cxx
)

target_link_libraries( dbdif_algo_test_all dbdif_algo testlib )

add_test( dbdif_algo_test_diff_geometry dbdif_algo_test_all test_diff_geometry )

add_executable( dbdif_algo_test_include test_include.cxx )

target_link_libraries( dbdif_algo_test_include dbdif_algo )
