# This is old/contrib/gel/CMakeLists.txt

project(unmaintained_gel)

set(UNMAINTAINED_GEL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}" CACHE STATIC "GEL Unmaintained/Old package include files" FORCE)

include_directories(${VXL_GEL_INCLUDE_DIR})
include_directories(${UNMAINTAINED_GEL_INCLUDE_DIR})

#add_subdirectory(vgel_kl)
add_subdirectory(vgel)
add_subdirectory(vsrl)
