# vpgld/algo/tests/CMakeLists.txt
# Tests for vpgld_algo library

include_directories(${VXL_OXL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/bmvgd/bmvl)

add_executable( vpgld_algo_test_all
  # Driver source and utilities
  test_driver.cxx
  # The tests
  test_fm_estimation.cxx
  test_triangulation.cxx
)

target_link_libraries( vpgld_algo_test_all vpgld_algo bdifd_algo testlib )
add_test( vpgld_algo_test_fm_estimation vpgld_algo_test_all test_fm_estimation)
add_test( vpgld_algo_test_triangulation vpgld_algo_test_all test_triangulation)

add_executable( vpgld_algo_test_include test_include.cxx )
target_link_libraries( vpgld_algo_test_include vpgl_algo mvl )
