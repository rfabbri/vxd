# basic/dbsol/vis/CMakeLists.txt
# Development Brown Spatial Object Visualization Library


if (VXL_VGUI_FOUND)

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )

set( dbsol_vis_sources

dbsol_soview2D_circ_arc.cxx   dbsol_soview2D_circ_arc.h
#dbsol_circ_arc_tool.cxx       dbsol_circ_arc_tool.h
#dbsol_vsol2D_displayer.cxx    dbsol_vsol2D_displayer.h
#dbsol2D_tableau.cxx           dbsol2D_tableau.h
#dbsol2D_tableau_sptr.h

dbsol_cut_contour_tool.cxx     dbsol_cut_contour_tool.h
)

aux_source_directory(Templates dbsol_vis_sources)

add_library(dbsol_vis  ${dbsol_vis_sources})
target_link_libraries(dbsol_vis bvis1 bgui dbsol)



endif (VXL_VGUI_FOUND)
