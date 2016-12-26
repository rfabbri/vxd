# This is dbsol/algo/CMakeLists.txt
# The Development Brown Spatial Object Library - Specific Algorithms

set( dbsol_algo_sources
  dbsol_curve_algs.cxx            dbsol_curve_algs.h
  dbsol_geno.cxx                  dbsol_geno.h
  dbsol_geno_shock.cxx            dbsol_geno_shock.h
  dbsol_linear_transform_2d.cxx   dbsol_linear_transform_2d.h
  dbsol_corner_finder.cxx         dbsol_corner_finder.h

  dbsol_curve_3d_algs.cxx         dbsol_curve_3d_algs.h
  dbsol_img_curve_algs.cxx        dbsol_img_curve_algs.h
  dbsol_img_curve_algs_two.cxx       dbsol_img_curve_algs_two.h
)

add_library( dbsol_algo ${dbsol_algo_sources} )

target_link_libraries( dbsol_algo dbsol dbnl_algo bil_algo)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif( BUILD_EXAMPLES )
