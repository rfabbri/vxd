// This is dvpgl_camera_compute_ransac.h
#ifndef dvpgl_camera_compute_ransac_h
#define dvpgl_camera_compute_ransac_h
//:
//\file
//\brief Compute camera matrix from 3D-2D point correspondences and RANSAC
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 12/06
//

#include <rrel/rrel_estimation_problem.h>
#include <vpgl/vpgl_proj_camera.h>
#include <vcl_vector.h>
#include <vgl/vgl_fwd.h>
#include <vnl/vnl_fwd.h>


//: Main class for computing the camera matrix from lists of 3D-2D points.
//
// \remarks
// Maybe should be called compute_robust
// Maybe homog point internal representation would be better suited for purely projective problems
//
// \todo 
// Include methods for setting ransac parameters
class dvpgl_camera_compute_ransac
{
 public:
  dvpgl_camera_compute_ransac() {}

  //: Compute from two sets of corresponding points.
  // Put the resulting camera into camera, return true if successful.
  bool compute( const vcl_vector< vgl_homg_point_2d<double> >& image_pts,
                const vcl_vector< vgl_homg_point_3d<double> >& world_pts,
                vpgl_proj_camera<double>& camera );

  //: After "compute" this will have true in the indices determined to be outliers.
  vcl_vector<bool> outliers;
};


//: Helper class for dvpgl_camera_compute_ransac using rrel.
class rrel_proj_problem : public rrel_estimation_problem
{
 public:
  //: Construct the problem object with corresponding points.
  rrel_proj_problem( const vcl_vector< vgl_homg_point_2d<double> >& image_pts,
                const vcl_vector< vgl_homg_point_3d<double> >& world_pts);

  virtual ~rrel_proj_problem(){};

  //: Total number of correspondences.
  unsigned int num_samples() const{ return image_pts_.size(); }

  //: The degrees of freedom in the residual.
  // Each coordinate of the correspondence pair has Gaussian error, so
  // the Euclidean distance residual has 5 degrees of freedom.
  unsigned int residual_dof() const { return 5; }

  //: Generate a parameter estimate from a minimal sample.
  bool fit_from_minimal_set( const vcl_vector<int>& point_indices,
                             vnl_vector<double>& params ) const;

  //: Compute unsigned fit residuals relative to the parameter estimate.
  void compute_residuals( const vnl_vector<double>& params,
                          vcl_vector<double>& residuals ) const;

  //: Convert a projective matrix into a parameter vector.
  virtual void  proj_to_params( const vpgl_proj_matrix<double>&  Pr,
                              vnl_vector<double>& p) const;

  //: Convert a parameter vector into a projective matrix
  virtual void  params_to_proj( const vnl_vector<double>& p,
                              vpgl_proj_matrix<double>& Pr) const;

  //: Weighted least squares parameter estimate.
  //  The normalized covariance is not yet filled in.
  bool weighted_least_squares_fit( vnl_vector<double>& params,
    vnl_matrix<double>& norm_covar, const vcl_vector<double>* weights=0 ) const;

  // Toggles detailed printing of computations.
  bool verbose;

 protected:
  const vcl_vector< vgl_homg_point_2d<double> > *image_pts_;
  const vcl_vector< vgl_homg_point_2d<double> > *world_pts_;
};

#endif // dvpgl_camera_compute_ransac_h

