# This is /basic/dbil/algo/CMakeLists.txt
# The Development Brown Image Algorithms Library 

 
include_directories( ${VXL_BRL_INCLUDE_DIR} )
include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/basic)

set( dbil_algo_sources
  dbil_gradient_color.h          dbil_gradient_color.cxx
  dbil_exact_distance_transform.h dbil_exact_distance_transform.cxx
)

if(OCTAVE_FOUND)
    set(dbil_algo_sources
        ${dbil_algo_sources}
        dbil_octave.h dbil_octave.cxx
    )
endif(OCTAVE_FOUND)

#aux_source_directory(Templates dbil_algo_sources)

add_library( dbil_algo ${dbil_algo_sources} )

target_link_libraries( dbil_algo vil_algo vul)

if(OCTAVE_FOUND)
    target_link_libraries(dbil_algo dbul)
endif(OCTAVE_FOUND)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
