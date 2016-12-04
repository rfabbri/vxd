# basic/CMakeLists.txt
# Basic Libraries (not directly intended to go into vxl/core)

project(basic)

include_directories( ${vxd_SOURCE_DIR}/basic )
include_directories( ${basic_BINARY_DIR} )

# The Brown Basic (templates) Library (Development)
add_subdirectory(bbld) 
