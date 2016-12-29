# brcv/mvg/dvpgl/algo/cmd/CMakeLists.txt
# Photogrametry Algorithms Command Line Tools.


include_directories(${lems_SOURCE_DIR}/brcv/mvg/bmvl )
include_directories(${VXL_GEL_INCLUDE_DIR} )

add_executable(dvpgl_optimize_cmd dvpgl_optimize_cmd.cxx)
target_link_libraries(dvpgl_optimize_cmd vpgl_algo brct)
