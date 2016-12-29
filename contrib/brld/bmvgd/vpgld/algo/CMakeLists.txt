# bmvgd/vpgld/algo/CMakeLists.txt
# Photogrametry Algorithms Library.

include_directories(${VXL_BRL_INCLUDE_DIR}/bbas )
include_directories(${VXL_GEL_INCLUDE_DIR}/mrc)
include_directories(${VXL_RPL_INCLUDE_DIR})

# Code in this library has been promoted to sourceforge
# Recreate the library if needed

set( vpgld_algo_sources
  vpgld_triangulation.h vpgld_triangulation.cxx 
  vpgld_distmap_bundle_adjust.h vpgld_distmap_bundle_adjust.cxx 
  # Under development: vpgld_camera_compute_ransac.h vpgld_camera_compute_ransac.cxx 
)

add_library(vpgld_algo ${vpgld_algo_sources})


target_link_libraries(vpgld_algo vpgl_algo triangulation_kanatani)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

add_subdirectory(triangulation_kanatani)

add_subdirectory(cmd)

add_subdirectory(pro)

