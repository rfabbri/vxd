# Try to find Matlab
# Once done this will define
#
# MATLAB_FOUND        - system has MATLAB
# MATLAB_INCLUDE_DIR  - where the Matlab include directory can be found
# MATLAB_LIBRARY      - Link this to use Matlab
#




if (WIN32)
  if (CYGWIN)

    # FIX Me

  else (CYGWIN)
    find_path(MATLAB_INCLUDE_DIR engine.h
      PATHS c:/ d:/
    )

    find_path(MATLAB_LIBRARY_PATH  mx eng
	PATHS      C:/ d:/
    )

    

  endif (CYGWIN)

else (WIN32)
    find_path(MATLAB_INCLUDE_DIR engine.h
    /usr/include
    /usr/local/MATLAB/extern/include
   /usr/local
  )

  find_path(MATLAB_LIBRARY_PATH libeng.so
    PATHS 
     /usr/lib
    /usr/local 
    /usr/local/MATLAB/bin/glnx86
  )   
  #FIX Me

endif (WIN32)


set( MATLAB_FOUND "NO" )
if(MATLAB_LIBRARY)
  set( MATLAB_FOUND "YES" )
endif(MATLAB_LIBRARY)

