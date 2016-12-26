# This is basic/dbspi/pro/CMakeLists.txt
# Brown Spatial Index Processing library

include_directories( ${lems_SOURCE_DIR}/breye1 )

set(dbspi_pro_sources
#   dbspi_curvemap_process.cxx   dbspi_curvemap_process.h
   dbspi_curvemap_storage.cxx   dbspi_curvemap_storage.h    dbspi_curvemap_storage_sptr.h
)

#aux_source_directory(Templates dbspi_pro_sources)

add_library(dbspi_pro ${dbspi_pro_sources})

target_link_libraries(dbspi_pro dbspi vidpro1)


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)

