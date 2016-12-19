# breyes/bvis1/examples/CMakeLists.txt


if (VXL_VGUI_FOUND)

set(brown_eyes_sources

   bvis1_brown_eyes_menu.cxx       bvis1_brown_eyes_menu.h
   bvis1_brown_eyes_main.cxx
)

aux_source_directory(Templates bvis1_sources)

add_executable(brown_eyes1 ${brown_eyes_sources})

target_link_libraries(brown_eyes1 bvis1)

endif (VXL_VGUI_FOUND)
