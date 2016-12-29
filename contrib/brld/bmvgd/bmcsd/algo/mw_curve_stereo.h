// This is mw_curve_stereo.h
#ifndef mw_curve_stereo_h
#define mw_curve_stereo_h
//:
//\file
//\brief Multiview curve stereo
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri@gmail.com)
//\date 06/08/2009 08:08:46 PM EDT
//

#include <vsol/vsol_polyline_2d_sptr.h>
#include <bbld/bbld_subsequence.h>
#include <dbdif/dbdif_rig.h>

#include <mw/mw_util.h>
#include <mw/mw_intersection_sets.h>

#define MW_INVALID_CURVE_ID ((unsigned)-1)

//: This class implements multiview curve fragment matching and reconstruction
// from calibrated cameras. This is a working but ad-hoc implementation that
// came from my GUI tools.
//
// At any point in time, the state of the class is a set of curve fragments in
// each calibrated image, together with a selected curve segment. The methods
// act given this state, thus many of them assume a selected curve at any point
// in time. Methods that want to reconstruct all curves must therefore change
// the selected curve to the desired one before running the required functions.
//
// The flaw in this design is that it is not always clear which methods need a
// selected curve. 
//
// \todo URGENT the initial curve candidates by bounding box intersection are
// wrong.
//
class mw_curve_stereo {
public:
  //: Constructor
  mw_curve_stereo();
 
  //: Destructor
  virtual ~mw_curve_stereo() {}

  // Setters and Getters ------------------------------------------------------

  //: Sets the number of views to \p nviews. The vcl_vector frames_v provides
  // the index of each view into a larger set of frames
  // \return false in case of any error.
  // \todo Perhaps protected and called from e.g. set_cameras?
  virtual bool set_nviews(unsigned nviews);
  unsigned nviews() const { return nviews_; }

  //: \return the index of the first view, i.e., the view for which we seek
  // matching curves
  unsigned v0() const { return 0; }

  //: \return the index of the second view, i.e., the view at which we seek
  // matching curves for those coming from the first view.
  unsigned v1() const { return 1; }

  //: set the cameras for each view
  void set_cams(const vcl_vector<dbdif_camera> &cams);
  const dbdif_camera &cams(unsigned v) const { return cam_[v]; }

  //: set the curve fragments from a vector of curves for each view.
  // curves[v0()] and curves[v1()] are required. The others are optional.
  void set_curves(const vcl_vector<vcl_vector< vsol_polyline_2d_sptr > > &curves);

  //: returns the curve fragments from a vector of curves for each view
  const vsol_polyline_2d_sptr &curves(unsigned iview, unsigned icurve) const
  {  assert (icurve < num_curves(iview)); return vsols_[iview][icurve]; }

  //: returns the number of curve fragments in view \p iview
  unsigned num_curves(unsigned iview) const { assert (iview < nviews()); return vsols_[iview].size(); }

  //: returns the number of samples in the curve fragment \p iurve view \p iview, 
  unsigned num_samples(unsigned iview, unsigned icurve) const 
  { return vsols_[iview][icurve]->size(); }

  //: Selected curve in each view. 
  // \returns NULL in case there is no selected curve in view[\p v]
  const vsol_polyline_2d_sptr &selected_crv(unsigned v) const { return selected_crv_[v]; }

  //: \return id of the selected curve in each view, so that the selected
  // curve can be retrieved as curves(v, j), with j being the returned value.
  unsigned selected_crv_id(unsigned v) const { return selected_crv_id_[v]; }

  //: The scrv polyline sptr must point to a polyline within curves().
  void set_selected_crv(unsigned iview, const vsol_polyline_2d_sptr &scrv);

  //: sets the curve \c curves(iview, id) as the selected curve
  void set_selected_crv_by_id(unsigned iview, unsigned id) 
    { selected_crv_id_[iview] = id; selected_crv_[iview] = vsols_[iview][id]; }

  //: Used to prune the curves (after epipolar breakage) by enforcing a minimum
  // number of samples.
  void set_min_samples_per_curve_frag(unsigned m) 
    { tau_min_samples_per_curve_frag_ = m; prune_by_length_ = false; }

  //: Used to prune the curves (after epipolar breakage) by enforcing a minimum
  // length.
  void set_min_length_per_curve_frag(double m) 
    { tau_min_length_per_curve_frag_ = m; prune_by_length_ = true; }

