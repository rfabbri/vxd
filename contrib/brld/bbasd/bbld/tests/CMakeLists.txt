# bbasd/bbld/tests/CMakeLists.txt
# Tests for bbld library

include_directories( ${VXL_GEL_INCLUDE_DIR} )

add_executable( bbld_test_all
  test_driver.cxx
  test_subsequence.cxx
)

#target_link_libraries( bbld_test_all bbld vbl testlib  )
target_link_libraries( bbld_test_all vsol ${VXL_LIB_PREFIX}vbl ${VXL_LIB_PREFIX}testlib  )

add_test( NAME bbld_test_subsequence  COMMAND $<TARGET_FILE:bbld_test_all>  test_subsequence )

add_executable( bbld_test_include test_include.cxx )

#target_link_libraries( bbld_test_include bbld )
target_link_libraries( bbld_test_include vsol ${VXL_LIB_PREFIX}vbl )
