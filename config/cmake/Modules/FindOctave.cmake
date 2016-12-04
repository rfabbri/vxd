# Finds libraries and include files associated with Octave
# author: firat 

set(OCTAVE_FOUND "NO")

if(WIN32)
    find_path(OCTAVE_SHARED_LIBRARY_PATH liboctinterp.dll
        C:/ D:/
    )
    find_path(OCTAVE_INCLUDE_PATH octave.h
        C:/ D:/
    )    
else(WIN32)
    if(UNIX)
        find_path(OCTAVE_SHARED_LIBRARY_PATH liboctinterp.so
            /usr/lib/*
        )
        find_path(OCTAVE_INCLUDE_PATH octave.h
            /usr/include/*
        )           
    endif(UNIX)
endif(WIN32)  

if(OCTAVE_SHARED_LIBRARY_PATH)    
    if(OCTAVE_INCLUDE_PATH)
        if(WIN32)
            if(EXISTS "${OCTAVE_SHARED_LIBRARY_PATH}/liboctinterp.dll")
                if(EXISTS "${OCTAVE_INCLUDE_PATH}/octave.h")
                   set(OCTAVE_FOUND "YES") 
                endif(EXISTS "${OCTAVE_INCLUDE_PATH}/octave.h")
            endif(EXISTS "${OCTAVE_SHARED_LIBRARY_PATH}/liboctinterp.dll")
        else(WIN32)
            if(UNIX)
                if(EXISTS "${OCTAVE_SHARED_LIBRARY_PATH}/liboctinterp.so")
                    if(EXISTS "${OCTAVE_INCLUDE_PATH}/octave.h")
                       set(OCTAVE_FOUND "YES") 
                    endif(EXISTS "${OCTAVE_INCLUDE_PATH}/octave.h")
                endif(EXISTS "${OCTAVE_SHARED_LIBRARY_PATH}/liboctinterp.so")
            endif(UNIX)
        endif(WIN32)        
    endif(OCTAVE_INCLUDE_PATH)
endif(OCTAVE_SHARED_LIBRARY_PATH)

