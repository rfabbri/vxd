# Finds libraries and include files associated with Matlab Engine
# author: firat 

set(MATLAB_ENGINE_FOUND "NO")

if(WIN32)
    find_path(MATLAB_ENGINE_SHARED_LIBRARY_PATH liboctinterp.dll
        C:/ D:/
    )
    find_path(MATLAB_ENGINE_INCLUDE_PATH MATLAB_ENGINE.h
        C:/ D:/
    )    
else(WIN32)
    if(UNIX)
        find_path(MATLAB_ENGINE_SHARED_LIBRARY_PATH libeng.so
            /opt/matlab/bin/glnxa64/
        )
        find_path(MATLAB_ENGINE_INCLUDE_PATH engine.h
            /opt/matlab/extern/include/
        )           
    endif(UNIX)
endif(WIN32)  

if(MATLAB_ENGINE_SHARED_LIBRARY_PATH)    
    if(MATLAB_ENGINE_INCLUDE_PATH)
        if(WIN32)
            if(EXISTS "${MATLAB_ENGINE_SHARED_LIBRARY_PATH}/liboctinterp.dll")
                if(EXISTS "${MATLAB_ENGINE_INCLUDE_PATH}/MATLAB_ENGINE.h")
                   set(MATLAB_ENGINE_FOUND "YES") 
                endif(EXISTS "${MATLAB_ENGINE_INCLUDE_PATH}/MATLAB_ENGINE.h")
            endif(EXISTS "${MATLAB_ENGINE_SHARED_LIBRARY_PATH}/liboctinterp.dll")
        else(WIN32)
            if(UNIX)
                if(EXISTS "${MATLAB_ENGINE_SHARED_LIBRARY_PATH}/libeng.so")
                    if(EXISTS "${MATLAB_ENGINE_INCLUDE_PATH}/engine.h")
                       set(MATLAB_ENGINE_FOUND "YES") 
                    endif(EXISTS "${MATLAB_ENGINE_INCLUDE_PATH}/engine.h")
                endif(EXISTS "${MATLAB_ENGINE_SHARED_LIBRARY_PATH}/libeng.so")
            endif(UNIX)
        endif(WIN32)        
    endif(MATLAB_ENGINE_INCLUDE_PATH)
endif(MATLAB_ENGINE_SHARED_LIBRARY_PATH)

