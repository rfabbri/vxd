# lemsvxlsrc/bbasd/bnld/CMakeLists.txt
# The Development Brown Numerical Library

set( bnld_sources
                                       bnld_min_max_3.h
    bnld_cbrt.cxx                      bnld_cbrt.h
    bnld_fresnel.cxx                   bnld_fresnel.h
    bnld_sinc.cxx                      bnld_sinc.h
    bnld_newton_root.cxx               bnld_newton_root.h
    bnld_gaussian.h
    bnld_math.h
    bnld_angle.h
    bnld_solve_quadratic_equation.h        bnld_solve_quadratic_equation.cxx
    bnld_solve_trigonometric_equation.h    bnld_solve_trigonometric_equation.cxx
)

add_library(bnld ${bnld_sources})

target_link_libraries( bnld vnl )

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

add_subdirectory( algo )

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif( BUILD_EXAMPLES )

