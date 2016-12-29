# This is brcv/mvg/dbecl/CMakeLists.txt
# Brown Epipolar Curve Library

include_directories(${VXL_GEL_INCLUDE_DIR} )
include_directories(${VXL_GEL_INCLUDE_DIR}/mrc )

set(dbecl_sources
    dbecl_epipole.cxx           dbecl_epipole.h            dbecl_epipole_sptr.h
    dbecl_episeg.cxx            dbecl_episeg.h             dbecl_episeg_sptr.h
    dbecl_episeg_point.cxx      dbecl_episeg_point.h       dbecl_episeg_point_sptr.h
    dbecl_episeg_from_curve_converter.cxx  dbecl_episeg_from_curve_converter.h
    dbecl_epiband.cxx dbecl_epiband.h
    dbecl_epiband_iterator.cxx dbecl_epiband_iterator.h
    dbecl_epiband_builder.cxx dbecl_epiband_builder.h
)

aux_source_directory(Templates dbecl_sources)

add_library(dbecl ${dbecl_sources})

target_link_libraries(dbecl vpgl dbgl_algo vbl vul vnl vil vsol vgl)

# Processing Library
add_subdirectory(pro)

# Visualization Library
add_subdirectory(vis)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
