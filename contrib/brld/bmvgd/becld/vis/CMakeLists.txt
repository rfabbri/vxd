# This is brcv/mvg/dbecl/vis/CMakeLists.txt
# Brown Epipolar Curve Visualization library

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )

if (VXL_VGUI_FOUND)

set(dbecl_vis_sources
   dbecl_episeg_soview2D.cxx     dbecl_episeg_soview2D.h
   dbecl_episeg_displayer.cxx    dbecl_episeg_displayer.h
   dbecl_episeg_tools.cxx        dbecl_episeg_tools.h
)

#aux_source_directory(Templates dbecl_vis_sources)

add_library(dbecl_vis ${dbecl_vis_sources})

target_link_libraries(dbecl_vis dbecl_pro bvis1)


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)


endif (VXL_VGUI_FOUND)
