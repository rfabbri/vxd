# This is dbgl/algo/CMakeLists.txt
# The Development Brown Geometry Library - Specific Algorithms

# Set up dbgl_where_lemsvxl_lib_dir.h to have a macro set to $LIBRARY_OUTPUT_PATH
configure_file(
  ${lems_SOURCE_DIR}/basic/dbgl/algo/dbgl_where_lemsvxl_lib_dir.h.in
  ${lems_BINARY_DIR}/basic/dbgl/algo/dbgl_where_lemsvxl_lib_dir.h ESCAPE_QUOTES
)

# Copy the Eulerspiral lookup-table file to the LEMSVXL library folder
configure_file(
  ${lems_SOURCE_DIR}/basic/dbgl/algo/dbgl_eulerspiral_lookup_table.bvl
  ${LIBRARY_OUTPUT_PATH}/dbgl_eulerspiral_lookup_table.bvl COPYONLY
)

add_definitions(-DDBGL_WHERE_LEMSVXL_LIB_DIR_H_EXISTS)

include_directories( ${lems_BINARY_DIR}/basic/dbgl/algo/ )
include_directories( ${lems_BINARY_DIR}/basic/dbgl/ )

set( dbgl_algo_sources
  dbgl_where_lemsvxl_lib_dir.h.in
  dbgl_biarc.cxx               dbgl_biarc.h
  dbgl_eulerspiral.cxx         dbgl_eulerspiral.h
  dbgl_arc_algo.cxx            dbgl_arc_algo.h
  dbgl_arc_center.cxx          dbgl_arc_center.h
  dbgl_intersect.cxx           dbgl_intersect.h
  dbgl_distance.cxx            dbgl_distance.h
  dbgl_pt_tri_dist_3d.cxx

  dbgl_closest_point.cxx       dbgl_closest_point.h
  dbgl_curve_smoothing.cxx     dbgl_curve_smoothing.h
  
  # temporary circular arc files with different intrinsic
  # representation from dbgl_arc
  # will merge the two later
  dbgl_circ_arc.cxx            dbgl_circ_arc.h
  dbgl_conic_arc.cxx         dbgl_conic_arc.h
  
  # interpolation
  internals/arcspline.h         internals/arcspline.cxx
  dbgl_fit_circ_arc_spline.h    dbgl_fit_circ_arc_spline.cxx
  
  dbgl_similarity_3d.h
  
  # find symmetry point
  dbgl_compute_symmetry_point.h   dbgl_compute_symmetry_point.cxx

  #differential Geometry
  dbgl_diffgeom.h               dbgl_diffgeom.cxx
)

add_library( dbgl_algo ${dbgl_algo_sources} )

target_link_libraries( dbgl_algo vnl_algo vsl vgl_algo dbnl dbgl vul ray_tri)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif( BUILD_EXAMPLES )

