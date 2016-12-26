# bsold/io/Cmakelists.txt

set(bsold_io_sources
  bsold_file_io_extras.h bsold_file_io_extras.cxx 
)

aux_source_directory(Templates bsold_io_sources)

add_library(bsold_io ${bsold_io_sources})

target_link_libraries(bsold_io bsold vil)

#if( BUILD_TESTING )
#  add_subdirectory(tests)
#endif()
