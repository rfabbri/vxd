# lemsvxlsrc/basic/dbgl/CMakeLists.txt
# The Development Brown Geometry Library

set( dbgl_sources

   dbgl_fuzzy_boolean.h
   dbgl_dist.h                dbgl_dist.cxx
   dbgl_triangle.h            dbgl_triangle.cxx
   dbgl_barycentric.h         dbgl_barycentric.cxx

   dbgl_param_curve.h         dbgl_param_curve.cxx
   dbgl_poly_curve.h          dbgl_poly_curve.cxx     
   dbgl_poly_curve_line.h     dbgl_poly_curve_line.cxx
   dbgl_eulerspiral_base.h    dbgl_eulerspiral_base.cxx
   dbgl_arc_base.h            dbgl_arc_base.cxx
   dbgl_arc.h                 dbgl_arc.cxx
   dbgl_eno_curve.h           dbgl_eno_curve.cxx

   dbgl_param_curve_3d.h      dbgl_param_curve_3d.cxx
   dbgl_eno_curve_3d.h        dbgl_eno_curve_3d.cxx
   dbgl_linear_curve_3d.h     dbgl_linear_curve_3d.cxx
   
   dbgl_area.h                dbgl_area.hxx
   dbgl_rigid_align.cxx       dbgl_rigid_align.h
)

aux_source_directory(Templates dbgl_sources)

add_library(dbgl ${dbgl_sources})
  
target_link_libraries( dbgl vgl vcl vnl vnl_algo)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

# add_subdirectory( algo )

#if( BUILD_EXAMPLES )
#  add_subdirectory( examples )
#endif( BUILD_EXAMPLES )

