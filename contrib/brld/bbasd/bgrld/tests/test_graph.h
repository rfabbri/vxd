// This is basic/dbgrl/tests/test_graph.h
#ifndef test_graph_h_
#define test_graph_h_
//:
// \file
// \brief Test code for using the templatized graph class
// \author Amir Tamrakar
// \date 06/18/05
//

#include <dbgrl/dbgrl_graph.h>

#include "test_edge.h"
#include "test_node.h"

//class test_node;
//class test_edge;

//: a test graph class subclasssed from dbgrl_graph
class test_graph : public dbgrl_graph<test_node, test_edge>
{
public:
  test_graph() : dbgrl_graph<test_node, test_edge>() {}
  ~test_graph(){}

};

#endif //test_graph_h_
