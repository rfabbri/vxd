# This is brcv/mvg/dbdif/pro/CMakeLists.txt
# Brown Multiview Differential Geometry Library

include_directories( ${lems_SOURCE_DIR}/breye1 )

set(dbdif_pro_sources
dbdif_3rd_order_geometry_storage.cxx dbdif_3rd_order_geometry_storage.h dbdif_3rd_order_geometry_storage_sptr.h
dbdif_edgel_data_process.cxx dbdif_edgel_data_process.h
)


add_library(dbdif_pro ${dbdif_pro_sources})

target_link_libraries(dbdif_pro dbdif_algo vidpro1 bpro1)
