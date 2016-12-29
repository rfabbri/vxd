# This is brcv/mvg/becld/vis/CMakeLists.txt
# Brown Epipolar Curve Visualization library

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )

if (VXL_VGUI_FOUND)

set(becld_vis_sources
   becld_episeg_soview2D.cxx     becld_episeg_soview2D.h
   becld_episeg_displayer.cxx    becld_episeg_displayer.h
   becld_episeg_tools.cxx        becld_episeg_tools.h
)

#aux_source_directory(Templates becld_vis_sources)

add_library(becld_vis ${becld_vis_sources})

target_link_libraries(becld_vis becld_pro bvis1)


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)


endif (VXL_VGUI_FOUND)
