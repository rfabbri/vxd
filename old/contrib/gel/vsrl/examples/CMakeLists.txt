# gel/vsrl/examples/CMakeLists.txt

link_libraries( vsrl vil1 )

if( BUILD_RPL )

  add_executable( dense_matcher_full dense_matcher_full.cxx )
  add_executable( make_3d_output make_3d_output.cxx )

endif()

# Removed vsrl_viewer until old vidl dependency is removed.
#if( HAS_VGUI )
#if( BUILD_TBL )
#if( BUILD_BRL )
#if( BUILD_RPL )
#
#  add_executable( vsrl_viewer vsrl_viewer.cxx )
#  TARGET_link_libraries(vsrl_viewer vgui)
#
#endif()
#endif()
#endif()
#endif()
