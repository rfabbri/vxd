// This is bmcsd_odt_curve_stereo.h
#ifndef bmcsd_odt_curve_stereo_h
#define bmcsd_odt_curve_stereo_h
//:
//\file
//\brief Multiview curve stereo using distance transforms and orientation
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 06/22/2009 05:30:17 PM EDT
//

#include <dbdet/edge/dbdet_edgemap_sptr.h>
#include <dbdet/pro/dbdet_sel_storage_sptr.h>
#include <bdifd/bdifd_frenet.h>
#include <dbcsi/dbcsi_curve_distance.h>
#include <bmcsd/algo/bmcsd_dt_curve_stereo.h>
#include <mw/bmcsd_curve_3d_attributes.h>


//: Matches curve fragments using costs based on the distance transform and
// closest feature map (label map) of the edge map at each image, as well as
// orientation. This class assumes the user provides precomputed distance
// transform maps and label maps for each image, and that they are compatible
// with the provided edge map. The reason is that we want to run this class
// repeatedly for different pairs of views without having to recompute the DT.

class mw_discrete_corresp;

class bmcsd_odt_curve_stereo : public bmcsd_dt_curve_stereo {
public:
  bmcsd_odt_curve_stereo();
  virtual ~bmcsd_odt_curve_stereo() {}

  // Setters and Getters ------------------------------------------------------

  //: \see base class
  virtual bool set_nviews(unsigned nviews);

  //: set the edgemaps for each view.
  void set_all_edgemaps(const vcl_vector<dbdet_edgemap_sptr> &em);

  //: set the symbolic edge linker storages for each view.
  void set_all_sels(const vcl_vector<dbdet_sel_storage_sptr> &sels);

  //: set the tangents for the samples of the curve fragments. The indexing goes
  // tangents[id_view][id_curve][id_sample]
  void set_tangents(const vcl_vector<vcl_vector<vcl_vector<double> > > &tangents);

  //: sets the threshold in angular difference (radians) for wich an edgel is
  //considered an inlier to the reprojected curve point-tangent sample in each
  //view.
  void set_dtheta_threshold(double dtheta) { tau_dtheta_ = dtheta; }
  double dtheta_threshold() const { return tau_dtheta_; }
  unsigned min_inliers_per_view() const { return tau_min_inliers_per_view_; }
  unsigned min_total_inliers() const { return tau_min_total_inliers_; }

  //: Sets the minimum number of inliers per view. If a view has less than this
  // number of inliers, it will not vote at all towards the curve. This is to
  // increase robustness to occlusion and is used in the *_extras verion of the
  // stereo matcher.
  void set_min_inliers_per_view(unsigned n) { tau_min_inliers_per_view_ = n; }

  //: Sets the minimum number of total inliers per curve correspondence. If a
  // curve correspondence has less than this number of votes, it will have zero
  // votes.
  void set_min_total_inliers(unsigned n) { tau_min_total_inliers_ = n; }

  //: Sets minimum epipolar tangency delta angle. Effective range is
  // [0,vnl_math::pi/2)
  void set_min_epiangle(double dtheta) { tau_min_epiangle_ = dtheta; }

  //: Sets the lowe-like disambiguation ratio
  void set_min_first_to_second_best_ratio(double ratio) 
  { tau_min_first_to_second_best_ratio_ = ratio; }

  double min_first_to_second_best_ratio() const 
  { return tau_min_first_to_second_best_ratio_; }

  //: The threshold on single unique correspondences when applying lowe's
  // disambiguation criterion. This will cause lonely corresps to be accepted
  // only if their #votes surpass tau_min_first_to_second_best_ratio_*tau_lonely_
  void set_lonely_threshold(unsigned threshold) { tau_lonely_ = threshold; }

  unsigned lonely_threshold() const 
  { return tau_lonely_; }

  void set_min_num_inlier_edgels_per_curvelet(unsigned m)
  { tau_min_num_inlier_edgels_per_curvelet_ = m; }

  unsigned min_num_inlier_edgels_per_curvelet()
  { return tau_min_num_inlier_edgels_per_curvelet_; }

  //: \returns tangents(view id \p v, curve id \p c, point id \p p)
  // OBS: we might not have tangents for all views. 
  double curve_tangents(unsigned v, unsigned c, unsigned p) const 
  { assert(v < curve_tangents_.size()); return curve_tangents_[v][c][p]; }

  bool has_curve_tangents() const { return !pt_tgts_.empty(); }

  // Stereo Matching Methods ---------------------------------------------------

