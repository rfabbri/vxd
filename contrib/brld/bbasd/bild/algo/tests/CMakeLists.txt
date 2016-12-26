# basic/dbil/algo/tests/CMakeLists.txt


add_executable( dbil_algo_test_all
  test_driver.cxx
  test_exact_distance_transform.cxx
)

target_link_libraries( dbil_algo_test_all dbil_algo testlib )

add_test( dbil_algo_test_exact_distance_transform dbil_algo_test_all test_exact_distance_transform )


add_executable( dbil_algo_test_include test_include.cxx )
target_link_libraries( dbil_algo_test_include dbil_algo )
