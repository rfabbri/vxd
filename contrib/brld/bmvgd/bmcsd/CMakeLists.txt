# Curve Multiview Lib (Curve Sketch System Base, VXD version)

set( bmcsd_sources
   bmcsd_util.hxx       bmcsd_util_defs.cxx bmcsd_util.h
   bmcsd_discrete_corresp.cxx      bmcsd_discrete_corresp.h
   bmcsd_discrete_corresp_3.cxx      bmcsd_discrete_corresp_3.h
   bmcsd_discrete_corresp_n.cxx      bmcsd_discrete_corresp_n.h
   bmcsd_view_set.cxx bmcsd_view_set.h bmcsd_stereo_views_sptr.h 
   bmcsd_curve_3d_attributes.cxx bmcsd_curve_3d_attributes.h 
   bmcsd_curve_3d_sketch.cxx bmcsd_curve_3d_sketch.h 
)

aux_source_directory(Templates bmcsd_sources)
aux_source_directory(Templates bvis1_sources)

add_library(bmcsd ${bmcsd_sources})

target_link_libraries(bmcsd vpgl bsold_algo bnld vil buld vbl_io)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
