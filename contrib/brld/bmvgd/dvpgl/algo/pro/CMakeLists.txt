# This is brcv/mvg/dvpgl/pro/algo/CMakeLists.txt

include_directories( ${lems_SOURCE_DIR}/breye1 )

set( dvpgl_algo_pro_sources
   dvpgl_vsol_lens_warp_process.cxx    dvpgl_vsol_lens_warp_process.h
   dvpgl_image_lens_warp_process.cxx   dvpgl_image_lens_warp_process.h
)


add_library(dvpgl_algo_pro ${dvpgl_algo_pro_sources})

target_link_libraries(dvpgl_algo_pro vpgl_algo bpgl_algo vidpro1 )


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)

