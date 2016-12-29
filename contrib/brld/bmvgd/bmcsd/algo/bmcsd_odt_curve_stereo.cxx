#include "mw_odt_curve_stereo.h"
#include <dbdet/edge/dbdet_edgemap.h>
#include <dbdet/pro/dbdet_sel_storage.h>
#include <dbdif/dbdif_rig.h>

#include <dbecl/dbecl_epipole.h>
#include <dbecl/dbecl_episeg.h>
#include <dbecl/dbecl_episeg_sptr.h>
#include <dbecl/dbecl_episeg_from_curve_converter.h>

#include <mw/mw_epi_interceptor.h>
#include <mw/mw_discrete_corresp.h>
#include <mw/algo/mw_algo_util.h>
#include <mw/algo/mw_discrete_corresp_algo.h>

mw_odt_curve_stereo::
mw_odt_curve_stereo()
  :
    tau_dtheta_(vnl_math::pi/18.0),
    tau_min_inliers_per_view_(20),
    tau_min_total_inliers_(0),
    tau_min_epiangle_(vnl_math::pi/6.0),
    tau_min_first_to_second_best_ratio_(1.5),
    tau_lonely_(tau_min_total_inliers_), //:< be more stringent on lonely corresps
    tau_min_num_inlier_edgels_per_curvelet_(3)
{
}

bool mw_odt_curve_stereo::
set_nviews(unsigned nv)
{
  if (!mw_dt_curve_stereo::set_nviews(nv))
    return false;

  em_.reserve(nviews());
  pt_tgts_.reserve(nviews());
  return true;
}

void mw_odt_curve_stereo::
set_all_edgemaps(const vcl_vector<dbdet_edgemap_sptr> &em)
{
  assert(em.size() == nviews());

  em_ = em;
}

//: set the symbolic edge linker storages for each view.
void mw_odt_curve_stereo::
set_all_sels(const vcl_vector<dbdet_sel_storage_sptr> &sels)
{
  assert(sels.size() == nviews());

  sels_ = sels;
}
  
void mw_odt_curve_stereo::
set_tangents(const vcl_vector<vcl_vector<vcl_vector<double> > > &tangents)
{
  assert (tangents.size() >= 2);
  curve_tangents_ = tangents;

  pt_tgts_.resize(nviews());

  for (unsigned v=0; v < tangents.size(); ++v) {
    assert(tangents[v].size() == num_curves(v));
    pt_tgts_[v].resize(tangents[v].size());

    for (unsigned c=0; c < tangents[v].size(); ++c) {
      assert(tangents[v][c].size() == num_samples(v, c));
      pt_tgts_[v][c].resize(tangents[v][c].size());

      for (unsigned p=0; p < tangents[v][c].size(); ++p) {
        //: FIXME hack of using 3rd order when we don't need it.
        dbdif_3rd_order_point_2d pimg;
        const vgl_point_2d<double> &pt = curves(v,c)->vertex(p)->get_p();

        pimg.gama[0] = pt.x();
        pimg.gama[1] = pt.y();
        pimg.t[0] = vcl_cos(tangents[v][c][p]);
        pimg.t[1] = vcl_sin(tangents[v][c][p]);

        dbdif_3rd_order_point_2d p3d_dummy;
        cams(v).img_to_world(&pimg, &p3d_dummy);
        pt_tgts_[v][c][p] = p3d_dummy; // intentional slicing.
      }
    }
  }
}

//#ifndef NDEBUG
//#define MW_VERBOSE_DEBUG 1
//#endif

