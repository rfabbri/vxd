# This CMakeLists.txt file handles the creation of files needed by
# other client projects that use LEMS VXL.  Nothing is built by this
# CMakeLists.txt file.  This CMakeLists.txt file must be processed by
# CMake after all the other CMakeLists.txt files in the LEMS tree,
# which is why the add_subdirectory(config/cmake/export) command is at the end
# of the top level CMakeLists.txt file.


# Save the compiler settings so another project can import them.
set(LEMS_BUILD_SETTINGS_FILE ${lems_BINARY_DIR}/LEMSBuildSettings.cmake)

# Save library dependencies.
set(LEMS_LIBRARY_DEPENDS_FILE ${lems_BINARY_DIR}/LEMSLibraryDepends.cmake)
export_library_dependencies(${LEMS_LIBRARY_DEPENDS_FILE})

# Create the VXLConfig.cmake file for the build tree.
configure_file(${LEMS_CMAKE_DIR}/Modules/LEMSConfig.cmake.in
               ${lems_BINARY_DIR}/LEMSConfig.cmake @ONLY IMMEDIATE)
