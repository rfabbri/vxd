#include "bmcsd_discrete_corresp_algo.h"
#include "bmcsd_discrete_corresp_algo.hxx"

#include <bmcsd/bmcsd_util.h>
#include <bmcsd/algo/bmcsd_curve_stereo.h>
#include <vsol/vsol_polyline_2d_sptr.h>

//: ***beta version***
void bmcsd_discrete_corresp_algo::
print_difference(const bmcsd_discrete_corresp_n &c1, const bmcsd_discrete_corresp_n &c2)
{

  vcl_cout << "------ Comparing ------\n";
  if (c1.n() != c2.n()) {
    vcl_cout << "Different dimensions.\n";
    return;
  }
  
  vbl_sparse_array_base<bmcsd_match_attribute,bmcsd_ntuplet>::const_iterator r;
  for (r = c1.l_.begin(); r != c1.l_.end(); ++r){
    if (!c2.l_.fullp(r->first)) {
      vcl_cout << "C1 contains: " << (r->first)<< " but C2 doesn't\n";
    }
  }

  for (r = c2.l_.begin(); r != c2.l_.end(); ++r){
    if (!c1.l_.fullp(r->first)) {
      vcl_cout << "C2 contains: " << r->first << " but C1 doesn't\n";
    }
  }
}

void bmcsd_discrete_corresp_algo::
compose(const bmcsd_discrete_corresp &a, const bmcsd_discrete_corresp &b, 
    bmcsd_discrete_corresp *ab_ptr)
{
  bmcsd_discrete_corresp &ab = *ab_ptr;
  ab.set_size(b.n_objects_view_0(), a.n_objects_view_1());

  assert(b.n_objects_view_1() == a.n_objects_view_0());

  for (unsigned i=0; i < b.n0(); ++i) {
    // For each (i,k) given in b[i], traverse a[k] and insert.
    for (bmcsd_discrete_corresp::one_corresp_list_const_iter bitr = b[i].begin();
        bitr != b[i].end(); ++bitr) {

      unsigned k = bitr->id();
      ab[i].insert(ab[i].end(), a[k].begin(), a[k].end());
    }
  }
}

unsigned long 
bmcsd_discrete_corresp_algo::
compute_checksum(const bmcsd_curve_stereo &s)
{
  unsigned long cksum=0;
  double len=0;
  for (unsigned i=0; i < s.num_curves(s.v0()); ++i) {
    cksum += static_cast<unsigned long>(s.curves(s.v0(), i)->size());
    len += s.curves(s.v0(), i)->length();
  }
  for (unsigned i=0; i < s.num_curves(s.v1()); ++i) {
    cksum += static_cast<unsigned long>(s.curves(s.v1(), i)->size());
    len += s.curves(s.v1(), i)->length();
  }

  cksum += 100001*s.num_curves(s.v0());
  cksum += 130217*s.num_curves(s.v1());
  cksum += 37*static_cast<unsigned long>(len);
  return cksum;
}

unsigned long 
bmcsd_discrete_corresp_algo::
compute_checksum(
    const vcl_vector<vsol_polyline_2d_sptr> &pts0, 
    const vcl_vector<vsol_polyline_2d_sptr> &pts1)
{
  unsigned long cksum=0;
  double len=0;

  for (unsigned i=0; i < pts0.size(); ++i) {
    cksum += static_cast<unsigned long>(pts0[i]->size());
    len += pts0[i]->length();
  }

  for (unsigned i=0; i < pts1.size(); ++i) {
    cksum += static_cast<unsigned long>(pts1[i]->size());
    len += pts1[i]->length();
  }

  cksum += 100001*pts0.size();
  cksum += 130217*pts1.size();
  cksum += 37*static_cast<unsigned long>(len);
  return cksum;
}

void 
bmcsd_discrete_corresp_algo::
exp_stats(const bmcsd_discrete_corresp *cp, dborl_exp_stat &s, const bmcsd_discrete_corresp *gt_ptr) const
{
  assert(c->size() == gt_ptr->size());

  const bmcsd_discrete_corresp &gt = *gt_ptr;
  const bmcsd_discrete_corresp &c = *c;

  s.positive_cnt_ = gt.num_corresps();
  s.negative_cnt_ = gt.n0()*gt.n1() - s.positive_cnt_;

  for (unsigned i=0; i < size() + 1; ++i) {
    for (one_corresp_list_const_iter itr = c[i].begin(); itr != c[i].end(); ++itr) {
      if (!vnl_math_isfinite(itr->cost_))  // infinite cost equals no correspondence.
        continue;
      one_corresp_list_const_iter
        result = find_if(gt[i].begin(), gt[i].end(), bmcsd_attributed_object_eq(itr->obj_)); 
      if (result == gt[i].end() || !vnl_math_isfinite(itr->cost_))
        s.increment_FP();
      else
        s.increment_TP();
    }
  }
  s.increment_FN_by(s.positive_cnt_ - s.TP_);
  s.increment_TN_by(s.negative_cnt_ - s.FP_);
}

