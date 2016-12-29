# brcv/mvg/vpgld/CMakeLists.txt
# Photogrametry Library.

include_directories( ${lems_SOURCE_DIR}/brcv/mvg )
include_directories(${VXL_BRL_INCLUDE_DIR}/bbas)
include_directories(${VXL_GEL_INCLUDE_DIR})
include_directories(${VXL_GEL_INCLUDE_DIR}/mrc)

#set( vpgld_sources
#  EMPTY
#)

#aux_source_directory(Templates vpgld_sources)

#add_library(vpgld ${vpgld_sources})
#target_link_libraries( vpgld vnl vnl_algo vnl_io vgl vgl_algo vgl_io vgl_xio vsl)


#if( BUILD_TESTING )
#  add_subdirectory(tests)
#endif( BUILD_TESTING )

add_subdirectory(algo)
add_subdirectory(io)
add_subdirectory(pro)
add_subdirectory(vis)
