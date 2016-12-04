# /basic/dbbl/CMakeLists.txt
# Development Brown Basics (templates) Library

# set(dbbl_sources 
#   dbbl_subsequence.h
#    )
# 
# #aux_source_directory(Templates dbbl_sources)
# 
# add_library(dbbl  ${dbbl_sources})
# 
# target_link_libraries(dbbl vbl)

if( BUILD_TESTING )
  add_subdirectory(tests)
endif( BUILD_TESTING )
