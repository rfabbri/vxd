# gel/vmal/CMakeLists.txt

doxygen_add_library(contrib/gel/vmal
  DEPENDS contrib/gel/vgel contrib/oxl/osl
  PACKAGE contrib/gel
  DESCRIPTION "Multi-View Line matching"
  )
if(BUILD_OXL)
set(vmal_sources
 vmal_dense_matching.cxx    vmal_dense_matching.h
 vmal_homog2d.cxx           vmal_homog2d.h
 vmal_lines_correlation.cxx vmal_lines_correlation.h
 vmal_operators.cxx         vmal_operators.h
 vmal_refine_lines.cxx      vmal_refine_lines.h
 vmal_track_lines.cxx       vmal_track_lines.h
 vmal_convert_vtol.cxx      vmal_convert_vtol.h
)
aux_source_directory(Templates vmal_sources)

# vmal_kl not compiled until old vidl dependency can be removed.
#if (BUILD_UNMAINTAINED_LIBRARIES OR BUILD_FOR_VXL_DASHBOARD)
#set(vmal_sources ${vmal_sources}
# vmal_kl.cxx                vmal_kl.h
# vmal_kl_params.cxx         vmal_kl_params.h
#)
#endif (BUILD_UNMAINTAINED_LIBRARIES OR BUILD_FOR_VXL_DASHBOARD)


 include_directories(${OXL_INCLUDE_DIR})
 set(vmal_sources ${vmal_sources}
 vmal_convert_osl.cxx       vmal_convert_osl.h
 vmal_rectifier.cxx         vmal_rectifier.h
 vmal_multi_view_data.cxx   vmal_multi_view_data.h
 vmal_multi_view_data_edge_sptr.h
 vmal_multi_view_data_vertex_sptr.h
)


add_library(vmal ${vmal_sources})
install_targets(/lib vmal)
INSTALL_NOBASE_HEADER_FILES(/include/vxl/contrib/gel/vmal ${vmal_sources})

if(BUILD_OXL)
 TARGET_link_libraries(vmal mvl osl vil)
endif(BUILD_OXL)

if (BUILD_UNMAINTAINED_LIBRARIES OR BUILD_FOR_VXL_DASHBOARD)
 TARGET_link_libraries(vmal vgel_kl)
endif (BUILD_UNMAINTAINED_LIBRARIES OR BUILD_FOR_VXL_DASHBOARD)

TARGET_link_libraries(vmal vtol vnl_algo vil1 vnl vbl)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
endif(BUILD_OXL)
