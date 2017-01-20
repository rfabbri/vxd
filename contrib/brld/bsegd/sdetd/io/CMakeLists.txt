# This is bsegd/sdetd/io/CMakeLists.txt


set(sdetd_io_sources
  sdetd_load_edg.cxx sdetd_load_edg.h
  sdetd_cvlet_map_io.cxx sdetd_cvlet_map_io.h
)

# aux_source_directory(Templates sdetd_io_sources)

add_library(sdetd_io ${sdetd_io_sources})

target_link_libraries(sdetd_io sdet)

if( BUILD_TESTING )
#  add_subdirectory(tests)
endif( BUILD_TESTING )
