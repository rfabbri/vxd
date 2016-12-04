# This is brcv/mvg/dbdif/algo/CMakeLists.txt
# Brown Multiview Differential Geometry Algorithms Library

set(dbdif_algo_sources
  dbdif_data.cxx dbdif_data.h
  dbdif_transfer.cxx dbdif_transfer.h
)

add_library(dbdif_algo ${dbdif_algo_sources})

target_link_libraries(dbdif_algo dbdif)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
