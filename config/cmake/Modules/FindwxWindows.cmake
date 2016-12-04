## May 1, 2005 Ming-Ching Chang
## My modified version of CMake's FindwxWindows.cmake to remove CMake error
## if wxWindows is not installed.
##
##
## This module finds if wxWindows is installed and determines where the
## include files and libraries are. It also determines what the name of
## the library is. This code sets the following variables:
##
## ------------------------------------------------------------------
##
## WXWINDOWS_FOUND            = system has WxWindows (in desired config. build)
##
## WXWINDOWS_LIBRARIES        = full path to the wxWindows libraries
##                              on Unix/Linux with additional linker flags from
##                              "wx-config --libs"
## 
## CMAKE_WXWINDOWS_CXX_FLAGS  = Unix compiler flags for wxWindows, essentially
##                              "`wx-config --cxxflags`"
##
## WXWINDOWS_INCLUDE_DIR      = where to find headers "wx/wx.h" "wx/setup.h"
##
## WXWINDOWS_LINK_DIRECTORIES = link directories, useful for rpath on Unix
##
## WXWINDOWS_DEFINITIONS      = extra defines
##
## deprecated: 
##   * CMAKE_WX_CAN_COMPILE
##   * WXWINDOWS_LIBRARY
##   * CMAKE_WX_CXX_FLAGS
##   * WXWINDOWS_INCLUDE_PATH
##
## ------------------------------------------------------------------
##
## USAGE: 
##   # for convenience include Use_wxWindows.cmake in your projects
##     CMakeLists.txt: 
##   
##   # if you need OpenGL support please
## set(WXWINDOWS_USE_GL 1) 
##   # in your CMakeLists.txt *before* you include this file.
##
##   # just include Use_wxWindows.cmake 
##   # in your projects CMakeLists.txt
## include( ${CMAKE_ROOT}/Modules/Use_wxWindows.cmake)
## 
## ------------------------------------------------------------------
## Author Jan Woetzel <http://www.mip.informatik.uni-kiel.de/~jw> (07/2003)
## ------------------------------------------------------------------
## 
## -changed variable names to conventions from cmakes readme.txt (Jan Woetzel
##  07/07/2003)
## -added definition WINVER for WIN32 (Jan Woetzel 07/07//2003)
## -added if(CMAKE_WXWINDOWS_CONFIG_EXECUTABLE) found and changed
##  CMAKE_WX_CONFIG to CMAKE_WXWINDOWS_CONFIG_EXECUTABLE (Jan Woetzel
##  07/22/2003)
## -removed OPTION for CMAKE_WXWINDOWS_USE_GL. Force the developer to SET it.
## 
## status: 
## tested with:
##   -cmake 1.6.7, Linux (Suse 7.3), wxWindows 2.4.0, gcc 2.95
##   -cmake 1.6.7, Linux (Suse 8.2), wxWindows 2.4.0, gcc 3.3
##   -cmake 1.6.7, Linux (Suse 8.2), wxWindows 2.4.1-patch1,  gcc 3.3
##   -cmake 1.6.7, MS Windows XP home, wxWindows 2.4.1, MS Visual Studio .net 7
##    2002 (static build)
## 
## TODO: 
##  -OPTION for unicode builds 
##  -change WXWINDOWS_USE_GL to use FindOpenGL.cmake or let the user do it
##  -testing of DLL linking under MS WIN32
## 

if(WIN32)
  set(WIN32_STYLE_FIND 1)
endif(WIN32)
if(MINGW)
  set(WIN32_STYLE_FIND 0)
  set(UNIX_STYLE_FIND 1)
endif(MINGW)
if(UNIX)
  set(UNIX_STYLE_FIND 1)
endif(UNIX)


