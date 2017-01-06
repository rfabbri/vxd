// This is lemsvxlsrc/basic/dbgrl/dbgrl_hg_hyperedge.h
//:
// \file
// author: Ming-Ching Chang
// date:   Apr 04, 2005

#ifndef _dbgrl_hg_hyperedge_h_
#define _dbgrl_hg_hyperedge_h_

#include <vcl_vector.h>
#include <vcl_cassert.h>

class dbgrl_hg_vertex;
class dbgrl_hg_edge;

class dbgrl_hg_hyperedge
{
protected:
  int id_;
  //: e1=(v1, v2), e2=(v2, v3), ..., en=(vn, v1) 
  vcl_vector<dbgrl_hg_edge*>    connecting_edges_;
  vcl_vector<dbgrl_hg_vertex*>  connecting_vertices_;

public:
  const int id() const {
    return id_;
  }
  void set_id (int id) {
    id_ = id;
  }
  //: ====== Graph connectivity query functions ======
  dbgrl_hg_edge* connecting_edges (unsigned int i) {
    assert (i<connecting_edges_.size());
    return connecting_edges_[i];
  }
  vcl_vector<dbgrl_hg_edge*>& connecting_edges() {
    return connecting_edges_;
  }

  dbgrl_hg_vertex* connecting_vertices (unsigned int i) {
    assert (i<connecting_vertices_.size());
    return connecting_vertices_[i];
  }
  vcl_vector<dbgrl_hg_vertex*>& connecting_vertices() {
    return connecting_vertices_;
  }

  //: ====== Graph connectivity modification functions ======
  void connect_edge (dbgrl_hg_edge* edge) {
    connecting_edges_.push_back (edge);
  }
  bool disconnect_edge (dbgrl_hg_edge* edge) {
    vcl_vector<dbgrl_hg_edge*>::iterator it = connecting_edges_.begin();
    for (; it != connecting_edges_.end(); it++) {
      dbgrl_hg_edge* e = (*it);
      if (e == edge) { //found it
        connecting_edges_.erase (it);
        return true;
      }
    }
    assert (0);
    return false;
  }

  void connect_vertex (dbgrl_hg_vertex* vertex) {
    connecting_vertices_.push_back (vertex);
  }
  bool disconnect_vertex (dbgrl_hg_vertex* vertex) {
    vcl_vector<dbgrl_hg_vertex*>::iterator it = connecting_vertices_.begin();
    for (; it != connecting_vertices_.end(); it++) {
      dbgrl_hg_vertex* v = (*it);
      if (v == vertex) { //found it
        connecting_vertices_.erase (it);
        return true;
      }
    }
    assert (0);
    return false;
  }

  //: ====== Constructor/Destructor and other functions ======
  dbgrl_hg_hyperedge ()
  {
  }

  ~dbgrl_hg_hyperedge ()
  {
  }
};

#endif
