# This is old/core/CMakeLists.txt

project(unmaintained_vxdcore)

set(UNMAINTAINED_VXDCORE_INCLUDE_DIR
  "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}" CACHE STATIC "VXD core unmaintained package include files" FORCE)

include_directories(${UNMAINTAINED_VXDCORE_INCLUDE_DIR})

# deprecated: old video library:
# add_subdirectory(vil1)
add_subdirectory(vidl1)
