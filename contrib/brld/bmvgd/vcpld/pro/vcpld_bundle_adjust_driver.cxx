#include "vcpld_bundle_adjust_driver.h"
#include <mw/pro/bprod_fragment_tangents_filter.h>
#include <mw/pro/vcpld_bundle_adjust_sink.h>
#include <vcl_set.h>
#include <vcl_algorithm.h>


void vcpld_bundle_adjust_driver::
get_curve_visibility_using_reference_views(
    vcl_vector<vcl_vector<bool> > *mask_ptr )
{
  vcl_vector<vcl_vector<bool> > &mask = *mask_ptr;

  mask.resize(nviews_, 
      vcl_vector<bool>(csk_->total_num_points(), false));

  unsigned i=0;
  for (unsigned c=0; c < csk_->num_curves(); ++c) {
    vcl_cout << "Inliers for curve[" << c << "]: ";

    vcl_vector<unsigned> inlier_views;

    //    source views not automatically included since curves must be away from
    //    boundary to be inlier view to even their source views.
    //
//    unsigned view0 = csk_->attributes()[c].v_->stereo0();
//    if (view0 < maxviews_)
//      inlier_views.push_back(view0);

//    unsigned view1 = csk_->attributes()[c].v_->stereo1();
//    if (view1 < maxviews_)
//      inlier_views.push_back(view1);

    for (unsigned k=0; k < csk_->attributes()[c].inlier_views_.size(); ++k) {
      unsigned view = csk_->attributes()[c].inlier_views_[k];
      if (viewset_.find(view) != viewset_.end())
        inlier_views.push_back(view);
    }

    if (inlier_views.size() < 2) {
      i += csk_->curves_3d()[c].size();
      vcl_cout << vcl_endl;
      continue;
    }

    for (unsigned k=0; k < inlier_views.size(); ++k)
      vcl_cout << inlier_views[k] << " ";
    vcl_cout << vcl_endl;

    for (unsigned p=0; p < csk_->curves_3d()[c].size(); ++p, ++i) {
      for (unsigned k=0; k < inlier_views.size(); ++k) {
        vcl_vector<unsigned>::iterator 
          it = find(views_.begin(), views_.end(), inlier_views[k]);
        assert(it != views_.end());
        unsigned v_orig = it - views_.begin();
        mask[v_orig][i] = true;
      }
    }
//    vcl_cout << vcl_endl;
  }
//  for (unsigned i=0; i < maxviews_; ++i) {
//    vcl_cout << "view[" << i <<"]";
//    for (unsigned k=0; k < mask[k].size(); ++k) {
//      if (mask[i][k])
//        vcl_cout << k << " ";
//    }
//    vcl_cout << vcl_endl;
//  }
}

bool vcpld_bundle_adjust_driver::
get_views_to_optimize(
    const vcl_set<unsigned> &viewset,
    vcl_vector<unsigned> *views_ptr
    )
{
  vcl_vector<unsigned> &views = *views_ptr;
  views.reserve(dpath_.nviews());

  vcl_vector<vcl_set<unsigned> > is_participating(dpath_.nviews());

  // We're assuming the input views were already chained, meaning the reference
  // views are like (i0,i1), (i1, i2), (i2, i3),...
  for (unsigned c=0; c < csk_->num_curves(); ++c) {
    //
    //    Don't include source views automatically since the curves must be away
    //    from the boundary to be an inlier curve there.
    //
//    unsigned id_from = csk_->attributes()[c].v_->stereo0();
//    unsigned id_to = csk_->attributes()[c].v_->stereo1();
//    assert(id_from < dpath_.nviews());
//    assert(id_to < dpath_.nviews());

//    if (id_from < maxviews)
//      is_participating[id_from].insert(1);

//    if (id_to < maxviews)
//      is_participating[id_to].insert(1);

    for (unsigned i=0; i < csk_->attributes()[c].inlier_views_.size(); ++i) {
      unsigned iview = csk_->attributes()[c].inlier_views_[i];

      if (viewset.find(iview) != viewset.end())
        is_participating[iview].insert(1);
    }
  }

  // Now check for consistency
//  unsigned num_endpoint_views = 0;
  for (unsigned c=0; c < dpath_.nviews(); ++c) {
    if (is_participating[c].empty())
      continue;
    views.push_back(c);
//    if (is_participating[c].size() == 1) {
//      num_endpoint_views++;
//      if (num_endpoint_views > 2) {
//        vcl_cerr << "vcpld_bundle_adjust_driver: error: input views are not chained.\n";
//        return false;
//      }
//    }
  }

  vcl_cout << "vcpld_bundle_adjust_driver: views successfully chained\n";
  return true;
}

bool vcpld_bundle_adjust_driver::
init()
{
  //: For each view, setup the following processes:
  //
  // Sources:
  // + cam_src_[v] load initial camera
  // + edg_src_[v] load edgemaps
  //
  // Filters:
  // + edg_dt_[v]: compute dt and label map from subpixel edgemap
  //
  // + bundle_adjust_filter_
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

  vcl_cout << "Initializing bundle adjustment driver" << vcl_endl;

  // Determine what views to use. Go through curve sketch, and gather the views.
  bool ret = get_views_to_optimize(viewset_, &views_);
  if (!ret) {
    vcl_cerr << "Views to optimize could not be determined\n";
    return false;
  }

  nviews_ = views_.size();

  vcpld_bundle_adjust_sink *cb = new vcpld_bundle_adjust_sink(optimize_one_view_, curve_ransac_);
  curve_bundler_ = cb;
  get_curve_visibility_using_reference_views(&cb->mask_);

  vcl_cout << "Number of views being optimized: " << nviews() << vcl_endl;
  cam_src_.reserve(nviews());
  edg_src_.reserve(nviews());
  edg_dt_.reserve(nviews());

  // ------------- Setup the processing nodes -------------

  // Setup nodes that act on a single frame
  for (unsigned i=0; i < nviews(); ++i) {
    unsigned v = views_[i];
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

    // 1 dt and label loader
    p = new bprod_edg_dt;
    edg_dt_.push_back(p);
    p->connect_input(0, edg_src_[i], 0);
  }

  csk_->get_curves_as_vgl_points(&cb->pts3d_);

  vcl_cout << 
    "Optimizing " << cb->pts3d_.size() << " curves across " << nviews() << " views."
    << vcl_endl;

  cb->setup_inputs(
      cam_src_,
      edg_src_,
      edg_dt_
      );

  initialized_ = true;
  return true;
}

bool vcpld_bundle_adjust_driver::
run(unsigned long timestamp)
{
  vcl_cout << "Running stereo driver.\n";
  assert(initialized_);
  bprod_signal retval = curve_bundler_->run(timestamp);

  if (retval == BPROD_INVALID)
    return false;

  vcpld_bundle_adjust_sink *cb = 
    static_cast<vcpld_bundle_adjust_sink *>(curve_bundler_.ptr());

  optimized_cam_ = &cb->cam_;
  optimized_pts3d_ =  &cb->pts3d_;

  return true;
}
