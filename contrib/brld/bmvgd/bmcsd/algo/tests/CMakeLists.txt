# This is /contrib/brld/bmvgd/bmcsd/algo/tests

add_executable ( bmcsd_algo_test_all
   # Driver source and utilities
   test_driver.cxx

   # The tests
   test_bmcsd_algo_discrete_corresp_n_algo.cxx
)

add_test( bmcsd_algo_test_discrete_corresp_n_algo bmcsd_algo_test_all test_discrete_corresp_n_algo )
target_link_libraries( bmcsd_algo_test_all bmcsd_algo testlib )

add_executable( bmcsd_algo_test_include test_include.cxx )
target_link_libraries(bmcsd_algo_test_include bmcsd)
