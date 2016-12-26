// This is basic/dbnl/dbnl_gaussian.h

#ifndef dbnl_gaussian_h_
#define dbnl_gaussian_h_

//:
// \file
// \brief Gaussian functions
// 
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date Nov 4, 2007
//
// \verbatim
//  Modifications:
// \endverbatim

#include <vnl/vnl_math.h>

//: 1D Gaussian pdf function
inline double dbnl_gaussian_pdf(double x, double mu, double sigma)
{
  return vcl_exp( -(x-mu)*(x-mu) / (2 * sigma*sigma)) * 
    vnl_math::two_over_sqrtpi * vnl_math::sqrt2 / (4*sigma) ;
}

#endif // dbnl_gaussian_h_
