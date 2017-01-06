// This is lemsvxlsrc/basic/dbgrl/dbgrl_hg_vertex.h
//:
// \file
// author: Ming-Ching Chang
// date:   Apr 04, 2005

#ifndef _dbgrl_hg_vertex_h_
#define _dbgrl_hg_vertex_h_

#include <vcl_vector.h>
#include <vcl_cassert.h>

#include <dbgrl/dbgrl_vertex.h>

class dbgrl_hg_edge;
class dbgrl_hg_hyperedge;

class dbgrl_hg_vertex //: public dbgrl_vertex
{
protected:
  int id_;
  vcl_vector<dbgrl_hg_edge*>      connecting_edges_;
  vcl_vector<dbgrl_hg_hyperedge*> connecting_hyperedges_;

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

  dbgrl_hg_hyperedge* connecting_hyperedges (unsigned int i) {
    assert (i<connecting_hyperedges_.size());
    return connecting_hyperedges_[i];
  }
  vcl_vector<dbgrl_hg_hyperedge*>& connecting_hyperedges() {
    return connecting_hyperedges_;
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

  void connect_hyperedge (dbgrl_hg_hyperedge* hyperedge) {
    connecting_hyperedges_.push_back (hyperedge);
  }
  bool disconnect_hyperedge (dbgrl_hg_hyperedge* hyperedge) {
    vcl_vector<dbgrl_hg_hyperedge*>::iterator it = connecting_hyperedges_.begin();
    for (; it != connecting_hyperedges_.end(); it++) {
      dbgrl_hg_hyperedge* he = (*it);
      if (he == hyperedge) { //found it
        connecting_hyperedges_.erase (it);
        return true;
      }
    }
    assert (0);
    return false;
  }

  //: ====== Constructor/Destructor and other functions ======
  dbgrl_hg_vertex () //: dbgrl_vertex () 
  {
  }

  ~dbgrl_hg_vertex ()
  {
  }
};

#endif
