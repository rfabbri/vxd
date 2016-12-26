# basic/dbpro/vis/CMakeLists.txt
# Development Brown Processing Visualization Extention Library


if (VXL_VGUI_FOUND)

set(dbpro_vis_sources

  dbpro_run_tableau.cxx             dbpro_run_tableau.h           dbpro_run_tableau_sptr.h
  dbpro_basic_gui_observers.h

)


#aux_source_directory(Templates dbpro_vis_sources)

add_library(dbpro_vis  ${dbpro_vis_sources})

target_link_libraries(dbpro_vis dbpro vgui)

endif (VXL_VGUI_FOUND)
