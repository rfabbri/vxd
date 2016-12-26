# This is bbasd/bsold/algo/tests

add_executable ( bsold_algo_test_all
   # Driver source and utilities
   test_driver.cxx

   # The tests
   test_interp_linear.cxx
   test_interp_eno.cxx
   test_geno.cxx
   test_geno_shock.cxx

   test_interp_eno_3d.cxx
   test_interp_linear_3d.cxx
)

target_link_libraries( bsold_algo_test_all bsold_algo testlib )

add_test( bsold_algo_test_interp_linear bsold_algo_test_all test_interp_linear)
add_test( bsold_algo_test_geno bsold_algo_test_all test_geno)
add_test( bsold_algo_test_geno_shock bsold_algo_test_all test_geno_shock)

add_executable( bsold_algo_test_include test_include.cxx )
target_link_libraries( bsold_algo_test_include bsold_algo )
