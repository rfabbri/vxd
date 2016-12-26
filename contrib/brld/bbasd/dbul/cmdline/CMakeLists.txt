# /basic/dbul/cmdline/CMakeLists.txt
# Development Brown Utility Library - Command line utilities

set(dbul_embed_files_to_vxl_source_sources
    dbul_embed_files_to_vxl_source_utils.h dbul_embed_files_to_vxl_source_utils.cxx dbul_embed_files_to_vxl_source_main.cxx
)

add_executable(dbul_embed_files_to_vxl_source ${dbul_embed_files_to_vxl_source_sources})
target_link_libraries( dbul_embed_files_to_vxl_source vul dbul)
