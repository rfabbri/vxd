# This is bsold/algo/CMakeLists.txt
# The Development Brown Spatial Object Library - Specific Algorithms

set( bsold_algo_sources
  bsold_curve_algs.cxx            bsold_curve_algs.h
  bsold_geno.cxx                  bsold_geno.h
  bsold_geno_shock.cxx            bsold_geno_shock.h
  bsold_linear_transform_2d.cxx   bsold_linear_transform_2d.h
  bsold_corner_finder.cxx         bsold_corner_finder.h

  bsold_curve_3d_algs.cxx         bsold_curve_3d_algs.h
  bsold_img_curve_algs.cxx        bsold_img_curve_algs.h
  bsold_img_curve_algs_two.cxx       bsold_img_curve_algs_two.h
)

add_library( bsold_algo ${bsold_algo_sources} )

target_link_libraries( bsold_algo bsold bnld_algo bil_algo)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif( BUILD_EXAMPLES )
