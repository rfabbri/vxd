# Brown Curve-based Spatial Indexing Library  
# brcv/rec/dbcsi/CMakeLists.txt

include_directories(${VXL_GEL_INCLUDE_DIR})
include_directories(${VXL_BRL_INCLUDE_DIR})
include_directories( ${lems_SOURCE_DIR}/brcv/seg )
include_directories( ${lems_SOURCE_DIR}/brcv/rec )

set(dbcsi_sources
    dbcsi_shotton_ocm.h             dbcsi_shotton_ocm.cxx
    dbcsi_curve_distance.h          dbcsi_curve_distance.cxx
)

aux_source_directory(Templates dbcsi_sources)

add_library(dbcsi  ${dbcsi_sources})

target_link_libraries(dbcsi dbspi dbdet_algo )

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
