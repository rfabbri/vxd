// This is basic/dbgrl/dbgrl_vertex.h
#ifndef dbgrl_vertex_h_
#define dbgrl_vertex_h_

//:
// \file
// \brief A templated vertex class for a generic graph
// \author Amir Tamrakar
// \date 02/30/05
//
// The vertex contains sets of incoming and outgoing 
// edges to other vertices in the graph
//
// \verbatim
//  Modifications
//   Amir Tamrakar 02/30/05    Initial version.
//   Anil Usumezbas 01/11/10   Added a pair of member function to reverse 
//                             the order of existing edge lists
// \endverbatim

#include <vcl_list.h>
#include <vcl_iostream.h>
#include <vbl/vbl_ref_count.h>
#include <vbl/vbl_smart_ptr.h>

//: A vertex in a graph
template<class E>
class dbgrl_vertex : public vbl_ref_count
{
protected:
  typedef vbl_smart_ptr<E> E_sptr;

 public:
  typedef typename vcl_list<E_sptr>::iterator edge_iterator;

  //: Constructor
  dbgrl_vertex() : vbl_ref_count(), out_edges_(), in_edges_() {}

  //: Destructor
  virtual ~dbgrl_vertex(){}

  //:  returns all outgoing edges of this vertex  
  const vcl_list<E_sptr>& out_edges() { return out_edges_; }

  //:  returns all incoming edges of this vertex  
  const vcl_list<E_sptr>& in_edges() { return in_edges_; }

  //: Returns an iterator to the beginning of the set of incoming edges
  edge_iterator in_edges_begin() { return in_edges_.begin(); }

  //: Returns an iterator to the end of the list of incoming edges
  edge_iterator in_edges_end() { return in_edges_.end(); }

  //: Returns an iterator to the beginning of the set of outgoing edges
  edge_iterator out_edges_begin() { return out_edges_.begin(); }

  //: Returns an iterator to the end of the list of outgoing edges
  edge_iterator out_edges_end() { return out_edges_.end(); }

  //: Returns the total number of edges at this vertex 
  int degree() const { return this->in_degree() + this->out_degree(); }

  //: Returns the number of incoming edges to this vertex 
  unsigned int in_degree() const { return in_edges_.size(); }

  //: Returns the number of outgoing edges to this vertex 
  unsigned int out_degree() const { return out_edges_.size(); }

  //: add an edge to the incoming edge list of this node
  void add_incoming_edge(E_sptr e) { in_edges_.push_back(e); }

  //: add an edge to the outgoing edge list of this node
  void add_outgoing_edge(E_sptr e) { out_edges_.push_back(e); }

  //: delete an edge incident on this node
  bool del_edge(E_sptr e);

  //: delete an edge from the incoming edge list
  bool del_in_edge(E_sptr e);

  //: delete an edge from the outgoing edge list
  bool del_out_edge(E_sptr e);

  //: delete all edges
  void del_all_edges();

  //: delete all the incoming edges
  void del_all_in_edges();

  //: delete all the outgoing edges
  void del_all_out_edges();

  //: reverse the order of the incoming edges
  void reverse_in_edges() { in_edges_.reverse(); }

  //: reverse the order of the outgoing edges
  void reverse_out_edges() { out_edges_.reverse(); }

  //: Print an ascii summary to the stream
  virtual void print_summary(vcl_ostream &os) const;

 protected:

  //: The pointers to outgoing edges
  vcl_list<E_sptr> out_edges_;

  //: The pointers to incoming edges
  vcl_list<E_sptr> in_edges_;
};

#endif // dbgrl_vertex_h_
