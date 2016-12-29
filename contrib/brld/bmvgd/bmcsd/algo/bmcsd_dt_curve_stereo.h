// This is bmcsd_dt_curve_stereo.h
#ifndef bmcsd_dt_curve_stereo_h
#define bmcsd_dt_curve_stereo_h
//:
//\file
//\brief Multiview curve stereo using distance transforms
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 06/22/2009 05:30:17 PM EDT
//

#include <mw/algo/bmcsd_curve_stereo.h>

//: Matches curve fragments using costs based on the distance transform and
// closest feature map (label map) of the edge map at each image. This class
// assumes the user provides precomputed distance transform maps and label maps
// for each image. The reason is that we want to run this class repeatedly for
// different pairs of views without having to recompute the DT.
class bmcsd_dt_curve_stereo : public bmcsd_curve_stereo {
public:
  bmcsd_dt_curve_stereo();
  virtual ~bmcsd_dt_curve_stereo() {}

  // Setters and Getters ------------------------------------------------------

  //: \see base class
  virtual bool set_nviews(unsigned nviews);

  //: set the distance transforms and labels for the edgemaps of each views
  void set_all_dt_label(
      const vcl_vector<vil_image_view<vxl_uint_32> > &dt,  
      const vcl_vector<vil_image_view<unsigned> > &label);

  //: sets the threshold for wich an edgel is considered an inlier to the
  // reprojected curve in each view.
  void set_tau_distance_squared(double tau) { tau_distance_squared_ = tau; }
  double tau_distance_squared() const { return tau_distance_squared_; }

  // Stereo Matching Methods ---------------------------------------------------

  //: Matches the selected subcurve to another subcurve in its epipolar beam such
  // that the overlaps reproject with the maximum number of inliers. The inliers
  // are measured from the distance map. 
  //
  // \param[out] i_best : index into \c crv_candidates_ptrs() of top matching curve
  // \param[out] votes : vcl_vector of the number of inliers, s.t. votes[i] ==
  // total number of inliers for crv_candidates_ptrs(i).
  bool match_using_dt(unsigned *i_best, vcl_vector<unsigned long> *votes);
  //: Convenient overload for match_using_dt
  bool match_using_dt(unsigned *i_best);

protected:
  bool has_dt_label() { return !dt_.empty() && !label_.empty(); }

  //: \return true in case edgemaps, labels, and a curve is selected in view 0,
  // as well as if other preconditions for matching are satisfied.
  bool ready_for_matching();

  // Accessors for children ---------------------------------------------------
  const vil_image_view<vxl_uint_32> &dt(unsigned v) const {return dt_[v]; }
  const vil_image_view<unsigned> &label(unsigned v) const {return label_[v]; }

private:
  //: distance transform images for each view
  vcl_vector<vil_image_view<vxl_uint_32> > dt_;
  //: closest label transform images for each view
  vcl_vector<vil_image_view<unsigned> > label_;
  double tau_distance_squared_;
};

#endif // bmcsd_dt_curve_stereo_h
