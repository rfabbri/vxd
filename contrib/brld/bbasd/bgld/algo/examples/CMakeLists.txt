# This is basic/bgld/algo/examples/CMakeLists.txt


# Program to generate look-up table for Euler Spiral computation
add_executable( example_generate_eulerspiral_lookup_table example_generate_eulerspiral_lookup_table.cxx )
target_link_libraries( example_generate_eulerspiral_lookup_table vsl bgld_algo )


# Program to generate a table of number of iterations for different Euler Spiral computation
add_executable( example_generate_eulerspiral_num_iter_table example_generate_eulerspiral_num_iter_table.cxx )
target_link_libraries( example_generate_eulerspiral_num_iter_table bgld_algo )

