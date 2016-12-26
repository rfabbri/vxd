if (VXL_VGUI_FOUND)
include_directories( ${lems_SOURCE_DIR}/breye1 )
endif (VXL_VGUI_FOUND)

include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bseg )
include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${VXL_GEL_INCLUDE_DIR}/mrc )
include_directories(${VXL_OXL_INCLUDE_DIR} )
include_directories(${VXL_MUL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/brcv/seg )
include_directories( ${lems_SOURCE_DIR}/brcv/mvg)
include_directories( ${lems_SOURCE_DIR}/brcv/mvg/bmvl)
include_directories( ${lems_SOURCE_DIR}/brcv/rec )
include_directories( ${lems_SOURCE_DIR}/basic )


include_directories( ${lems_SOURCE_DIR}/contrib/rfabbri )

# Personal Multiview Lib

set( bmcsd_sources
   bmcsd_util.hxx       bmcsd_util_defs.cxx bmcsd_util.h
   bmcsd_curves.cxx      bmcsd_curves.h
   bmcsd_subpixel_point_set.cxx bmcsd_subpixel_point_set.h
   bmcsd_epi_interceptor.cxx bmcsd_epi_interceptor.h
   bmcsd_intersection_sets.cxx bmcsd_intersection_sets.h
   bmcsd_discrete_corresp.cxx      bmcsd_discrete_corresp.h
   bmcsd_discrete_corresp_3.cxx      bmcsd_discrete_corresp_3.h
   bmcsd_discrete_corresp_n.cxx      bmcsd_discrete_corresp_n.h
   bmcsd_dist.cxx       bmcsd_dist.h
   bmcsd_view_set.cxx bmcsd_view_set.h bmcsd_stereo_views_sptr.h 
   bmcsd_curve_3d_attributes.cxx bmcsd_curve_3d_attributes.h 
   bmcsd_curve_3d_sketch.cxx bmcsd_curve_3d_sketch.h 
)

aux_source_directory(Templates bmcsd_sources)
aux_source_directory(Templates bvis1_sources)

add_library(mw ${bmcsd_sources})

target_link_libraries(mw vpgl bsold_algo bnld vil buld vbl_io)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

add_subdirectory(algo)

add_subdirectory(pro)

add_subdirectory(cmd)

if (VXL_VGUI_FOUND)
  add_subdirectory(vis)
  add_subdirectory(app)
endif (VXL_VGUI_FOUND)

option(BUILD_MATLAB_MEX "Build Matlab mex files" NO)

if(BUILD_MATLAB_MEX)
  add_subdirectory(mex)
endif(BUILD_MATLAB_MEX)
