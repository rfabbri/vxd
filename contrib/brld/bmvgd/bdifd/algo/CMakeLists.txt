# This is bmvgd/bdifd/algo/CMakeLists.txt
# Brown Multiview Differential Geometry Algorithms Library

set(bdifd_algo_sources
  bdifd_data.cxx bdifd_data.h
  bdifd_transfer.cxx bdifd_transfer.h
)

add_library(bdifd_algo ${bdifd_algo_sources})

target_link_libraries(bdifd_algo bdifd)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
