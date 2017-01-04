# This is brl/bsegd/sdetd/pro/CMakeLists.txt
# Brown Segmentation Detection Processing library (VXD version)

set(sdetd_pro_sources
   sdetd_sel_storage.cxx              sdetd_sel_storage.h
   sdetd_sel_storage_sptr.h
)

add_library(sdetd_pro ${sdetd_pro_sources})

target_link_libraries(sdetd_pro sdetd_algo bpro1)
