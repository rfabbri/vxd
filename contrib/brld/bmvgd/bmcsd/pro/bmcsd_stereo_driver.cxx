#include "bmcsd_stereo_driver.h"
#include <mw/pro/bprod_fragment_tangents_filter.h>
#include <bmcsd/pro/bmcsd_load_curvelet_source.h>
#include <bmcsd/pro/bmcsd_stereo_filter.h>
#include <vcl_algorithm.h>

bool bmcsd_concurrent_stereo_driver::
init()
{
  //: For each view, setup the following processes:
  //
  // Sources:
  // + cam_src_[v] load camera
  // + edg_src_[v] load edgemap
  // + frag_src_[v] load curve fragments
  //
  // Filters:
  // + edg_dt_[v]: compute dt and label map from subpixel edgemap
  // - frag_tangents_[v]: interpolates the curve fragments and extracts their
  // tangents
  //
  // - curve_stereo_: does multiview stereo to match two views. 
  //    - Inputs, 
  //    for v among the stereo views:
  //       - cam_src_[v] 
  //       - frag_src_[v]
  //       - frag_tangents_[v]
  //
  //    for c among the confirmation views:
  //       - cam_src_[c] 
  //       - edg_src_[c]
  //       - edg_dt_[c]
  //
  // - curve_stereo_job[i] 
  //    - Inputs are the outputs of max_concurrent_matchers_
  // curve_stereo_ filters, namely 3D curves and attributes for those curves.
  //    - Outputs concatenated 3D recs.
  //
  // - output_job
  //    - Inputs are the output 3D curves and attributes from all
  //    concurrent_jobs
  //    - It executes each input sequentially.

  //  unsigned num_curve_stereo_runs = f_.num_instances();
  //  unsigned num_curve_stereo_jobs = num_curve_stereo_runs/max_concurrent_matchers_;

  vcl_cout << "Initializing stereo driver w nviews = " << nviews() << vcl_endl;
  cam_src_.reserve(nviews());
  edg_src_.reserve(nviews());
  if (use_curvelets_)
    cvlet_src_.reserve(nviews());
  edg_dt_.reserve(nviews());
  frag_src_.reserve(nviews());
  frag_tangents_.reserve(nviews());
  // ------------- Setup the processing nodes -------------

  // Setup nodes that act on a single frame
  for (unsigned v=0; v < nviews(); ++v) {
    // Attach sources to files -----

    // 1 Cam loader
    bprod_process_sptr 
      p = new bmcsd_load_camera_source<double>(
          dpath_[v].cam_full_path(), dpath_[v].cam_file_type());
    cam_src_.push_back(p);
 
    // 1 Edge map loader
    static const bool my_bSubPixel = true;
    static const double my_scale=1.0;
    p = new bmcsd_load_edg_source(dpath_[v].edg_full_path(), my_bSubPixel, my_scale);
    edg_src_.push_back(p);

    // 1 curve fragment loader
    bmcsd_load_vsol_polyline_source 
      *p_curve_src = new bmcsd_load_vsol_polyline_source(dpath_[v].frag_full_path());

    frag_src_.push_back(p_curve_src);

    // 1 curvelet map loader
    if (use_curvelets_)  {
      bmcsd_load_curvelet_source
        *p_curvelet_src = new bmcsd_load_curvelet_source(dpath_[v].cvlet_full_path());
      cvlet_src_.push_back(p_curvelet_src);
    }

    // 1 dt and label loader
    p = new bprod_edg_dt;
    edg_dt_.push_back(p);
    p->connect_input(0, edg_src_[v], 0);


    // 1 curve fragment tangent interpolator
    p = new bprod_fragment_tangents_filter();
    frag_tangents_.push_back(p);
    p->connect_input(0, frag_src_[v], 0);
  }

  curve_stereo_.reserve(f_.num_instances());

  //: Setup nodes that act on multiple frames
  for (unsigned i=0; i < f_.num_instances(); ++i) {
    bmcsd_stereo_filter *p = new bmcsd_stereo_filter();
    curve_stereo_.push_back(p);

    p->setup_inputs(
        f_.instance(i),
        cam_src_,
        edg_src_,
        edg_dt_,
        frag_src_,
        cvlet_src_,
        frag_tangents_);
    
  }

  update_stereo_params();

  curve_stereo_jobs_.reserve(static_cast<unsigned>(vcl_ceil(curve_stereo_.size()/max_concurrent_matchers_)));
  //: Setup the nodes to do simultaneous processing
  for (unsigned i=0; i < curve_stereo_.size(); ++i) {
    if (i % max_concurrent_matchers_ == 0) {
      unsigned num_matchers
       = vcl_min(static_cast<unsigned long>(max_concurrent_matchers_), 
           static_cast<unsigned long>(curve_stereo_.size()-i));

      curve_stereo_jobs_.push_back(new bmcsd_stereo_jobs(num_matchers));
    }

    bprod_process_sptr p = curve_stereo_jobs_.back();
    unsigned idx = 3*(i % max_concurrent_matchers_);
    p->connect_input(idx,   curve_stereo_[i], 0);
    p->connect_input(idx+1, curve_stereo_[i], 1);
    p->connect_input(idx+2, curve_stereo_[i], 2);
  }

  //: Now connect all curve_stereo_jobs_ into a single output job.
  output_job_ = new bmcsd_stereo_aggregator(curve_stereo_jobs_.size());
  for (unsigned i=0; i < curve_stereo_jobs_.size(); ++i) {
    output_job_->connect_input(3*i,   curve_stereo_jobs_[i], 0 /* crv_3d */);
    output_job_->connect_input(3*i+1, curve_stereo_jobs_[i], 1 /* attr */);
    output_job_->connect_input(3*i+2, curve_stereo_jobs_[i], 2 /* corresp */);
  }

  initialized_ = true;
  return true;
}

