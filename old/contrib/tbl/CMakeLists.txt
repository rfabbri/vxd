# This is old/contrib/tbl/CMakeLists.txt

project(unmaintained_tbl)

set(UNMAINTAINED_TBL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}" CACHE STATIC "TB Unmaintained package include files" FORCE)

include_directories(${VXL_TBL_INCLUDE_DIR})
include_directories(${UNMAINTAINED_TBL_INCLUDE_DIR})

add_subdirectory(vepl1)
