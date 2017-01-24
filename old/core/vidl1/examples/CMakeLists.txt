# vxl/vidl1/examples/CMakeLists.txt
include( ${VXL_CMAKE_DIR}/UseVGUI.cmake )


add_executable( vidl1_mpegcodec_example vidl1_mpegcodec_example.cxx )
target_link_libraries( vidl1_mpegcodec_example vidl1 )

add_executable( vidl1_video2images vidl1_video2images.cxx )
target_link_libraries( vidl1_video2images vidl1 vil vul vcl )


if( HAS_VGUI )

include_directories( ${VXLCORE_INCLUDE_DIR} )

set(vidl1_player_sources
    vidl1_player_main.cxx
    vidl1_player_menus.cxx      vidl1_player_menus.h
    vidl1_player_manager.cxx    vidl1_player_manager.h
   )

add_executable (vidl1_player ${vidl1_player_sources})
target_link_libraries(vidl1_player vidl1 vgui)

endif()