  unsigned min_samples_per_curve_frag() const
    { return tau_min_samples_per_curve_frag_; }

  double min_length_per_curve_frag() const
    { return tau_min_length_per_curve_frag_; }

  //: Epilines for subcurve's starting point, from view[v0()] to each other
  // view.
  // Index to vector designates other views; index 0 is 2nd view and index 1 is
  // 3rd view.
  const vgl_homg_line_2d<double> &ep_ini(unsigned v) const 
    { assert (v < nviews_-1); return ep_ini_[v]; }

  //: Epilines for subcurve's endpoint, from view[v0()] to each other view.
  // Index to vector designates other views; index 0 is 2nd view and index 1 is
  // 3rd view.
  const vgl_homg_line_2d<double> &ep_end(unsigned v) const 
    { assert (v < nviews_-1); return ep_end_[v]; }

  //: Epipolar lines of all subcurve's points in view[v0()] in all other views.
  // Index to vector designates other views; index 0 is 2nd view and index 1 is
  // 3rd view.
  const vcl_vector<vgl_homg_line_2d<double> > &ep(unsigned v) const 
    { assert (v < nviews_-1); return ep_[v]; }

  //: Epipolar lines of all the selected curve in view[v1()] in all other views.
  // Index to vector designates other views; index 0 is 3rd view, index 1 is
  // 4rth view, and so forth. Only the epipolar lines for the points having some
  // correspondent point in the selected curve of view[v0()] are stored.
  const vgl_homg_line_2d<double> &ep_v1_ini(unsigned v) const  
    { assert (v < nviews_-2); return ep_ini_v1_[v]; }
  const vgl_homg_line_2d<double> &ep_v1_end(unsigned v) const 
    { assert (v < nviews_-2); return ep_end_v1_[v]; }
  const vcl_vector<vgl_homg_line_2d<double> > &ep_v1(unsigned v) const 
    { assert (v < nviews_-2);  return ep_v1_[v]; }

  //: Epipolars in view[v0()] coming from view[v1()]. Index indicates point.
  const vgl_homg_line_2d<double> &ep_left(unsigned pt_id) const 
    { return ep_left_[pt_id]; }

  //: all points for the selected subcurve of selected_crv(v0())
  const vsol_polyline_2d_sptr &subcurve() const { return subcurve_; }

  //: sets the selected subcurve() in view[v0()] to have the endpoints with
  // index \p ini_id and \p end_id in selected_crv(v0()). The endpoint index
  // needn't be in increasing order.
  void set_subcurve(unsigned ini_id, unsigned end_id);

  const mw_intersection_sets &isets() const { return isets_; }

  //: Returns the curve candiate \p ic, for \p ic = 0,...,num_candidates()-1,
  // from view[i2].
  // \see compute_epipolar_beam_candidates()
  const vsol_polyline_2d_sptr &crv_candidates_ptrs(unsigned ic) const
    { assert(ic < num_candidates()); return crv_candidates_ptrs_[ic]; }

  //: Returns the index of the curve candiate \p ic, for \p ic = 0,...,num_candidates()-1,
  // into the curves for view[i2nd]
  // \see compute_epipolar_beam_candidates()
  unsigned crv_candidates(unsigned ic) const
    { assert(ic < num_candidates()); return crv_candidates_[ic]; }

  unsigned num_candidates() const { return crv_candidates_ptrs_.size(); }

  // Reconstruction and Reprojection Methods ----------------------------------

  //: Reconstructs the currently selected curve using different methods and
  // writes the results to files.
  //
  // \todo currently this method is not written to be called inside loops, as it
  // will reconstruct using many different methods and write to files.
  void reconstruct_multiview();

  //: \param[in] other_views[i] specify from which other view does pt_img[i+1]
  // come from.  It is assumed that pt_img[v0()] always comes from views[v0()].
  void linearly_reconstruct_pts(
      const vcl_vector<vsol_point_2d_sptr> &pt_img,
      const vcl_vector<unsigned> &other_views,
      vgl_point_3d<double> *pt_3D) const;

