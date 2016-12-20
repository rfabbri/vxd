# This is brcv/mvg/bdifd/pro/CMakeLists.txt
# Brown Multiview Differential Geometry Library

set(bdifd_pro_sources
bdifd_3rd_order_geometry_storage.cxx bdifd_3rd_order_geometry_storage.h bdifd_3rd_order_geometry_storage_sptr.h
bdifd_edgel_data_process.cxx bdifd_edgel_data_process.h
)


add_library(bdifd_pro ${bdifd_pro_sources})

target_link_libraries(bdifd_pro bdifd_algo vidpro1 bpro1)
