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

set( mw_sources
   mw_util.hxx       mw_util_defs.cxx mw_util.h
   mw_curves.cxx      mw_curves.h
   mw_subpixel_point_set.cxx mw_subpixel_point_set.h
   mw_epi_interceptor.cxx mw_epi_interceptor.h
   mw_intersection_sets.cxx mw_intersection_sets.h
   mw_discrete_corresp.cxx      mw_discrete_corresp.h
   mw_discrete_corresp_3.cxx      mw_discrete_corresp_3.h
   mw_discrete_corresp_n.cxx      mw_discrete_corresp_n.h
   mw_dist.cxx       mw_dist.h
   dbmcs_view_set.cxx dbmcs_view_set.h dbmcs_stereo_views_sptr.h 
   dbmcs_curve_3d_attributes.cxx dbmcs_curve_3d_attributes.h 
   dbmcs_curve_3d_sketch.cxx dbmcs_curve_3d_sketch.h 
)

aux_source_directory(Templates mw_sources)
aux_source_directory(Templates bvis1_sources)

add_library(mw ${mw_sources})

target_link_libraries(mw vpgl dbsol_algo dbnl vil dbul vbl_io)

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
