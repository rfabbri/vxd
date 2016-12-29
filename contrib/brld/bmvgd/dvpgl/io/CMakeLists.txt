# This is brcv/mvg/dvpgl/io/CMakeLists.txt


set(dvpgl_io_sources
#    dpgl_io_camera.h               vpgl_io_camera.hxx      vpgl_io_camera.cxx
#    dpgl_io_proj_camera.h             vpgl_io_proj_camera.hxx
#    dpgl_io_perspective_camera.h      vpgl_io_perspective_camera.hxx
    dvpgl_io_cameras.h   dvpgl_io_cameras.hxx
    dvpgl_perspective_camera_vsl.h dvpgl_perspective_camera_vsl.hxx
    dvpgl_proj_camera_vsl.h dvpgl_proj_camera_vsl.hxx
)

aux_source_directory(Templates dvpgl_io_sources)

add_library(dvpgl_io ${dvpgl_io_sources})

target_link_libraries(dvpgl_io vpgl_io vsl vbl_io)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
