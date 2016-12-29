# This is brcv/mvg/dvpgl/pro/CMakeLists.txt

include_directories( ${lems_SOURCE_DIR}/breye1 )

set( dvpgl_pro_sources
   dvpgl_fm_storage.cxx  dvpgl_fm_storage.h  dvpgl_fm_storage_sptr.h
   dvpgl_camera_storage.cxx  dvpgl_camera_storage.h  dvpgl_camera_storage_sptr.h
)


add_library(dvpgl_pro ${dvpgl_pro_sources})

target_link_libraries(dvpgl_pro vpgl_io dvpgl_io vidpro1 dvpgl_algo_pro )


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)

