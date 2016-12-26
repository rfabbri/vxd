# This is basic/dbspi/CMakeLists.txt
# Brown Spatial Index Library


include_directories(${VXL_GEL_INCLUDE_DIR} )

set(dbspi_sources
    dbspi_chamfer.hxx          dbspi_chamfer.h
    dbspi_curve_map.cxx        dbspi_curve_map.h
    dbspi_curve_distance.cxx   dbspi_curve_distance.h
)

aux_source_directory(Templates dbspi_sources)

add_library(dbspi ${dbspi_sources})

target_link_libraries(dbspi vsol vil)

# Processing Library
add_subdirectory(pro)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