bool mw_odt_curve_stereo::
match_using_orientation_dt(unsigned *i_best, vcl_vector<unsigned long> *votes_ptr)
{
#ifndef NDEBUG
  if (!ready_for_oriented_matching())
    return false;
#endif

  if (num_candidates() == 0) {
    votes_ptr->clear();
    return true;
  }

  vcl_vector<unsigned long> &votes = *votes_ptr;
  votes.resize(num_candidates(), 0);

  unsigned ini_id, 
           end_id;

  get_increasing_endpoints(&ini_id, &end_id);

  dbdif_rig rig(cams(v0()).Pr_, cams(v1()).Pr_);

  // For each candidate curve
  reprojection_crv_.resize(num_candidates());
  for (unsigned ic=0; ic < num_candidates(); ++ic) {
    dbdif_1st_order_curve_3d curve_3d;
    reconstruct_candidate_1st_order(ini_id, end_id, ic, rig, &curve_3d);

#ifdef MW_VERBOSE_DEBUG
    vcl_cout << "Votes for curve[" << ic << "] ===========" << vcl_endl;
#endif
    reprojection_crv_[ic].resize(nviews());
    for (unsigned v=0; v < nviews(); ++v) {
      if (v == v0() || v == v1() )
        continue;

      // Compute reprojected_curve into view v
      dbdif_1st_order_curve_2d reprojected_curve;
      project_curve_1st_order(v, curve_3d, &reprojected_curve);
      assert (reprojected_curve.size() == curve_3d.size());

      mw_util::clip_to_img_bounds(dt(v), &reprojected_curve);

      // translate reproj. curve into edgel sequence
      dbcsi_edgel_seq reproj_edgels;
      mw_algo_util::dbdif_to_dbdet(reprojected_curve, &reproj_edgels);

      assert (reproj_edgels.size() == reprojected_curve.size());

      // For debug purposes:
      reprojection_crv_[ic][v] = reproj_edgels;

      // Compute match cost
      unsigned d_vote = dbcsi_curve_distance::num_inliers_dt_oriented(
          reproj_edgels, tau_distance_squared(), tau_dtheta_, dt(v), label(v), *em_[v]);

      votes[ic] += d_vote;

#ifdef MW_VERBOSE_DEBUG
      vcl_cout << "\t\t\tinliers on view[" << v << "] = " << d_vote << vcl_endl;
#endif
    }
#ifdef MW_VERBOSE_DEBUG
    vcl_cout << "\t\tcurve[" << ic << "] has " << votes[ic] << " total inliers\n";
    vcl_cout << "\t\tend ===========" << vcl_endl;
#endif
  }

  assert(!votes.empty());
  mw_util::max(votes, *i_best);

#ifdef MW_VERBOSE_DEBUG
  vcl_cout << "Best curve has index " << *i_best << 
    " among candidates, with #votes = " << votes[*i_best] << vcl_endl;
  vcl_cout << "Finished curve matching using _oriented_ reprojection error" << vcl_endl;
#endif

#ifdef MW_VERBOSE_DEBUG
  if (votes[*i_best] < 2)
    vcl_cerr << "Warning: match is not reliable\n";
#endif

  return true;
}

bool mw_odt_curve_stereo::
match_using_orientation_dt_extras(unsigned *i_best, vcl_vector<unsigned long> *votes_ptr)
{
  if (!match_using_orientation_dt_extras(votes_ptr))
    return false;

  mw_util::max(*votes_ptr, *i_best);

#ifdef MW_VERBOSE_DEBUG
  vcl_cout << "Best curve has index " << *i_best << 
    " among candidates, with #votes = " << (*votes_ptr)[*i_best] << vcl_endl;
  vcl_cout << "Finished curve matching using _oriented_ reprojection error" << vcl_endl;

  if ((*votes_ptr)[*i_best] < 2)
    vcl_cerr << "Warning: match is not reliable\n";
#endif

  return true;
}

