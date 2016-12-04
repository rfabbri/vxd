# VXD
#
# For now, you can communicate at the VXL maintainers list
#   vxl-maintainers@lists.sf.net
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

# Set policies for cmake
# Support for CMake 2.6
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)
if(POLICY CMP0033)
  ####
  # Remove warning by disabling test.  The proper fix
  # will require careful effort.
  # Documentation for what would be a better solution
  # is given at https://github.com/vxl/vxl/issues/127
  # and https://github.com/vxl/vxl/pull/122
  cmake_policy(SET CMP0033 OLD)
endif()
# TODO perhaps copy over the remaining ones from vxl

project(vxd)

set (LIBRARY_OUTPUT_PATH ${vxd_BINARY_DIR}/lib CACHE PATH "Single output directory for building all libraries.")

find_package(VXL)
if (VXL_FOUND)
  # include the standard stuff, such as Dart test
  set(VXL_PROVIDE_STANDARD_OPTIONS 1)
  include(${VXL_CMAKE_DIR}/UseVXL.cmake)
else (VXL_FOUND)
  if (VXL_BINARY_PATH)
    set(VXL_DIR ${VXL_BINARY_PATH})
    message( "Use preferred VXL_DIR in place of VXL_BINARY_PATH" )
  endif( VXL_BINARY_PATH )
endif(VXL_FOUND)

# CMake support directory.
set(VXD_CMAKE_DIR ${vxd_SOURCE_DIR}/config/cmake)

# Basic Utility Libraries (that we are unsure will make to vxl/core)
add_subdirectory( basic ) 

if(VXL_FOUND)
  # This is required on some platforms because various libraries
  # include OpenGL indirectly even when it is not used
  include_directories( ${VXL_VGUI_INCLUDE_DIR} )

  option(USE_Boost "Use Boost libraries" NO)
  if ( USE_Boost )
    set(Boost_USE_STATIC_LIBS   OFF)
    find_package( Boost 1.35.0 COMPONENTS iostreams)
  else ( USE_Boost )
    set( Boost_FOUND FALSE )
  endif( USE_Boost )
endif(VXL_FOUND)

# adds a test for Endianness and a global variable that should be useful for file format programming -MM
include(TestBigEndian)
TEST_BIG_ENDIAN(MY_BIG_ENDIAN)
