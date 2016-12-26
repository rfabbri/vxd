# basic/bild/algo/tests/CMakeLists.txt


add_executable( bild_algo_test_all
  test_driver.cxx
  test_exact_distance_transform.cxx
)

target_link_libraries( bild_algo_test_all bild_algo testlib )

add_test( bild_algo_test_exact_distance_transform bild_algo_test_all test_exact_distance_transform )


add_executable( bild_algo_test_include test_include.cxx )
target_link_libraries( bild_algo_test_include bild_algo )
