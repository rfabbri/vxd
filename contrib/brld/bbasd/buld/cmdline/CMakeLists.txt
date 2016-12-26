# /bbasd/buld/cmdline/CMakeLists.txt
# Development Brown Utility Library - Command line utilities

set(buld_embed_files_to_vxl_source_sources
    buld_embed_files_to_vxl_source_utils.h buld_embed_files_to_vxl_source_utils.cxx buld_embed_files_to_vxl_source_main.cxx
)

add_executable(buld_embed_files_to_vxl_source ${buld_embed_files_to_vxl_source_sources})
target_link_libraries( buld_embed_files_to_vxl_source vul buld)
