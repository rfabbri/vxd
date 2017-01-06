# bbasd/bgrld/algo/CMakeLists.txt

set( bgrld_algo_sources
  bgrld_algs.cxx              bgrld_algs.h
)

#aux_source_directory(Templates bgrld_algo_sources)

add_library( bgrld_algo  ${bgrld_algo_sources})

target_link_libraries( bgrld_algo bgrld )

#if( BUILD_EXAMPLES )
#  add_subdirectory( examples )
#endif( BUILD_EXAMPLES )
#if( BUILD_TESTING )
#  add_subdirectory(tests)
#endif( BUILD_TESTING )
