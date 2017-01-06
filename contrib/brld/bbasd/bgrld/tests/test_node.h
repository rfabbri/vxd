// This is bbasd/bgrld/tests/test_node.h
#ifndef test_node_h_
#define test_node_h_
//:
// \file
// \brief Test code for using the templatized graph class
// \author Based on original code by  Amir Tamrakar
// \date 06/18/05
//

#include <bgrld/bgrld_vertex.h>

class test_edge;

//: a test node class subclasssed from bgrld_vertex
class test_node : public bgrld_vertex<test_edge>
{
protected:
  int id_;

public:
  test_node(): bgrld_vertex<test_edge>(), id_(0) {}
  test_node(int id): bgrld_vertex<test_edge>(), id_(id) {}
  virtual ~test_node(){}

  int id(){return id_;}
  void set_id(int id){id_ = id;}

  //: Print an ascii summary to the stream
  virtual void print_summary(vcl_ostream &os) const
  {
    os << id_ << ": [" << this->degree() << "]" << vcl_endl;
  }
  
};

#endif //test_node_h_
