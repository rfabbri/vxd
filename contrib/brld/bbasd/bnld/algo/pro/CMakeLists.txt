# bbasd/bnld/algo/pro/CMakeLists.txt

include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/breye1 )


set(bnld_pro_sources
   bnld_function_sampling_process.cxx bnld_function_sampling_process.h
)


#aux_source_directory(Templates bnld_pro_sources)

add_library(bnld_pro  ${bnld_pro_sources})

target_link_libraries(bnld_pro vidpro1 vsol bnld_algo)
