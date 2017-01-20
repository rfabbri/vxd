include_directories(${VXL_BRL_INCLUDE_DIR})

set(bmcsd_pro_sources
  bmcsd_load_camera_process.cxx bmcsd_load_camera_process.h

  bmcsd_stereo_filter.cxx bmcsd_load_camera_source.h bmcsd_load_cemv_source.h bmcsd_load_curvelet_source.h bmcsd_load_edg_source.h bmcsd_load_vsol_polyline_source.h bmcsd_fragment_tangents_filter.h bmcsd_stereo_filter.h

  bmcsd_stereo_driver.cxx bmcsd_stereo_driver.h
)

add_library(bmcsd_pro ${bmcsd_pro_sources})

target_link_libraries(bmcsd_pro bmcsd_algo bpro1 bprod vpgld_pro vpgld_io bdifd_algo vsl)