bool mw_odt_curve_stereo::
match_using_orientation_dt_extras(vcl_vector<unsigned long> *votes_ptr)
{
#ifndef NDEBUG
  if (!ready_for_oriented_matching())
    return false;
#endif

  if (num_candidates() == 0) {
    votes_ptr->clear();
    return true;
  }

  vcl_vector<unsigned long> &votes = *votes_ptr;
  votes.resize(num_candidates(), 0);

  unsigned ini_id, 
           end_id;

  get_increasing_endpoints(&ini_id, &end_id);

  dbdif_rig rig(cams(v0()).Pr_, cams(v1()).Pr_);

  // For each candidate curve
  reprojection_crv_.resize(num_candidates());
  for (unsigned ic=0; ic < num_candidates(); ++ic) {
    dbdif_1st_order_curve_3d curve_3d;
    reconstruct_candidate_1st_order(ini_id, end_id, ic, rig, &curve_3d);

#ifdef MW_VERBOSE_DEBUG
    vcl_cout << "Votes for curve[" << ic << "] ===========" << vcl_endl;
#endif
    reprojection_crv_[ic].resize(nviews());
    for (unsigned v=0; v < nviews(); ++v) {
      if (v == v0() || v == v1() )
        continue;

      // Compute reprojected_curve into view v
      dbdif_1st_order_curve_2d reprojected_curve;
      project_curve_1st_order(v, curve_3d, &reprojected_curve);
      assert (reprojected_curve.size() == curve_3d.size());

      mw_util::clip_to_img_bounds(dt(v), &reprojected_curve);

      // translate reproj. curve into edgel sequence
      dbcsi_edgel_seq reproj_edgels;
      mw_algo_util::dbdif_to_dbdet(reprojected_curve, &reproj_edgels);

      assert (reproj_edgels.size() == reprojected_curve.size());

      // For debug purposes:
      reprojection_crv_[ic][v] = reproj_edgels;

      // Compute match cost
      unsigned d_vote;
      if (sels_.empty()) {
        d_vote = dbcsi_curve_distance::num_inliers_dt_oriented(
            reproj_edgels, tau_distance_squared(), tau_dtheta_, dt(v), label(v), *em_[v]);
      } else {
        d_vote = dbcsi_curve_distance::num_inlier_curvelets_dt_oriented(
            reproj_edgels, tau_distance_squared(), tau_dtheta_, dt(v), label(v), sels_[v]->CM(),
            tau_min_num_inlier_edgels_per_curvelet_);
      }

      votes[ic] += (d_vote < tau_min_inliers_per_view_)? 0 : d_vote;

#ifdef MW_VERBOSE_DEBUG
      vcl_cout << "\t\tinliers on view[" << v << "] = " << d_vote << vcl_endl;
#endif
    }

    if (votes[ic] < tau_min_total_inliers_)
      votes[ic] = 0;

#ifdef MW_VERBOSE_DEBUG
    vcl_cout << "\t\tcurve[" << ic << "] has " << votes[ic] << " total inliers\n";
    vcl_cout << "\t\tend ===========" << vcl_endl;
#endif
  }

  return true;
}

void mw_odt_curve_stereo::
reconstruct_candidate_1st_order(unsigned ini_id, unsigned end_id, unsigned ic, 
    const dbdif_rig &rig, dbdif_1st_order_curve_3d *crv_ptr)
{
  dbdif_1st_order_curve_3d &curve_3d = *crv_ptr;
  unsigned ini_id_sub, end_id_sub;
  get_matching_subcurve(ic, ini_id, end_id, &ini_id_sub, &end_id_sub);

  set_selected_crv_by_id(v1(), crv_candidates(ic));

  curve_3d.reserve(end_id_sub - ini_id_sub + 1);
  reconstruct_subcurve_1st_order(ini_id_sub, end_id_sub, rig, &curve_3d);

  assert(curve_3d.size() == end_id_sub - ini_id_sub  + 1);
}

void mw_odt_curve_stereo::
reconstruct_subcurve_1st_order(
    unsigned ini_id_sub, 
    unsigned end_id_sub, 
    const dbdif_rig &rig,
    dbdif_1st_order_curve_3d *curve_3d
    ) const
{
  static const unsigned second_view = 1;

  curve_3d->reserve(end_id_sub - ini_id_sub + 1);
  for (unsigned di0=0; di0 + ini_id_sub <= end_id_sub; ++di0) {
    dbdif_1st_order_point_3d pt_3D;
    reconstruct_curve_point_1st_order(second_view, ini_id_sub, di0, rig, &pt_3D);
    curve_3d->push_back(pt_3D);
  }
}

