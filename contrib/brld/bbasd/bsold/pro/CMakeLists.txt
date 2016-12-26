# bbasd/bsold/pro/CMakeLists.txt
# Development Brown Spatial Object Processing Library

include_directories( ${lems_SOURCE_DIR}/breye1 )

set(bsold_pro_sources
   bsold_curve_sampling_process.cxx    bsold_curve_sampling_process.h
   bsold_translate_2d_process.cxx      bsold_translate_2d_process.h
   bsold_roi_image_process.cxx         bsold_roi_image_process.h
   bsold_scan_polygon_process.cxx      bsold_scan_polygon_process.h
   bsold_smooth_contours_process.cxx   bsold_smooth_contours_process.h
   bsold_merge_storage_process.cxx     bsold_merge_storage_process.h
   bsold_save_ps_file_process.cxx      bsold_save_ps_file_process.h
   bsold_fit_circ_arc_spline_process.cxx    bsold_fit_circ_arc_spline_process.h
)


# aux_source_directory(Templates bsold_pro_sources)

add_library(bsold_pro  ${bsold_pro_sources})

target_link_libraries(bsold_pro bsold_algo bsold_io vidpro1 )

