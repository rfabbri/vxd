# This is bgld/algo/CMakeLists.txt
# The Development Brown Geometry Library - Specific Algorithms

# Set up bgld_where_brld_lib_dir.h to have a macro set to $LIBRARY_OUTPUT_PATH
configure_file(
  ${BRLD_SOURCE_DIR}/bbasd/bgld/algo/bgld_where_brld_lib_dir.h.in
  ${BRLD_BINARY_DIR}/bbasd/bgld/algo/bgld_where_brld_lib_dir.h ESCAPE_QUOTES
)

# Copy the Eulerspiral lookup-table file to the BRLD library folder
configure_file(
  ${BRLD_SOURCE_DIR}/bbasd/bgld/algo/bgld_eulerspiral_lookup_table.bvl
  ${LIBRARY_OUTPUT_PATH}/bgld_eulerspiral_lookup_table.bvl COPYONLY
)

add_definitions(-DBGLD_WHERE_BRLD_LIB_DIR_H_EXISTS)

include_directories( ${BRLD_BINARY_DIR}/bbasd/bgld/algo/ )
include_directories( ${BRLD_BINARY_DIR}/bbasd/bgld/ )

set( bgld_algo_sources
  bgld_where_brld_lib_dir.h.in
  bgld_biarc.cxx               bgld_biarc.h
  bgld_eulerspiral.cxx         bgld_eulerspiral.h
  bgld_arc_algo.cxx            bgld_arc_algo.h
  bgld_arc_center.cxx          bgld_arc_center.h
  bgld_intersect.cxx           bgld_intersect.h
  bgld_distance.cxx            bgld_distance.h
  bgld_pt_tri_dist_3d.cxx

  bgld_closest_point.cxx       bgld_closest_point.h
  bgld_curve_smoothing.cxx     bgld_curve_smoothing.h
  
  # temporary circular arc files with different intrinsic
  # representation from bgld_arc
  # will merge the two later
  bgld_circ_arc.cxx            bgld_circ_arc.h
  bgld_conic_arc.cxx         bgld_conic_arc.h
  
  # interpolation
  internals/arcspline.h         internals/arcspline.cxx
  bgld_fit_circ_arc_spline.h    bgld_fit_circ_arc_spline.cxx
  
  bgld_similarity_3d.h
  
  # find symmetry point
  bgld_compute_symmetry_point.h   bgld_compute_symmetry_point.cxx

  #differential Geometry
  bgld_diffgeom.h               bgld_diffgeom.cxx
)

add_library( bgld_algo ${bgld_algo_sources} )

target_link_libraries(bgld_algo vnl_algo vsl vgl_algo dbnl bgld vul ray_tri)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )

if( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif( BUILD_EXAMPLES )

