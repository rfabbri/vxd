# This is basic/bspid/pro/CMakeLists.txt
# Brown Spatial Index Processing library

include_directories( ${lems_SOURCE_DIR}/breye1 )

set(bspid_pro_sources
#   bspid_curvemap_process.cxx   bspid_curvemap_process.h
   bspid_curvemap_storage.cxx   bspid_curvemap_storage.h    bspid_curvemap_storage_sptr.h
)

#aux_source_directory(Templates bspid_pro_sources)

add_library(bspid_pro ${bspid_pro_sources})

target_link_libraries(bspid_pro bspid vidpro1)


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)