if(WIN32_STYLE_FIND)
  
  ## ######################################################################
  ##
  ## Windows specific:
  ##
  ## candidates for root/base directory of wxwindows
  ## should have subdirs include and lib containing include/wx/wx.h
  ## fix the root dir to avoid mixing of headers/libs from different
  ## versions/builds:
  
  set (WXWINDOWS_POSSIBLE_ROOT_PATHS
    $ENV{WXWIN}
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\wxWindows_is1;Inno Setup: App Path]" )
  
  find_path(WXWINDOWS_ROOT_DIR  include/wx/wx.h 
    ${WXWINDOWS_POSSIBLE_ROOT_PATHS} )
  
  ## message("DBG found WXWINDOWS_ROOT_DIR: ${WXWINDOWS_ROOT_DIR}")
  
  ## find libs for combination of static/shared with release/debug
  set (WXWINDOWS_POSSIBLE_LIB_PATHS
    "${WXWINDOWS_ROOT_DIR}/lib" )
  
  find_library(WXWINDOWS_STATIC_LIBRARY
    NAMES wx wxmsw
    PATHS ${WXWINDOWS_POSSIBLE_LIB_PATHS}
    DOC "wxWindows static release build library" )
  
  find_library(WXWINDOWS_STATIC_DEBUG_LIBRARY
    NAMES wxd wxmswd
    PATHS ${WXWINDOWS_POSSIBLE_LIB_PATHS} 
    DOC "wxWindows static debug build library" )
  
  find_library(WXWINDOWS_SHARED_LIBRARY
    NAMES wxmsw24 wxmsw241 wxmsw240 wx23_2 wx22_9 
    PATHS ${WXWINDOWS_POSSIBLE_LIB_PATHS} 
    DOC "wxWindows shared release build library" )
  
  find_library(WXWINDOWS_SHARED_DEBUG_LIBRARY
    NAMES wxmsw24d wxmsw241d wxmsw240d wx23_2d wx22_9d 
    PATHS ${WXWINDOWS_POSSIBLE_LIB_PATHS} 
    DOC "wxWindows shared debug build library " )
  
  
  ## if there is at least one shared lib available
  ## let user choose wether to use shared or static wxwindows libs 
  if(WXWINDOWS_SHARED_LIBRARY OR WXWINDOWS_SHARED_DEBUG_LIBRARY)
    ## default value OFF because wxWindows MSVS default build is static
    option(WXWINDOWS_USE_SHARED_LIBS
      "Use shared versions (dll) of wxWindows libraries?" OFF)
    mark_as_advanced(WXWINDOWS_USE_SHARED_LIBS)
  endif(WXWINDOWS_SHARED_LIBRARY OR WXWINDOWS_SHARED_DEBUG_LIBRARY)
  
  
  ## add system libraries wxwindows depends on
  set(CMAKE_WXWINDOWS_LIBRARIES ${CMAKE_WXWINDOWS_LIBRARIES}
    comctl32
    rpcrt4
    wsock32
    ##  presumably ctl3d32 is not neccesary (Jan Woetzel 07/2003)
    #   ctl3d32
    #    debug ${WXWINDOWS_ROOT_DIR}/lib/zlibd.lib  optimized ${WXWINDOWS_ROOT_DIR}/lib/zlibd.lib
    #    debug ${WXWINDOWS_ROOT_DIR}/lib/regexd.lib optimized ${WXWINDOWS_ROOT_DIR}/lib/regexd.lib 
    #    debug ${WXWINDOWS_ROOT_DIR}/lib/pngd.lib   optimized ${WXWINDOWS_ROOT_DIR}/lib/pngd.lib
    #    debug ${WXWINDOWS_ROOT_DIR}/lib/jpegd.lib  optimized ${WXWINDOWS_ROOT_DIR}/lib/jpegd.lib
    #    debug ${WXWINDOWS_ROOT_DIR}/lib/tiffd.lib  optimized ${WXWINDOWS_ROOT_DIR}/lib/tiff.lib
    )
  
  # JW removed option and force the develper th SET it. 
  # option(WXWINDOWS_USE_GL "use wxWindows with GL support (use additional
  # opengl, glu libs)?" OFF)
  
  ## opengl/glu: (TODO/FIXME: better use FindOpenGL.cmake here 
  if (WXWINDOWS_USE_GL)
    set(CMAKE_WXWINDOWS_LIBRARIES ${CMAKE_WXWINDOWS_LIBRARIES}
      opengl32
      glu32
      )
  endif (WXWINDOWS_USE_GL)
  
  
  ##
  ## select between use of  shared or static wxWindows lib then set libs to use
  ## for debug and optimized build.  so the user can switch between debug and
  ## release build e.g. within MS Visual Studio without running cmake with a
  ## different build directory again.
  ## 
  ## then add the build specific include dir for wx/setup.h
  ## 
  
  if(WXWINDOWS_USE_SHARED_LIBS)
    ##message("DBG wxWindows use shared lib selected.")
    
    ## shared: both wx (debug and release) found?
    if(WXWINDOWS_SHARED_DEBUG_LIBRARY AND WXWINDOWS_SHARED_LIBRARY)
      ##message("DBG wx shared: debug and optimized found.")
      set(WXWINDOWS_LIBRARIES ${CMAKE_WXWINDOWS_LIBRARIES}
        debug     ${WXWINDOWS_SHARED_DEBUG_LIBRARY}
        optimized ${WXWINDOWS_SHARED_LIBRARY} )
      find_path(WXWINDOWS_INCLUDE_DIR_SETUPH  wx/setup.h
        ${WXWINDOWS_ROOT_DIR}/lib/mswdlld
        ${WXWINDOWS_ROOT_DIR}/lib/mswdll )
    endif(WXWINDOWS_SHARED_DEBUG_LIBRARY AND WXWINDOWS_SHARED_LIBRARY)
    
    ## shared: only debug wx lib found?
    if(WXWINDOWS_SHARED_DEBUG_LIBRARY)
      if(NOT WXWINDOWS_SHARED_LIBRARY)
        ##message("DBG wx shared: debug (but no optimized) found.")
        set(WXWINDOWS_LIBRARIES ${CMAKE_WXWINDOWS_LIBRARIES}
          ${WXWINDOWS_SHARED_DEBUG_LIBRARY} )
        find_path(WXWINDOWS_INCLUDE_DIR_SETUPH  wx/setup.h
          ${WXWINDOWS_ROOT_DIR}/lib/mswdlld )
      endif(NOT WXWINDOWS_SHARED_LIBRARY)
    endif(WXWINDOWS_SHARED_DEBUG_LIBRARY)
    
    ## shared: only release wx lib found?
    if(NOT WXWINDOWS_SHARED_DEBUG_LIBRARY)
      if(WXWINDOWS_SHARED_LIBRARY)
        ##message("DBG wx shared: optimized (but no debug) found.")
        set(WXWINDOWS_LIBRARIES ${CMAKE_WXWINDOWS_LIBRARIES}
          ${WXWINDOWS_SHARED_DEBUG_LIBRARY} )
        find_path(WXWINDOWS_INCLUDE_DIR_SETUPH  wx/setup.h
          ${WXWINDOWS_ROOT_DIR}/lib/mswdll )
      endif(WXWINDOWS_SHARED_LIBRARY)    
    endif(NOT WXWINDOWS_SHARED_DEBUG_LIBRARY)
    
    ## shared: none found?
    #if(NOT WXWINDOWS_SHARED_DEBUG_LIBRARY)
    #  if(NOT WXWINDOWS_SHARED_LIBRARY)
    #    message(SEND_ERROR 
    #      "No shared wxWindows lib found, but WXWINDOWS_USE_SHARED_LIBS=${WXWINDOWS_USE_SHARED_LIBS}.")
    #  endif(NOT WXWINDOWS_SHARED_LIBRARY)
    #endif(NOT WXWINDOWS_SHARED_DEBUG_LIBRARY)


  else(WXWINDOWS_USE_SHARED_LIBS)
    ##message("DBG wxWindows static lib selected.")

    ## static: both wx (debug and release) found?
    if(WXWINDOWS_STATIC_DEBUG_LIBRARY AND WXWINDOWS_STATIC_LIBRARY)
      ##message("DBG wx static: debug and optimized found.")
      set(WXWINDOWS_LIBRARIES ${CMAKE_WXWINDOWS_LIBRARIES}
        debug     ${WXWINDOWS_STATIC_DEBUG_LIBRARY}
        optimized ${WXWINDOWS_STATIC_LIBRARY} )
      find_path(WXWINDOWS_INCLUDE_DIR_SETUPH  wx/setup.h
        ${WXWINDOWS_ROOT_DIR}/lib/mswd
        ${WXWINDOWS_ROOT_DIR}/lib/msw )
    endif(WXWINDOWS_STATIC_DEBUG_LIBRARY AND WXWINDOWS_STATIC_LIBRARY)
    
    ## static: only debug wx lib found?
    if(WXWINDOWS_STATIC_DEBUG_LIBRARY)
      if(NOT WXWINDOWS_STATIC_LIBRARY)
        ##message("DBG wx static: debug (but no optimized) found.")
        set(WXWINDOWS_LIBRARIES ${CMAKE_WXWINDOWS_LIBRARIES}
          ${WXWINDOWS_STATIC_DEBUG_LIBRARY} )
        find_path(WXWINDOWS_INCLUDE_DIR_SETUPH  wx/setup.h
          ${WXWINDOWS_ROOT_DIR}/lib/mswd )
      endif(NOT WXWINDOWS_STATIC_LIBRARY)
    endif(WXWINDOWS_STATIC_DEBUG_LIBRARY)
    
    ## static: only release wx lib found?
    if(NOT WXWINDOWS_STATIC_DEBUG_LIBRARY)
      if(WXWINDOWS_STATIC_LIBRARY)
        ##message("DBG wx static: optimized (but no debug) found.")
        set(WXWINDOWS_LIBRARIES ${CMAKE_WXWINDOWS_LIBRARIES}
          ${WXWINDOWS_STATIC_DEBUG_LIBRARY} )
        find_path(WXWINDOWS_INCLUDE_DIR_SETUPH  wx/setup.h
          ${WXWINDOWS_ROOT_DIR}/lib/msw )
      endif(WXWINDOWS_STATIC_LIBRARY)
    endif(NOT WXWINDOWS_STATIC_DEBUG_LIBRARY)
    
    ## static: none found?
    #if(NOT WXWINDOWS_STATIC_DEBUG_LIBRARY)
    #  if(NOT WXWINDOWS_STATIC_LIBRARY)
    #    message(SEND_ERROR 
    #      "No static wxWindows lib found, but WXWINDOWS_USE_SHARED_LIBS=${WXWINDOWS_USE_SHARED_LIBS}.")
    #  endif(NOT WXWINDOWS_STATIC_LIBRARY)
    #endif(NOT WXWINDOWS_STATIC_DEBUG_LIBRARY)
    
  endif(WXWINDOWS_USE_SHARED_LIBS)  
  
  
  ## not neccessary in wxWindows 2.4.1 
  ## but it may fix a previous bug, see
  ## http://lists.wxwindows.org/cgi-bin/ezmlm-cgi?8:mss:37574:200305:mpdioeneabobmgjenoap
  option(WXWINDOWS_SET_DEFINITIONS "Set additional defines for wxWindows" OFF)
  mark_as_advanced(WXWINDOWS_SET_DEFINITIONS)
  if (WXWINDOWS_SET_DEFINITIONS) 
    set(WXWINDOWS_DEFINITIONS "-DWINVER=0x400")
  else (WXWINDOWS_SET_DEFINITIONS) 
    # clear:
    set(WXWINDOWS_DEFINITIONS "")
  endif (WXWINDOWS_SET_DEFINITIONS) 
  
  
  ## Find the include directories for wxwindows
  ## the first, build specific for wx/setup.h was determined before.
  ## add inc dir for general for "wx/wx.h" 
  set (WXWINDOWS_POSSIBLE_INCLUDE_PATHS
    "${WXWINDOWS_ROOT_DIR}/include"
    )
  find_path(WXWINDOWS_INCLUDE_DIR  wx/wx.h 
    ${WXWINDOWS_POSSIBLE_INCLUDE_PATHS}
    )  
  # append the build specific include dir for wx/setup.h:
  if (WXWINDOWS_INCLUDE_DIR_SETUPH)
    set(WXWINDOWS_INCLUDE_DIR ${WXWINDOWS_INCLUDE_DIR} ${WXWINDOWS_INCLUDE_DIR_SETUPH} )
  endif (WXWINDOWS_INCLUDE_DIR_SETUPH)
  
  mark_as_advanced(
    WXWINDOWS_ROOT_DIR
    WXWINDOWS_INCLUDE_DIR
    WXWINDOWS_INCLUDE_DIR_SETUPH
    WXWINDOWS_STATIC_LIBRARY
    WXWINDOWS_STATIC_DEBUG_LIBRARY
    WXWINDOWS_SHARED_LIBRARY
    WXWINDOWS_SHARED_DEBUG_LIBRARY
    )
  
  
