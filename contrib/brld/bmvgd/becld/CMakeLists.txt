# This is bmvgd/becld/CMakeLists.txt
# Brown Epipolar Curve Library

include_directories(${VXL_GEL_INCLUDE_DIR} )
include_directories(${VXL_GEL_INCLUDE_DIR}/mrc )

set(becld_sources
    becld_epipole.cxx           becld_epipole.h            becld_epipole_sptr.h
    becld_episeg.cxx            becld_episeg.h             becld_episeg_sptr.h
    becld_episeg_point.cxx      becld_episeg_point.h       becld_episeg_point_sptr.h
    becld_episeg_from_curve_converter.cxx  becld_episeg_from_curve_converter.h
    becld_epiband.cxx becld_epiband.h
    becld_epiband_iterator.cxx becld_epiband_iterator.h
    becld_epiband_builder.cxx becld_epiband_builder.h
    becld_epiline_interceptor.cxx becld_epiline_interceptor.h
    becld_intersection_sets.cxx becld_intersection_sets.h
)

aux_source_directory(Templates becld_sources)

add_library(becld ${becld_sources})

target_link_libraries(becld vpgl bgld_algo vbl vul vnl vil vsol vgl)

# Processing Library
add_subdirectory(pro)

# Visualization Library
add_subdirectory(vis)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
