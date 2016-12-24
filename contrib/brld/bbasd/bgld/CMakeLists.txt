# vxd/contrib/brld/bbasd/bgld/CMakeLists.txt
# The Development Brown Geometry Library

set( bgld_sources

   bgld_fuzzy_boolean.h
   bgld_dist.h                bgld_dist.cxx
   bgld_triangle.h            bgld_triangle.cxx
   bgld_barycentric.h         bgld_barycentric.cxx

   bgld_param_curve.h         bgld_param_curve.cxx
   bgld_poly_curve.h          bgld_poly_curve.cxx     
   bgld_poly_curve_line.h     bgld_poly_curve_line.cxx
   bgld_eulerspiral_base.h    bgld_eulerspiral_base.cxx
   bgld_arc_base.h            bgld_arc_base.cxx
   bgld_arc.h                 bgld_arc.cxx
   bgld_eno_curve.h           bgld_eno_curve.cxx

   bgld_param_curve_3d.h      bgld_param_curve_3d.cxx
   bgld_eno_curve_3d.h        bgld_eno_curve_3d.cxx
   bgld_linear_curve_3d.h     bgld_linear_curve_3d.cxx
   
   bgld_area.h                bgld_area.hxx
   bgld_rigid_align.cxx       bgld_rigid_align.h
)

aux_source_directory(Templates bgld_sources)

add_library(bgld ${bgld_sources})
  
target_link_libraries( bgld vgl vcl vnl vnl_algo)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

add_subdirectory( algo )

#if( BUILD_EXAMPLES )
#  add_subdirectory( examples )
#endif( BUILD_EXAMPLES )

