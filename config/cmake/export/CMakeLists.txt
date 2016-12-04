# This CMakeLists.txt file handles the creation of files needed by
# other client projects that use VXD.  Nothing is built by this
# CMakeLists.txt file.  This CMakeLists.txt file must be processed by
# CMake after all the other CMakeLists.txt files in the VXD tree,
# which is why the add_subdirectory(config/cmake/export) command is at the end
# of the top level CMakeLists.txt file.


# Save the compiler settings so another project can import them.
set(VXD_BUILD_SETTINGS_FILE ${vxd_BINARY_DIR}/VXDBuildSettings.cmake)

# Save library dependencies.
set(VXD_LIBRARY_DEPENDS_FILE ${vxd_BINARY_DIR}/VXDLibraryDepends.cmake)
export_library_dependencies(${VXD_LIBRARY_DEPENDS_FILE})

# Create the VXLConfig.cmake file for the build tree.
configure_file(${VXD_CMAKE_DIR}/Modules/VXDConfig.cmake.in
               ${vxd_BINARY_DIR}/VXDConfig.cmake @ONLY IMMEDIATE)
