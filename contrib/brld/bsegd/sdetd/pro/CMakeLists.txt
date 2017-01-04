# This is brcv/seg/dbdet/pro/CMakeLists.txt
# Brown Detection Processing library

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories( ${lems_SOURCE_DIR}/basic )

set(dbdet_pro_sources
   dbdet_lowe_keypoint_process.cxx    dbdet_lowe_keypoint_process.h
#  dbdet_keypoint_match_process.cxx   dbdet_keypoint_match_process.h
   dbdet_keypoint_storage.cxx         dbdet_keypoint_storage.h         
   dbdet_keypoint_storage_sptr.h

   dbdet_lvwr_process.cxx             dbdet_lvwr_process.h
   
   dbdet_contour_tracer_process.h     dbdet_contour_tracer_process.cxx
   dbdet_blob_finder_process.h        dbdet_blob_finder_process.cxx
   
   dbdet_image_gradient_process.h     dbdet_image_gradient_process.cxx
   
   dbdet_generic_edge_detector_process.cxx  
   dbdet_generic_edge_detector_process.h

   dbdet_generic_color_edge_detector_process.cxx
   dbdet_generic_color_edge_detector_process.h
   
   dbdet_generic_multiscale_edge_detector_process.cxx
   dbdet_generic_multiscale_edge_detector_process.h
   
   dbdet_generic_multicue_edge_detector_process.cxx
   dbdet_generic_multicue_edge_detector_process.h
   
   dbdet_mask_edges_process.cxx  
   dbdet_mask_edges_process.h
  
   dbdet_prune_curves_process.cxx  
   dbdet_prune_curves_process.h
   
   dbdet_third_order_edge_detector_process.h
   dbdet_third_order_edge_detector_process.cxx

   dbdet_third_order_edge_detector_vxl_process.h
   dbdet_third_order_edge_detector_vxl_process.cxx
   
   dbdet_third_order_color_edge_detector_process.cxx
   dbdet_third_order_color_edge_detector_process.h
   
   dbdet_composite_edge_detection_process.h
   dbdet_composite_edge_detection_process.cxx
   
   dbdet_detect_topographic_curves_process.cxx  
   dbdet_detect_topographic_curves_process.h
   
   dbdet_compass_edge_detector_process.cxx
   dbdet_compass_edge_detector_process.h
   
   dbdet_compass_color_edge_detector_process.cxx
   dbdet_compass_color_edge_detector_process.h
   
   dbdet_edgemap_from_edge_image_process.cxx
   dbdet_edgemap_from_edge_image_process.h
   
   dbdet_convert_edgemap_to_image_process.cxx
   dbdet_convert_edgemap_to_image_process.h
   
   dbdet_prune_edgemap_process.cxx
   dbdet_prune_edgemap_process.h
   
   dbdet_edgemap_storage.h            dbdet_edgemap_storage.cxx
   dbdet_edgemap_storage_sptr.h
   dbdet_load_edg_process.cxx         dbdet_load_edg_process.h
   dbdet_save_edg_process.cxx         dbdet_save_edg_process.h
   dbdet_save_edg_k_process.cxx       dbdet_save_edg_k_process.h
   
   dbdet_sel_storage.cxx              dbdet_sel_storage.h         
   dbdet_sel_storage_sptr.h
   dbdet_sel_process.h                dbdet_sel_process.cxx
   
   dbdet_load_cvlet_map_process.h     dbdet_load_cvlet_map_process.cxx
   dbdet_save_cvlet_map_process.h     dbdet_save_cvlet_map_process.cxx
   
   dbdet_sel_extract_contours_process.h dbdet_sel_extract_contours_process.cxx
   
   dbdet_generic_linker_process.h     dbdet_generic_linker_process.cxx
   dbdet_hybrid_linker_process.h      dbdet_hybrid_linker_process.cxx
   
   dbdet_subsample_edges.h            dbdet_subsample_edges.cxx
   
   dbdet_combine_contour_cues_process.cxx
   dbdet_combine_contour_cues_process.h
   
   dbdet_load_cem_process.h           dbdet_load_cem_process.cxx
   dbdet_save_cem_process.h           dbdet_save_cem_process.cxx
   
   dbdet_seg3d_info_storage.cxx       dbdet_seg3d_info_storage.h
   dbdet_seg3d_info_storage_sptr.h
   dbdet_new_seg3d_info_xml_process.cxx  dbdet_new_seg3d_info_xml_process.h
   dbdet_save_seg3d_info_xml_process.cxx  dbdet_save_seg3d_info_xml_process.h
   dbdet_load_seg3d_info_xml_process.cxx  dbdet_load_seg3d_info_xml_process.h

   dbdet_evaluation_process.h      dbdet_evaluation_process.cxx
   dbdet_prune_fragments_Logistic_Regression.h	dbdet_prune_fragments_Logistic_Regression.cxx
   
   dbdet_save_edg_seq_process.h dbdet_save_edg_seq_process.cxx
   dbdet_save_cem_seq_process.h dbdet_save_cem_seq_process.cxx
)

if(OCTAVE_FOUND)
    set(dbdet_pro_sources
        ${dbdet_pro_sources}
        dbdet_pb_edge_detector_process.h dbdet_pb_edge_detector_process.cxx
        dbdet_kovesi_edge_linker_process.h dbdet_kovesi_edge_linker_process.cxx
    )
endif(OCTAVE_FOUND)

#aux_source_directory(Templates dbdet_pro_sources)

add_library(dbdet_pro ${dbdet_pro_sources})

target_link_libraries(dbdet_pro dbdet_algo dbdet_xio vidpro1 bvis1 vgui bil)

#if(BUILD_TESTING)
#  add_subdirectory(tests)
#endif(BUILD_TESTING)

