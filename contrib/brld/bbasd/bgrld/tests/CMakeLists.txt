# This bbasd/bgrld/tests/CMakeLists.txt

include_directories( ${lems_SOURCE_DIR}/basic )

set( bgrld_test_sources
  test_driver.cxx
  
  test_node.h    test_node_sptr.h
  test_edge.h    test_edge_sptr.h
  test_graph.h   test_graph_sptr.h
  
  graph_test.cxx
)

aux_source_directory(Templates bgrld_test_sources)

add_executable ( bgrld_test_all ${bgrld_test_sources})

target_link_libraries( bgrld_test_all bgrld testlib )

add_test( bgrld_test_graph   bgrld_test_all graph_test )

#this is for test_includes
add_executable( bgrld_test_include test_include.cxx)
target_link_libraries( bgrld_test_include bgrld)
