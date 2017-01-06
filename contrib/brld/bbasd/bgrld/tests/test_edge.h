// This is basic/dbgrl/tests/test_edge.h
#ifndef test_edge_h_
#define test_edge_h_
//:
// \file
// \brief Test code for using the templatized graph class
// \author Amir Tamrakar
// \date 06/18/05
//

#include <dbgrl/dbgrl_edge.h>

#include "test_node.h"
#include "test_node_sptr.h"

//: a test edge class subclasssed from dbgrl_vertex
class test_edge : public dbgrl_edge<test_node>
{
protected:
  int id_;

public:
  test_edge() : dbgrl_edge<test_node>(), id_(-1){}
  test_edge(int id) : dbgrl_edge<test_node>(), id_(id){}
  test_edge(test_node_sptr v1, test_node_sptr v2) : dbgrl_edge<test_node>(v1,v2), id_(-1){}
  test_edge(int id, test_node_sptr v1, test_node_sptr v2) : dbgrl_edge<test_node>(v1,v2), id_(id){}
  virtual ~test_edge(){}

  int id(){return id_;}
  void set_id(int id){id_ = id;}

  //: Print an ascii summary to the stream
  virtual void print_summary(vcl_ostream &os) const
  {
    os << id_ << ": [" << source_->id() << "] - [" << target_->id() << "]" << vcl_endl;
  }

};

#endif //test_edge_h_