else(WIN32_STYLE_FIND)
  if (UNIX_STYLE_FIND) 
    ## ######################################################################
    ## 
    ## UNIX/Linux specific:
    ## 
    ## use backquoted wx-config to query and set flags and libs:
    ## 06/2003 Jan Woetzel
    ## 
    
    option(WXWINDOWS_USE_SHARED_LIBS "Use shared versions (.so) of wxWindows libraries" ON)
    mark_as_advanced(WXWINDOWS_USE_SHARED_LIBS)

    # JW removed option and force the develper th SET it. 
    # option(WXWINDOWS_USE_GL "use wxWindows with GL support (use additional
    # --gl-libs for wx-config)?" OFF)
    
    # wx-config should be in your path anyhow, usually no need to set WXWIN or
    # search in ../wx or ../../wx
    find_program(CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE wx-config
      $ENV{WXWIN}
      ../wx/bin
      ../../wx/bin )
    
    # check wether wx-config was found:
    if(CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE)    

      # use shared/static wx lib?
      # remember: always link shared to use systems GL etc. libs (no static
      # linking, just link *against* static .a libs)
      if(WXWINDOWS_USE_SHARED_LIBS)
        set(WX_CONFIG_ARGS_LIBS "--libs")
      else(WXWINDOWS_USE_SHARED_LIBS)
        set(WX_CONFIG_ARGS_LIBS "--static --libs")
      endif(WXWINDOWS_USE_SHARED_LIBS)
      
      # do we need additionial wx GL stuff like GLCanvas ?
      if(WXWINDOWS_USE_GL)
        set(WX_CONFIG_ARGS_LIBS "${WX_CONFIG_ARGS_LIBS} --gl-libs" )
      endif(WXWINDOWS_USE_GL)
      ##message("DBG: WX_CONFIG_ARGS_LIBS=${WX_CONFIG_ARGS_LIBS}===")
      
      # set CXXFLAGS to be fed into CMAKE_CXX_FLAGS by the user:
      set(CMAKE_WXWINDOWS_CXX_FLAGS "`${CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE} --cxxflags`")
      ##message("DBG: for compilation:
      ##CMAKE_WXWINDOWS_CXX_FLAGS=${CMAKE_WXWINDOWS_CXX_FLAGS}===")

      # keep the back-quoted string for clarity
      set(WXWINDOWS_LIBRARIES "`${CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE} ${WX_CONFIG_ARGS_LIBS}`")
      ##message("DBG2: for linking:
      ##WXWINDOWS_LIBRARIES=${WXWINDOWS_LIBRARIES}===")
      
      # evaluate wx-config output to separate linker flags and linkdirs for
      # rpath:
      exec_program(${CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE}
        ARGS ${WX_CONFIG_ARGS_LIBS}
        OUTPUT_VARIABLE WX_CONFIG_LIBS )
      
      ## extract linkdirs (-L) for rpath
      ## use regular expression to match wildcard equivalent "-L*<endchar>"
      ## with <endchar> is a space or a semicolon
      string(REGEX MATCHALL "[-][L]([^ ;])+" WXWINDOWS_LINK_DIRECTORIES_WITH_PREFIX "${WX_CONFIG_LIBS}" )
      #message("DBG  WXWINDOWS_LINK_DIRECTORIES_WITH_PREFIX=${WXWINDOWS_LINK_DIRECTORIES_WITH_PREFIX}")
      
      ## remove prefix -L because we need the pure directory for LINK_DIRECTORIES
      ## replace -L by ; because the separator seems to be lost otherwise (bug or
      ## feature?)
      if(WXWINDOWS_LINK_DIRECTORIES_WITH_PREFIX)
        string(REGEX REPLACE "[-][L]" ";" WXWINDOWS_LINK_DIRECTORIES ${WXWINDOWS_LINK_DIRECTORIES_WITH_PREFIX} )
        #message("DBG  WXWINDOWS_LINK_DIRECTORIES=${WXWINDOWS_LINK_DIRECTORIES}")
      endif(WXWINDOWS_LINK_DIRECTORIES_WITH_PREFIX)

      
      ## replace space separated string by semicolon separated vector to make it
      ## work with LINK_DIRECTORIES
      separate_arguments(WXWINDOWS_LINK_DIRECTORIES)
      
      mark_as_advanced(
        CMAKE_WXWINDOWS_CXX_FLAGS
        WXWINDOWS_INCLUDE_DIR
        WXWINDOWS_LIBRARIES
        CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE
        )
      
      
      # we really need wx-config...
      # Ming: comment out for now
    else(CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE)    
    #  message(SEND_ERROR "Cannot find wx-config anywhere on the system. Please put the file into your path or specify it in CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE.")
      mark_as_advanced(CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE)
    endif(CMAKE_WXWINDOWS_WXCONFIG_EXECUTABLE)

    
    
  else(UNIX_STYLE_FIND)
    message(SEND_ERROR "FindwxWindows.cmake:  Platform unknown/unsupported by FindwxWindows.cmake. It's neither WIN32 nor UNIX")
  endif(UNIX_STYLE_FIND)
endif(WIN32_STYLE_FIND)


if(WXWINDOWS_LIBRARIES)
  if(WXWINDOWS_INCLUDE_DIR OR CMAKE_WXWINDOWS_CXX_FLAGS)
    
    ## found all we need.
    set(WXWINDOWS_FOUND 1)
    
    ## set deprecated variables for backward compatibility: 
    set(CMAKE_WX_CAN_COMPILE   ${WXWINDOWS_FOUND})
    set(WXWINDOWS_LIBRARY      ${WXWINDOWS_LIBRARIES})
    set(WXWINDOWS_INCLUDE_PATH ${WXWINDOWS_INCLUDE_DIR})
    set(CMAKE_WX_CXX_FLAGS     ${CMAKE_WXWINDOWS_CXX_FLAGS})
    
  endif(WXWINDOWS_INCLUDE_DIR OR CMAKE_WXWINDOWS_CXX_FLAGS)
endif(WXWINDOWS_LIBRARIES)
