# basic/dbnl/algo/pro/CMakeLists.txt

include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/breye1 )


set(dbnl_pro_sources
   dbnl_function_sampling_process.cxx dbnl_function_sampling_process.h
)


#aux_source_directory(Templates dbnl_pro_sources)

add_library(dbnl_pro  ${dbnl_pro_sources})

target_link_libraries(dbnl_pro vidpro1 vsol dbnl_algo)
