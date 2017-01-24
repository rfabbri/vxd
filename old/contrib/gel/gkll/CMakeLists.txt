# gel/gkll/CMakeLists.txt

doxygen_add_library(contrib/gel/gkll
  DEPENDS contrib/gel/vgel
  PACKAGE contrib/gel
  DESCRIPTION "Kanade-Lucas_Tomasi feature tracker"
  )

set(gkll_sources
 gkll_kl.cxx              gkll_kl.h
 gkll_kl_params.cxx       gkll_kl_params.h
)
aux_source_directory(Templates gkll_sources)

if(BUILD_OXL)
set(gkll_sources ${gkll_sources}
 gkll_multi_view_data.cxx gkll_multi_view_data.h
 gkll_multi_view_data_vertex_sptr.h
)
endif(BUILD_OXL)

add_library(gkll ${gkll_sources})
install_targets(/lib gkll)
INSTALL_NOBASE_HEADER_FILES(/include/vxl/contrib/gel/gkll ${gkll_sources})

if(BUILD_OXL)
 include_directories(${OXL_INCLUDE_DIR})
 TARGET_link_libraries(gkll mvl)
endif(BUILD_OXL)

TARGET_link_libraries(gkll vgel_kl vtol vil1 vbl)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