  //: \param[in] views: views[i] specify from which other view does pt_img[i+1]
  // come from.  It is assumed that pt_img[v0()] always comes from views[v0()].
  void nonlinearly_optimize_reconstruction(
      const vcl_vector<vsol_point_2d_sptr> &pt_img,
      const vcl_vector<unsigned> &other_views,
      const vgl_point_3d<double> &pt_3D_initial,
      vgl_point_3d<double> *pt_3D) const;

  //: \param[in] views: views[i] specify from which other view does pt_img[i+1]
  // come from.  It is assumed that pt_img[v0()] always comes from views[v0()].
  //
  // \seealso caller + implementation
  //
  void get_reconstructions(
      const vcl_vector<unsigned> &other_views, 
      unsigned ini_id, 
      unsigned di0, 
      mw_vector_3d *pt_3D, 
      mw_vector_3d *pt_3D_linear) const;

  //: Reconstructs a 3D point, given the 2D image point \p p_0 in the first view
  // and the index \p v of the second view to use. The point in the second view is
  // found by intersecting the epipolar line of p_0 with the selected curve of
  // that view.
  //
  // \param[in] di0 is the index of the point p_0 in the selected sub-curve of view v0()
  // \param[in] ini_id is how far the initial point of the subcurve is inside the
  // containing curve fragment container.
  //
  void reconstruct_curve_point_kanatani(
      unsigned v,
      unsigned ini_id,
      unsigned di0, 
      mw_vector_3d *pt_3D) const;

  void project_curve(
      unsigned view, 
      const vcl_vector<mw_vector_3d> &crv3d,
      vcl_vector<vsol_point_2d_sptr> *proj
      ) const;

  //: Reprojects the selected subcurve into all views[i] for i >= 2 by
  // specifying the index of the corresponding curve. It also computes all
  // epipolar lines involved in the reprojection, which are accessible through
  // ep_v1().
  //
  // \param[in] crv2_id: index into crv_candidates_ptrs_ of the candidate curve to consider
  void reproject_in_all_views(
      unsigned crv2_id, vcl_vector< vcl_vector<vsol_point_2d_sptr> > *reproj);

  //:
  // \param[in] crv2_id: index into crv_candidates_ptrs_ of the candidate curve to consider
  //
  // \param[out] crv1_pt_id: indices into crv_ (selected curve in image v1()) of point to
  // reconstruct
  //
  // \param[out] crv2_pt_id: indices into crv_candidates_ptrs_[crv2_id] of point to
  // reconstruct. Has same size as crv1_pt_id
  //
  // \todo rewrite 
  void reconstruct_and_reproject(
      unsigned crv2_id, 
      unsigned view, 
      vcl_vector<vsol_point_2d_sptr> &reproj, 
      vcl_vector<mw_vector_3d> &crv3d, 
      vcl_vector<unsigned> &crv1_pt_id,
      vcl_vector<unsigned> &crv2_pt_id,
      dbdif_rig &rig) const;

  //: 
  // \param[in] crv2_id: index into crv_candidates_ptrs_ of the candidate curve to consider
  //
  // \param[out] crv3d : reconstructed curves. crv3d[i] == reconstruction of
  // selected segment matched with i-th candidate in 2nd view.
  //
  // \param[out] crv1_pt_id: crv1_pt_id[i] == index in crv_ of point used
  // to generate crv3d[i]
  //
  // \param[out] crv2_pt_id: crv2_pt_id[i] == index in cand_crv_ptr_[crv2_id] of point used
  // to generate crv3d[i]
  //
  void reconstruct_one_candidate(
      unsigned crv2_id, 
      vcl_vector<mw_vector_3d> &crv3d, 
      const vcl_vector<unsigned> &crv1_pt_id,
      const vcl_vector<unsigned> &crv2_pt_id,
      dbdif_rig &rig) const;

  //:
  // \param[in] crv2_id: index into crv_candidates_ptrs_ of the candidate curve to consider
  //
  // \param[out] crv1_pt_id: indices into crv_ (selected curve in image v1()) of point to
  // reconstruct
  //
  // \param[out] crv2_pt_id: indices into crv_candidates_ptrs_[crv2_id] of point to
  // reconstruct. Has same size as crv1_pt_id
  //
  // \todo rewrite 
  void define_match_for_reconstruction(
      unsigned crv2_id,
      vcl_vector<unsigned> &crv1_pt_id,
      vcl_vector<unsigned> &crv2_pt_id,
      dbdif_rig &rig) const;