  //: Matches the selected subcurve to another subcurve in its epipolar beam such
  // that the overlaps reproject with the maximum number of inliers. The inliers
  // are measured from the distance map. 
  //
  // \param[out] i_best : index into \c crv_candidates_ptrs() of top matching curve
  // \param[out] votes : vcl_vector of the number of inliers, s.t. votes[i] ==
  // total number of inliers for crv_candidates_ptrs(i). 
  //
  // If there is no reliable curve, or if there are no candidates to begin with,
  // votes will be empty, but return value will be true.
  //
  // \return false if some error ocurred during matching.
  bool match_using_orientation_dt(unsigned *i_best, vcl_vector<unsigned long> *votes);

  //: Run match_using_orientation_dt, then perform additional tests to determine
  // if a match is reliable. The following criterion is used
  //
  // view-voting: gather supporting views instead of amassing all edgels
  // from all views into a single vote. A view is considered an inlier view if
  // the number of inlier edgels is greater than some threshold.
  //    new_votes[v] = (votes[v] > min_votes_per_view);
  //
  // This is used to prevent influence of clutter from views where the curve is
  // occluded.
  //
  bool match_using_orientation_dt_extras(vcl_vector<unsigned long> *votes_ptr);

  //: Convenience function returning the index of the best match.
  bool match_using_orientation_dt_extras(unsigned *i_best, vcl_vector<unsigned long> *votes_ptr);

  //: Reconstructs given subcurve specified by index ini_id and end_id into
  // selected_crv_id(v0()), by assuming correspondence to candidate curve with index \p ic.
  // This version reconstructs the tangent information as well.
  void reconstruct_candidate_1st_order(unsigned ini_id, unsigned end_id, unsigned ic, 
      const bdifd_rig &rig, bdifd_1st_order_curve_3d *crv_ptr);

  //: Reconstructs subcurve given by curve selected_crv_id(v0()) and endpoints with
  // index ini_id_sub, end_id_sub, using view[v0()] and the corresponding
  // selected_crv_id(v1()) in view[v1()]. This version reconstructs the tangent
  // information as well.
  void reconstruct_subcurve_1st_order(
      unsigned ini_id_sub, 
      unsigned end_id_sub, 
      const bdifd_rig &rig,
      bdifd_1st_order_curve_3d *curve_3d
      ) const;

  //: Reconstructs a 3D point-tangent, given the 2D image point-tangent \c p_0
  // in the first view and the index \p v of the second view to use. The point in
  // the second view is found by intersecting the epipolar line of p_0 with the
  // selected curve of that view.
  //
  // \param[in] di0 is the index of the point p_0 in the selected sub-curve of view 0
  // \param[in] ini_id is how far the initial point of the subcurve is inside the
  // containing curve fragment container.
  //
  void reconstruct_curve_point_1st_order(
      unsigned v,
      unsigned ini_id,
      unsigned di0, 
      const bdifd_rig &rig,
      bdifd_1st_order_point_3d *pt_3D
      ) const;

  //: Projects a 1st order curve from 3D world to 2D image coordinates on \p view.
  // \todo this is a general-purpose utility. If bdifd was well-designed, it'd
  // have this.
  void project_curve_1st_order(
      unsigned view, 
      const bdifd_1st_order_curve_3d &crv3d,
      bdifd_1st_order_curve_2d *proj_ptr
      ) const;

  //: \return true in case edgemaps, labels, and a curve is selected in view 0,
  // as well as if other preconditions for oriented matching are satisfied.
  bool ready_for_oriented_matching();

  //: The reprojection curve in each view \p c obtained by corresponding
  // the subcurve in view[0] and the epipolar band candidate \p c in view[1].
  // \return the curve-tangent of the reprojection curve, indexed as
  // reprojection_crv(curve index, view index, point index)
  dbdet_edgel reprojection_crv(unsigned c, unsigned v, unsigned p) const
  { return reprojection_crv_[c][v][p]; }

  //: Size of the reprojection curve(candidate index, view index). 
  // \see reprojection_crv
  unsigned reprojection_crv_size(unsigned c, unsigned v) const
  { return reprojection_crv_[c][v].size(); } 

  bool has_reprojection_crv() const { return !reprojection_crv_.empty(); }

  //: This version uses angle from set_min_epiangle to determine
  // keep only the episegs having all point-tangents within an angle of
  // the epipolar line. Such angle is set in set_min_epiangle().
  virtual void break_curves_into_episegs_pairwise(
      vcl_vector<vcl_vector< vsol_polyline_2d_sptr > > *broken_vsols,
      vcl_vector<bbld_subsequence_set> *ss_ptr
      ) const;

