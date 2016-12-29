# brcv/mvg/dvpgl/algo/CMakeLists.txt
# Photogrametry Algorithms Library.

include_directories(${VXL_BRL_INCLUDE_DIR}/bbas )
include_directories(${VXL_GEL_INCLUDE_DIR}/mrc)
include_directories(${VXL_RPL_INCLUDE_DIR})

# Code in this library has been promoted to sourceforge
# Recreate the library if needed

set( dvpgl_algo_sources
  dvpgl_triangulation.h dvpgl_triangulation.cxx 
  dvpgl_distmap_bundle_adjust.h dvpgl_distmap_bundle_adjust.cxx 
  # Under development: dvpgl_camera_compute_ransac.h dvpgl_camera_compute_ransac.cxx 
)

add_library(dvpgl_algo ${dvpgl_algo_sources})


target_link_libraries(dvpgl_algo vpgl_algo triangulation_kanatani)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

add_subdirectory(triangulation_kanatani)

add_subdirectory(cmd)

add_subdirectory(pro)

