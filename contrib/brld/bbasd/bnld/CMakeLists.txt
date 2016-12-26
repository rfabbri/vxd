# lemsvxlsrc/basic/dbnl/CMakeLists.txt
# The Development Brown Numerical Library

set( dbnl_sources
                                       dbnl_min_max_3.h
    dbnl_cbrt.cxx                      dbnl_cbrt.h
    dbnl_fresnel.cxx                   dbnl_fresnel.h
    dbnl_sinc.cxx                      dbnl_sinc.h
    dbnl_newton_root.cxx               dbnl_newton_root.h
    dbnl_gaussian.h
    dbnl_math.h
    dbnl_angle.h
    dbnl_solve_quadratic_equation.h        dbnl_solve_quadratic_equation.cxx
    dbnl_solve_trigonometric_equation.h    dbnl_solve_trigonometric_equation.cxx
)

add_library(dbnl ${dbnl_sources})

target_link_libraries( dbnl vnl )

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

add_subdirectory( algo )

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif( BUILD_EXAMPLES )