  //: Reconstructs subcurve given by curve selected_crv_[v0()] and endpoints with
  // index ini_id_sub, end_id_sub, using view[v0()] and the corresponding
  // selected_crv(v1()) in view[v1()]
  void reconstruct_subcurve(
      unsigned ini_id_sub, 
      unsigned end_id_sub, 
      vcl_vector<mw_vector_3d> *curve_3d) const;

  //: Input:
  //   - a selected subcurve curve in view_[v0()] specified by the
  //   containing curve fragment selected_crv_[v0()] and endpoint indices \p ini_id
  //   and \p end_id, with \p ini_id <= \p end_id.
  //   
  //   - an index \p candidate_index of a candidate curve fragment stored in
  //   crv_candidates_ptrs_.
  //
  //  Output: 
  //   - a new pair of endpoint indices into selected_crv_[v0()], such that all the
  //   points of the joining subcurve has an epipolar intersection into the
  //   candidate curve.
  //
  //  We assume the curves have been previously broken at epipolar tangencies in
  //  order to avoid multiple epipolar intersections.
  void get_matching_subcurve(
      unsigned candidate_index,
      unsigned ini_id,
      unsigned end_id,
      unsigned *ini_id_sub,
      unsigned *end_id_sub
      ) const;

  // Other Methods ------------------------------------------------------------

  //: Break curves into epipolar segments that are not epipolar tangent to any
  // epipolar line formed from view[v0()]. The vcl_vector \p broken_vsols is
  // appropriately resized.
  //
  // \todo find a true multiview way of breaking the curves
  virtual void break_curves_into_episegs_pairwise(
      vcl_vector<vcl_vector< vsol_polyline_2d_sptr > > *broken_vsols,
      vcl_vector<bbld_subsequence_set> *ss_ptr) const;

  //: Stand-alone episeg breaker.
  // \see break_curves_into_episegs_pairwise
  static void break_curves_into_episegs(
    const vcl_vector< vsol_polyline_2d_sptr >  &vsols,
    vcl_vector<vsol_polyline_2d_sptr> *vsols2,
    const vgl_homg_point_2d<double> &e,
    bbld_subsequence_set *ss_ptr);

  //: Same as set_curves, then break_curves_into_episegs_pairwise, prune, then
  // set_curves again. 
  //
  // Preconditions: curves must have been set.
  //
  // \param[out] curves_ss : Represents a subset of subcurves from the pool of
  // original curves. This allows us to go back to the original curves after
  // breaking them into episegs and/or after pruning them.
  virtual void break_into_episegs_and_replace_curve(vcl_vector<bbld_subsequence_set> *curves_ss);

  //: Grow the selected curve in view[v0()] by advancing the endpoint once. 
  // \return NULL if the result would be out of bounds, o.w. the new endpoint.
  const vsol_point_2d_sptr advance_endpoint();

  //: Shrink the selected curve in view[v0()] by advancing the endpoint once
  // \return NULL if the result would be out of bounds, o.w. the new endpoint.
  const vsol_point_2d_sptr recede_endpoint(); 

  //: initializes a subcurve of the selected curve fragment in view[v0()] by
  // specifying the index of a single initial point in this curve fragment.
  // The parameter \p pt_id indexes into selected_crv(v0()).
  void initialize_subcurve(unsigned pt_id);

  //: update the enpoint of the subcurve in \c view[v0()] by setting it to the
  // point \c selected_curve(v0())->vertex(end_id)
  void update_endpoint(unsigned end_id);

  void swap_endpoints();

  //: returns the indices of the subcurve endpoints in increasing order, so that
  // the subcurve can be traced by traversing selected_curve(v0()) from index \p ini
  // to \p end
  inline void get_increasing_endpoints(unsigned *ini, unsigned *end) const; 

  // Stereo Matching Methods ---------------------------------------------------

  //: Any curve fragment that touches at least one epipolar line of the beam is a
  // taken as candidate. By "touches" we mean having a point in the vicinity of an
  // epipolar line. The threshold used to be 1.4px (\todo option to set it).
  //
  // The candidates can be accessed through \see crv_candidates_ptrs() as
  // pointers and \see crv_candidates() as indices into the curve set in view[v1()]
  //
  // \param[in]   
  void compute_epipolar_beam_candidates();

