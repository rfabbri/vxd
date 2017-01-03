// This is seg/dbsks/dbcsi_shotton_ocm.h
#ifndef dbcsi_shotton_ocm_h_
#define dbcsi_shotton_ocm_h_

//:
// \file
// \brief A class for oriented chamfer matching
//        
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date April 24, 2009
//
// \verbatim
//  Modifications
//       April 24, 2009 - Nhon Trinh - initial version, moved from dbsks
// \endverbatim


#include <vil/vil_image_view.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <vbl/vbl_array_2d.h>
#include <vbl/vbl_array_3d.h>
#include <vnl/vnl_vector.h>


// ============================================================================
class dbcsi_shotton_ocm
{
public:
  // Constructor / destructor -------------------------------------------------
  
  //: constructor
  dbcsi_shotton_ocm(){};

  //: destructor
  virtual ~dbcsi_shotton_ocm(){};

  // User-interface functions -------------------------------------------------

  //: Set the member variables
  void set_edgemap(const vil_image_view<float >& edgemap)
  { this->edgemap_ = edgemap; }

  //: Set related parameters to compute ocm cost
  // "nchannel" is the number of bins to conver [0, pi]
  void set_ocm_params(float edge_threshold, float distance_threshold,
    float tol_near_zero, int nchannel)
  {
    this->edge_threshold_ = edge_threshold;
    this->distance_threshold_ = distance_threshold;
    this->tol_near_zero_ = tol_near_zero;
    this->nchannel_ = nchannel;
  }

  //: Set lambda (\in [0, 1]) - linear combination weight
  void set_lambda(float lambda)
  {
    this->lambda_ = lambda;
  }

  //: Compute Chamfer Matching cost image from DT and its gradient
  void compute();

  // Utility functions --------------------------------------------------------

  //: Cost of a oriented point
  float f(double x, double y, double tx, double ty);

  //: cost of an oriented point, represented by its index
  float f(int i, int j, int orient_channel);

  //: cost of an oriented point, represented by its index
  float f(vcl_vector<int >& x, vcl_vector<int >& y, vcl_vector<int >& orient_channel);

protected:
  //: Compute distance transform of an edgemap
  void compute_dt(const vil_image_view<float >& edgemap,
                              float edge_threshold,
                              vil_image_view<float >& dt_edgemap);

  //: Compute gradient of an image using Gaussian kernel
  void compute_gradient(vil_image_view<float >& dt,
                         vil_image_view<float >& dt_grad_x,
                         vil_image_view<float >& dt_grad_y,
                         vil_image_view<float >& dt_grad_mag);

  //: Compute chamfer cost at every pixel on the image
  // Requirements: DT and gradients of DT have been computed
  void compute_chamfer_cost();

  //: Compute orientation cost at every pixel on the image
  // Requirements: DT and gradients of DT have been computed
  void compute_orientation_cost();

  // Member variables ---------------------------------------------------------
  vil_image_view<float > edgemap_;
  float edge_threshold_;
  float distance_threshold_;
  float tol_near_zero_; // due to discretization
  float lambda_; // parameter determing weights of chamfer cost and distance cost

  // Cached variables
  vil_image_view<float > dt_;
  vil_image_view<float > dt_grad_x_;
  vil_image_view<float > dt_grad_y_;
  vil_image_view<float > dt_grad_mag_;
  vbl_array_2d<float > chamfer_cost_;
  vbl_array_3d<float > orient_cost_;

  // sine and cosine of center angle for each channel
  vnl_vector<double > channel_cos_;
  vnl_vector<double > channel_sin_;
  vnl_vector<double > channel_angle_;
  double step_angle_;
  int nchannel_;
};


#endif // seg/dbsks/dbcsi_shotton_ocm.h