bool bmcsd_concurrent_stereo_driver::
run(unsigned long timestamp)
{
  vcl_cout << "Running stereo driver.\n";
  assert(initialized_);
  bprod_signal retval = output_job_->run(timestamp);

  if (retval == BPROD_INVALID)
    return false;

  bmcsd_stereo_aggregator *o = dynamic_cast<bmcsd_stereo_aggregator *> (output_job_.ptr());
  crv3d_ = &o->crv3d_;
  attr_  = &o->attr_;
  corresp_  = &o->corresp_;

  return true;
}

void bmcsd_concurrent_stereo_driver::
update_stereo_params()
{
  for (unsigned i=0; i < frag_src_.size(); ++i) {
    bmcsd_load_vsol_polyline_source *p = 
      dynamic_cast<bmcsd_load_vsol_polyline_source *>(frag_src_[i].ptr());
    if (prune_by_length_)
      p->set_min_length(tau_min_length_per_curve_frag_);
    else
      p->set_min_samples(tau_min_samples_per_curve_frag_);
  }

  for (unsigned i=0; i < curve_stereo_.size(); ++i) {
    bmcsd_stereo_filter *p = dynamic_cast<bmcsd_stereo_filter *>(curve_stereo_[i].ptr());
    assert(p);

    p->s_.set_tau_distance_squared(tau_distance_*tau_distance_);
    p->s_.set_dtheta_threshold(tau_dtheta_*vnl_math::pi/180.0);
    p->s_.set_min_inliers_per_view(tau_min_inliers_per_view_);
    if (prune_by_length_)
      p->s_.set_min_length_per_curve_frag(tau_min_length_per_curve_frag_);
    else
      p->s_.set_min_samples_per_curve_frag(tau_min_samples_per_curve_frag_);
    p->s_.set_min_total_inliers(tau_min_total_inliers_);
    p->s_.set_min_first_to_second_best_ratio(tau_min_first_to_second_best_ratio_);
    p->s_.set_lonely_threshold(tau_lonely_);
    p->s_.set_min_epiangle(tau_min_epiangle_*vnl_math::pi/180.0);
    p->s_.set_min_epipolar_overlap(tau_min_epipolar_overlap_);
    p->s_.set_min_num_inlier_edgels_per_curvelet(tau_min_num_inlier_edgels_per_curvelet_);
  }
}
