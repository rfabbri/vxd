# bsegd/sdetd/algo/CMakeLists.txt
# Algorithms for Brown Detection and Segmentation Libraries

include_directories(${VXL_BRL_INCLUDE_DIR}/bseg)
include_directories(${VXL_GEL_INCLUDE_DIR})


set( sdetd_algo_sources
# IO utilities
  sdetd_convert_edgemap.h                   sdetd_convert_edgemap.cxx
)

add_library(sdetd_algo ${sdetd_algo_sources})
target_link_libraries(sdetd_algo sdet vil)


#if( BUILD_TESTING )
  #  add_subdirectory(tests)
#endif( BUILD_TESTING )
