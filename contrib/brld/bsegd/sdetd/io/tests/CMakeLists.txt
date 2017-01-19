# This is /seg/dbdet/algo/tests

include_directories(${BRL_INCLUDE_DIR} )
include_directories(${BRL_INCLUDE_DIR}/bseg )
include_directories(${lems_SOURCE_DIR}/basic/dbtest )

add_executable ( dbdet_algo_test_all
   # Driver source and utilities
   test_driver.cxx

   # The tests
   test_load_edg.cxx
   
   test_sel.cxx

   test_edgemap.cxx

   test_compute_cues.cxx

   test_curve_fragment_postprocess.cxx

   test_contour_breaker.cxx

   test_graphical_model_contour_merge.cxx
)

target_link_libraries( dbdet_algo_test_all dbdet_algo sdet testlib dbtest)

add_test( dbdet_algo_test_load_edg dbdet_algo_test_all test_load_edg )

add_test( dbdet_algo_test_sel   dbdet_algo_test_all test_sel )

add_test ( dbdet_algo_test_edgemap dbdet_algo_test_all test_edgemap )

add_test ( dbdet_algo_test_curve_fragment_postprocess dbdet_algo_test_all test_curve_fragment_postprocess )

add_test ( dbdet_algo_test_compute_cues dbdet_algo_test_all test_compute_cues )

add_test ( dbdet_algo_test_contour_breaker dbdet_algo_test_all test_contour_breaker )

add_test ( dbdet_algo_test_graphical_models_contour_merge dbdet_algo_test_all test_graphical_model_contour_merge )

add_executable( dbdet_algo_test_include test_include.cxx )

target_link_libraries( dbdet_algo_test_include dbdet_algo )

