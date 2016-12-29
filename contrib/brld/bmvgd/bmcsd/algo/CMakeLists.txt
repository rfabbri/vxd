# Brown Multiview Curve Stereo Lib (Curve Sketch System Algorithms, VXD version)

set(bmcsd_algo_sources
  bmcsd_curve_stereo.cxx bmcsd_curve_stereo.h
)

add_library(bmcsd_algo ${bmcsd_algo_sources})

target_link_libraries(bmcsd_algo bmcsd)

# if( BUILD_TESTING )
#   add_subdirectory(tests)
# endif( BUILD_TESTING )
