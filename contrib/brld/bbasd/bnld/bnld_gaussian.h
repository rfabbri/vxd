// This is bbasd/bnld/bnld_gaussian.h

#ifndef bnld_gaussian_h_
#define bnld_gaussian_h_

//:
// \file
// \brief Gaussian functions
// 
// \author Based on original code by  Nhon Trinh ( @lems.brown.edu)
// \date Nov 4, 2007
//
// \verbatim
//  Modifications:
// \endverbatim

#include <vnl/vnl_math.h>

//: 1D Gaussian pdf function
inline double bnld_gaussian_pdf(double x, double mu, double sigma)
{
  return vcl_exp( -(x-mu)*(x-mu) / (2 * sigma*sigma)) * 
    vnl_math::two_over_sqrtpi * vnl_math::sqrt2 / (4*sigma) ;
}

#endif // bnld_gaussian_h_
