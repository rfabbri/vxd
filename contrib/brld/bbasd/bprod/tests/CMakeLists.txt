# basic/bprod/tests/CMakeLists.txt


add_executable( bprod_test_all
  test_driver.cxx
  test_process.cxx
  test_process_factory.cxx
  test_delay_filter.cxx
  test_storage.cxx
  bprod_sample_processes.cxx
)

target_link_libraries( bprod_test_all bprod testlib  )

add_test( bprod_test_process          bprod_test_all test_process )
add_test( bprod_test_process_factory  bprod_test_all test_process_factory )
add_test( bprod_test_delay_filter     bprod_test_all test_delay_filter )
add_test( bprod_test_storage          bprod_test_all test_storage )


add_executable( bprod_test_include test_include.cxx )
target_link_libraries( bprod_test_include bprod)
