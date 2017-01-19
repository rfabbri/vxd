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
# Use @rpath on OS X
if( POLICY CMP0042 )
  cmake_policy(SET CMP0042 NEW)
endif()
# Honor visibility properties for all target
if( POLICY CMP0063 )
  cmake_policy(SET CMP0063 NEW)
endif()
# Honor visibility properties for static libraries
if( POLICY CMP0063 )
  cmake_policy(SET CMP0063 NEW)
endif()

project(vxd)

#-----------------------------------------------------------------------------
if(NOT COMMAND SETIFEMPTY)
  macro(SETIFEMPTY)
    set(KEY ${ARGV0})
    set(VALUE ${ARGV1})
    if(NOT ${KEY})
      set(${ARGV})
    endif()
  endmacro()
endif()

#-----------------------------------------------------------------------------
SETIFEMPTY(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
SETIFEMPTY(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
SETIFEMPTY(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

#-----------------------------------------------------------------------------
SETIFEMPTY(CMAKE_INSTALL_LIBRARY_DESTINATION lib)
SETIFEMPTY(CMAKE_INSTALL_ARCHIVE_DESTINATION lib)
SETIFEMPTY(CMAKE_INSTALL_RUNTIME_DESTINATION bin)

# Allow external project to override the export target
if(NOT VXD_NO_EXPORT)
  SETIFEMPTY(VXD_INSTALL_EXPORT_NAME VXDTargets)
endif()

SETIFEMPTY(VXD_INSTALL_RUNTIME_DIR bin)
SETIFEMPTY(VXD_INSTALL_LIBRARY_DIR lib)
SETIFEMPTY(VXD_INSTALL_ARCHIVE_DIR lib)
SETIFEMPTY(VXD_INSTALL_INCLUDE_DIR include/vxd)
if(NOT VXD_LIB_PREFIX)
  set( VXD_LIB_PREFIX "") # This is typically empty
endif()

# CMake support directory.
set(VXD_ROOT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(VXD_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR}/config/cmake/Modules)

include(${VXD_CMAKE_DIR}/VXDStandardOptions.cmake)
# Only include these if we have vxd-specific cmake utilities. We currently use VXL's:
#include(${CMAKE_CURRENT_LIST_DIR}/config/cmake/config/vxd_utils.cmake)

# Location of VXD's FindXXX.cmake CMake modules.
# This is identical to VXD_CMAKE_DIR.  Perhaps we should eliminate MODULE_PATH?
set( MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/config/cmake/Modules CACHE STATIC "VXD module path" )

# Options to add extra compiler and linker flags
#
# These options allow you to specify additional flags without
# affecting the default flags for a particular platform or build type.
# This is especially useful for adding extra warning flags.
set( VXD_EXTRA_CMAKE_C_FLAGS             CACHE STRING "Extra flags appended to CMAKE_C_FLAGS" )
set( VXD_EXTRA_CMAKE_CXX_FLAGS           CACHE STRING "Extra flags appended to CMAKE_CXX_FLAGS" )
set( VXD_EXTRA_CMAKE_EXE_LINKER_FLAGS    CACHE STRING "Extra flags appended to CMAKE_EXE_LINKER_FLAGS" )
set( VXD_EXTRA_CMAKE_MODULE_LINKER_FLAGS CACHE STRING "Extra flags appended to CMAKE_MODULE_LINKER_FLAGS" )
set( VXD_EXTRA_CMAKE_SHARED_LINKER_FLAGS CACHE STRING "Extra flags appended to CMAKE_SHARED_LINKER_FLAGS" )
set( CMAKE_C_FLAGS             "${CMAKE_C_FLAGS} ${VXD_EXTRA_CMAKE_C_FLAGS}" )
set( CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} ${VXD_EXTRA_CMAKE_CXX_FLAGS}" )
set( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${VXD_EXTRA_CMAKE_EXE_LINKER_FLAGS}" )
set( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${VXD_EXTRA_CMAKE_MODULE_LINKER_FLAGS}" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${VXD_EXTRA_CMAKE_SHARED_LINKER_FLAGS}" )

#-------------------------------------------------------------------
#-- BUILD CONFIG OPTIONS

# In order to be able to link vxl libraries into shared libraries on 64 bit linux, the -fPIC
# compiler flag must be added.  Only do this if we are on a x86_64 *nix platform, we're building
# static libraries, and the user has not explicitly requested position dependent code.
if(UNIX)
  if(NOT VXL_BUILD_SHARED_LIBS AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    option(BUILD_POSITION_DEPENDENT_CODE "Generate position dependent code (i.e. code cannot be used in shared library)" OFF)
    mark_as_advanced(BUILD_POSITION_DEPENDENT_CODE)
    if(NOT BUILD_POSITION_DEPENDENT_CODE)
      message(STATUS "Adding -fPIC compiler flag to generate position independent code.")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    endif()
  endif()
endif()




# Some types of path names can cause havoc with regular expressions,
# so avoid those.
if( ${PROJECT_BINARY_DIR} MATCHES  \\+ )
  message(SEND_ERROR "You cannot have a + in your binary path")
endif()
if( ${CMAKE_CURRENT_LIST_DIR} MATCHES  \\+ )
  message(SEND_ERROR "You cannot have a + in your source path")
endif()

# include CMakeListsLocal.txt from source directory if it exists
# also include it from the binary dir if different from source dir
if( ${PROJECT_BINARY_DIR} MATCHES ${CMAKE_CURRENT_LIST_DIR} )
  include( ${CMAKE_CURRENT_LIST_DIR}/CMakeListsLocal.txt OPTIONAL )
else()
  include( ${CMAKE_CURRENT_LIST_DIR}/CMakeListsLocal.txt OPTIONAL )
  include( ${PROJECT_BINARY_DIR}/CMakeListsLocal.txt OPTIONAL )
endif()

find_package(VXL PATHS "../vxl-bin")
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

add_subdirectory(contrib) 
add_subdirectory(old) 

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

# Copy the UseVXD.cmake file to the binary directory so that client
# projects don't need to find the source directory first. They can run
# the UseVXD.cmake from the vxd binary directory, and determine the
# vxd source directory by loading the cache.
configure_file( ${VXD_CMAKE_DIR}/UseVXD.cmake
                ${vxd_BINARY_DIR}/UseVXD.cmake COPYONLY )
# For use in client projects that call UseVXD.cmake
set(VXD_LIBRARY_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} CACHE STATIC "Where all the vxd libraries are, for clients to use." )

# Copy CTestCustom.cmake to top of build tree
#configure_file( ${VXD_CMAKE_DIR}/CTestCustom.cmake
#  ${vxd_BINARY_DIR}/CTestCustom.cmake COPYONLY )

# This command must be the last command in this file
if(NOT VXD_NO_EXPORT)
  include(${CMAKE_CURRENT_LIST_DIR}/config/cmake/export/VXDCreateProject.cmake)
endif()
