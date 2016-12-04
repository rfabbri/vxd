set (HAS_MATLAB "NO")
if (BUILD_MATLAB)

  # Find the MATLAB library
  include( ${VXD_CMAKE_DIR}/Modules/FindMatlab.cmake )
 if(MATLAB_INCLUDE_DIR)
  if(MATLAB_LIBRARY_PATH) 
   if(NOT HAS_MATLAB)
      set(HAS_MATLAB "YES")
      include_directories( ${MATLAB_INCLUDE_DIR} )
      link_directories( ${MATLAB_LIBRARY_PATH})
      link_libraries(libeng libmx   )
      add_definitions( -DHAS_MATLAB )
     endif(NOT HAS_MATLAB)
  endif(MATLAB_LIBRARY_PATH)
endif(MATLAB_INCLUDE_DIR)

endif (BUILD_MATLAB)
