# This is brcv/mvg/dbdif/CMakeLists.txt
# Brown Multiview Differential Geometry Library

include_directories(${VXL_GEL_INCLUDE_DIR} )
include_directories(${VXL_GEL_INCLUDE_DIR}/mrc )


set(dbdif_sources
dbdif_analytic.cxx dbdif_analytic.h
dbdif_camera.cxx dbdif_camera.h dbdif_frenet.h 
dbdif_rig.cxx dbdif_rig.h
dbdif_util.cxx dbdif_util.h
)

aux_source_directory(Templates dbdif_sources)

add_library(dbdif ${dbdif_sources})

target_link_libraries(dbdif vpgl vsol)

# Algorithms Library
add_subdirectory(algo)

# Processing Library
add_subdirectory(pro)

# Visualization Library
#add_subdirectory(vis)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
