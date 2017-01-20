// This is vcpld_bundle_adjust_sink.h
#ifndef vcpld_bundle_adjust_sink_h
#define vcpld_bundle_adjust_sink_h
//:
//\file
//\brief Dbpro process for curve bundle adjustment
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//
#include <bprod/bprod_process.h>
#include <vcpld/algo/vcpld_distmap_bundle_adjust.h>
#include <vcpld/algo/vcpld_distmap_optimize_camera.h>
#include <bmcsd/pro/bmcsd_load_camera_source.h>
#include <bmcsd/pro/bmcsd_load_edg_source.h>

class vcpld_bundle_adjust_sink : public bprod_sink {
public:
  typedef vcl_vector<vgl_point_3d<double> > single_3d_curve;

  typedef enum { CAM_ID, EDG_ID, DT_ID, LBL_ID } view_input_id;

  //\param[in] optimize_one_view: do optimization of each view's 6DOF
  //separately, instead of a full bundle adjustment.
  vcpld_bundle_adjust_sink  (bool optimize_one_view,
                             bool curve_ransac) 
    :
    optimize_one_view_(optimize_one_view),
    curve_ransac_(curve_ransac)
  {}

  bprod_signal execute() 
  {
    get_cameras();
    get_edgemaps();
    get_dt_label();

    if (!optimize_one_view_) {
      // Full bundle adjustment

      bool retval  = b_.optimize(
          cam_,
          pts3d_,
          dt_,
          label_,
          em_,
          mask_);

      if (!retval) {
        vcl_cerr << "Problem calling optimize from bundle adj sink\n";
        return BPROD_INVALID;
      }
    } else {
      // Optimize each view at a time.

      for (unsigned v=0; v < cam_.size(); ++v) {

        //: mask out points not to be used.

        vcl_vector< single_3d_curve > pts3d_v;
        pts3d_v.reserve(pts3d_.size());

        unsigned i=0;
        for (unsigned c=0; c < pts3d_.size(); ++c) {
          if (!pts3d_[c].empty() && mask_[v][i] == true) {
            pts3d_v.push_back(pts3d_[c]);
          }
          i += pts3d_[c].size();
        }

        vpgl_perspective_camera<double> opt;
        if (curve_ransac_) {
          opt = vcpld_distmap_optimize_camera::opt_orient_pos_subset(
              cam_[v],
              pts3d_v,
              dt_[v],
              label_[v],
              em_[v]);
        } else {
          opt = vcpld_distmap_optimize_camera::opt_orient_pos(
              cam_[v],
              pts3d_v,
              dt_[v],
              label_[v],
              em_[v]);
        }

        cam_[v] = opt;
      }
    }

    return BPROD_VALID;
  }

  void setup_inputs(
        vcl_vector<bprod_process_sptr> &cam_src, 
        vcl_vector<bprod_process_sptr> &edg_src, 
        vcl_vector<bprod_process_sptr> &edg_dt);

  //: Driver sets mask_ and pts3d_ these before calling execute()
  vcl_vector<vcl_vector<bool> > mask_;

  // Driver reads these outputs:
  vcl_vector<vpgl_perspective_camera<double> > cam_;
  vcl_vector< single_3d_curve > pts3d_;

private:
  unsigned nviews() const { return nviews_; }
  unsigned nviews_;
  unsigned inputs_per_view_;
  bool optimize_one_view_;
  bool curve_ransac_;

  vcl_vector<sdet_edgemap_sptr> em_;
  typedef vil_image_view<vxl_uint_32> dt_t;
  typedef vil_image_view<unsigned> label_t;
  vcl_vector<vil_image_view<vxl_uint_32> > dt_;
  vcl_vector<vil_image_view<unsigned> > label_;
  vcpld_distmap_bundle_adjust b_;

  void get_cameras();
  void get_edgemaps();
  void get_dt_label();
};


#endif // vcpld_bundle_adjust_sink_h
