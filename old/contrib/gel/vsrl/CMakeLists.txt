# gel/vsrl/CMakeLists.txt

# doxygen_add_library(contrib/gel/vsrl
#   DEPENDS
#     contrib/brl/bseg/sdet
#     contrib/gel/vgel
#     contrib/tbl/vepl1
#     core/vgui
#     core/vil1
#     contrib/rpl/rsdl
#   PACKAGE contrib/gel
#   DESCRIPTION "Dense image matcher"
# )

set(vsrl_sources
 vsrl_3d_output.cxx             vsrl_3d_output.h
                                vsrl_dense_matcher.h
 vsrl_diffusion.cxx             vsrl_diffusion.h
 vsrl_dynamic_program.cxx       vsrl_dynamic_program.h
 vsrl_image_correlation.cxx     vsrl_image_correlation.h
 vsrl_intensity_token.cxx       vsrl_intensity_token.h
 vsrl_null_token.cxx            vsrl_null_token.h
 vsrl_parameters.cxx            vsrl_parameters.h
 vsrl_raster_dp_setup.cxx       vsrl_raster_dp_setup.h
 vsrl_results_dense_matcher.cxx vsrl_results_dense_matcher.h
 vsrl_saliency_diffusion.cxx    vsrl_saliency_diffusion.h
 vsrl_step_diffusion.cxx        vsrl_step_diffusion.h
 vsrl_stereo_dense_matcher.cxx  vsrl_stereo_dense_matcher.h
                                vsrl_token.h
 vsrl_token_saliency.cxx        vsrl_token_saliency.h
 vsrl_window_accumulator.cxx    vsrl_window_accumulator.h
 vsrl_region_disparity.cxx      vsrl_region_disparity.h
)

if (VXL_VGUI_FOUND)
 include_directories(${VXL_VGUI_INCLUDE_DIR})
  

 set(vsrl_sources ${vsrl_sources}
  vsrl_point_picker.cxx         vsrl_point_picker.h    vsrl_point_picker_sptr.h
  vsrl_menus.h vsrl_menus.cxx
 )
 include_directories(${VXL_TBL_INCLUDE_DIR})
 include_directories(${VXL_BRL_INCLUDE_DIR}/bseg)
 include_directories(${VXL_RPL_INCLUDE_DIR}) # rsdl is in rpl
 set(vsrl_sources ${vsrl_sources}
  vsrl_manager.cxx              vsrl_manager.h
 )
endif ()

add_library(vsrl ${vsrl_sources})

if (VXL_VGUI_FOUND)
 if (VXL_BUILD_BRL)
#  target_link_libraries(vsrl sdet jseg)
# remove missing jseg error
  target_link_libraries(vsrl sdet)
 endif ()
 target_link_libraries(vsrl vgui)
 if (VXL_BUILD_TBL)
  target_link_libraries(vsrl vepl1)
 endif ()
 if (VXL_BUILD_RPL)
  target_link_libraries(vsrl rsdl)
 endif ()
endif ()

target_link_libraries(vsrl vtol vdgl vgl vil1 vnl vgel)

# vgel uses mvl from oxl
include_directories(${VXL_OXL_INCLUDE_DIR})

if( BUILD_TESTING )
  add_subdirectory(tests)
endif()

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif ()
