// This is file seg/dbsks/dbcsi_shotton_ocm.cxx

//:
// \file

#include "dbcsi_shotton_ocm.h"


#include <dbdet/algo/dbdet_subpix_convolution.h>
#include <dbdet/edge/dbdet_gaussian_kernel.h>
#include <bil/algo/bil_edt.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_numeric_traits.h>


// ============================================================================
//  dbcsi_shotton_ocm
// ============================================================================


// ------------------------------------------------------------------------------
//: Compute chamfer cost for the whole image
void dbcsi_shotton_ocm::
compute()
{
  //: Compute distance transform of the edge map 
  this->compute_dt(this->edgemap_, this->edge_threshold_, this->dt_);
  
  // Compute gradient maps of the DT
  this->compute_gradient(this->dt_, 
    this->dt_grad_x_, this->dt_grad_y_, this->dt_grad_mag_);

  // Compute chamfer cost
  this->compute_chamfer_cost();

  // Compute orientation cost
  this->compute_orientation_cost();
}



// ------------------------------------------------------------------------------
//: Cost of a oriented point
float dbcsi_shotton_ocm::
f(double x, double y, double tx, double ty)
{
  int im_i = vnl_math::rnd(x);
  int im_j = vnl_math::rnd(y);

  // range check
  if (im_i < 0 || (im_j+1) >= int(this->edgemap_.ni()) || 
    im_j < 0 || (im_j+1) >= int(this->edgemap_.nj()) )
  {
    return 1;
  }

  // figure out the channel corresponding the the contour tangent (tx, ty)
  double max_dot = -1;
  unsigned max_idx = 0;
  for (unsigned i =0; i < this->channel_sin_.size(); ++i)
  {
    double dot = vnl_math::abs(tx*this->channel_cos_[i] + ty*this->channel_sin_[i]);
    if (dot > max_dot)
    {
      max_dot = dot;
      max_idx = i;
    }
  }

  // final cost is a linear combination of two cost component: chamfer and orientation
  return ((1-this->lambda_) * this->chamfer_cost_(im_i, im_j) + 
    this->lambda_ * this->orient_cost_(im_i, im_j, max_idx));
}




// -----------------------------------------------------------------------------
//: cost of an oriented point, represented by its index
float dbcsi_shotton_ocm::
f(int i, int j, int orient_channel)
{
  if (i < 0 || (i+1) >= int(this->edgemap_.ni()) || 
    j <0 || (j+1) >= int(this->edgemap_.nj()) )
  {
    return 1.0f;
  }

  assert(orient_channel >= 0);

  // since the pre-computed cost grid only cover half of circle [0, pi]
  // if the given angle (index) is larger than pi than need to modify
  if (orient_channel >= (int)this->channel_angle_.size())
    orient_channel -= this->channel_angle_.size();

  // final cost is a linear combination of chamfer cost and orientation cost
  return ((1-this->lambda_)*this->chamfer_cost_(i, j) + 
    this->lambda_*this->orient_cost_(i, j, orient_channel));
}



// -----------------------------------------------------------------------------
//: cost of an oriented point, represented by its index
float dbcsi_shotton_ocm::
f(vcl_vector<int >& x, vcl_vector<int >& y, vcl_vector<int >& orient_channel)
{
  if (x.empty())
    return 1.0f;

  float sum_cost = 0;
  unsigned npts = x.size();
  for (unsigned k =0; k != npts; ++k)
  {
    sum_cost += this->f(x[k], y[k], orient_channel[k]);
  }

  return sum_cost / npts;
}


// ----------------------------------------------------------------------------
//: Compute distance transform of an edgemap
void dbcsi_shotton_ocm::
compute_dt(const vil_image_view<float >& edgemap, float edge_threshold,
           vil_image_view<float >& dt_edgemap)
{
  // threshold the edge map to generate a binary image
  vil_image_view<unsigned int > image_unsigned(edgemap.ni(), edgemap.nj());
  for (unsigned int i=0; i<edgemap.ni(); ++i)
  {
    for (unsigned int j=0; j<edgemap.nj(); ++j)
    {
      image_unsigned(i, j) = (edgemap(i, j) < edge_threshold) ? 0 : 255;
    }
  }

  // Compute distance transform of the edgemap
  bil_edt_signed(image_unsigned, dt_edgemap);
  return;
}



