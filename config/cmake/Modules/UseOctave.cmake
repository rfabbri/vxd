# Finds libraries and include files associated with Octave
# author: firat

include( ${VXD_CMAKE_DIR}/Modules/FindOctave.cmake )
if(OCTAVE_FOUND)    
    include_directories(${OCTAVE_INCLUDE_PATH})
    link_directories(${OCTAVE_SHARED_LIBRARY_PATH})   
    add_definitions( -DOCTAVE_FOUND=1 )
else(OCTAVE_FOUND)
    add_definitions( -DOCTAVE_FOUND=0 )
endif(OCTAVE_FOUND)



