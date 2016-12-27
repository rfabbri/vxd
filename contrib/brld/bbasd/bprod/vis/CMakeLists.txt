# basic/bprod/vis/CMakeLists.txt
# Development Brown Processing Visualization Extention Library


if (VXL_VGUI_FOUND)
find_package(OpenGL)

set(bprod_vis_sources

  bprod_run_tableau.cxx             bprod_run_tableau.h           bprod_run_tableau_sptr.h
  bprod_basic_gui_observers.h

)


#aux_source_directory(Templates bprod_vis_sources)

add_library(bprod_vis  ${bprod_vis_sources})

target_link_libraries(bprod_vis bprod vgui ${OPENGL_LIBRARIES})

endif (VXL_VGUI_FOUND)
