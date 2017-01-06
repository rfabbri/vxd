// This is bbasd/bgrld/tests/test_graph.h
#ifndef test_graph_h_
#define test_graph_h_
//:
// \file
// \brief Test code for using the templatized graph class
// \author Based on original code by  Amir Tamrakar
// \date 06/18/05
//

#include <bgrld/bgrld_graph.h>

#include "test_edge.h"
#include "test_node.h"

//class test_node;
//class test_edge;

//: a test graph class subclasssed from bgrld_graph
class test_graph : public bgrld_graph<test_node, test_edge>
{
public:
  test_graph() : bgrld_graph<test_node, test_edge>() {}
  ~test_graph(){}

};

#endif //test_graph_h_
