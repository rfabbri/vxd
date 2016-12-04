# - Try to find QGLViewer
# Once done this will define
#
#  QGLVIEWER_FOUND - system has QGLViewer
#  QGLVIEWER_INCLUDE_DIR - the QGLViewer include directory
#  QGLVIEWER_LIBRARY - Link these to use QGLViewer
#  QGLVIEWER_DEFINITIONS - Compiler switches required for using QGLViewer
#


if (APPLE)

	find_path(QGLVIEWER_INCLUDE_DIR QGLViewer/qglviewer.h
	  /usr/include
      /usr/local/include
      /opt/local/include
	  ${QGLVIEWER_INCLUDE_DIR}
	  )
	
	find_library(QGLVIEWER_LIBRARY QGLViewer
	  /usr/lib
	  /usr/local/lib
	  /opt/local/lib
	  )
	
		
	if(QGLVIEWER_INCLUDE_DIR AND QGLVIEWER_LIBRARY)
		   set(QGLVIEWER_FOUND TRUE)
	endif(QGLVIEWER_INCLUDE_DIR AND QGLVIEWER_LIBRARY)
		
endif(APPLE)


if(QGLVIEWER_FOUND)
  if(NOT QGLViewer_FIND_QUIETLY)
	message(STATUS "Found QGLViewer: ${QGLVIEWER_LIBRARY}")
  endif(NOT QGLViewer_FIND_QUIETLY)
else(QGLVIEWER_FOUND)
  if(QGLViewer_FIND_REQUIRED)
	message(FATAL_ERROR "Could not find QGLViewer")
  endif(QGLViewer_FIND_REQUIRED)
endif(QGLVIEWER_FOUND)
	
# show the QGLVIEWER_INCLUDE_DIR and QGLVIEWER_LIBRARY variables only in the advanced view
mark_as_advanced(QGLVIEWER_INCLUDE_DIR QGLVIEWER_LIBRARY )