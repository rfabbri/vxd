# Feb 09 2005, by MingChing to enable contrib app to use SoWin.

set (HAS_SOWIN "NO")
if (USE_SOWIN)

  # Find the SoWin library
  include( ${VXD_CMAKE_DIR}/Modules/FindSoWin.cmake )
  
  if (SOWIN_FOUND)
    include_directories( ${COIN_INCLUDE_DIR} )
    add_definitions( -DHAS_SOWIN )
    set (HAS_SOWIN "YES")
    link_libraries( ${SOWIN_LIBRARY} )
  endif (SOWIN_FOUND)

endif (USE_SOWIN)

#Ming: a hack here!
#option(HAS_BGUI3D "Force HAS_BGUI3D" YES)
#add_definitions( -DHAS_BGUI3D )
