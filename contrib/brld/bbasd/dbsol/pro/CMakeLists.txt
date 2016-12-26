# basic/dbsol/pro/CMakeLists.txt
# Development Brown Spatial Object Processing Library

include_directories( ${lems_SOURCE_DIR}/breye1 )

set(dbsol_pro_sources
   dbsol_curve_sampling_process.cxx    dbsol_curve_sampling_process.h
   dbsol_translate_2d_process.cxx      dbsol_translate_2d_process.h
   dbsol_roi_image_process.cxx         dbsol_roi_image_process.h
   dbsol_scan_polygon_process.cxx      dbsol_scan_polygon_process.h
   dbsol_smooth_contours_process.cxx   dbsol_smooth_contours_process.h
   dbsol_merge_storage_process.cxx     dbsol_merge_storage_process.h
   dbsol_save_ps_file_process.cxx      dbsol_save_ps_file_process.h
   dbsol_fit_circ_arc_spline_process.cxx    dbsol_fit_circ_arc_spline_process.h
)


# aux_source_directory(Templates dbsol_pro_sources)

add_library(dbsol_pro  ${dbsol_pro_sources})

target_link_libraries(dbsol_pro dbsol_algo vidpro1 )

