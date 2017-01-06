#include "mw_discrete_corresp_algo.h"

// Most non-template implementations are in mw_discrete_corresp_algo_defs.cxx


template <typename Tptr> void mw_discrete_corresp_algo::
match_subcurves(const vcl_vector<Tptr> &from, const vcl_vector<Tptr> &to,
    mw_discrete_corresp *c_ptr)
{
  mw_discrete_corresp &c = *c_ptr;

  c.set_size(from.size(), to.size());
  for (unsigned i=0; i < from.size(); ++i) {
    vcl_vector<vsol_point_2d_sptr> from_i_pts;
    mw_util::get_vsol_point_vector(*from[i], &from_i_pts);
    bool from_contains_to = false;
    unsigned k=0;
    for (; k < to.size(); ++k) {
      vcl_vector<vsol_point_2d_sptr> to_k_pts;
      mw_util::get_vsol_point_vector(*to[k], &to_k_pts);
      if (vcl_search(from_i_pts.begin(), from_i_pts.end(), 
                     to_k_pts.begin(), to_k_pts.end(), 
                     mw_dereference_equal()) != from_i_pts.end()) {
        c[i].push_back(mw_attributed_object(k));
        from_contains_to = true;
      } else if (
          !from_contains_to && 
          vcl_search(to_k_pts.begin(), to_k_pts.end(), 
                     from_i_pts.begin(), from_i_pts.end(), 
                     mw_dereference_equal()) != to_k_pts.end()) {
        c[i].push_back(mw_attributed_object(k));
        // curve from[i] contained in curve to[k]. No more correspondences to
        // search that are affected by from[i].
        break;
      }
    }
  }
}

#undef MW_DISCRETE_CORRESP_ALGO_INSTANTIATE
#define MW_DISCRETE_CORRESP_ALGO_INSTANTIATE(T) \
template void mw_discrete_corresp_algo::match_subcurves(\
    const vcl_vector<T> &from, const vcl_vector<T> &to, mw_discrete_corresp *c_ptr);

