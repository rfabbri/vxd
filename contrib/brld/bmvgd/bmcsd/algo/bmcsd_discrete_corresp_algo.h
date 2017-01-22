// This is mw_point_matcher.h
#ifndef bmcsd_discrete_corresp_algo_h
#define bmcsd_discrete_corresp_algo_h
//:
//\file
//\brief Code to deal with discrete corresp. structures.
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date Mon Nov 6  2006
//
#include <vcl_algorithm.h>
#include <bmcsd/bmcsd_discrete_corresp.h>
#include <bmcsd/bmcsd_discrete_corresp_n.h>
#include <bmcsd/bmcsd_util.h>



class bmcsd_curve_stereo;
class buld_exp_stat;

class bmcsd_discrete_corresp_algo {
  public:

  static void 
  print_difference(const bmcsd_discrete_corresp_n &c1, const bmcsd_discrete_corresp_n &c2);

  //: Compose two correspondences:
  //
  // \p a maps a0 to a1
  // \p b maps b0 to a0
  // \p ab maps b0 to a1
  static void compose(const bmcsd_discrete_corresp &a, const bmcsd_discrete_corresp &b, 
      bmcsd_discrete_corresp *ab_ptr);

  //: Forms corresp indicating which element from the vcl_vector \p from is in
  // to.
  template <typename Tptr> static void 
  match_equal(const vcl_vector<Tptr> &from, const vcl_vector<Tptr> &to,
      bmcsd_discrete_corresp *c_ptr)
  {
    bmcsd_discrete_corresp &c = *c_ptr;

    c.set_size(from.size(), to.size());
    for (unsigned i=0; i < from.size(); ++i) {
      bool found = false;
      unsigned k=0;
      for (; k < to.size(); ++k) {
        if (*to[k] == *from[i]) {
          found = true;
          break;
        }
      }
      if (found)
        c[i].push_back(bmcsd_attributed_object(k));
    }
  }

  //: Forms corresp indicating which element from the vcl_vector \p from is a
  // subcurve of some curve in \p to.
  template <typename Tptr> static void 
  match_subcurves(const vcl_vector<Tptr> &from, const vcl_vector<Tptr> &to,
      bmcsd_discrete_corresp *c_ptr);

  //: Given:
  //
  //  acorr between a0 and a1
  //
  //  and given b0 having some curves in common with a0
  //  and given b1 having some curves in common with a1
  //
  //  extend acorr to a correspondence bcorr between b0 and b1,
  //  defined by acorr restricted to the common elements.
  template <typename Tptr> static void 
  extend(
      const vcl_vector<Tptr> &a0, const vcl_vector<Tptr> &a1,
      const vcl_vector<Tptr> &b0, const vcl_vector<Tptr> &b1,
      const bmcsd_discrete_corresp &acorr,
      bmcsd_discrete_corresp *bcorr_ptr)
  {
    bmcsd_discrete_corresp &bcorr = *bcorr_ptr;

    bmcsd_discrete_corresp equal_ba0;
    match_equal(b0, a0, &equal_ba0);

    bmcsd_discrete_corresp equal_ab1;
    match_equal(a1, b1, &equal_ab1);

    bmcsd_discrete_corresp a_comp_equal_ba0;
    compose(acorr, equal_ba0, &a_comp_equal_ba0);
    compose(equal_ab1, a_comp_equal_ba0, &bcorr);
    bcorr.set_checksum(compute_checksum(b0, b1));
  }

  //: Given:
  //
  //  acorr between a0 and a1
  //
  //  and given b0 having some curves as subcurves of some curves in a0
  //  and given b1 having some curves as subcurves of some curves in a1
  //
  //  extend acorr to a correspondence bcorr between b0 and b1,
  //  defined by acorr restricted to the common elements.
  template <typename Tptr> static void 
  extend_to_subcurves(
      const vcl_vector<Tptr> &a0, const vcl_vector<Tptr> &a1,
      const vcl_vector<Tptr> &b0, const vcl_vector<Tptr> &b1,
      const bmcsd_discrete_corresp &acorr,
      bmcsd_discrete_corresp *bcorr_ptr)
  {
    bmcsd_discrete_corresp &bcorr = *bcorr_ptr;

    bmcsd_discrete_corresp equal_ba0;
    match_subcurves(b0, a0, &equal_ba0);

    bmcsd_discrete_corresp equal_ab1;
    match_subcurves(a1, b1, &equal_ab1);

    bmcsd_discrete_corresp a_comp_equal_ba0;
    compose(acorr, equal_ba0, &a_comp_equal_ba0);
    compose(equal_ab1, a_comp_equal_ba0, &bcorr);
    bcorr.set_checksum(compute_checksum(b0, b1));
  }

  static unsigned long compute_checksum(const bmcsd_curve_stereo &s);
  static unsigned long compute_checksum(
      const vcl_vector<vsol_polyline_2d_sptr> &pts0, 
      const vcl_vector<vsol_polyline_2d_sptr> &pts1);

  //: computes ROC statistics (TP, FP, TN, FN, etc) on the space of
  // correspondences being (i,k) with i=0,...,n0()-1 and k=0,...,n1()-1.
  // Let c be (*this). Then, given a correspondence (i,k) in this space, then:
  //
  // if (i,k) in c[i] and in gt[i], it is a true positive.
  // if (i,k) in c[i] and _not_ in gt[i], it is a false positive.
  // if (i,k) not in c[i] and not in gt[i], it is a true negative.
  // if (i,k) not in c[i] and in gt[i], it is a false negative.
  //
  // \remarks we treat infinite correspondence as non-existent.
  static void exp_stats(const bmcsd_discrete_corresp *c, buld_exp_stat &s, const bmcsd_discrete_corresp *gt);


  //: This is a variant of exp_stats where, if c[i] contained in gt[i], then it
  // is as if all remaining gt[i] are also in c[i]. That is, c[i] needs to hit
  // a subset of gt[i]. 
  //
  // This is useful when gt[i] provide equally good alternatives for
  // correspondences (i,k), so that we're happy if c[i] hits the ground 
  // truth gt[i], but we don't penalize if not all gt[i] have been hit.
  //
  // The problem is that we will be giving more weight for the correspondences
  // having multiple alternatives. If \p c misses that, it is penalized multiple
  // times.
  //
  // \remarks We treat infinite cost correspondences as non-existent
  static void exp_stats_hitmiss(const bmcsd_discrete_corresp *c, buld_exp_stat &s, const bmcsd_discrete_corresp *gt);
};

#define BMCSD_DISCRETE_CORRESP_ALGO_INSTANTIATE(T) extern "please include bmcsd_discrete_corresp_algo.hxx first"

#endif // bmcsd_discrete_corresp_algo_h
