# basic/dbil/vis/CMakeLists.txt
# Development Brown Image Visualization Extention Library

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bseg )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )

if (VXL_VGUI_FOUND)

set(dbil_vis_sources 

#tools

   dbil_image_tool.h                       dbil_image_tool.cxx
   dbil_image_intensity_inspector.h        dbil_image_intensity_inspector.cxx
   dbil_image_patch_gradient_tool.h        dbil_image_patch_gradient_tool.cxx
   dbil_image_region_stats_tool.h          dbil_image_region_stats_tool.cxx

   dbil_image_observer.h
)


#aux_source_directory(Templates bvis1_sources)

add_library(dbil_vis ${dbil_vis_sources})

target_link_libraries(dbil_vis bvis1 bil_algo vgl)

endif (VXL_VGUI_FOUND)
