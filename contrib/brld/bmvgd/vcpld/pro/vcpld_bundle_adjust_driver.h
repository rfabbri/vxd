// This is vcpld_bundle_adjust_driver.h
#ifndef vcpld_bundle_adjust_driver_h
#define vcpld_bundle_adjust_driver_h
//:
//\file
//\brief Class to run vcpld_*bundle_adjust using a 3D curve sketch
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date Fri Nov 13 18:03:26 EST 2009
//

#include <mw/pro/vcpld_bundle_adjust_sink.h>
#include <bmcsd/bmcsd_curve_3d_sketch.h>
#include <mw/algo/vcpld_distmap_bundle_adjust.h>
#include <mw/algo/bmcsd_data.h>

class vcpld_bundle_adjust_driver {
public: 

  // The caller is responsible for managing csk memory, deleting csk only after
  // this class has been used.
  // \param[in] optimize_one_view: optimizes every view independently, without
  // optimizing the 3D structure.
  vcpld_bundle_adjust_driver(
      const bmcsd_curve_stereo_data_path &dpath, 
      bmcsd_curve_3d_sketch *csk,
      const vcl_set<unsigned> &viewset,
      bool optimize_one_view,
      bool curve_ransac
      )
    :
      dpath_(dpath),
      csk_(csk), 
      viewset_(viewset),
      optimize_one_view_(optimize_one_view),
      curve_ransac_(curve_ransac),
      initialized_(false)
  { }

  ~vcpld_bundle_adjust_driver() {}

  //: Initializes the processing, e.g. setting up a processing graph, computing
  // loading frame data, etc.
  bool init();

  //: Runs all instances of the 2-view matching and reconstruction
  bool run(unsigned long timestamp=1);

  const vcl_vector< vpgl_perspective_camera<double> > &optimized_cameras()
  { return *optimized_cam_; }

  unsigned nviews() const { return nviews_; }

  const vcl_vector<unsigned> &views() const { return views_; }

protected:
  //: Inputs
  bmcsd_curve_stereo_data_path dpath_;
  const bmcsd_curve_3d_sketch *csk_;

  //: Outputs
  vcl_vector< vpgl_perspective_camera<double> > *optimized_cam_;
  typedef vcl_vector<vgl_point_3d<double> > curve_points;
  vcl_vector< curve_points > *optimized_pts3d_;

  unsigned nviews_;
  //: the views that were selected to be optimized
  vcl_vector<unsigned> views_;

  //: the superset of views the user requested to be optimized
  vcl_set<unsigned> viewset_;

  //: true if each view's 6DOF is to be optimized independently
  bool optimize_one_view_;

  //: if optimize_one_view_ is true, this does random subset selection of curves
  // during optimization.
  bool curve_ransac_;

  //: true if init has been called
  bool initialized_;
  //: pool of processors.
  vcl_vector<bprod_process_sptr> cam_src_;
  vcl_vector<bprod_process_sptr> edg_src_;
  vcl_vector<bprod_process_sptr> edg_dt_;
  bprod_process_sptr curve_bundler_;

  bool get_views_to_optimize(const vcl_set<unsigned> &viewset, 
      vcl_vector<unsigned> *views_ptr);
  void get_curve_visibility_using_reference_views(
      vcl_vector<vcl_vector<bool> > *mask_ptr );
};

#endif // vcpld_bundle_adjust_driver_h
