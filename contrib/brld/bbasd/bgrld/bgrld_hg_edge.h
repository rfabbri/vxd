// This is lemsvxlsrc/bbasd/bgrld/bgrld_hg_edge.h
//:
// \file
// author: Ming-Ching Chang
// date:   Apr 04, 2005

#ifndef _bgrld_hg_edge_h_
#define _bgrld_hg_edge_h_

#include <vcl_vector.h>
#include <vcl_cassert.h>

#include <bgrld/bgrld_edge.h>

class dbhg_hg_vertex;
class dbhg_hg_hyperedge;

class bgrld_hg_edge //: public bgrld_edge
{
protected:
  int id_;
  //: connecting_vertices_[0] is the starting vertex, 
  //  connecting_vertices_[1] is the ending vertex.
  bgrld_hg_vertex*                connecting_vertices_[2];

  vcl_vector<bgrld_hg_hyperedge*> connecting_hyperedges_;

public:
  const int id() const {
    return id_;
  }
  void set_id (int id) {
    id_ = id;
  }
  //: ====== Graph connectivity query functions ======
  bgrld_hg_vertex* connecting_vertices (int i) {
    return connecting_vertices_[i];
  }
  bgrld_hg_vertex* connecting_s_vertex () {
    return connecting_vertices_[0];
  }
  bgrld_hg_vertex* connecting_e_vertex () {
    return connecting_vertices_[1];
  }
  bgrld_hg_vertex* get_other_connecting_vertex (bgrld_hg_vertex* vertex) {
    if (connecting_vertices_[0] == vertex)
      return connecting_vertices_[1];
    else {
      assert (vertex == connecting_vertices_[1]);
      return connecting_vertices_[0];
    }
  }
  const bool is_self_loop () const {
    return (connecting_vertices_[0] == connecting_vertices_[1]);
  }
  
  bgrld_hg_hyperedge* connecting_hyperedges (unsigned int i) {
    assert (i<connecting_hyperedges_.size());
    return connecting_hyperedges_[i];
  }
  vcl_vector<bgrld_hg_hyperedge*>& connecting_hyperedges() {
    return connecting_hyperedges_;
  }

  //: ====== Graph connectivity modification functions ======
  void connect_vertex (int i, bgrld_hg_vertex* vertex) {
    assert (i==0 || i==1);
    connecting_vertices_[i] = vertex;
  }
  void disconnect_vertex (int i) {
    assert (i==0 || i==1);
    connecting_vertices_[i] = NULL;
  }
  
  void connect_hyperedge (bgrld_hg_hyperedge* hyperedge) {
    connecting_hyperedges_.push_back (hyperedge);
  }
  bool disconnect_hyperedge (bgrld_hg_hyperedge* hyperedge) {
    vcl_vector<bgrld_hg_hyperedge*>::iterator it = connecting_hyperedges_.begin();
    for (; it != connecting_hyperedges_.end(); it++) {
      bgrld_hg_hyperedge* he = (*it);
      if (he == hyperedge) { //found it
        connecting_hyperedges_.erase (it);
        return true;
      }
    }
    assert (0);
    return false;
  }

  //: ====== Constructor/Destructor and other functions ======
  bgrld_hg_edge () //: bgrld_edge () 
  {
    connecting_vertices_[0] = NULL;
    connecting_vertices_[1] = NULL;
  }

  ~bgrld_hg_edge ()
  {
  }

};

#endif
