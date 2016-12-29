# This is brcv/mvg/becld/pro/CMakeLists.txt
# Brown Epipolar Curve Processing library

include_directories( ${lems_SOURCE_DIR}/breye1 )

set(becld_pro_sources
   becld_builder_process.cxx   becld_builder_process.h
   becld_episeg_storage.cxx    becld_episeg_storage.h     becld_episeg_storage_sptr.h
)

#aux_source_directory(Templates becld_pro_sources)

add_library(becld_pro ${becld_pro_sources})

target_link_libraries(becld_pro becld vidpro1)


if(BUILD_TESTING)
#  add_subdirectory(tests)
endif(BUILD_TESTING)

