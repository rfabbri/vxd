
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
set(LEMS_CMAKE_DIR ${lems_SOURCE_DIR}/config/cmake)

# Brown 3rd Party Libraries
add_subdirectory( db3p )

# Brown Vision Algorithms
option(BUILD_ALGO "Build Depreciated Algorithms" NO)
if(BUILD_ALGO)
  add_subdirectory( algo )
endif(BUILD_ALGO)

# Basic Utility Libraries
add_subdirectory( basic ) 
 
# Brown Computer Vision Libraries
add_subdirectory( brcv )

# Brown Eyes Code
option(BUILD_BREYE "Build Brown Eyes" NO)
if(BUILD_BREYE)
  add_subdirectory( breye )
endif(BUILD_BREYE)

# Brown Eyes Code
option(BUILD_BREYE1 "Build Old Brown Eyes" YES)
if(BUILD_BREYE1)
  add_subdirectory( breye1 )
endif(BUILD_BREYE1)

#Apps
option(BUILD_APPS "Build Apps" NO)
if(BUILD_APPS)
  add_subdirectory( apps )
endif(BUILD_APPS)

option(BUILD_PLAYLAND "Build the personal testing area" NO)
if(BUILD_PLAYLAND)
  add_subdirectory( playland )
endif(BUILD_PLAYLAND)

if( DART_ROOT )
  option(BUILD_CONTRIB "Build the contrib development area" YES)
else( DART_ROOT )
  option(BUILD_CONTRIB "Build the contrib development area" NO)
endif( DART_ROOT )
if(BUILD_CONTRIB)
  add_subdirectory( contrib )
endif(BUILD_CONTRIB)

if(VXL_FOUND)

  # This is required on some platforms because various libraries
  # include OpenGL indirectly even when it is not used
  include_directories( ${VXL_VGUI_INCLUDE_DIR} )

  #update vxl/config/Modules/NewCMakeModules folder to get the following
  include( ${VXL_CMAKE_DIR}/NewCMake/FindDirectShow.cmake )

  if(DIRECTSHOW_FOUND)
    include_directories(${DIRECTSHOW_INCLUDE_DIR})
    add_definitions(-DHAS_DSHOW)
    # For Euresys Multicam Driver's DirectShow source filter.
    # - should I put this in a Module of it's own?
    #include( ${MODULE_PATH}/NewCMake/FindDirectShow_ESF.cmake )
    find_path(DIRECTSHOW_ESF_INCLUDE_DIR ESFilter.h
      "C:/Program Files/Euresys/MultiCam/Include"
      DOC "What is the path where the file ESFilter.h can be found"
    )
    if(DIRECTSHOW_ESF_INCLUDE_DIR)
      set(DIRECTSHOW_ESF_FOUND "YES")
    endif(DIRECTSHOW_ESF_INCLUDE_DIR)
    if(DIRECTSHOW_ESF_FOUND)
      include_directories(${DIRECTSHOW_ESF_INCLUDE_DIR})
      add_definitions(-DHAS_DSHOW_ESF)
    endif(DIRECTSHOW_ESF_FOUND)
  endif(DIRECTSHOW_FOUND)

  option(USE_Boost "Use Boost libraries" NO)
  if ( USE_Boost )
    set(Boost_USE_STATIC_LIBS   OFF)
    find_package( Boost 1.35.0 COMPONENTS iostreams)
  else ( USE_Boost )
    set( Boost_FOUND FALSE )
  endif( USE_Boost )
  
  #required by dbul library
  include( ${LEMS_CMAKE_DIR}/Modules/UseOctave.cmake )

endif(VXL_FOUND)


if(NOT BUILD_BROWN_INTERLEAVED)
  # This SUBDIRS command must be the last SUBDIRS command in this file
  add_subdirectory( config/cmake/export )
endif(NOT BUILD_BROWN_INTERLEAVED)


# adds a test for Endianness and a global variable that should be useful for file format programming -MM
include(TestBigEndian)
TEST_BIG_ENDIAN(MY_BIG_ENDIAN)