  // A curve in view[v1()] has to intercept at least \p min_intercepts epipolar
  // lines of subcurve() in order to qualify as a candidate.
  void set_min_epipolar_overlap(unsigned m) { tau_min_epipolar_overlap_ = m; }

  // \param[out] crv2_id : id of selected_crv in the list of epipolar candidates for the currently
  // selected curve.
  //: \return false if not found
  bool get_index_of_candidate_curve( const vsol_polyline_2d_sptr & select_crv, unsigned *crv2_id) const;

  //: \return false if not found
  bool get_index_of_curve(
      const vsol_polyline_2d_sptr & select_crv, unsigned view, unsigned *crv2_id) const;

  //: gets the points of intersection of the epipolar lines of subcurve() and all the
  // candidate curve fragments. This assumes as candidates those curves that
  // have been found using \c compute_epipolar_beam_candidates.
  void get_candidate_intercepts(vcl_vector<vcl_vector<unsigned> > *cand_pt_id_ptr);

protected:

  //: Returns index into isets_.L_ of crv_candidates(\p ic). This is protected
  // because it doesn't seem to be useful for users.
  unsigned crv_candidates_id(unsigned ic) const
    { assert(ic < num_candidates()); return crv_candidates_id_[ic]; }

  //: fundamental matrices for each pair of views
  vcl_vector<vcl_vector<vpgl_fundamental_matrix<double> > > fm_;

  //: set of curves at each view. vsols_[v0()] and vsols_[v1()] are required.
  // The others are optional.
  vcl_vector<vcl_vector< vsol_polyline_2d_sptr > > vsols_; 

private:
  //: cameras for each view
  vcl_vector<dbdif_camera> cam_; 
  unsigned nviews_;

  //: \see selected_crv() 
  vcl_vector<vsol_polyline_2d_sptr> selected_crv_; 

  //: \see selected_crv_id() 
  vcl_vector<unsigned> selected_crv_id_; 

  //: subcurve endpoints p0 and pn
  unsigned p0_id_;
  unsigned pn_id_;

  //: \see subcurve() 
  vsol_polyline_2d_sptr subcurve_;

  //: \see ep_ini()
  vcl_vector<vgl_homg_line_2d<double> > ep_ini_;

  //: \see ep_end()
  vcl_vector<vgl_homg_line_2d<double> > ep_end_;

  //: \see ep()
  vcl_vector<vcl_vector<vgl_homg_line_2d<double> > > ep_; 

  //: Epipolar lines from view[v1()] to view[i] for i >= 2.
  // Index to vector designates other views; index 0 is 3rd view, index 1 is
  // 4rth view, and so forth.
  vcl_vector<vgl_homg_line_2d<double> >  ep_ini_v1_;
  vcl_vector<vgl_homg_line_2d<double> >  ep_end_v1_;
  vcl_vector<vcl_vector<vgl_homg_line_2d<double> > > ep_v1_; 

  // \see ep_left()
  vcl_vector<vgl_homg_line_2d<double> > ep_left_; 

  //: index into vsols_[i2] of candidate (whole) curves
  vcl_vector<unsigned> crv_candidates_;

  //: \see crv_candidates_id() 
  vcl_vector<unsigned> crv_candidates_id_;  

  //: is the following really needed?
  vcl_vector<vsol_polyline_2d_sptr> crv_candidates_ptrs_;

  //: a representation of the points of intersection of curves in view[v1()] with
  // the beam of epipolar lines of the selected subcurve of view[v0()].
  mw_intersection_sets  isets_;

  //: \see set_min_samples_per_curve_frag
  unsigned tau_min_samples_per_curve_frag_;

  //: \see set_min_length_per_curve_frag
  double tau_min_length_per_curve_frag_;
  unsigned tau_min_epipolar_overlap_;

  bool prune_by_length_;

  //: used in get_index_of_curve
  mutable bool cached_curve_id_from_sptr_;
  //: used in get_index_of_curve
  mutable vcl_map<vsol_polyline_2d_sptr, unsigned> curve_v0_id_from_sptr_;
};

inline void mw_curve_stereo::
get_increasing_endpoints(unsigned *ini, unsigned *end) const
{
  if (p0_id_ < pn_id_) {
    *ini = p0_id_;
    *end = pn_id_;
  } else {
    *ini = pn_id_;
    *end = p0_id_;
  }
}

#endif // mw_curve_stereo_h
