# This is brcv/mvg/dbecl/pro/CMakeLists.txt
# Brown Epipolar Curve Processing library

include_directories( ${lems_SOURCE_DIR}/breye1 )

set(dbecl_pro_sources
   dbecl_builder_process.cxx   dbecl_builder_process.h
   dbecl_episeg_storage.cxx    dbecl_episeg_storage.h     dbecl_episeg_storage_sptr.h
)

#aux_source_directory(Templates dbecl_pro_sources)

add_library(dbecl_pro ${dbecl_pro_sources})

target_link_libraries(dbecl_pro dbecl vidpro1)


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)

