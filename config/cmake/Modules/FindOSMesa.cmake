#
# OSMesa # Original date  : Nov 22, 2005
# author: T. Orechia
#

if (NOT WIN32)
  find_path(OSMESA_INCLUDE_PATH  GL/osmesa.h
    /usr/X11R6/include
    /usr/include
    /usr/local/include
  )

  find_library(OSMESA_LIBRARY_PATH OSMesa
    /usr/X11R6/lib
    /usr/lib
    /usr/local/lib
  )


  if (OSMESA_INCLUDE_PATH)
   if (OSMESA_LIBRARY_PATH)
    if (NOT HAS_OSMESA)
      set(HAS_OSMESA "YES")
      set( OSMESA_LIBRARIES ${OSMESA_LIBRARY_PATH} )
      include_directories( ${OSMESA_INCLUDE_PATH} )
     endif (NOT HAS_OSMESA)
    endif (OSMESA_LIBRARY_PATH)
   endif (OSMESA_INCLUDE_PATH)

endif (NOT WIN32)

