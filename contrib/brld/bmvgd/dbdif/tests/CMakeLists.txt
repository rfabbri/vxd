# This is brcv/mvg/dbdif/tests/CMakeLists.txt

add_executable ( dbdif_test_all
   # Driver source and utilities
   test_driver.cxx

   # The tests
   test_camera.cxx
)

target_link_libraries( dbdif_test_all dbdif testlib )

add_test( dbdif_test_camera dbdif_test_all test_camera )

add_executable( dbdif_test_include test_include.cxx )

target_link_libraries( dbdif_test_include dbdif )
