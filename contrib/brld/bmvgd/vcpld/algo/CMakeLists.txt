# Multiview Curve Photogrammetry Algorithms
include_directories(${VXL_BRL_INCLUDE_DIR})

set(vcpld_algo_sources
  vcpld_distmap_bundle_adjust.cxx vcpld_distmap_bundle_adjust.h
)

add_library(vcpld_algo ${vcpld_algo_sources})

target_link_libraries(vcpld_algo sdet vpgl)

# if( BUILD_TESTING )
#   add_subdirectory(tests)
# endif( BUILD_TESTING )
