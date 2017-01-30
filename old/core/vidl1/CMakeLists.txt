# vxl/vidl1/CMakeLists.txt
# FIXME use the newer vidl in your code FIXME
# codecs are here STUBBED AWAY: abort() will be called upon
# using any method that uses codecs.
# It is impossible to maintain the configuration for codecs
# up to date for an old lib like this.
# It is easier for you to switch your original code to use vidl,
# which is maintained by a large community, than to make vidl1 work
# with codecs and complicated dependencies.


# include( ${VXL_CMAKE_DIR}/FindMPEG2.cmake )
include( ${VXL_CMAKE_DIR}/NewCMake/FindFFMPEG.cmake )
# include( ${LEMS_CMAKE_DIR}/config/vxl_utils.cmake )

#  if( MPEG2_FOUND )
#    include_directories(${MPEG2_INCLUDE_DIR})
#    add_definitions(-DHAS_MPEG2)
#  endif()

if( FFMPEG_FOUND )
  include_directories(${FFMPEG_INCLUDE_DIR})
# some versions of FFMPEG require this definition before including
# the headers for C++ compatibility
#  add_definitions(-D__STDC_CONSTANT_MACROS )
endif()

# if ( FFMPEG_FOUND_SEVERAL )
#   add_definitions(-DHAS_FFMPEG_SEVERAL)
# endif ()

set(vidl1_sources
  dll.h

  vidl1_vob_frame_index.cxx    vidl1_vob_frame_index.h
  vidl1_file_sequence.cxx      vidl1_file_sequence.h
  vidl1_codec.cxx vidl1_codec.h vidl1_codec_sptr.h
  vidl1_clip.cxx  vidl1_clip.h  vidl1_clip_sptr.h
  vidl1_movie.cxx vidl1_movie.h vidl1_movie_sptr.h
  vidl1_frame.cxx vidl1_frame.h vidl1_frame_sptr.h
  vidl1_frame_resource.cxx     vidl1_frame_resource.h
  vidl1_image_list_codec.cxx vidl1_image_list_codec.h vidl1_image_list_codec_sptr.h
  vidl1_io.cxx    vidl1_io.h
)

# if( MPEG2_FOUND )
#   set(vidl1_sources ${vidl1_sources}
#     vidl1_mpegcodec.cxx vidl1_mpegcodec.h vidl1_mpegcodec_sptr.h
#     vidl1_mpegcodec_helper.cxx vidl1_mpegcodec_helper.h
#     vidl1_yuv_2_rgb.h
#   )
# endif()

# if( FFMPEG_FOUND )
# STUBBED AWAY: use vidl!
  set(vidl1_sources ${vidl1_sources}
    vidl1_ffmpeg_codec.cxx vidl1_ffmpeg_codec.h
  )
#endif()

aux_source_directory(Templates vidl1_sources)


# if (NOT AVIFILE_FOUND)
#   include(${CMAKE_ROOT}/Modules/FindAVIFile.cmake)
#   if(AVIFILE_FOUND)
#     include_directories( ${AVIFILE_INCLUDE_DIR} )
#     add_definitions (${AVIFILE_DEFINITIONS} )
#   endif()
# endif ()

# STUBBED AWAY: use vidl!
# set(AVI_FOUND "NO")
# if (AVIFILE_FOUND)
   set(vidl1_sources ${vidl1_sources}
       vidl1_avicodec.h
       vidl1_avifile_avicodec.h vidl1_avifile_avicodec.cxx
      )
# STUBBED AWAY: if on windows, use vidl!
#   set(AVI_LIBRARIES ${AVIFILE_LIBRARIES})
#   set(AVI_FOUND "YES")
#   add_definitions( -DHAS_AVI )
# else ()
#   if (WIN32)
#     if( NOT CYGWIN )
#       set(vidl1_sources ${vidl1_sources}
#           vidl1_avicodec.h
#           vidl1_win_avicodec.h vidl1_win_avicodec.cxx
#          )
# 
#       # Borland compilers may fail to link vfw32.lib if they are not
#       # installed correctly.  Make sure the ilink32.cfg file
#       # lists the directory containing the library in the -L option.
#       # For example: -L"c:\Borland\BCC55\Lib";"c:\Borland\BCC55\Lib\PSDK"
#       set(AVI_LIBRARIES vfw32 gdi32)
#       set(AVI_FOUND "YES")
#       add_definitions( -DHAS_AVI )
#     endif()
#   endif ()
# endif ()


add_library(vidl1 ${vidl1_sources})
# install_targets(/lib vidl1)
# INSTALL_NOBASE_HEADER_FILES(/include/vxl/core/vidl1 ${vidl1_sources})

target_link_libraries( vidl1 vbl vul vil vcl )
# if( MPEG2_FOUND )
#   target_link_libraries( vidl1 ${MPEG2_LIBRARIES} )
# endif()
#if( FFMPEG_FOUND )
#  target_link_libraries( vidl1 ${FFMPEG_LIBRARIES} )
#endif()
# if( AVI_FOUND )
#   target_link_libraries( vidl1 ${AVI_LIBRARIES} )
# endif()

if( BUILD_TESTING )
  add_subdirectory( tests )
endif()

## if( BUILD_EXAMPLES )
##   add_subdirectory( examples )
## endif()
