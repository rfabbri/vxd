// This is basic/dbnl/dbnl_cbrt.cxx
//:
// \file

#include <vcl_cassert.h>
#include <vcl_cmath.h>
#include <dbnl/dbnl_cbrt.h>

double dbnl_cbrt (const double& inputx)
{
  int i;
  double  w1, w2, y1, p, q, r;
  double eps = 1.0e-10;
  
  if (inputx == 0.0)
    return (0.0);

  w1 = 1.0;
  for (i = 0; i < 100; i++) {
    p = 2.0*w1*w1;
    q = inputx/w1;
    r = p+q;

    if(r == 0.0)
      w2 = 0.5*w1;
    else
      w2 = 0.5 * (w1 + 3.0 * inputx / r);
    y1 = w1 * w1 * w1;

    if (vcl_fabs(y1 - inputx) <= eps)
      return(w2);
    else  
      w1 = w2;
  }
  assert (0);
  return sqrt(inputx);
}


///move to test:
///void main() {
/// double  x, y;
/// printf("    X       Cubic_root(X) \n\n");
/// for(x = -10.0; x <= 10.0; x += 1.) {
///      printf(" %6.2f      %10.7f \n", x, cbrt(x));
/// }
///}