/*
void 
bmcsd_discrete_corresp_algo::
exp_stats_hitmiss(const bmcsd_discrete_corresp *c, dborl_exp_stat &s, const bmcsd_discrete_corresp *gt_ptr) const
{
  assert(this->size() == gt_ptr->size());

  const bmcsd_discrete_corresp &gt = *gt_ptr;
  const bmcsd_discrete_corresp &c = *this;

  s.negative_cnt_ = gt.count_empty();
  s.positive_cnt_ = gt.n0() - s.negative_cnt_;

  for (unsigned i=0; i < n0(); ++i) {
    if (c[i].empty()) {
      if (gt[i].empty())
        s.increment_TN();
      else
        s.increment_FN();
    } else {
      bool c_contained_in_gt = true;
      if (c[i].size() > gt[i].size())
        c_contained_in_gt = false;

      for (one_corresp_list_const_iter itr = c[i].begin(); 
          itr != c[i].end() && c_contained_in_gt; 
          ++itr) {
        one_corresp_list_const_iter
          result = find_if(gt[i].begin(), gt[i].end(), bmcsd_attributed_object_eq(itr->obj_)); 
        if (result == gt[i].end())
          c_contained_in_gt = false;
      }

      if (c_contained_in_gt)
        s.increment_TP();
      else
        s.increment_FP();
    }
  }
}
*/

void 
bmcsd_discrete_corresp_algo::
exp_stats_hitmiss(const bmcsd_discrete_corresp *c, dborl_exp_stat &s, const bmcsd_discrete_corresp *gt_ptr) const
{
#ifndef NDEBUG
  vcl_cout << "This corresp size: " << this->size() << " gt size: " << gt_ptr->size() << vcl_endl;
  assert(this->size() == gt_ptr->size());
#endif

  const bmcsd_discrete_corresp &gt = *gt_ptr;
  const bmcsd_discrete_corresp &c = *c;

  s.positive_cnt_ = gt.num_corresps();

  assert (gt.n0()*gt.n1() >= static_cast<unsigned>(s.positive_cnt_));
  s.negative_cnt_ = static_cast<int>(gt.n0()*gt.n1()) - static_cast<int>(s.positive_cnt_);

  for (unsigned i=0; i < size() + 1; ++i) {
    bool c_contained_in_gt = true;
    bool c_empty = true;
    for (one_corresp_list_const_iter itr = c[i].begin(); 
        itr != c[i].end() && c_contained_in_gt; 
        ++itr) {
      if (!vnl_math_isfinite(itr->cost_))  // infinite cost equals no correspondence.
        continue;
      c_empty = false;
      one_corresp_list_const_iter
        result = find_if(gt[i].begin(), gt[i].end(), bmcsd_attributed_object_eq(itr->obj_)); 
      if (result == gt[i].end() || !vnl_math_isfinite(itr->cost_)) {
        c_contained_in_gt = false;
        break;
      }
    }

    if (c_empty)
      continue;
    if (c_contained_in_gt) {
      for (one_corresp_list_const_iter itr = gt[i].begin(); 
          itr != gt[i].end(); ++itr) {
        if (!vnl_math_isfinite(itr->cost_))  // infinite cost equals no correspondence.
          continue;
        s.increment_TP();
      }
    } else {
      for (one_corresp_list_const_iter itr = c[i].begin(); 
          itr != c[i].end(); ++itr) {
        if (!vnl_math_isfinite(itr->cost_))  // infinite cost equals no correspondence.
          continue;
        s.increment_FP();
      }
    }
  }
  s.increment_FN_by(s.positive_cnt_ - s.TP_);
  s.increment_TN_by(s.negative_cnt_ - s.FP_);
}

BMCSD_DISCRETE_CORRESP_ALGO_INSTANTIATE(vsol_polyline_2d_sptr);
