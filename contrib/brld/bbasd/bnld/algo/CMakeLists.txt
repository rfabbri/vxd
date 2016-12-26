# The Development Brown Numerical Library - Specific Algorithms
include_directories( ${vxl_SOURCE_DIR} )
include_directories( ${VXL_NETLIB_INCLUDE_DIR} )

set( dbnl_algo_sources
   dbnl_eno.cxx               dbnl_eno.h
   dbnl_eno_interp.cxx
   dbnl_eno_poly.cxx
   dbnl_eno_measures.cxx      dbnl_eno_measures.h
   dbnl_eno_zerox.cxx         dbnl_eno_zerox.h
   dbnl_eno_zerox_label.cxx   dbnl_eno_zerox_label.h
   dbnl_eno_shock.cxx         dbnl_eno_shock.h
   dbnl_eno_image.cxx         dbnl_eno_image.h
   dbnl_eno_zerox_image.cxx   dbnl_eno_zerox_image.h
   dbnl_bbf_tree.h            dbnl_bbf_tree.hxx
   dbnl_matrix_multiply.cxx   dbnl_matrix_multiply.h
   dbnl_eno_third_order.cxx   dbnl_eno_third_order.h
   dbnl_brent_root.cxx        dbnl_brent_root.h

)

aux_source_directory(Templates dbnl_algo_sources)

add_library( dbnl_algo ${dbnl_algo_sources} )

target_link_libraries( dbnl_algo dbnl vnl_algo vil brent)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif( BUILD_EXAMPLES )

add_subdirectory(pro)


