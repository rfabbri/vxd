# breye1/CMakeLists.txt
# Old Brown Eyes package.

include_directories(${VXL_VGUI_INCLUDE_DIR})
include_directories(${VXL_BRL_INCLUDE_DIR})

# Brown Process Library (Old)
add_subdirectory(bpro1)

# Video Process Library (Old)
add_subdirectory(vidpro1)

# Visualizer (will move to brl someday) (Old)
add_subdirectory(bvis1)
