# basic/dbil/pro/CMakeLists.txt
# Development Brown Image Processing Extention Library

include_directories( ${lems_SOURCE_DIR}/breye1 )
include_directories(${VXL_BRL_INCLUDE_DIR}/bbas )
include_directories(${VXL_BRL_INCLUDE_DIR}/bseg )
include_directories(${VXL_GEL_INCLUDE_DIR} )
include_directories(${VXL_OXL_INCLUDE_DIR} )

set(dbil_pro_sources
                                        dbil_pro_register.h 
   dbil_gaussian_blur_process.cxx       dbil_gaussian_blur_process.h
   dbil_diff_process.cxx                dbil_diff_process.h
   dbil_morphology_filters.cxx          dbil_morphology_filters.h
                                        dbil_convert_filters.h
                                        dbil_math_filters.h
                                        dbil_gauss_filter.h
                                        dbil_sobel_1x3_filter.h
                                        dbil_subpix_edge_filter.h
                                        dbil_grad_sqr_filter.h
                                        dbil_transform3_1_filter.h
   dbil_monotone_process.cxx            dbil_monotone_process.h
   dbil_wshed2d_process.cxx             dbil_wshed2d_process.h
   dbil_distance_transform_process.cxx  dbil_distance_transform_process.h
   dbil_harr_wavelet_process.cxx        dbil_harr_wavelet_process.h
   dbil_blob_tracker_process.cxx        dbil_blob_tracker_process.h
dbil_osl_canny_edges_process.h  dbil_osl_canny_edges_process.cxx
)


aux_source_directory(Templates dbil_pro_sources)

add_library(dbil_pro ${dbil_pro_sources})

target_link_libraries(dbil_pro dbil_algo vidpro1 bpro1 bil vil vil_algo dbpro bil_algo osl )

