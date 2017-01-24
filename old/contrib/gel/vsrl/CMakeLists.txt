# gel/vsrl/CMakeLists.txt

doxygen_add_library(contrib/gel/vsrl
  DEPENDS
    contrib/brl/bseg/sdet
    contrib/gel/vgel
    contrib/tbl/vepl1
    core/vgui
    core/vil1
    contrib/rpl/rsdl
  PACKAGE contrib/gel
  DESCRIPTION "Dense image matcher"
)

include_directories(${GEL_INCLUDE_DIR})

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

# vsrl_manager depends on vgui, on tbl/vepl1 and on brl/bseg/sdet
# vsrl_menus and vsrl_point_picker depend on vgui
include( ${VXL_CMAKE_DIR}/UseVGUI.cmake )

if (HAS_VGUI)
 include_directories(${VXLCORE_INCLUDE_DIR})

 set(vsrl_sources ${vsrl_sources}
  vsrl_point_picker.cxx         vsrl_point_picker.h    vsrl_point_picker_sptr.h
  vsrl_menus.h vsrl_menus.cxx
 )
 if (BUILD_TBL)
 if (BUILD_BRL)
 if (BUILD_RPL)
   include_directories(${TBL_INCLUDE_DIR})
   include_directories(${BRL_INCLUDE_DIR}/bseg)
   include_directories(${RPL_INCLUDE_DIR}) # rsdl is in rpl
   set(vsrl_sources ${vsrl_sources}
    vsrl_manager.cxx              vsrl_manager.h
   )
 endif (BUILD_RPL)
 endif (BUILD_BRL)
 endif (BUILD_TBL)
endif (HAS_VGUI)

add_library(vsrl ${vsrl_sources})
install_targets(/lib vsrl)
INSTALL_NOBASE_HEADER_FILES(/include/vxl/contrib/gel/vsrl ${vsrl_sources})

if (HAS_VGUI)
 if (BUILD_BRL)
#  TARGET_link_libraries(vsrl sdet jseg)
# remove missing jseg error
  TARGET_link_libraries(vsrl sdet)
 endif (BUILD_BRL)
 TARGET_link_libraries(vsrl vgui)
 if (BUILD_TBL)
  TARGET_link_libraries(vsrl vepl1)
 endif (BUILD_TBL)
 if (BUILD_RPL)
  TARGET_link_libraries(vsrl rsdl)
 endif (BUILD_RPL)
endif (HAS_VGUI)

TARGET_link_libraries(vsrl vtol vdgl vgl vil1 vnl vgel)

# vgel uses mvl from oxl
include_directories(${OXL_INCLUDE_DIR})

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif (BUILD_EXAMPLES)
