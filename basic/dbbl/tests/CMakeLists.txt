# basic/dbbl/tests/CMakeLists.txt
# Tests for dbbl library

include_directories( ${VXL_GEL_INCLUDE_DIR} )

add_executable( dbbl_test_all
  test_driver.cxx
  test_subsequence.cxx
)

#target_link_libraries( dbbl_test_all dbbl vbl testlib  )
target_link_libraries( dbbl_test_all vsol ${VXL_LIB_PREFIX}vbl ${VXL_LIB_PREFIX}testlib  )

add_test( NAME dbbl_test_subsequence  COMMAND $<TARGET_FILE:dbbl_test_all>  test_subsequence )

add_executable( dbbl_test_include test_include.cxx )

#target_link_libraries( dbbl_test_include dbbl )
target_link_libraries( dbbl_test_include vsol ${VXL_LIB_PREFIX}vbl )
