# Brown Detection Libraries
# contains Livewire Manual Segmentation Algorithm 
# brcv/seg/dbdet/CMakeLists.txt
project(dbdet)

# include directories into here if needed
include_directories( ${VXL_VCL_INCLUDE_DIR} )
include_directories( ${VXL_OXL_INCLUDE_DIR} )
include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${VXL_TBL_INCLUDE_DIR} )
include_directories( ${VXL_MUL_INCLUDE_DIR} )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas/bil/algo )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bseg )


option(DBDET_USE_APPROX_SINE "Use approximate sines in dbdet_curve_model for faster edge linking" NO)


configure_file( ${dbdet_SOURCE_DIR}/dbdet_config.h.in
                ${dbdet_BINARY_DIR}/dbdet_config.h )


set(dbdet_sources
   lvwr/dbdet_lvwr.cxx         lvwr/dbdet_lvwr.h
   lvwr/dbdet_lvwr_params.cxx  lvwr/dbdet_lvwr_params.h
   lvwr/dbdet_lvwr_fit.cxx         lvwr/dbdet_lvwr_fit.h

   tracer/dbdet_contour_tracer.cxx tracer/dbdet_contour_tracer.h 
   tracer/dbdet_contour_tracer_sptr.h 
   tracer/dbdet_contour_tracer_core.h
   tracer/dbdet_contour_tracer_core2.cxx 
   tracer/dbdet_contour_tracer_core.cxx 
   tracer/dbdet_contour_tracer_curvature_smoothing.cxx 

   dbdet_scale_space_peaks.cxx   dbdet_scale_space_peaks.h
   dbdet_keypoint.cxx            dbdet_keypoint.h           dbdet_keypoint_sptr.h
   dbdet_keypoint_corr3d.cxx     dbdet_keypoint_corr3d.h    dbdet_keypoint_corr3d_sptr.h
   dbdet_lowe_keypoint.cxx       dbdet_lowe_keypoint.h
   dbdet_frenet_keypoint.cxx     dbdet_frenet_keypoint.h
   dbdet_surf_keypoint.h         dbdet_surf_keypoint.cxx
   
   edge/dbdet_kernel.h
   edge/dbdet_gaussian_kernel.h 
   edge/dbdet_interp_kernel.h
   edge/dbdet_orient_kernel.h
   edge/dbdet_edgemap.h              edge/dbdet_edgemap_sptr.h
 
   sel/dbdet_sel_utils.h
   sel/dbdet_appearance.h            sel/dbdet_appearance.cxx
   sel/dbdet_edgel.h                 sel/dbdet_edgel.cxx
   sel/dbdet_curve_model.h           sel/dbdet_curve_model.cxx
   sel/dbdet_curvelet.h              sel/dbdet_curvelet.cxx
   sel/dbdet_edgel_link_graph.h
   sel/dbdet_curve_fragment_graph.h
   sel/dbdet_curvelet_map.h          sel/dbdet_curvelet_map.cxx
   sel/dbdet_hyp_tree_graph.h
   sel/dbdet_EHT.h
   sel/dbdet_CFTG.h

   filter/dbdet_texton_classifier.h  filter/dbdet_texton_classifier.cxx
   filter/dbdet_filter_bank.h        filter/dbdet_filter_bank.cxx
   
   #bel/Point.h               
   #bel/Curve.cxx               bel/Curve.h
   #bel/FromEndJunction.cxx     bel/FromEndJunction.h
   #bel/List.h
   #bel/LVimage.h
   #bel/Table1D.h               bel/Table2D.h

   dbdet_seg3d_info.cxx        dbdet_seg3d_info.h         dbdet_seg3d_info_sptr.h

)

aux_source_directory(Templates dbdet_sources)
aux_source_directory(sel/Templates dbdet_sources)

add_library(dbdet ${dbdet_sources})

# add the required libraries into this list
target_link_libraries( dbdet vil1 vil bil_algo dbnl_algo osl vepl vepl vdgl vnl_io bdgl dbgl_algo brip pdf1d mbl)


if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)

add_subdirectory(algo)

add_subdirectory(pro)

add_subdirectory( xio )

if (VXL_VGUI_FOUND)
  add_subdirectory(vis)
endif (VXL_VGUI_FOUND)

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif( BUILD_EXAMPLES )


