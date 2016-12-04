# - Try to find QGLViewer
# Once done this will define
#
#  QWT_FOUND - system has QWT
#  QWT_INCLUDE_DIR - the QWT include directory
#  QWT_LIBRARY - Link these to use QWT
#  QWT_DEFINITIONS - Compiler switches required for using QWT
#


if (APPLE)

	find_path(QWT_INCLUDE_DIR qwt.h
	  /usr/include
      /usr/local/include
      /opt/local/include
	  ${QWT_INCLUDE_DIR}
	  )
	
	find_library(QWT_LIBRARY qwt
	  /usr/lib
	  /usr/local/lib
	  /opt/local/lib
	  )
	
		
	if(QWT_INCLUDE_DIR AND QWT_LIBRARY)
		   set(QWT_FOUND TRUE)
	endif(QWT_INCLUDE_DIR AND QWT_LIBRARY)
		
endif(APPLE)


if(QWT_FOUND)
  if(NOT QWT_FIND_QUIETLY)
	message(STATUS "Found QWT: ${QWT_LIBRARY}")
  endif(NOT QWT_FIND_QUIETLY)
else(QWT_FOUND)
  if(QWT_FIND_REQUIRED)
	message(FATAL_ERROR "Could not find QWT")
  endif(QWT_FIND_REQUIRED)
endif(QWT_FOUND)
	
# show the QWT_INCLUDE_DIR and QWT_LIBRARY variables only in the advanced view
mark_as_advanced(QWT_INCLUDE_DIR QWT_LIBRARY )