#include "mw_dt_curve_stereo.h"
#include <dbspi/dbspi_curve_distance.h>


mw_dt_curve_stereo::
mw_dt_curve_stereo()
  :
    tau_distance_squared_(10.0*10.0)
{
}

bool mw_dt_curve_stereo::
set_nviews(unsigned nv)
{
  if (!mw_curve_stereo::set_nviews(nv))
    return false;
  dt_.reserve(nviews());
  label_.reserve(nviews());
  return true;
}

void mw_dt_curve_stereo::
set_all_dt_label(
    const vcl_vector<vil_image_view<vxl_uint_32> > &dt,  
    const vcl_vector<vil_image_view<unsigned> > &label)
{
  assert (dt.size() == nviews());
  assert (label.size() == nviews());
  dt_ = dt;
  label_ = label;
}

bool mw_dt_curve_stereo::
match_using_dt(unsigned *i_best) 
{
  vcl_vector<unsigned long> votes;
  return match_using_dt(i_best, &votes);
}

bool mw_dt_curve_stereo::
match_using_dt(unsigned *i_best, vcl_vector<unsigned long> *votes_ptr)
{
  // Selected curve in img_[0] is  selected_crv_[0]; the selected segment is subcurve_
  // The candidate curves in img[1] are crv_candidates_ptrs() and are e.g.
  // the curves that intercept at least one epipolar of subcurve_.

#ifndef NDEBUG
  if (!ready_for_matching())
    return false;
#endif

  vcl_vector<unsigned long> &votes = *votes_ptr;
  votes.resize(num_candidates(), 0);

  unsigned ini_id, 
           end_id;

  get_increasing_endpoints(&ini_id, &end_id);

  vsol_polyline_2d_sptr original_selected_crv = selected_crv(1); 

  // For each candidate curve
  for (unsigned ic=0; ic < num_candidates(); ++ic) {
    unsigned ini_id_sub, end_id_sub;
    get_matching_subcurve(ic, ini_id, end_id, &ini_id_sub, &end_id_sub);

    set_selected_crv(1, crv_candidates_ptrs(ic));

    vcl_vector<mw_vector_3d> curve_3d;
    reconstruct_subcurve(ini_id_sub, end_id_sub, &curve_3d);

    vcl_cout << "Accumulating votes for curve[" << ic << "]" << vcl_endl;
    vcl_cout << "\t\tstart ===========" << vcl_endl;
    for (unsigned v=0; v < nviews(); ++v) {
      if (v == 0 || v == 1)
        continue;

      // Compute reprojected_curve into view v

      vcl_vector<vsol_point_2d_sptr> reprojected_curve; 

      project_curve(v, curve_3d, &reprojected_curve);

      mw_util::clip_to_img_bounds(dt_[v], &reprojected_curve);

      // Compute match cost

      unsigned d_vote = dbspi_curve_distance::num_inliers_dt(
          reprojected_curve, tau_distance_squared_, dt_[v], label_[v]);

      votes[ic] += d_vote;

      vcl_cout << "\t\tinliers on view[" << v << "] = " << d_vote << vcl_endl;
    }
    vcl_cout << "\t\tcurve[" << ic << "] has " << votes[ic] << " total inliers\n";
    vcl_cout << "\t\tend ===========" << vcl_endl;
  }
  set_selected_crv(1, original_selected_crv);

  mw_util::max(votes, *i_best);

  vcl_cout << "Best curve has index " << *i_best << 
    " among candidates, with #votes = " << votes[*i_best] << vcl_endl;
  vcl_cout << "Finished curve matching using distance reprojection error" << vcl_endl;

  return true;
}

bool mw_dt_curve_stereo::
ready_for_matching()
{
  if (!subcurve()) {
    vcl_cerr << "No selected curve in frame 0\n";
    return false;
  }
  if (!has_dt_label()) {
    vcl_cerr << "No dt or labels were set.\n";
    return false;
  }

  return true;
}
