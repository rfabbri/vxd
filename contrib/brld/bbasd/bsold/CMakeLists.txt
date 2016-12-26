# lemsvxlsrc/basic/dbsol/CMakeLists.txt
# The Development Brown Spatial Object Library
project(dbsol)

include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )
include_directories(${lems_SOURCE_DIR}/basic)

configure_file( ${dbsol_SOURCE_DIR}/dbsol_config.h.in
                ${dbsol_BINARY_DIR}/dbsol_config.h )

set( dbsol_sources
  dbsol_interp_curve_2d.cxx dbsol_interp_curve_2d.h dbsol_interp_curve_2d_sptr.h  
  dbsol_geno_curve_2d.cxx dbsol_geno_curve_2d.h dbsol_geno_curve_2d_sptr.h
  dbsol_gshock.cxx dbsol_gshock.h dbsol_gshock_sptr.h

  dbsol_gshock_curve_2d.cxx dbsol_gshock_curve_2d.h dbsol_gshock_curve_2d_sptr.h
  dbsol_geno_measures.cxx  dbsol_geno_measures.h

  dbsol_circ_arc_2d.cxx   dbsol_circ_arc_2d.h
  dbsol_circ_arc_2d_sptr.h
  dbsol_file_io.cxx        dbsol_file_io.h
  dbsol_algos.cxx          dbsol_algos.h

  dbsol_interp_curve_3d.cxx dbsol_interp_curve_3d.h dbsol_interp_curve_3d_sptr.h  
)

aux_source_directory(Templates dbsol_sources)

add_library(dbsol ${dbsol_sources})

if(Boost_FOUND)
  target_link_libraries( dbsol dbgl dbgl_algo vgl vsol vil ${Boost_LIBRARIES} )
  include_directories( ${Boost_INCLUDE_DIRS} )
else()
  target_link_libraries( dbsol dbgl dbgl_algo vgl vsol vil )
endif()

if( Boost_FOUND )
  target_link_libraries( dbsol ${Boost_LIBRARIES})
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
