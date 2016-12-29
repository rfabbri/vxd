# b3pd/CMakeLists.txt
# Brown 3rd Party Libraries (VXD version)

####################################################################
#                                                                  #
# This is a place to store third party libraries that are required #
# by other code in this repository.  Please view the README file.  #
#                                                                  #
####################################################################

project(b3pd)

set(B3PD_INCLUDE_DIR ${b3pd_SOURCE_DIR} CACHE STATIC "b3pd include files")

add_subdirectory(ray_tri)
add_subdirectory(brent)
add_subdirectory(triangulation_kanatani)
