# Brown Multiview Curve Stereo Lib (Curve Sketch System Algorithms, VXD version)

include_directories(${VXL_BRL_INCLUDE_DIR})

set(bmcsd_algo_sources
  bmcsd_curve_stereo.cxx bmcsd_curve_stereo.h
  bmcsd_dt_curve_stereo.cxx bmcsd_dt_curve_stereo.h
  bmcsd_algo_util.cxx bmcsd_algo_util.h
)

add_library(bmcsd_algo ${bmcsd_algo_sources})

target_link_libraries(bmcsd_algo bmcsd vpgld_algo becld bdifd bspid)

# if( BUILD_TESTING )
#   add_subdirectory(tests)
# endif( BUILD_TESTING )
