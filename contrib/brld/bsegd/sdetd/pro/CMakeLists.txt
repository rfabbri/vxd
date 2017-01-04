# This is brl/bsegd/sdetd/pro/CMakeLists.txt
# Brown Segmentation Detection Processing library (VXD version)

set(sdetd_pro_sources
   sdetd_sel_storage.cxx              sdet_sel_storage.h         
   sdetd_sel_storage_sptr.h
)

#aux_source_directory(Templates sdet_pro_sources)

add_library(sdetd_pro ${sdet_pro_sources})

target_link_libraries(sdetd_pro sdetd_algo bpro1)

#if(BUILD_TESTING)
#  add_subdirectory(tests)
#endif(BUILD_TESTING)

