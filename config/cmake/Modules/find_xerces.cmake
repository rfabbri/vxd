#
# xerces 1.4 # Original date  : Dec 07, 2002
# author: J. L. Mundy
#
# Xerces 2.5 updated by T Orechia Date: May 16, 2004
# Xerces 2.6 updated by Matt Leotta Date: March 17, 2004

if (WIN32)
  if (CYGWIN)

    find_path(XERCES_INCLUDE_PATH xercesc/dom/DOM.hpp
      /usr/include
      /usr/local/include
    )

    find_library(XERCES_LIBRARY_PATH xerces-c
      /usr/lib
      /usr/local/lib
    )

  else (CYGWIN)
  
    find_path(XERCES_INCLUDE_PATH xercesc/dom/DOM.hpp
      ${vxl_SOURCE_DIR}/../xerces-c_2_6_0/include
      ${vxl_SOURCE_DIR}/../../xerces-c_2_6_0/include
      ${vxl_SOURCE_DIR}/../../../xerces-c_2_6_0/include)

    find_library(XERCES_LIBRARY_PATH xerces-c_2
      ${vxl_SOURCE_DIR}/../xerces-_c2_6_0/lib
      ${vxl_SOURCE_DIR}/../../xerces-_c2_6_0/lib
      ${vxl_SOURCE_DIR}/../../../xerces-c_2_6_0/lib )

  endif (CYGWIN)

else (WIN32)

  find_path(XERCES_INCLUDE_PATH xercesc/dom/DOM.hpp
    /usr/include
    /usr/local/include
  )

  find_library(XERCES_LIBRARY_PATH xerces-c
    /usr/lib
    /usr/local/lib
  )

endif (WIN32)



if(XERCES_INCLUDE_PATH)
  if(XERCES_LIBRARY_PATH)
    if(NOT HAS_XERCES)
      set(HAS_XERCES "YES")
      #set(VER , "2_5_0")
      #set(SO_TARGET_VERSION, "25")
      #set(SO_TARGET_VERSION_MAJOR,"0")
      include_directories( ${XERCES_INCLUDE_PATH} )
      set( XERCES_LIBRARIES ${XERCES_LIBRARY_PATH} )
     endif(NOT HAS_XERCES)
   endif(XERCES_LIBRARY_PATH)
endif(XERCES_INCLUDE_PATH)

