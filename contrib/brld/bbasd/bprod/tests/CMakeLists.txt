# basic/dbpro/tests/CMakeLists.txt


add_executable( dbpro_test_all
  test_driver.cxx
  test_process.cxx
  test_process_factory.cxx
  test_delay_filter.cxx
  test_storage.cxx
  dbpro_sample_processes.cxx
)

target_link_libraries( dbpro_test_all dbpro testlib  )

add_test( dbpro_test_process          dbpro_test_all test_process )
add_test( dbpro_test_process_factory  dbpro_test_all test_process_factory )
add_test( dbpro_test_delay_filter     dbpro_test_all test_delay_filter )
add_test( dbpro_test_storage          dbpro_test_all test_storage )


add_executable( dbpro_test_include test_include.cxx )
target_link_libraries( dbpro_test_include dbpro)
