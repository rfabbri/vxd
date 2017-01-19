set(bmcsd_pro_sources
  bmcsd_load_camera_process.cxx bmcsd_load_camera_process.h
)

add_library(bmcsd_pro ${bmcsd_pro_sources})

target_link_libraries(bmcsd_pro bmcsd_algo bpro1 vpgld_pro vpgld_io bdifd_algo vsl)

