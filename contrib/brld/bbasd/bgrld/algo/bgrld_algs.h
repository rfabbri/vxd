// This is basic/dbgrl/algo/dbgrl_algs.h
#ifndef dbgrl_algs_h_
#define dbgrl_algs_h_
//:
// \file
// \brief Basic graph algorithms
// \author Ozge C. Ozcanli
//
// \verbatim
//  Modifications
//   O.C.Ozcanli  02/25/2007   added euler tour algorithm
//
// \endverbatim 

#include <vcl_vector.h>

#include "../dbgrl_graph.h"

//: return the euler tour of the graph starting from the given edge and in the direction towards the given node
//  (the given node should be either source or target of the given node)
//  ALG:
//  starting from the "next edge" of the given node wrt the given edge, add the next edges of successor nodes as a chain 
//  stop when the initial edge is re-encountered
template <class G, class E, class V>
void euler_tour(vbl_smart_ptr<G> g, vbl_smart_ptr<E> e, vbl_smart_ptr<V> n, vcl_vector<vbl_smart_ptr<E> >& edges); 

//: return the depth of the graph starting from the given node (!!!assumes there are no loops)
template <class G, class E, class V>
int depth_no_loop(vbl_smart_ptr<G> g, vbl_smart_ptr<E> e, vbl_smart_ptr<V> n); 


#endif // dbgrl_algs_h_
