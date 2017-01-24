# ./gel/vgel/examples/CMakeLists.txt

set( kl_tracker_sources
  kl_tracker.cxx
)

add_executable( kl_tracker ${kl_tracker_sources} )

target_link_libraries( kl_tracker vgel vtol vil )