void mw_odt_curve_stereo::
reconstruct_curve_point_1st_order(
    unsigned v,
    unsigned ini_id,
    unsigned di0, 
    const dbdif_rig &rig,
    dbdif_1st_order_point_3d *pt_3D
    ) const
{
  static const unsigned id_v0 = 0;

  dbdif_1st_order_point_2d p0_w
    = pt_tgts_[id_v0][selected_crv_id(id_v0)][ini_id + di0];

  // Corresponding points

  unsigned nearest_sample_id;
  {
  vgl_point_2d<double> pt;
  mw_epi_interceptor::curve_line_intersection_simple(
      *selected_crv(v), ep(v-1)[di0], &pt, &nearest_sample_id);
  }

  dbdif_1st_order_point_2d 
    p1_w = pt_tgts_[v][selected_crv_id(v)][nearest_sample_id];

  if (!p0_w.valid || !p1_w.valid)
    vcl_cerr << "Warning: invalid points!!\n";

  // Reconstructions
  rig.reconstruct_1st_order(p0_w, p1_w, pt_3D);
}

void mw_odt_curve_stereo::
project_curve_1st_order(
    unsigned view, 
    const dbdif_1st_order_curve_3d &crv3d,
    dbdif_1st_order_curve_2d *proj_ptr
    ) const
{
  dbdif_1st_order_curve_2d &proj = *proj_ptr;

  // Reproject into 3rd view
  proj.resize(crv3d.size());
  for (unsigned i=0; i < crv3d.size(); ++i) {
    // - get image coordinates
    // bool not_degenerate = 
    cams(view).project_1st_order_to_image(crv3d[i], &proj[i]);
  }
}

bool mw_odt_curve_stereo::
ready_for_oriented_matching()
{
  if (!ready_for_matching()) {
    vcl_cerr << "Not ready for non-oriented matching.\n";
    return false;
  }

  if (!has_edgemaps()) {
    vcl_cerr << "No edgemaps were set.\n";
    return false;
  }

  if (!has_curve_tangents()) {
    vcl_cerr << "No curve tangents were set.\n";
    return false;
  }
  return true;
}

void mw_odt_curve_stereo::
break_curves_into_episegs_pairwise(
    vcl_vector<vcl_vector< vsol_polyline_2d_sptr > > *broken_vsols,
    vcl_vector<bbld_subsequence_set> *ss_ptr
    ) const
{
  vcl_cout << "Called ODT episeg breaker with tau_min_epiangle = " 
    << tau_min_epiangle_*180.0/vnl_math::pi << " degrees" << vcl_endl;
  assert(has_curve_tangents());
  vcl_vector<bbld_subsequence_set> &ss = *ss_ptr;
  broken_vsols->resize(nviews());
  ss.resize(nviews());
  vgl_homg_point_2d<double> e, e_prime;
  fm_[v0()][v1()].get_epipoles(e, e_prime);

  {
  bbld_subsequence_set s_a;
  vcl_vector<vsol_polyline_2d_sptr> vsols_broken_at_turns;

  break_curves_into_episegs(vsols_[v0()], &vsols_broken_at_turns, e, &s_a);

  vcl_vector<vcl_vector<double> > tangents_a;
  consolidate_subsequences(curve_tangents_[v0()], s_a, &tangents_a);

  break_curves_into_episegs_angle(vsols_broken_at_turns, tangents_a,
      tau_min_epiangle_, &(*broken_vsols)[v0()], e, &ss[v0()]);
  compose_subsequences(s_a, &ss[v0()]);
  }
  
  {
  bbld_subsequence_set s_a;
  vcl_vector<vsol_polyline_2d_sptr> vsols_broken_at_turns;

  break_curves_into_episegs(vsols_[v1()], &vsols_broken_at_turns, e_prime, &s_a);

  vcl_vector<vcl_vector<double> > tangents_a;
  consolidate_subsequences(curve_tangents_[v1()], s_a, &tangents_a);

  break_curves_into_episegs_angle(vsols_broken_at_turns, tangents_a,
      tau_min_epiangle_, &(*broken_vsols)[v1()], e_prime, &ss[v1()]);
  compose_subsequences(s_a, &ss[v1()]);
  }

  for (unsigned v=0; v < nviews(); v++) {
    if (vsols_[v].empty() || v == v0() || v == v1() || 
        v >= curve_tangents_.size() || curve_tangents_[v].empty())
      continue;
    fm_[v0()][v].get_epipoles(e, e_prime);

    bbld_subsequence_set s_a;
    vcl_vector<vsol_polyline_2d_sptr> vsols_broken_at_turns;

    break_curves_into_episegs(vsols_[v], &vsols_broken_at_turns, e_prime, &s_a);
    
    vcl_vector<vcl_vector<double> > tangents_a;
    consolidate_subsequences(curve_tangents_[v], s_a, &tangents_a);

    break_curves_into_episegs_angle(vsols_broken_at_turns, tangents_a,
        tau_min_epiangle_, &(*broken_vsols)[v], e_prime, &ss[v]);
    compose_subsequences(s_a, &ss[v]);
  }
}

