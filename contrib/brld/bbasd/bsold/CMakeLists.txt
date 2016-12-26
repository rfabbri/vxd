# lemsvxlsrc/bbasd/bsold/CMakeLists.txt
# The Development Brown Spatial Object Library
project(bsold)

include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )
include_directories(${lems_SOURCE_DIR}/basic)

configure_file( ${bsold_SOURCE_DIR}/bsold_config.h.in
                ${bsold_BINARY_DIR}/bsold_config.h )

set( bsold_sources
  bsold_interp_curve_2d.cxx bsold_interp_curve_2d.h bsold_interp_curve_2d_sptr.h  
  bsold_geno_curve_2d.cxx bsold_geno_curve_2d.h bsold_geno_curve_2d_sptr.h
  bsold_gshock.cxx bsold_gshock.h bsold_gshock_sptr.h

  bsold_gshock_curve_2d.cxx bsold_gshock_curve_2d.h bsold_gshock_curve_2d_sptr.h
  bsold_geno_measures.cxx  bsold_geno_measures.h

  bsold_circ_arc_2d.cxx   bsold_circ_arc_2d.h
  bsold_circ_arc_2d_sptr.h
  bsold_file_io.cxx        bsold_file_io.h
  bsold_algos.cxx          bsold_algos.h

  bsold_interp_curve_3d.cxx bsold_interp_curve_3d.h bsold_interp_curve_3d_sptr.h  
)

aux_source_directory(Templates bsold_sources)

add_library(bsold ${bsold_sources})

if(Boost_FOUND)
  target_link_libraries( bsold dbgl dbgl_algo vgl vsol vil ${Boost_LIBRARIES} )
  include_directories( ${Boost_INCLUDE_DIRS} )
else()
  target_link_libraries( bsold dbgl dbgl_algo vgl vsol vil )
endif()

if( Boost_FOUND )
  target_link_libraries( bsold ${Boost_LIBRARIES})
endif( Boost_FOUND )

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

add_subdirectory( algo )

add_subdirectory( pro )

add_subdirectory( vis )


#if( BUILD_EXAMPLES )
#  add_subdirectory( examples )
#endif( BUILD_EXAMPLES )
