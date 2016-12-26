# basic/bild/pro/CMakeLists.txt
# Development Brown Image Processing Extention Library

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories(${VXL_BRL_INCLUDE_DIR}/bbas )
include_directories(${VXL_BRL_INCLUDE_DIR}/bseg )
include_directories(${VXL_GEL_INCLUDE_DIR} )
include_directories(${VXL_OXL_INCLUDE_DIR} )

set(bild_pro_sources
                                        bild_pro_register.h 
   bild_gaussian_blur_process.cxx       bild_gaussian_blur_process.h
   bild_diff_process.cxx                bild_diff_process.h
   bild_morphology_filters.cxx          bild_morphology_filters.h
                                        bild_convert_filters.h
                                        bild_math_filters.h
                                        bild_gauss_filter.h
                                        bild_sobel_1x3_filter.h
                                        bild_subpix_edge_filter.h
                                        bild_grad_sqr_filter.h
                                        bild_transform3_1_filter.h
   bild_monotone_process.cxx            bild_monotone_process.h
   bild_wshed2d_process.cxx             bild_wshed2d_process.h
   bild_distance_transform_process.cxx  bild_distance_transform_process.h
   bild_harr_wavelet_process.cxx        bild_harr_wavelet_process.h
   bild_blob_tracker_process.cxx        bild_blob_tracker_process.h
bild_osl_canny_edges_process.h  bild_osl_canny_edges_process.cxx
)


aux_source_directory(Templates bild_pro_sources)

add_library(bild_pro ${bild_pro_sources})

target_link_libraries(bild_pro bild_algo vidpro1 bpro1 bil vil vil_algo dbpro bil_algo osl )

