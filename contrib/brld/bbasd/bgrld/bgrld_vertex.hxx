// This is bbasd/bgrld/bgrld_vertex.hxx
#ifndef bgrld_vertex_txx_
#define bgrld_vertex_txx_

//:
// \file

#include <bgrld/bgrld_vertex.h>

//: delete an edge incident on this node
template <class E>
bool bgrld_vertex<E>::del_edge(E_sptr e)
{
  for ( edge_iterator itr = in_edges_.begin();
        itr != in_edges_.end(); ++itr )
  {
    if ((*itr) == e){
      //edge found
      in_edges_.erase(itr);
      return true; 
    }
  }

  for ( edge_iterator itr = out_edges_.begin();
        itr != out_edges_.end(); ++itr )
  {
    if ((*itr) == e){
      //edge found
      out_edges_.erase(itr);
      return true; 
    }
  }

  //edge not found 
  return false;
}

//: delete an edge from the incoming edge list
template <class E>
bool bgrld_vertex<E>::del_in_edge(E_sptr e)
{
  for ( edge_iterator itr = in_edges_.begin();
        itr != in_edges_.end(); ++itr )
  {
    if ((*itr) == e){
      //edge found
      in_edges_.erase(itr);
      return true; 
    }
  }
  //edge not found 
  return false;
}

//: delete an edge from the outgoing edge list
template <class E>
bool bgrld_vertex<E>::del_out_edge(E_sptr e)
{
  for ( edge_iterator itr = out_edges_.begin();
        itr != out_edges_.end(); ++itr )
  {
    if ((*itr) == e){
      //edge found
      out_edges_.erase(itr);
      return true; 
    }
  }
  //edge not found 
  return false;
}

//: delete all edges
template <class E>
void bgrld_vertex<E>::del_all_edges()
{
  del_all_in_edges();
  del_all_out_edges();
}

//: delete all the incoming edges
template <class E>
void bgrld_vertex<E>::del_all_in_edges()
{
  //remove connections from all the in_edges to the node
  for ( edge_iterator itr = in_edges_.begin();
        itr != in_edges_.end(); ++itr )    
    (*itr)->set_source(0);

  in_edges_.clear();
}

//: delete all the outgoing edges
template <class E>
void bgrld_vertex<E>::del_all_out_edges()
{
  //remove connections from all the out_edges to the node
  for ( edge_iterator itr = out_edges_.begin();
        itr != out_edges_.end(); ++itr )    
    (*itr)->set_source(0);

  out_edges_.clear();
}

//: Print an ascii summary to the stream
template <class E>
void bgrld_vertex<E>::print_summary( vcl_ostream& os ) const
{
  os << "[" << this->degree() << "]" << vcl_endl;
}

#define BGRLD_VERTEX_INSTANTIATE(E) \
template class bgrld_vertex<E>

#endif // bgrld_vertex_txx_

