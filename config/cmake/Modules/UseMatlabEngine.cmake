# Finds libraries and include files associated with Matlab Engine
# author: firat

include( ${LEMS_CMAKE_DIR}/Modules/FindMatlabEngine.cmake )
if(MATLAB_ENGINE_FOUND)    
    include_directories(${MATLAB_ENGINE_INCLUDE_PATH})
    link_directories(${MATLAB_ENGINE_SHARED_LIBRARY_PATH})   
    add_definitions( -DMATLAB_ENGINE_FOUND=1 )
else(MATLAB_ENGINE_FOUND)
    add_definitions( -DMATLAB_ENGINE_FOUND=0 )
endif(MATLAB_ENGINE_FOUND)



