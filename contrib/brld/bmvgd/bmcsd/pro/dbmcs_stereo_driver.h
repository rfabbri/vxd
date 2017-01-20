// This is bmcsd_stereo_driver.h
#ifndef bmcsd_stereo_driver_h
#define bmcsd_stereo_driver_h
//:
//\file
//\brief Class to run mw_*_curve_stereo in a multiview sequence
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri@gmail.com)
//\date 08/12/2009 02:19:28 PM PDT
//

#include <bmcsd/bmcsd_view_set.h>
#include <bmcsd/bmcsd_curve_3d_attributes.h>
#include <bmcsd/bmcsd_curve_3d_sketch.h>
#include <bmcsd/algo/bmcsd_odt_curve_stereo.h>
#include <mw/algo/mw_data.h>
#include <mw/pro/bmcsd_stereo_filter.h>

//: Performs multiview stereo on a video sequence, by running many instances of
// two-view stereo in subsets of the frames.
class bmcsd_stereo_driver {
public: 

  //: Specifies the input data by its path \p dpath, and the frames to be used
  // in each instance of 2-view stereo in \p frames_to_match.
  //
  // The number of confirmation views is specified in \p frames_to_match.
  bmcsd_stereo_driver(
      const bmcsd_curve_stereo_data_path &dpath, 
      const bmcsd_stereo_instance_views &frames_to_match)
    :
      dpath_(dpath),
      f_(frames_to_match),
      nviews_(dpath.nviews()),
      tau_distance_(10.0),
      tau_dtheta_(10.0), //< degrees
      tau_min_inliers_per_view_(5),
      tau_min_total_inliers_(20),
      tau_min_samples_per_curve_frag_(30),
      tau_min_length_per_curve_frag_(40.0),
      prune_by_length_(true),
      tau_min_epiangle_(30.0),
      tau_min_first_to_second_best_ratio_(1.5),
      tau_lonely_(tau_min_total_inliers_),
      tau_min_epipolar_overlap_(5),
      tau_min_num_inlier_edgels_per_curvelet_(3),
      use_curvelets_(false),
      initialized_(false)
  {
    assert(!f_.empty());
  }

  virtual ~bmcsd_stereo_driver() {};

  //: Might be bogus - this assumes all instances have the same number of
  // confirmation views. Not true.
  unsigned num_confirmation_views() const {
    assert (f_.num_instances()); 
    return f_.instance(0)->num_confirmation_views(); 
  }

  //: sets the threshold in angular difference for which an edgel is considered
  // an inlier to the reprojected curve point-tangent sample in each view.
  void set_dtheta_threshold(double dtheta) { tau_dtheta_ = dtheta; }

  //: Sets the minimum number of inliers per view. If a view has less than this
  // number of inliers, it will not vote at all towards the curve. This is to
  // increase robustness to occlusion and is used in the *_extras verion of the
  // stereo matcher.
  void set_min_inliers_per_view(unsigned n) { tau_min_inliers_per_view_ = n; }

  void set_min_total_inliers(unsigned n) { tau_min_total_inliers_ = n; }

  //: sets the threshold for which an edgel is considered an inlier to the
  // reprojected curve in each view.
  void set_distance_threshold(double tau) { tau_distance_ = tau; }

  //: Used to prune the curves by enforcing a minimum number of samples.
  void set_min_samples_per_curve_frag(unsigned m) 
    { tau_min_samples_per_curve_frag_ = m; prune_by_length_ = false; }

  //: Used to prune the curves by enforcing a minimum number of samples.
  void set_min_length_per_curve_frag(double m) 
    { tau_min_length_per_curve_frag_ = m; prune_by_length_ = true; }

  //: Angle in degrees used to remove epipolar tangencies
  void set_min_epiangle(double theta) 
    { tau_min_epiangle_ = theta; }

  //: see bmcsd_curve_stereo.h
  void set_min_first_to_second_best_ratio(double ratio) 
  { tau_min_first_to_second_best_ratio_ = ratio; }

  //: see bmcsd_curve_stereo.h
  void set_lonely_threshold(unsigned threshold) { tau_lonely_ = threshold; }

  void set_use_curvelets(unsigned use_curvelets) { use_curvelets_ = use_curvelets; }

