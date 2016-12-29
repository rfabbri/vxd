# This is bmvgd/vpgld/io/CMakeLists.txt


set(vpgld_io_sources
#    dpgl_io_camera.h               vpgl_io_camera.hxx      vpgl_io_camera.cxx
#    dpgl_io_proj_camera.h             vpgl_io_proj_camera.hxx
#    dpgl_io_perspective_camera.h      vpgl_io_perspective_camera.hxx
    vpgld_io_cameras.h   vpgld_io_cameras.hxx
    vpgld_perspective_camera_vsl.h vpgld_perspective_camera_vsl.hxx
    vpgld_proj_camera_vsl.h vpgld_proj_camera_vsl.hxx
)

aux_source_directory(Templates vpgld_io_sources)

add_library(vpgld_io ${vpgld_io_sources})

target_link_libraries(vpgld_io vpgl_io vsl vbl_io)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
