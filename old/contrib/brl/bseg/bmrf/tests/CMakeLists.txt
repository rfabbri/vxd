# This is contrib/brl/bseg/bmrf/tests/CMakeLists.txt

include_directories(${BRL_INCLUDE_DIR} )

add_executable ( bmrf_test_all
  test_driver.cxx
  test_epi_point.cxx
  test_epi_seg.cxx
  test_node.cxx
  test_network.cxx
  test_network_builder.cxx
)
TARGET_link_libraries( bmrf_test_all bmrf vgl vil vbl_io vbl vpl testlib )

add_test( bmrf_test_epi_point       ${EXECUTABLE_OUTPUT_PATH}/bmrf_test_all test_epi_point)
add_test( bmrf_test_epi_seg         ${EXECUTABLE_OUTPUT_PATH}/bmrf_test_all test_epi_seg)
add_test( bmrf_test_node            ${EXECUTABLE_OUTPUT_PATH}/bmrf_test_all test_node)
add_test( bmrf_test_network         ${EXECUTABLE_OUTPUT_PATH}/bmrf_test_all test_network)
add_test( bmrf_test_network_builder ${EXECUTABLE_OUTPUT_PATH}/bmrf_test_all test_network_builder)

add_executable( bmrf_test_include test_include.cxx)
TARGET_link_libraries( bmrf_test_include bmrf)
