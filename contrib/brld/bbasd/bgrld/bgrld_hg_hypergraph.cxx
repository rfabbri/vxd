// This is lemsvxlsrc/brcv/shp/dbhg/bgrld_hg_hypergraph.cxx
//:
// \file
// \author Based on original code by Ming-Ching Chang
// date:   Apr 04, 2005

#include <bgrld/bgrld_hg_hypergraph.h>


////////////////////////////////////////////////////////////////////
//: ===== Internal Low-level Graph operation (without handling connectivity) =====
bool bgrld_hg_hypergraph::_internal_del_vertex (bgrld_hg_vertex* vertex)
{
  assert (vertex->connecting_hyperedges().size() == 0);
  return bgrld_hg_graph::_internal_del_vertex (vertex);
}

bool bgrld_hg_hypergraph::_internal_del_edge (bgrld_hg_edge* edge)
{
  assert (edge->connecting_hyperedges().size() == 0);
  return bgrld_hg_graph::_internal_del_edge (edge);
}

void bgrld_hg_hypergraph::_internal_add_hyperedge (bgrld_hg_hyperedge* hyperedge) 
{
  ///assert (hyperedge->id() > 0);
  ///hyperedges_.insert (vcl_pair<int, bgrld_hg_hyperedge*> (hyperedge->id(), hyperedge));
}

bool bgrld_hg_hypergraph::_internal_del_hyperedge (bgrld_hg_hyperedge* hyperedge) 
{
  assert (hyperedge->connecting_edges().size() == 0);
  assert (hyperedge->connecting_vertices().size() == 0);

  ///int n_removed = hyperedges_.erase (hyperedge->id());
  ///return (n_removed == 1);
  return true;
}

////////////////////////////////////////////////////////////////////
//: ===== High-level Graph operation (handling connectivity) =====
bool bgrld_hg_hypergraph::remove_vertex (bgrld_hg_vertex* vertex)
{
  vcl_vector<bgrld_hg_edge*>::iterator E_it = vertex->connecting_edges().begin();
  while (E_it != vertex->connecting_edges().end()) {
    bgrld_hg_edge* edge = (*E_it);
    _internal_disconnect_edge_vertex (edge, vertex);
  }

  vcl_vector<bgrld_hg_hyperedge*>::iterator H_it = vertex->connecting_hyperedges().begin();
  while (H_it != vertex->connecting_hyperedges().end()) {
    bgrld_hg_hyperedge* hyperedge = (*H_it);
    _internal_disconnect_hyperedge_vertex (hyperedge, vertex);
  }

  return _internal_del_vertex (vertex);
}

/*bool bgrld_hg_hypergraph::remove_vertex (int id)
{
  bgrld_hg_vertex* vertex = vertices (id);
  if (vertex)
    return remove_vertex (vertex);
  else
    return false;
}*/

bool bgrld_hg_hypergraph::remove_edge (bgrld_hg_edge* edge)
{
  bgrld_hg_vertex* sv = edge->connecting_s_vertex();
  bgrld_hg_vertex* ev = edge->connecting_e_vertex();

  if (sv)
    _internal_disconnect_edge_vertex (edge, sv);
  if (ev)
    _internal_disconnect_edge_vertex (edge, ev);

  for (unsigned int i=0; i<edge->connecting_hyperedges().size(); i++) {
    bgrld_hg_hyperedge* hyperedge = edge->connecting_hyperedges(i);
    _internal_disconnect_hyperedge_edge (hyperedge, edge);
  }

  return _internal_del_edge (edge);
}

/*bool bgrld_hg_hypergraph::remove_edge (int id)
{
  bgrld_hg_edge* edge = edges (id);
  if (edge)
    return remove_edge (edge);
  else
    return false;
}*/

bool bgrld_hg_hypergraph::remove_hyperedge (bgrld_hg_hyperedge* hyperedge)
{
  vcl_vector<bgrld_hg_edge*>::iterator E_it = hyperedge->connecting_edges().begin();
  while (E_it != hyperedge->connecting_edges().end()) {
    bgrld_hg_edge* edge = (*E_it);
    _internal_disconnect_hyperedge_edge (hyperedge, edge);
  }

  vcl_vector<bgrld_hg_vertex*>::iterator V_it = hyperedge->connecting_vertices().begin();
  while (V_it != hyperedge->connecting_vertices().end()) {
    bgrld_hg_vertex* vertex = (*V_it);
    _internal_disconnect_hyperedge_vertex (hyperedge, vertex);
  }

  return _internal_del_hyperedge (hyperedge);
}

/*bool bgrld_hg_hypergraph::remove_hyperedge (int id)
{
  bgrld_hg_hyperedge* hyperedge = hyperedges (id);
  if (hyperedge)
    return remove_hyperedge (hyperedge);
  else
    return false;
}*/


////////////////////////////////////////////////////////////////////
//: ===== High-level Graph operation that the user should use =====
bool bgrld_hg_hypergraph::topo_remove_vertex (bgrld_hg_vertex* vertex)
{
  // ! Should check both edges and hyperedges!!
  if (vertex->connecting_edges().size() == 0) {
    assert (vertex->connecting_hyperedges().size() == 0);
    return remove_vertex (vertex);
  }
  else
    return false;
}

/*bool bgrld_hg_hypergraph::topo_remove_vertex (int id)
{
  bgrld_hg_vertex* vertex = vertices (id);
  if (vertex)
    return topo_remove_vertex (vertex);
  else
    return false;
}*/

bool bgrld_hg_hypergraph::topo_remove_edge (bgrld_hg_edge* edge)
{
  if (edge->connecting_hyperedges().size() == 0)
    return remove_edge (edge);
  else
    return false;
}

/*bool bgrld_hg_hypergraph::topo_remove_edge (int id)
{
  bgrld_hg_edge* edge = edges (id);
  if (edge)
    return topo_remove_edge (edge);
  else
    return false;
}*/

bool bgrld_hg_hypergraph::topo_remove_hyperedge (bgrld_hg_hyperedge* hyperedge)
{
  //: for bgrld_hg_hypergraph, can remove the hyperedge directly.
  return remove_hyperedge (hyperedge);
}

/*bool bgrld_hg_hypergraph::topo_remove_hyperedge (int id)
{
  //: for bgrld_hg_hypergraph, can remove the hyperedge directly.
  return remove_hyperedge (id);
}*/