void mw_odt_curve_stereo::
break_curves_into_episegs_angle(
  const vcl_vector< vsol_polyline_2d_sptr >  &vsols,
  const vcl_vector<vcl_vector<double> > &tgts,
  double min_epiangle,
  vcl_vector<vsol_polyline_2d_sptr> *vsols2,
  const vgl_homg_point_2d<double> &e,
  bbld_subsequence_set *ss_ptr
  )
{
  bbld_subsequence_set &ss = *ss_ptr;
  // ----------------------------------------------------------------------
  // Break curve

  dbecl_epipole_sptr epipole = new dbecl_epipole(e.x()/e.w(), e.y()/e.w());
  dbecl_episeg_from_curve_converter factory(epipole);

  factory.set_tangent_threshold(min_epiangle);

  // A) For each vsol, do:
  
  vcl_vector<dbecl_episeg_sptr> all_episegs;
  all_episegs.reserve(2*vsols.size());
  ss.reserve(2*vsols.size());
  for (unsigned i=0; i < vsols.size(); ++i) {
    // A1 - convert to digital curve
    vcl_vector<vsol_point_2d_sptr>  samples;
    samples.reserve(vsols[i]->size());
    for (unsigned k=0; k < vsols[i]->size(); ++k)
      samples.push_back(vsols[i]->vertex(k));

    vsol_digital_curve_2d_sptr dc = new vsol_digital_curve_2d(samples);
    // A2 - apply episeg
    bbld_subsequence_set ss_partition;
    vcl_vector<dbecl_episeg_sptr> eps = 
      factory.convert_curve_using_tangents(
          dc,
          tgts[i],
          &ss_partition,
          i);

    assert(ss_partition.num_subsequences() == eps.size());
    assert(!(vsols[i]->size() && eps.empty()));

    dbecl_delta_angle_predicate 
      is_angle_acceptable(dc, tgts[i], epipole, min_epiangle);

    // Keep only the episegs that are transversal to epilines.
    for(unsigned k=0; k < eps.size(); ++k ) {
      if ( ss_partition[k].size() && is_angle_acceptable(ss_partition[k].ini()) ) {
        all_episegs.push_back(eps[k]);
        ss.push_back(ss_partition[k]);
      }
    }
  }

  // B) Extract vsol from episegs
  vsols2->reserve(all_episegs.size());

  for(unsigned k=0; k < all_episegs.size(); ++k) {
    //if ( all_episegs[k]->max_index() 
    //   - all_episegs[k]->min_index()  < threshold_number_of_points)
    //  continue;

    vsols2->push_back(new vsol_polyline_2d);
    // B1 - traverse episegs
    for (unsigned i = ss[k].ini(); i < ss[k].end(); ++i)
      vsols2->back()->add_vertex( all_episegs[k]->curve()->point(i) );
  }
}

