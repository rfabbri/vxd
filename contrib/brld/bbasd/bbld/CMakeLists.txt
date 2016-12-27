# /bbasd/bbld/CMakeLists.txt
# Development Brown Basics (templates) Library

# set(bbld_sources 
#   bbld_subsequence.h
#    )
# 
# #aux_source_directory(Templates bbld_sources)
# 
# add_library(bbld  ${bbld_sources})
# 
# target_link_libraries(bbld vbl)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
