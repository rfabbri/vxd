# brl/vidpro1/CMakeLists.txt

project(vidpro1)

include_directories(${VXL_BRL_INCLUDE_DIR})
include_directories(${VXL_GEL_INCLUDE_DIR})
include_directories(${VXL_MUL_INCLUDE_DIR})
include_directories(${VXL_OXL_INCLUDE_DIR})
include_directories(${VXL_BRL_INCLUDE_DIR}/bseg)
include_directories(${VXL_BRL_INCLUDE_DIR}/bbas)
include_directories(${BRLD_SOURCE_DIR}/bbasd)


set(vidpro1_sources
   vidpro1_process_manager.cxx             vidpro1_process_manager.h             vidpro1_process_manager_sptr.h
   vidpro1_repository.cxx                  vidpro1_repository.h                  vidpro1_repository_sptr.h

# Video processes
   process/vidpro1_motion_process.cxx               process/vidpro1_motion_process.h
   process/vidpro1_horn_schunck_process.cxx         process/vidpro1_horn_schunck_process.h
   process/vidpro1_frame_diff_process.cxx           process/vidpro1_frame_diff_process.h
   process/vidpro1_frame_average_process.cxx        process/vidpro1_frame_average_process.h
   process/vidpro1_equalize_images_process.cxx      process/vidpro1_equalize_images_process.h

   process/vidpro1_brightness_contrast_process.cxx  process/vidpro1_brightness_contrast_process.h
   process/vidpro1_gaussian_blur_process.cxx        process/vidpro1_gaussian_blur_process.h
   process/vidpro1_resample_process.cxx             process/vidpro1_resample_process.h
   process/vidpro1_grey_image_process.cxx           process/vidpro1_grey_image_process.h
   process/vidpro1_RGB_to_IHS_process.cxx           process/vidpro1_RGB_to_IHS_process.h
   process/vidpro1_RGB_to_CIELab_process.cxx        process/vidpro1_RGB_to_CIELab_process.h
   process/vidpro1_split_into_RGB_process.cxx       process/vidpro1_split_into_RGB_process.h
   process/vidpro1_image_gradient_process.cxx       process/vidpro1_image_gradient_process.h

   process/vidpro1_VD_edge_process.cxx              process/vidpro1_VD_edge_process.h
   process/vidpro1_vsol_scale_process.cxx           process/vidpro1_vsol_scale_process.h
   process/vidpro1_harris_corners_process.cxx       process/vidpro1_harris_corners_process.h

   process/vidpro1_load_image_process.cxx           process/vidpro1_load_image_process.h
   process/vidpro1_save_image_process.cxx           process/vidpro1_save_image_process.h
   process/vidpro1_load_video_process.cxx           process/vidpro1_load_video_process.h
   process/vidpro1_save_video_process.cxx           process/vidpro1_save_video_process.h
   process/vidpro1_load_con_process.cxx             process/vidpro1_load_con_process.h
   process/vidpro1_save_con_process.cxx             process/vidpro1_save_con_process.h
   process/vidpro1_load_cem_process.cxx             process/vidpro1_load_cem_process.h
   process/vidpro1_save_cem_process.cxx             process/vidpro1_save_cem_process.h
   process/vidpro1_load_edg_process.cxx             process/vidpro1_load_edg_process.h
   process/vidpro1_save_ps_process.cxx              process/vidpro1_save_ps_process.h      
   process/vidpro1_edgeprune_process.cxx            process/vidpro1_edgeprune_process.h   
   process/vidpro1_smoothcem_process.cxx            process/vidpro1_smoothcem_process.h 
   process/vidpro1_random_del_polygon_process.h     process/vidpro1_random_del_polygon_process.cxx
   process/vidpro1_load_vsol_process.cxx            process/vidpro1_load_vsol_process.h
   process/vidpro1_save_vsol_process.cxx            process/vidpro1_save_vsol_process.h

   process/vidpro1_kl_process.cxx                   process/vidpro1_kl_process.h
   process/vidpro1_background_diff_process.cxx      process/vidpro1_background_diff_process.h 

   process/vidpro1_load_video_and_polys_process.cxx process/vidpro1_load_video_and_polys_process.h
   process/vidpro1_load_image_sequence_process.cxx  process/vidpro1_load_image_sequence_process.h

# Storage classes
   storage/vidpro1_image_storage.cxx        storage/vidpro1_image_storage.h        storage/vidpro1_image_storage_sptr.h
   storage/vidpro1_vsol2D_storage.cxx       storage/vidpro1_vsol2D_storage.h       storage/vidpro1_vsol2D_storage_sptr.h
   storage/vidpro1_vsol3D_storage.cxx       storage/vidpro1_vsol3D_storage.h       storage/vidpro1_vsol3D_storage_sptr.h
   storage/vidpro1_vtol_storage.cxx         storage/vidpro1_vtol_storage.h         storage/vidpro1_vtol_storage_sptr.h
   storage/vidpro1_fmatrix_storage.cxx      storage/vidpro1_fmatrix_storage.h      storage/vidpro1_fmatrix_storage_sptr.h
)


aux_source_directory(Templates vidpro1_sources)

add_library(vidpro1  ${vidpro1_sources})

if(DIRECTSHOW_FOUND)
  target_link_libraries(vidpro1 bpro1d sdet vil_io mvl dbsol_algo vnl_io vidl vgui)
else(DIRECTSHOW_FOUND)
  target_link_libraries(vidpro1 bpro1d sdet vil_io mvl dbsol_algo vnl_io vidl vgui)
endif(DIRECTSHOW_FOUND)


