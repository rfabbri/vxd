# basic/bild/vis/CMakeLists.txt
# Development Brown Image Visualization Extention Library

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bseg )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )

if (VXL_VGUI_FOUND)

set(bild_vis_sources 

#tools

   bild_image_tool.h                       bild_image_tool.cxx
   bild_image_intensity_inspector.h        bild_image_intensity_inspector.cxx
   bild_image_patch_gradient_tool.h        bild_image_patch_gradient_tool.cxx
   bild_image_region_stats_tool.h          bild_image_region_stats_tool.cxx

   bild_image_observer.h
)


#aux_source_directory(Templates bvis1_sources)

add_library(bild_vis ${bild_vis_sources})

target_link_libraries(bild_vis bvis1 bil_algo vgl)

endif (VXL_VGUI_FOUND)