// ----------------------------------------------------------------------------
//: Compute gradient of an image using Gaussian kernel
void dbcsi_shotton_ocm::
compute_gradient(vil_image_view<float >& dt,
                 vil_image_view<float >& dt_grad_x,
                 vil_image_view<float >& dt_grad_y,
                 vil_image_view<float >& dt_grad_mag)
{
  // Compute gradient of DT
  float sigma = 1.0f;
  int N = 0;
  dbdet_subpix_convolve_2d_sep(dt, dt_grad_x, 
    dbdet_Gx_kernel(sigma), float(), N);
  dbdet_subpix_convolve_2d_sep(dt, dt_grad_y, 
    dbdet_Gy_kernel(sigma), float(), N);

  //compute gradient magnitude
  dt_grad_mag.set_size(dt_grad_x.ni(), dt_grad_x.nj());

  //get the pointers to the memory chunks
  float *gx  =  dt_grad_x.top_left_ptr();
  float *gy  =  dt_grad_y.top_left_ptr();
  float *g_mag  =  dt_grad_mag.top_left_ptr();

  //compute the gradient magnitude
  for(unsigned long i=0; i<dt_grad_mag.size(); i++)
  {
    g_mag[i] = vcl_sqrt(gx[i]*gx[i] + gy[i]*gy[i]);
  }
  return;
}


// -----------------------------------------------------------------------------
//: Compute chamfer cost at every pixel on the image
// Requirements: DT and gradients of DT have been computed
void dbcsi_shotton_ocm::
compute_chamfer_cost()
{
  unsigned ni = this->edgemap_.ni();
  unsigned nj = this->edgemap_.nj();
  assert(ni > 0 && nj > 0);

  // Allocate memory
  this->chamfer_cost_.resize(ni, nj);

  // Compute chamfer cost for each pixel
  for (unsigned i =0; i < ni; ++i)
  {
    for (unsigned j =0; j <nj; ++j)
    {
      float distance = vnl_math::abs(this->dt_(i, j));

      // any pixel within 'tolerance' from an edge points is on an edge point
      if (distance < this->tol_near_zero_)
      {
        this->chamfer_cost_(i, j) = 0;
      }
      else
      {        
        float cost = (distance- this->tol_near_zero_) / this->distance_threshold_;
        this->chamfer_cost_(i, j) = vnl_math::min(cost, 1.0f);
      }
    }
  }
  return;
}



// -----------------------------------------------------------------------------
//: Compute orientation cost at every pixel on the image
// Requirements: DT and gradients of DT have been computed
// "num_angle_channels" is the number of bins to conver [0, 2pi]
// Requirements: num_angle_channels should be an even number
void dbcsi_shotton_ocm::
compute_orientation_cost()
{
  // allocate memory
  unsigned ni = this->edgemap_.ni();
  unsigned nj = this->edgemap_.nj();
  unsigned nchannel = this->nchannel_; //18; // covering [0, pi]
  double radians_per_channel = vnl_math::pi / nchannel;

  // precompute the sine and cosine of center angle for each channel
  this->channel_cos_.set_size(nchannel);
  this->channel_sin_.set_size(nchannel);
  this->channel_angle_.set_size(nchannel);
  for (unsigned i =0; i < nchannel; ++i)
  {
    this->channel_cos_[i] = vcl_cos(i*radians_per_channel);
    this->channel_sin_[i] = vcl_sin(i*radians_per_channel);
    this->channel_angle_[i] = i*radians_per_channel;
  }
  this->step_angle_ = radians_per_channel;


  // allocate memory for the orientation cost grid
  this->orient_cost_.resize(ni, nj, nchannel);

  // compute cost for each point in the grid
  for (unsigned i =0; i < ni; ++i)
  {
    for (unsigned j =0; j < nj; ++j)
    {
      // if the point is too far, we disregard the orientation
      if (vnl_math::abs(this->dt_(i, j)) > this->distance_threshold_)
      {
        for (unsigned channel =0; channel < nchannel; ++channel)
        {
          this->orient_cost_(i, j, channel) = 1;
        }
      }
      else
      {
        // tangent on the edges == direction of iso-contour on distance map
        float grad_x = this->dt_grad_x_(i, j);
        float grad_y = this->dt_grad_y_(i, j);
        
        // edge direction is orthogonal to gradient direction
        double edge_dir = vcl_atan2(grad_x, -grad_y);

        for (unsigned channel =0; channel < nchannel; ++channel)
        {
          double angle = channel * radians_per_channel;

          // modulo the angle difference to [0, pi]
          double angle_diff = vcl_fmod(angle - edge_dir, vnl_math::pi);
          if (angle_diff < 0)
            angle_diff += vnl_math::pi;

          // now convert this angle difference to [0, pi/2]
          angle_diff = vnl_math::min(angle_diff, vnl_math::pi - angle_diff);

          // normalize this cost to [0, 1]
          this->orient_cost_(i, j, channel) = float(angle_diff/vnl_math::pi_over_2);
        } // for
      } // else
    }  // j
  } // i
  return;
}








