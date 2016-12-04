// This is bdifd_data.h
#ifndef bdifd_data_h
#define bdifd_data_h
//:
//\file
//\brief Functions to create multiview datasets 
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date Sat May 13 18:41:53 EDT 2006
//

#include <bdifd/bdifd_camera.h>
#include <vsol/vsol_line_2d_sptr.h>

class bdifd_rig;

//: Defines some multiview differential-geometric synthetic data and utilities
class bdifd_data {
  public:

  static void
  project_into_cams(
      const vcl_vector<bdifd_3rd_order_point_3d> &crv3d, 
      const vcl_vector<bdifd_camera> &cam,
      vcl_vector<vcl_vector<bdifd_3rd_order_point_2d> > &xi //:< image coordinates
      );

  static void 
  project_into_cams(
      const vcl_vector<bdifd_3rd_order_point_3d> &crv3d, 
      const vcl_vector<bdifd_camera> &cam,
      vcl_vector<vcl_vector<vsol_point_2d_sptr> > &xi //:< image coordinates
      );

  static void 
  project_into_cams_without_epitangency(
      const vcl_vector<vcl_vector<bdifd_3rd_order_point_3d> > &crv3d,
      const vcl_vector<bdifd_camera> &cam,
      vcl_vector<vcl_vector<bdifd_3rd_order_point_2d> > &crv2d_gt,
      double epipolar_angle_thresh);

  static void
  space_curves_ctspheres( vcl_vector<vcl_vector<bdifd_3rd_order_point_3d> > &crv3d );

  static void
  space_curves_olympus_turntable( vcl_vector<vcl_vector<bdifd_3rd_order_point_3d> > &crv3d);

  static void 
  space_curves_digicam_turntable_sandbox( vcl_vector<vcl_vector<bdifd_3rd_order_point_3d> > &crv3d);

  static void space_curves_digicam_turntable_medium_sized(
    vcl_vector<vcl_vector<bdifd_3rd_order_point_3d> > &crv3d
    );

  static void 
  space_curves_ctspheres_old( vcl_vector<vcl_vector<bdifd_3rd_order_point_3d> > &crv3d );

  static void 
  project_into_cams(
      const vcl_vector<vcl_vector<bdifd_3rd_order_point_3d> > &crv3d,
      const vcl_vector<bdifd_camera> &cam,
      vcl_vector<vcl_vector<bdifd_3rd_order_point_2d> > &crv2d_gt);

  static void 
  max_err_reproj_perturb(
      const vcl_vector<vcl_vector<bdifd_3rd_order_point_2d> > &crv2d_gt_,
      const vcl_vector<bdifd_camera> &cam_,
      const bdifd_rig &rig,
      double &err_pos,
      double &err_t,
      double &err_k,
      double &err_kdot,
      unsigned &i_pos, 
      unsigned &i_t, 
      unsigned &i_k, 
      unsigned &i_kdot,
      unsigned &nvalid
      );

  static void 
  err_reproj_perturb(
      const vcl_vector<vcl_vector<bdifd_3rd_order_point_2d> > &crv2d_gt_,
      const vcl_vector<bdifd_camera> &cam_,
      const bdifd_rig &rig,
      vcl_vector<double> &err_pos_sq,
      vcl_vector<double> &err_t,
      vcl_vector<double> &err_k,
      vcl_vector<double> &err_kdot,
      vcl_vector<unsigned> &valid_idx
      );

  //---------------------------------------------------------------------------
  static void get_lines(
      vcl_vector<vsol_line_2d_sptr> &lines,
      const vcl_vector<bdifd_3rd_order_point_2d> &C_subpixel,
      bool do_perturb=false,
      double pert_pos=0.0,
      double pert_tan=0.0
      );

  static void 
  get_circle_edgels(
      double radius, 
      vcl_vector<vsol_line_2d_sptr> &lines,
      vcl_vector<bdifd_3rd_order_point_2d> &C_subpixel,
      bool do_perturb=false,
      double pert_pos=0.1,
      double pert_tan=10
      );

  static void get_ellipse_edgels(
      double ra, 
      double rb, 
      vcl_vector<vsol_line_2d_sptr> &lines,
      vcl_vector<bdifd_3rd_order_point_2d> &C_subpixel,
      bool do_perturb,
      double pert_pos,
      double pert_tan
      );

  static vgl_point_3d<double> 
  get_point_crv3d(const vcl_vector<vcl_vector<bdifd_3rd_order_point_3d> > &crv3d, unsigned i);

  //---------------------------------------------------------------------------
  // Utilities to output traditional point dataset (no differential geometry)

  static void 
  get_digital_camera_point_dataset(
      vcl_vector<vpgl_perspective_camera<double> > *pcams, 
      vcl_vector<vcl_vector<vgl_point_2d<double> > > *pimage_pts, 
      vcl_vector<vgl_point_3d<double> > *pworld_pts, 
      const vcl_vector<double> &view_angles);
};

//: Class dealing with a turntable camera configuration.
class bdifd_turntable {
public:
  //- function to set the turntable params

  //- create_camera(frame_idx, internal matrix)

  //- function to return/set params for ctspheres
  //- function to run/set params for olympus's turntable

  static vpgl_perspective_camera<double> *
  camera_ctspheres(
      unsigned frm_index,
      const vpgl_calibration_matrix<double> &K);

  static void 
  internal_calib_ctspheres(vnl_double_3x3 &m, double x_max_scaled=4000.0);

  static void 
  internal_calib_olympus(vnl_double_3x3 &m, double x_max_scaled=0, unsigned  crop_x=0, unsigned  crop_y=0);

  static vpgl_perspective_camera<double> * 
  camera_olympus(
      double theta,
      const vpgl_calibration_matrix<double> &K);


};

#endif // bdifd_data_h

