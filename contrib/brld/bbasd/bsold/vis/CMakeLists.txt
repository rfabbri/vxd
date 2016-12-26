# bbasd/bsold/vis/CMakeLists.txt
# Development Brown Spatial Object Visualization Library


if (VXL_VGUI_FOUND)

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )

set( bsold_vis_sources

bsold_soview2D_circ_arc.cxx   bsold_soview2D_circ_arc.h
#bsold_circ_arc_tool.cxx       bsold_circ_arc_tool.h
#bsold_vsol2D_displayer.cxx    bsold_vsol2D_displayer.h
#bsold2D_tableau.cxx           bsold2D_tableau.h
#bsold2D_tableau_sptr.h

bsold_cut_contour_tool.cxx     bsold_cut_contour_tool.h
)

aux_source_directory(Templates bsold_vis_sources)

add_library(bsold_vis  ${bsold_vis_sources})
target_link_libraries(bsold_vis bvis1 bgui bsold)



endif (VXL_VGUI_FOUND)
