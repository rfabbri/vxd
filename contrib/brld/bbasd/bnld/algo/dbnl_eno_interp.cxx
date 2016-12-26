#include "dbnl_eno.h"
#include <vcl_cstdio.h>
//:
// \file
// \author Ricardo Fabbri

//: interpolation code for non-integral coords
void dbnl_eno_interp ::
interpolate(double const data[DBNL_ENO_DATA_LENGTH], double const x[DBNL_ENO_DATA_LENGTH])
{
  double
    a2,      /* 2nd order coefficient of forward polynomial */
    b2,      /* 2nd order coefficient of backward polynomial */
    d21,d32,d31,
    det;

  /* compute leading coefficient of forward and backward polynomials */
  d21 = x[1]-x[2];
  d32 = x[2]-x[3];
  d31 = x[1]-x[3];

  det= d21*d31*d32;
  a2 = (d32*data[1] -d31*data[2] +d21*data[3]  )/det;

  d21 = x[0]-x[1];
  d32 = x[1]-x[2];
  d31 = x[0]-x[2];

  det= d21*d31*d32;
  b2 = (d32*data[0] -d31*data[1] +d21*data[2]  )/det;

  /* determine which direction to use for interpolation */
  forward_ = vcl_fabs(a2) < vcl_fabs(b2);
    
  /* choose polynomial with smaller variation, where variation is
     measured as absolute value of leading polynomial coefficient.*/
  a2 = forward_ ? a2:b2;
    
  /* compute and store all polynomial coefficients for this interpolant */
  coeffs_[dbnl_eno_poly::second_order_index] = a2;

  a2 = -a2;
  d21 = x[2]-x[1];
  coeffs_[dbnl_eno_poly::first_order_index] = 
     (data[2] - data[1])/d21 + a2*(x[1] + x[2]);

  coeffs_[dbnl_eno_poly::zero_order_index] =
     data[1] + x[1]*(a2*x[1] - coeffs_[dbnl_eno_poly::first_order_index]);

  order_=2;
}

//: fit poly to 3 pts
void dbnl_eno_interp::
interpolate(double d1, double d2, double d3, double x1, double x2, double x3)
{
  double
    a2,      /* 2nd order coefficient of forward polynomial */
    d21,d32,d31,
    det;

  forward_ = x3 > x1;

  // compute leading coefficient
  d21 = x1-x2;
  d32 = x2-x3;
  d31 = x1-x3;

  det= d21*d31*d32;
  a2 = ( d32*d1 -d31*d2 +d21*d3 )/det;

  // compute and store all polynomial coefficients for this interpolant
  coeffs_[dbnl_eno_poly::second_order_index] = a2;

  a2 = -a2;
  d21 = -d21;
  coeffs_[dbnl_eno_poly::first_order_index] = 
     (d2 - d1)/d21 + a2*(x1 + x2);

  coeffs_[dbnl_eno_poly::zero_order_index] =
     d1 + x1*(a2*x1 - coeffs_[dbnl_eno_poly::first_order_index]);

  order_=2;
}

//: specific interpolation code for integer coords
void dbnl_eno_interp ::
interpolate(double const data[DBNL_ENO_DATA_LENGTH])
{
  double
    a2,      /* 2nd order coefficient of forward polynomial */
    b2,      /* 2nd order coefficient of backward polynomial */
    c2;      /* 2nd order coefficient in choosen direction */
  int const
    off = 1;      /* offset in data array */

  /* compute leading coefficient of forward and backward polynomials */
  a2 = (data[off+2] - 2.0* data[off+1] + data[off+0])/2.0;
  b2 = (data[off+1] - 2.0* data[off+0] + data[off-1])/2.0;

  /* determine which direction to use for interpolation */
  forward_ = vcl_fabs(a2) < vcl_fabs(b2);
    
  /* choose polynomial with smaller variation, where variation is
     measured as absolute value of leading polynomial coefficient.*/
  c2 = forward_ ? a2:b2;
    
  /* compute and store all polynomial coefficients for this interpolant */
  coeffs_[dbnl_eno_poly::second_order_index] = c2;
  coeffs_[dbnl_eno_poly::first_order_index] = data[off+1] - (c2*(2*start_+1) + data[off+0]);
  coeffs_[dbnl_eno_poly::zero_order_index] =
    (data[off+0] + c2*start_*(start_+1)) - (start_*(data[off+1] - data[off+0]));

  order_=2;
}

void dbnl_eno_interp::
print(vcl_ostream &strm) const
{
   strm << "==== Interpolant ====" << vcl_endl
        << "forward: " << (forward_?"true":"false") << vcl_endl
        << "start,end:  " << start_ << " , " << end_ << vcl_endl;

   dbnl_eno_poly::print(strm); 
}
