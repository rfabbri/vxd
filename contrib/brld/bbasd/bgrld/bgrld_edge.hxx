// This is bbasd/bgrld/bgrld_edge.hxx
#ifndef bgrld_edge_txx_
#define bgrld_edge_txx_

//:
// \file

#include <bgrld/bgrld_edge.h>

//: Returns target(edge) if v = source(edge) and source(edge) otherwise. 
template <class V>
typename bgrld_edge<V>::V_sptr bgrld_edge<V>::opposite(V_sptr v)
{
  if (v==source_)
    return target_;
  else
    return source_;
}

//: Determine if ``this'' edge shares a vertex with other edge
// Return the shared node if so, otherwise return 0
template <class V>
typename bgrld_edge<V>::V_sptr bgrld_edge<V>::
shared_vertex(const bgrld_edge<V >& other) const
{
  if (this->source() == other.source() || this->source() == other.target())
    return this->source();
  if (this->target() == other.source() || this->target() == other.target())
    return this->target();
  return 0;
}


//: Print an ascii summary to the stream
template <class V>
void bgrld_edge<V>::print_summary( vcl_ostream& os ) const
{
  os << vcl_endl;
}

#define BGRLD_EDGE_INSTANTIATE(E) \
template class bgrld_edge<E>

#endif // bgrld_edge_txx_
