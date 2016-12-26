# /bbasd/buld/CMakeLists.txt
# Development Brown Utility Library
include_directories( ${VXL_BRL_INCLUDE_DIR} )

set(buld_sources 
    buld_solar_position.h   buld_solar_position.cxx
    buld_dir_file.h         buld_dir_file.cxx
    buld_parse_simple_file.h       buld_parse_simple_file.cxx
    buld_arg.h buld_arg.cxx
    buld_handle_files_embedded_in_vxl_source.h buld_handle_files_embedded_in_vxl_source.cxx
    buld_random.h    buld_random.cxx
   )

if(OCTAVE_FOUND)
    set(buld_sources
        ${buld_sources}
        buld_octave.h buld_octave.cxx
    )
endif(OCTAVE_FOUND)

#aux_source_directory(Templates buld_sources)

add_library(buld  ${buld_sources})

target_link_libraries(buld vul vpl)

if(OCTAVE_FOUND)
    target_link_libraries(buld octinterp bxml dbxml)
endif(OCTAVE_FOUND)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

add_subdirectory( cmdline )
