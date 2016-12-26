# This is basic/bspid/CMakeLists.txt
# Brown Spatial Index Library


include_directories(${VXL_GEL_INCLUDE_DIR} )

set(bspid_sources
    bspid_chamfer.hxx          bspid_chamfer.h
    bspid_curve_map.cxx        bspid_curve_map.h
    bspid_curve_distance.cxx   bspid_curve_distance.h
)

aux_source_directory(Templates bspid_sources)

add_library(bspid ${bspid_sources})

target_link_libraries(bspid vsol vil)

# Processing Library
add_subdirectory(pro)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
