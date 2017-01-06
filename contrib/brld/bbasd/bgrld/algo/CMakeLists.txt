# basic/dbgrl/algo/CMakeLists.txt

set( dbgrl_algo_sources
  dbgrl_algs.cxx              dbgrl_algs.h
)

#aux_source_directory(Templates dbgrl_algo_sources)

add_library( dbgrl_algo  ${dbgrl_algo_sources})

target_link_libraries( dbgrl_algo dbgrl )

#if( BUILD_EXAMPLES )
#  add_subdirectory( examples )
#endif( BUILD_EXAMPLES )
#if( BUILD_TESTING )
#  add_subdirectory(tests)
#endif( BUILD_TESTING )
