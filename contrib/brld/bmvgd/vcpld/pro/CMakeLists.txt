include_directories(${VXL_BRL_INCLUDE_DIR})
include_directories(${VXL_GEL_INCLUDE_DIR} )
include_directories(${VXL_GEL_INCLUDE_DIR}/mrc )

set(vcpld_pro_sources
  vcpld_bundle_adjust_driver.cxx vcpld_bundle_adjust_driver.h
  vcpld_bundle_adjust_sink.cxx vcpld_bundle_adjust_sink.h
)

add_library(vcpld_pro ${vcpld_pro_sources})

target_link_libraries(vcpld_pro vcpld_algo bmcsd_pro)