void mw_odt_curve_stereo::
break_into_episegs_and_replace_curve(
    vcl_vector<bbld_subsequence_set> *pcurves_ss)
{
  vcl_vector<bbld_subsequence_set> &sseq = *pcurves_ss;

  mw_curve_stereo::break_into_episegs_and_replace_curve(pcurves_ss);

  typedef vcl_vector<vcl_vector<double> > view_tangents;
  vcl_vector< view_tangents > new_tangents(2);

  consolidate_subsequences(curve_tangents_[0], sseq[0], &new_tangents[0]);
  consolidate_subsequences(curve_tangents_[1], sseq[1], &new_tangents[1]);

  set_tangents(new_tangents);
}

bool 
dbmcs_match_all_curves(
  mw_odt_curve_stereo &s, 
  mw_discrete_corresp *corresp_ptr)
{
  mw_discrete_corresp &corresp = *corresp_ptr;
  corresp.set_size(s.num_curves(s.v0()), s.num_curves(s.v1()));
  corresp.set_checksum(mw_discrete_corresp_algo::compute_checksum(s));

  unsigned const ncurves = s.num_curves(s.v0());

  vcl_cout << "Started: stereo matching of " << ncurves 
    << " curves in view[" << s.v0() << "].\n";

  vcl_cout << "Match parameters:" << 
    " tau_dtheta_=" << s.dtheta_threshold() <<
    " tau_distance_squared_=" << s.tau_distance_squared() <<
    " tau_min_total_inliers_=" << s.min_total_inliers() << 
    " tau_min_inliers_per_view_=" << s.min_inliers_per_view() <<
    " use curvelets?" << ((s.has_sels()) ? "yes" : "no") <<
    " tau_min_num_inlier_edgels_per_curvelet_=" << s.min_num_inlier_edgels_per_curvelet() << 
    vcl_endl;

  for (unsigned c=0; c < ncurves; ++c) {
    if (c % 10 == 0)
      vcl_cout << "Matching curve[" << c << "-" << c+10 << "]\n";
    s.set_selected_crv_by_id(s.v0(), c);
    unsigned const ini_id = 0;
    unsigned const end_id = s.selected_crv(s.v0())->size()-1;
    s.set_subcurve(ini_id, end_id);
    s.compute_epipolar_beam_candidates();

    assert(s.ready_for_oriented_matching());

    vcl_vector<unsigned long> votes;
    if (!s.match_using_orientation_dt_extras(&votes)) {
      vcl_cerr << "Error: problem during matching.\n";
      return false;
    }

    assert(votes.size() == s.num_candidates());
    for (unsigned i=0; i < s.num_candidates(); ++i)
      if (votes[i] > 0)
        corresp[c].push_back(mw_attributed_object(s.crv_candidates(i), false, votes[i]));
  }

  vcl_cout << "Ended: stereo matching " << ncurves << " curves in view[" << 
    s.v0() << "], with " << corresp.n0() - corresp.count_empty() << " having a corresp.\n";
  return true;
}

bool 
dbmcs_match_and_reconstruct_all_curves_attr(
    mw_odt_curve_stereo &s, 
    vcl_vector<dbdif_1st_order_curve_3d> *crv3d_ptr,
    mw_discrete_corresp *corresp_ptr,
    vcl_vector< dbmcs_curve_3d_attributes > *attr_ptr
    )
{
  // TODO: this is were we should pass attr so that we can gent the inlier
  // views.
  dbmcs_match_all_curves(s, corresp_ptr);

  corresp_ptr->keep_only_unambiguous_max(
      s.min_first_to_second_best_ratio(), 
      static_cast<double>(s.lonely_threshold()));

  // The following can be simulated by setting tau_first_to_second_best_ratio_ to zero
  // 
  //   corresp_ptr->keep_only_extreme_cost(true);

  return reconstruct_from_corresp_attr(s, *corresp_ptr, crv3d_ptr, attr_ptr);
}

