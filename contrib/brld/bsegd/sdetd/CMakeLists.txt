# Brown Segmentation Detection Libraries
# bsegd/sdetd/CMakeLists.txt
project(sdetd)

# include directories into here if needed


#configure_file( ${sdetd_SOURCE_DIR}/sdetd_config.h.in
#                ${sdetd_BINARY_DIR}/sdetd_config.h )


#set(sdetd_sources
#)

#aux_source_directory(Templates sdetd_sources)

#add_library(sdetd ${sdetd_sources})

# add the required libraries into this list
#target_link_libraries( sdetd vil1 vil bil_algo bnld_algo osl vepl vepl vdgl vnl_io bdgl bgld_algo brip pdf1d mbl)


#if(BUILD_TESTING)
#  add_subdirectory(tests)
#endif(BUILD_TESTING)

add_subdirectory(algo)
add_subdirectory(io)
add_subdirectory(pro)


# if (VXL_VGUI_FOUND)
#   add_subdirectory(vis)
# endif (VXL_VGUI_FOUND)
# 
# if( BUILD_EXAMPLES )
#   add_subdirectory( examples )
# endif( BUILD_EXAMPLES )
