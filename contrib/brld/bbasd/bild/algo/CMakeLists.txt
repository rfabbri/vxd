# This is /basic/bild/algo/CMakeLists.txt
# The Development Brown Image Algorithms Library 

 
include_directories( ${VXL_BRL_INCLUDE_DIR} )
include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/basic)

set( bild_algo_sources
  bild_gradient_color.h          bild_gradient_color.cxx
  bild_exact_distance_transform.h bild_exact_distance_transform.cxx
)

if(OCTAVE_FOUND)
    set(bild_algo_sources
        ${bild_algo_sources}
        bild_octave.h bild_octave.cxx
    )
endif(OCTAVE_FOUND)

#aux_source_directory(Templates bild_algo_sources)

add_library( bild_algo ${bild_algo_sources} )

target_link_libraries( bild_algo vil_algo vul)

if(OCTAVE_FOUND)
    target_link_libraries(bild_algo buld)
endif(OCTAVE_FOUND)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
