// This is basic/dbnl/dbnl_sinc.h

#ifndef dbnl_sinc_h_
#define dbnl_sinc_h_

//:
// \file
// \brief Sinc function
// 
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date 08/17/2005
//
// \verbatim
//  Modifications:
// \endverbatim

#include <vcl_cmath.h>
#include <vcl_limits.h>
#include <vnl/vnl_math.h>

//: Sinc function, defined as
// sinc(x) = sin(x)/x when x != 0 and
//         = 0        when x  = 0
// Reference: http://www.boost.org/boost/math/special_functions/sinc.hpp
inline double dbnl_sinc(double x)
{
  static double const taylor_0_bound = vcl_numeric_limits<double >::epsilon();
  static double const taylor_2_bound = vcl_sqrt(taylor_0_bound);
  static double const taylor_n_bound = vcl_sqrt(taylor_2_bound);

  if (vnl_math::abs(x) >= taylor_n_bound)
  {
    return vcl_sin(x)/x;
  }
  else
  {
    // approximate by taylor series in x at 0 up to order 0
    double result = 1;
    if (vnl_math::abs(x) >= taylor_0_bound)
    {
      // approximate by taylor series in x at 0 up to order 2
      double x2 = x*x;
      result -= x2/6;
      if (vnl_math::abs(x) > taylor_2_bound)
      {
        // approximate by taylor series in x at 0 up to order 4
        result += (x2*x2)/120;
      }
    }
    return result;
  }
}

#endif // basic/dbnl/dbnl_sinc.h
