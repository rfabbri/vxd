# This is bmvgd/vpgld/pro/CMakeLists.txt

include_directories( ${lems_SOURCE_DIR}/breye1 )

set( vpgld_pro_sources
   vpgld_fm_storage.cxx  vpgld_fm_storage.h  vpgld_fm_storage_sptr.h
   vpgld_camera_storage.cxx  vpgld_camera_storage.h  vpgld_camera_storage_sptr.h
)


add_library(vpgld_pro ${vpgld_pro_sources})

target_link_libraries(vpgld_pro vpgl_io vpgld_io vidpro1 vpgld_algo_pro )


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)

