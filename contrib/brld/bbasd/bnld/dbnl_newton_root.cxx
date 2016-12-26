// This is basic/dbnl/dbnl_newton_root.cxx
//:
// \file

#include "dbnl_newton_root.h"
#include <vnl/vnl_math.h>
#include <vcl_iostream.h>
// Maxinum allowed number of iterations
#define max_iter 100 


//: Root finder combining Newton method and secant method
// Reference: William Press, et.al.,
// "Numerical Recipes in C: The Art of Scientific Computing"
// ISBN 0-521-43108-5


bool dbnl_newton_secant_root( dbnl_newton_root_func& funcd, 
                             double x1, double x2, double xacc, double *root)
{
  int j;
  double df, dx, dxold, f, fh, fl;
  double temp, xh, xl, rts;

  funcd.compute(x1, &fl, &df);
  funcd.compute(x2, &fh, &df);
  if ((fl > 0.0 && fh > 0.0) || (fl < 0.0 && fh < 0.0))
    vcl_cerr << "Root must be bracketed in dbnl_newton_secant_safe" << vcl_endl;
  if (fl == 0.0) {
    *root = x1;
    return true;
  }
  if (fh == 0.0){
    *root = x2;
    return true;
  }

  if (fl < 0.0){
    xl = x1;
    xh = x2;
  }
  else{
    xh = x1;
    xl = x2;
  }
  rts = 0.5 * (x1 + x2);
  dxold = vnl_math::abs(x2-x1);
  dx = dxold;
  funcd.compute(rts, &f, &df);
  for (j = 1; j <= max_iter; j++){
    if ((((rts-xh)*df-f)*((rts-xl)*df-f) > 0.0) || 
      (vnl_math::abs(2.0*f) > vnl_math::abs(dxold*df)))
    {
      dxold = dx;
      dx = 0.5*(xh-xl);
      rts = xl + dx;
      if (xl == rts){
        *root = rts;
        return true;
      }
    }
    else{
      dxold = dx;
      dx = f/ df;
      temp = rts;
      rts -= dx;
      if (temp == rts){
        *root = rts;
        return true;
      }
    }
    if (vnl_math::abs(dx) < xacc){ // convergence criteria
      *root = rts;
      double t1, t2;
      funcd.compute(rts, &t1, &t2);
      return true;
    }
    funcd.compute(rts, &f, &df);
    // Maintain the bracket on the root
    if (f < 0.0)
      xl = rts;
    else
      xh = rts;
  }
  vcl_cerr << "Maximum number of iterations exceeded in dbnl_newton_secant_root" << vcl_endl;
  return false; // should never get here

}
                                    
                                 


