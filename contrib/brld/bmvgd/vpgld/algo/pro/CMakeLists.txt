# This is bmvgd/vpgld/pro/algo/CMakeLists.txt

include_directories( ${lems_SOURCE_DIR}/breye1 )

set( vpgld_algo_pro_sources
   vpgld_vsol_lens_warp_process.cxx    vpgld_vsol_lens_warp_process.h
   vpgld_image_lens_warp_process.cxx   vpgld_image_lens_warp_process.h
)


add_library(vpgld_algo_pro ${vpgld_algo_pro_sources})

target_link_libraries(vpgld_algo_pro vpgl_algo bpgl_algo vidpro1 )


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)

