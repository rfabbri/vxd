# gel/vgel/kl/CMakeLists.txt

set(vgel_kl_sources
                      base.h
 convolve.c           convolve.h
 klt.c                klt.h
 pnmio.c              pnmio.h
 selectGoodFeatures.c
 trackFeatures.c
 error.c              error.h
 klt_util.c           klt_util.h
 pyramid.c            pyramid.h
 storeFeatures.c
 writeFeatures.c
)

add_library(vgel_kl ${vgel_kl_sources})

if(UNIX)
  TARGET_link_libraries(vgel_kl m)
endif()

if( BUILD_TESTING )
  add_subdirectory(tests)
endif()