  //: Stand-alone episeg breaker using tangent angle information.
  // \see break_curves_into_episegs_pairwise
  static void break_curves_into_episegs_angle(
    const vcl_vector< vsol_polyline_2d_sptr >  &vsols,
    const vcl_vector<vcl_vector<double> > &tgts,
    double min_epiangle,
    vcl_vector<vsol_polyline_2d_sptr> *vsols2,
    const vgl_homg_point_2d<double> &e,
    bbld_subsequence_set *ss_ptr);

  //: Precondition: set_tangents and set_curve must have been called.
  virtual void break_into_episegs_and_replace_curve(
      vcl_vector<bbld_subsequence_set> *curves_ss);

  bool has_sels() { return !sels_.empty(); }

protected:
  bool has_edgemaps() { return !em_.empty(); }

private:
  //: edgemap for each view.
  vcl_vector<dbdet_edgemap_sptr> em_;

  //: symbolic edge linker structure for each view.
  vcl_vector<dbdet_sel_storage_sptr> sels_;

  typedef vcl_vector<bdifd_1st_order_curve_2d> first_order_curve_map;

  //: curve map for each view with 1st order differential geometry
  // (point-tangents), already in world coordinates. Indexing goes
  // pt_tgts_[id_view][id_curve][id_sample]
  vcl_vector< first_order_curve_map > pt_tgts_;
  double tau_dtheta_;
  unsigned tau_min_inliers_per_view_;
  unsigned tau_min_total_inliers_;
  double tau_min_epiangle_;
  double tau_min_first_to_second_best_ratio_;
  unsigned tau_lonely_;
  unsigned tau_min_num_inlier_edgels_per_curvelet_;

  //: \see curve_tangents()
  vcl_vector<vcl_vector<vcl_vector<double> > > curve_tangents_;
  vcl_vector<vcl_vector<dbcsi_edgel_seq> > reprojection_crv_;
};

//: Reconstruct all curves that match bewtween two views. 
// \param[in] s : must be all set and ready to match.
// \param[in] only_non_empty: true if only non-empty correspondences are
// reconstructed. You might want this to be false if you want the id into the 3D
// curve vector to match the id of the curves in the first image.
// \param[out] crv3d: crv3d[curve_id] == curve 3d for s.curves(v0(), curve_id);
// \param[out] corresp: corresp[c] == correspondence of s.curves(v0(), c) used for crv3d[c]
bool bmcsd_match_and_reconstruct_all_curves(
    bmcsd_odt_curve_stereo &s, 
    vcl_vector<bdifd_1st_order_curve_3d> *crv3d,
    mw_discrete_corresp *corresp
    );

//: version of bmcsd_match_and_reconstruct_all_curves which fills an attribute
// for each curve.
bool bmcsd_match_and_reconstruct_all_curves_attr(
    bmcsd_odt_curve_stereo &s, 
    vcl_vector<bdifd_1st_order_curve_3d> *crv3d_ptr,
    mw_discrete_corresp *corresp_ptr,
    vcl_vector< bmcsd_curve_3d_attributes > *attr_ptr
    );

//: Matches all curves bewtween two views.
// \param[in] s : must be all set and ready to match.
// \param[out] corresp_ptr : discrete correspondence between the first two
// views. Only the correspondences passing all thresholds are output.
bool bmcsd_match_all_curves(
  bmcsd_odt_curve_stereo &s, 
  mw_discrete_corresp *corresp_ptr);

//: For each curve i, reconstructs from corresp[i].front(). If you want to
// reconstruct from the best curve, call corresp.keep_only_extreme_cost(true)
// first. 
// \param[in] only_non_empty: true if only non-empty correspondences are
// reconstructed. You might want this to be false if you want the id into the 3D
// curve vector to match the id of the curves in the first image.
//
// \return true if success.
bool reconstruct_from_corresp(
    bmcsd_odt_curve_stereo &s, 
    const mw_discrete_corresp &corresp,
    vcl_vector<bdifd_1st_order_curve_3d> *crv3d_ptr
    );

//: variant of reconstruct_from_corresp which also outputs attributes.
bool 
reconstruct_from_corresp_attr(
    bmcsd_odt_curve_stereo &s, 
    const mw_discrete_corresp &corresp,
    vcl_vector<bdifd_1st_order_curve_3d> *crv3d_ptr,
    vcl_vector< bmcsd_curve_3d_attributes > *attr_ptr
    );

#endif // bmcsd_odt_curve_stereo_h
