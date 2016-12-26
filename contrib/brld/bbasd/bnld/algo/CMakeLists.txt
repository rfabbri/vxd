# The Development Brown Numerical Library - Specific Algorithms
include_directories( ${vxl_SOURCE_DIR} )
include_directories( ${VXL_NETLIB_INCLUDE_DIR} )

set( bnld_algo_sources
   bnld_eno.cxx               bnld_eno.h
   bnld_eno_interp.cxx
   bnld_eno_poly.cxx
   bnld_eno_measures.cxx      bnld_eno_measures.h
   bnld_eno_zerox.cxx         bnld_eno_zerox.h
   bnld_eno_zerox_label.cxx   bnld_eno_zerox_label.h
   bnld_eno_shock.cxx         bnld_eno_shock.h
   bnld_eno_image.cxx         bnld_eno_image.h
   bnld_eno_zerox_image.cxx   bnld_eno_zerox_image.h
   bnld_bbf_tree.h            bnld_bbf_tree.hxx
   bnld_matrix_multiply.cxx   bnld_matrix_multiply.h
   bnld_eno_third_order.cxx   bnld_eno_third_order.h
   bnld_brent_root.cxx        bnld_brent_root.h

)

aux_source_directory(Templates bnld_algo_sources)

add_library( bnld_algo ${bnld_algo_sources} )

target_link_libraries( bnld_algo bnld vnl_algo vil brent)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif( BUILD_EXAMPLES )

add_subdirectory(pro)


