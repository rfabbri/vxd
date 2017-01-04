# Brown Curve-based Spatial Indexing Library  
# brld/rec/bcsid/CMakeLists.txt

include_directories(${VXL_GEL_INCLUDE_DIR})
include_directories(${VXL_BRL_INCLUDE_DIR})

set(bcsid_sources
    bcsid_shotton_ocm.h             bcsid_shotton_ocm.cxx
    bcsid_curve_distance.h          bcsid_curve_distance.cxx
)

# aux_source_directory(Templates bcsid_sources)

add_library(bcsid  ${bcsid_sources})

target_link_libraries(bcsid bspid sdet brip)

# algorithms library
#add_subdirectory(algo)

# Processing Library
#add_subdirectory(pro)

# Visualization Library
#add_subdirectory(vis)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)

# Executables
#add_subdirectory(bin)
