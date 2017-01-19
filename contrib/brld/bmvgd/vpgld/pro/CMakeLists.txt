# This is bmvgd/vpgld/pro/CMakeLists.txt

set( vpgld_pro_sources
   vpgld_fm_storage.cxx  vpgld_fm_storage.h  vpgld_fm_storage_sptr.h
   vpgld_camera_storage.cxx  vpgld_camera_storage.h  vpgld_camera_storage_sptr.h
   vpgld_load_camera_process.cxx vpgld_load_camera_process.h
)


add_library(vpgld_pro ${vpgld_pro_sources})

target_link_libraries(vpgld_pro vpgl_io vpgld_io vidpro1 vpgld_algo_pro bdifd)
