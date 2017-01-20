# bmvgd/vpgld/vis/CMakeLists.txt

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )
include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/basic )
include_directories( ${lems_SOURCE_DIR}/breye )


if(BUILD_BREYE)
set( vpgld_vis_sources
vpgld_epipolar_tool.cxx      vpgld_epipolar_tool.h   
vpgld_epipolar_region_tool.cxx vpgld_epipolar_region_tool.h
#vpgld_image_correspondence_tool.cxx vpgld_image_correspondence_tool.h
menu/vpgld_camera_menu.cxx   menu/vpgld_camera_menu.h
)
else(BUILD_BREYE)
set( vpgld_vis_sources
vpgld_epipolar_tool.cxx      vpgld_epipolar_tool.h   
vpgld_epipolar_region_tool.cxx vpgld_epipolar_region_tool.h
)
endif(BUILD_BREYE)

add_library(vpgld_vis ${vpgld_vis_sources})

if(BUILD_BREYE)
  target_link_libraries(vpgld_vis bvis vpgl)
else(BUILD_BREYE)
  if(BUILD_BREYE1D)
    target_link_libraries(vpgld_vis bvis1 vpgl vgui)
  endif(BUILD_BREYE1D)
endif(BUILD_BREYE)

if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)