  void set_min_num_inlier_edgels_per_curvelet(unsigned m)
  { tau_min_num_inlier_edgels_per_curvelet_ = m; }

  //: see bmcsd_curve_stereo.h
  void set_min_epipolar_overlap(unsigned m) 
    { tau_min_epipolar_overlap_ = m; }

  //: Initializes the processing, e.g. setting up a processing graph, computing
  // tangents in the curve fragments, etc.
  virtual bool init() = 0;

  //: Runs all instances of the 2-view matching and reconstruction
  virtual bool run(unsigned long timestamp=1) = 0;

  //: The resulting 3D reconstruction after run();
  void get_curve_sketch(bmcsd_curve_3d_sketch *csk) const
    { csk->set(*crv3d_, *attr_); }

  //: The correspondence structures between the first two views, for each
  // instance (not necessarily in order of instances when run in parallel, but
  // you can recover the instance from the bmcsd_curve_3d_attributes).
  const bmcsd_discrete_corresp &corresp(unsigned i) const
    { return (*corresp_)[i]; }

  unsigned num_corresp() const
    { return corresp_->size(); }

  //: The number of computed results.
  unsigned num_outputted_data() const { 
      assert (corresp_->size() == crv3d_->size()); 
      assert (attr_->size() == corresp_->size()); 
      return attr_->size();
    }

  unsigned nviews() const { return nviews_; }

protected:
  //: Inputs
  bmcsd_curve_stereo_data_path dpath_;
  bmcsd_stereo_instance_views f_; 
  unsigned nviews_;

  //: Outputs
  bmcsd_curve_3d_sketch csk_;
  vcl_vector< bdifd_1st_order_curve_3d > *crv3d_;
  vcl_vector< bmcsd_curve_3d_attributes > *attr_;

  bprod_process_sptr output_job_;
  vcl_vector<bmcsd_discrete_corresp> *corresp_;

  //: Parameters
  // TODO: include other parameters such as:
  //  - tau_min_curve_points_in_epipband_
  double tau_distance_;
  double tau_dtheta_;
  unsigned tau_min_inliers_per_view_;
  unsigned tau_min_total_inliers_;
  unsigned tau_min_samples_per_curve_frag_;
  double tau_min_length_per_curve_frag_;
  bool prune_by_length_;
  double tau_min_epiangle_;
  double tau_min_first_to_second_best_ratio_;
  unsigned tau_lonely_;
  unsigned tau_min_epipolar_overlap_;
  unsigned tau_min_num_inlier_edgels_per_curvelet_;
  bool use_curvelets_;

  //: true if init has been called
  bool initialized_;
};

class bmcsd_concurrent_stereo_driver : public bmcsd_stereo_driver {
public:
  bmcsd_concurrent_stereo_driver(
      const bmcsd_curve_stereo_data_path &dpath, 
      const bmcsd_stereo_instance_views &frames_to_match)
    :
      bmcsd_stereo_driver(dpath, frames_to_match),
      max_concurrent_matchers_(2)
  {
  }

  //: The maximum numner of matchers to run simultaneously. Roughly equals the
  // number of cores in a computer.
  void set_max_concurrent_matchers(unsigned n) { max_concurrent_matchers_ = n; }

  //: Initializes the processing, e.g. setting up a processing graph, computing
  // tangents in the curve fragments, etc.
  virtual bool init();

  //: Runs all instances of the 2-view matching and reconstruction
  virtual bool run(unsigned long timestamp=1);

  //: Updates the basic stereo processors with newly set parameters.
  // This must be called every time new parameters are set.
  void update_stereo_params();

private:
  unsigned max_concurrent_matchers_;

  //: pool of processors.
  vcl_vector<bprod_process_sptr> cam_src_;
  vcl_vector<bprod_process_sptr> edg_src_;
  vcl_vector<bprod_process_sptr> edg_dt_;
  vcl_vector<bprod_process_sptr> frag_src_;
  vcl_vector<bprod_process_sptr> cvlet_src_;
  vcl_vector<bprod_process_sptr> frag_tangents_;
  vcl_vector<bprod_process_sptr> curve_stereo_;
  vcl_vector<bprod_process_sptr> curve_stereo_jobs_;
  bprod_process_sptr output_job_;
};

#endif // bmcsd_stereo_driver_h
