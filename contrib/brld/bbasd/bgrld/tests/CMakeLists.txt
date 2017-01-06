# This basic/dbgrl/tests/CMakeLists.txt

include_directories( ${lems_SOURCE_DIR}/basic )

set( dbgrl_test_sources
  test_driver.cxx
  
  test_node.h    test_node_sptr.h
  test_edge.h    test_edge_sptr.h
  test_graph.h   test_graph_sptr.h
  
  graph_test.cxx
)

aux_source_directory(Templates dbgrl_test_sources)

add_executable ( dbgrl_test_all ${dbgrl_test_sources})

target_link_libraries( dbgrl_test_all dbgrl testlib )

add_test( dbgrl_test_graph   dbgrl_test_all graph_test )

#this is for test_includes
add_executable( dbgrl_test_include test_include.cxx)
target_link_libraries( dbgrl_test_include dbgrl)
