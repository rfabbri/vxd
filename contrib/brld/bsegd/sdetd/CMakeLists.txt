# Brown Segmentation Detection Libraries
# bsegd/sdetd/CMakeLists.txt
project(sdetd)

# include directories into here if needed


#configure_file( ${dbdet_SOURCE_DIR}/dbdet_config.h.in
#                ${dbdet_BINARY_DIR}/dbdet_config.h )


#set(dbdet_sources
#)

#aux_source_directory(Templates dbdet_sources)

#add_library(dbdet ${dbdet_sources})

# add the required libraries into this list
#target_link_libraries( dbdet vil1 vil bil_algo dbnl_algo osl vepl vepl vdgl vnl_io bdgl dbgl_algo brip pdf1d mbl)


#if(BUILD_TESTING)
#  add_subdirectory(tests)
#endif(BUILD_TESTING)

add_subdirectory(algo)
add_subdirectory(pro)


# if (VXL_VGUI_FOUND)
#   add_subdirectory(vis)
# endif (VXL_VGUI_FOUND)
# 
# if( BUILD_EXAMPLES )
#   add_subdirectory( examples )
# endif( BUILD_EXAMPLES )
