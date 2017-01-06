// This is basic/dbgrl/dbgrl_edge.hxx
#ifndef dbgrl_edge_txx_
#define dbgrl_edge_txx_

//:
// \file

#include <dbgrl/dbgrl_edge.h>

//: Returns target(edge) if v = source(edge) and source(edge) otherwise. 
template <class V>
typename dbgrl_edge<V>::V_sptr dbgrl_edge<V>::opposite(V_sptr v)
{
  if (v==source_)
    return target_;
  else
    return source_;
}

//: Determine if ``this'' edge shares a vertex with other edge
// Return the shared node if so, otherwise return 0
template <class V>
typename dbgrl_edge<V>::V_sptr dbgrl_edge<V>::
shared_vertex(const dbgrl_edge<V >& other) const
{
  if (this->source() == other.source() || this->source() == other.target())
    return this->source();
  if (this->target() == other.source() || this->target() == other.target())
    return this->target();
  return 0;
}


//: Print an ascii summary to the stream
template <class V>
void dbgrl_edge<V>::print_summary( vcl_ostream& os ) const
{
  os << vcl_endl;
}

#define DBGRL_EDGE_INSTANTIATE(E) \
template class dbgrl_edge<E>

#endif // dbgrl_edge_txx_
