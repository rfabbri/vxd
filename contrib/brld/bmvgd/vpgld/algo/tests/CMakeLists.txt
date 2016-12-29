# dvpgl/algo/tests/CMakeLists.txt
# Tests for dvpgl_algo library

include_directories(${VXL_OXL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/brcv/mvg/bmvl)

add_executable( dvpgl_algo_test_all
  # Driver source and utilities
  test_driver.cxx
  # The tests
  test_fm_estimation.cxx
  test_triangulation.cxx
)

target_link_libraries( dvpgl_algo_test_all dvpgl_algo dbdif_algo brct mvl testlib )
add_test( dvpgl_algo_test_fm_estimation dvpgl_algo_test_all test_fm_estimation)
add_test( dvpgl_algo_test_triangulation dvpgl_algo_test_all test_triangulation)

add_executable( dvpgl_algo_test_include test_include.cxx )
target_link_libraries( dvpgl_algo_test_include vpgl_algo mvl )