bool 
reconstruct_from_corresp_attr(
    mw_odt_curve_stereo &s, 
    const mw_discrete_corresp &corresp,
    vcl_vector<dbdif_1st_order_curve_3d> *crv3d_ptr,
    vcl_vector< dbmcs_curve_3d_attributes > *attr_ptr
    )
{
  vcl_cout << "Reconstructing curves\n";

  vcl_vector<dbdif_1st_order_curve_3d> &crv3d = *crv3d_ptr;
  vcl_vector< dbmcs_curve_3d_attributes > &attr = *attr_ptr;
  unsigned const ncurves = s.num_curves(s.v0());
  assert (ncurves == corresp.size());
  crv3d.reserve(ncurves);
  attr.reserve(ncurves);

  dbdif_rig rig(s.cams(s.v0()).Pr_, s.cams(s.v1()).Pr_);
  for (unsigned c=0; c < ncurves; ++c) {
    if (corresp[c].empty())
      continue;
    s.set_selected_crv_by_id(s.v0(), c);
    unsigned const ini_id = 0;
    unsigned const end_id = s.selected_crv(s.v0())->size()-1;
    s.set_subcurve(ini_id, end_id);
    s.compute_epipolar_beam_candidates();

    unsigned ic;
    bool found = s.get_index_of_candidate_curve(s.curves(s.v1(), corresp[c].front().id()), &ic);
    assert (found);
    crv3d.resize(crv3d.size()+1);
    s.reconstruct_candidate_1st_order(ini_id, end_id, ic, rig, &crv3d.back());
    attr.resize(attr.size()+1);
    attr.back().set_i0_i1(c, corresp[c].front().id());
    attr.back().total_support_ = static_cast<unsigned>(corresp[c].front().cost());
    // XXX set inlier views here .
  }

  vcl_cout << "Done reconstructing curves\n";
  return true;
}

bool 
dbmcs_match_and_reconstruct_all_curves(
    mw_odt_curve_stereo &s, 
    vcl_vector<dbdif_1st_order_curve_3d> *crv3d_ptr,
    mw_discrete_corresp *corresp_ptr
    )
{
  dbmcs_match_all_curves(s, corresp_ptr);

  corresp_ptr->keep_only_unambiguous_max(
      s.min_first_to_second_best_ratio(), 
      static_cast<double>(s.lonely_threshold()));

  // The following can be simulated by setting tau_first_to_second_best_ratio_ to zero
  // 
  //   corresp_ptr->keep_only_extreme_cost(true);

  return reconstruct_from_corresp(s, *corresp_ptr, crv3d_ptr);
}

bool 
reconstruct_from_corresp(
    mw_odt_curve_stereo &s, 
    const mw_discrete_corresp &corresp,
    vcl_vector<dbdif_1st_order_curve_3d> *crv3d_ptr)
{
  vcl_cout << "Reconstructing curves\n";

  vcl_vector<dbdif_1st_order_curve_3d> &crv3d = *crv3d_ptr;
  unsigned const ncurves = s.num_curves(s.v0());
  assert (ncurves == corresp.size());
  crv3d.reserve(ncurves);

  dbdif_rig rig(s.cams(s.v0()).Pr_, s.cams(s.v1()).Pr_);
  for (unsigned c=0; c < ncurves; ++c) {
    if (corresp[c].empty())
      continue;
    s.set_selected_crv_by_id(s.v0(), c);
    unsigned const ini_id = 0;
    unsigned const end_id = s.selected_crv(s.v0())->size()-1;
    s.set_subcurve(ini_id, end_id);
    s.compute_epipolar_beam_candidates();

    unsigned ic;
    bool found = s.get_index_of_candidate_curve(s.curves(s.v1(), corresp[c].front().id()), &ic);
    assert (found);
    crv3d.resize(crv3d.size()+1);
    s.reconstruct_candidate_1st_order(ini_id, end_id, ic, rig, &crv3d.back());
  }

  vcl_cout << "Done reconstructing curves\n";
  return true;
}
