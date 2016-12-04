# Try to find SoWin
# Once done this will define
#
# SOWIN_FOUND        - system has SoWin from Open Inventor
# SOWIN_INCLUDE_DIR  - where the SoWin include directory can be found
# SOWIN_LIBRARY      - Link this to use SoWin
#
# Feb 09, 2005: by MingChing to enable contrib app to use SoWin.

if (WIN32)
  if (CYGWIN)

    find_library(COIN_LIBRARY SoWin
      /usr/lib
      /usr/local/lib
    )

  else (CYGWIN)

    find_path(SOWIN_INCLUDE_DIR Inventor/So.h
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\SIM\\Coin3D\\2;Installation Path]/include"
    )
    
    find_library(SOWIN_LIBRARY_DEBUG sowin1d
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\SIM\\Coin3D\\2;Installation Path]/lib"
    )

    find_library(SOWIN_LIBRARY_RELEASE sowin1
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\SIM\\Coin3D\\2;Installation Path]/lib"
    )

    if (SOWIN_LIBRARY_DEBUG AND SOWIN_LIBRARY_RELEASE)
      set(SOWIN_LIBRARY optimized ${SOWIN_LIBRARY_RELEASE}
                         debug ${SOWIN_LIBRARY_DEBUG})
    else (SOWIN_LIBRARY_DEBUG AND SOWIN_LIBRARY_RELEASE)
      if (SOWIN_LIBRARY_DEBUG)
        set (SOWIN_LIBRARY ${SOWIN_LIBRARY_DEBUG})
      endif (SOWIN_LIBRARY_DEBUG)
      if (SOWIN_LIBRARY_RELEASE)
        set (SOWIN_LIBRARY ${SOWIN_LIBRARY_RELEASE})
      endif (SOWIN_LIBRARY_RELEASE)
    endif (SOWIN_LIBRARY_DEBUG AND SOWIN_LIBRARY_RELEASE)

    if (SOWIN_LIBRARY)
      add_definitions ( -DSOWIN_NOT_DLL )
    #else (SOWIN_LIBRARY)
    #  set (SOWIN_LIBRARY coin2d CACHE STRING "SoWin Library (Debug) - Open Inventor GUI")
    endif (SOWIN_LIBRARY)
    
    #if (SOWIN_LIBRARY)
    #  add_definitions ( -DSOWIN_NOT_DLL )
    #else (SOWIN_LIBRARY)
    #  set (SOWIN_LIBRARY sowin1d CACHE STRING "SoWin Library (Debug) - Open Inventor GUI")
    #endif (SOWIN_LIBRARY)

  endif (CYGWIN)

else (WIN32)

  find_library(SOWIN_LIBRARY SoWin
    /usr/lib
    /usr/local/lib
  )   

endif (WIN32)

set( SOWIN_FOUND "NO" )
if(SOWIN_LIBRARY)  
  set( SOWIN_FOUND "YES" )
endif(SOWIN_LIBRARY)

