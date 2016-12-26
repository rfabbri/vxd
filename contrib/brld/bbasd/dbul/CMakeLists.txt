# /basic/dbul/CMakeLists.txt
# Development Brown Utility Library
include_directories( ${VXL_BRL_INCLUDE_DIR} )

set(dbul_sources 
    dbul_solar_position.h   dbul_solar_position.cxx
    dbul_dir_file.h         dbul_dir_file.cxx
    dbul_parse_simple_file.h       dbul_parse_simple_file.cxx
    dbul_arg.h dbul_arg.cxx
    dbul_handle_files_embedded_in_vxl_source.h dbul_handle_files_embedded_in_vxl_source.cxx
    dbul_random.h    dbul_random.cxx
   )

if(OCTAVE_FOUND)
    set(dbul_sources
        ${dbul_sources}
        dbul_octave.h dbul_octave.cxx
    )
endif(OCTAVE_FOUND)

#aux_source_directory(Templates dbul_sources)

add_library(dbul  ${dbul_sources})

target_link_libraries(dbul vul vpl)

if(OCTAVE_FOUND)
    target_link_libraries(dbul octinterp bxml dbxml)
endif(OCTAVE_FOUND)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

add_subdirectory( cmdline )
