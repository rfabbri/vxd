# vxd/config/cmake/Modules/UseVXD.cmake
# (also copied by CMake to the top-level of the vxd build tree)
#
# This CMake file may be included by projects outside VXD.  It
# configures them to make use of VXD headers and libraries.  The file
# is written to work in one of two ways.
#
# The preferred way to use VXD from an outside project with UseVXD.cmake:
#
#  find_package(VXD)
#  if(VXD_FOUND)
#    include(${VXD_CMAKE_DIR}/UseVXD.cmake)
#  else()
#    message("VXD_DIR should be set to the VXD build directory.")
#  endif()
#
# Read vxd/config/cmake/VXDConfig.cmake for the list of variables
# provided.  The names have changed to reduce namespace pollution.
# The old names can be made available by placing this line before
# including UseVXD.cmake:
#
# This UseVXD.cmake no longer adds options and testing features automatically
# to projects including it unless this line appears before including it:
#
#  set(VXD_PROVIDE_STANDARD_OPTIONS 1)
#
# For example, in order to enable full backward-compatibility while
# still using FIND_PACKAGE, use these lines:
#
#  find_package(VXD)
#  if(VXD_FOUND)
#    set(VXD_PROVIDE_STANDARD_OPTIONS 1)
#    include(${VXD_CMAKE_DIR}/UseVXD.cmake)
#  else()
#    message("VXD_DIR should be set to the VXD build directory.")
#  endif()
#
# The old way to use VXD from an outside project with UseVXD.cmake is
# also supported for backward-compatibility:
#
#  set(VXD_BINARY_PATH "" CACHE PATH "VXD build directory (location of UseVXD.cmake)")
#  if(VXD_BINARY_PATH)
#    include(${VXD_BINARY_PATH}/UseVXD.cmake)
#  else()
#    message("VXD_BINARY_PATH should be set to the VXD build directory (location of UseVXD.cmake)" )
#  endif()
#

# If this file has been included directly by a user project instead of
# through VXD_USE_FILE from VXDConfig.cmake, simulate old behavior.
if(NOT VXD_CONFIG_CMAKE)
  if(VXD_BINARY_PATH)

    # Let FIND_PACKAGE import the VXDConfig.cmake module.
    set(VXD_DIR ${VXD_BINARY_PATH})
    find_package(VXD)

    # Enable compatibility mode.
    set(VXD_PROVIDE_STANDARD_OPTIONS 1)

  endif()
endif()

# VXDConfig.cmake has now been included.  Use its settings.
if(VXD_CONFIG_CMAKE)
  # Load the compiler settings used for VXD.
  if(VXD_BUILD_SETTINGS_FILE)
    option( VXD_IMPORT_BUILD_SETTINGS "Import build settings (compiler flags, generator) from VXD?" YES )
    mark_as_advanced( VXD_IMPORT_BUILD_SETTINGS )
    if( VXD_IMPORT_BUILD_SETTINGS )
      include(${CMAKE_ROOT}/Modules/CMakeImportBuildSettings.cmake)
      CMAKE_IMPORT_BUILD_SETTINGS(${VXD_BUILD_SETTINGS_FILE})
    endif()
  endif()

  # Add link directories needed to use VXD.
  link_directories(${VXD_LIBRARY_DIR})

  # XXX for now, doxygen is not supported
  #  if(VXD_CMAKE_DOXYGEN_DIR)
    # Allow use of VXD's cmake/doxygen framework
    #    include(${VXD_CMAKE_DOXYGEN_DIR}/doxygen.cmake)
    #  endif()

  if(VXD_PROVIDE_STANDARD_OPTIONS)
    # Provide the standard set of VXD CMake options to the project.
    include(${VXD_CMAKE_DIR}/VXDStandardOptions.cmake)
  endif()
endif()
