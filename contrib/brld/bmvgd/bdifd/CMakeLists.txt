# This is brcv/mvg/bdifd/CMakeLists.txt
# Brown Multiview Differential Geometry Library

include_directories(${VXL_GEL_INCLUDE_DIR} )
include_directories(${VXL_GEL_INCLUDE_DIR}/mrc )


set(bdifd_sources
bdifd_analytic.cxx bdifd_analytic.h
bdifd_camera.cxx bdifd_camera.h bdifd_frenet.h 
bdifd_rig.cxx bdifd_rig.h
bdifd_util.cxx bdifd_util.h
)

aux_source_directory(Templates bdifd_sources)

add_library(bdifd ${bdifd_sources})

target_link_libraries(bdifd vpgl vsol)

# Algorithms Library
add_subdirectory(algo)

# Processing Library
add_subdirectory(pro)

# Visualization Library
#add_subdirectory(vis)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
