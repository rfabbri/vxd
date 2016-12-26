// This is basic/dbnl/dbnl_solve_trigonometric_equation.cxx
//:
// \file

#include "dbnl_solve_trigonometric_equation.h"
#include <vnl/vnl_math.h>
#include <vcl_cmath.h>


const double dbnl_solve_trigonometry_eps = 1e-12;


//: Solve trigonometry equation a sin(x) + b cos(x) + c = 0
// returned roots have format: root(cos(x), sin(x))
void dbnl_solve_1st_order_trig_equation(double a, double b, double c,
                                   vcl_vector<double >& sin_x,
                                   vcl_vector<double >& cos_x)
{
  // sanitize storage variables
  cos_x.clear();
  sin_x.clear();

  // DEGENERATE CASES

  // no root because a^2 + b^2 = 0 and c != 0
  if (a==0 && b==0 && c != 0)
    return;

  // infinitely many roots. Return 3 roots
  if (a==0 && b==0 && c == 0)
  {
    // root1
    cos_x.push_back(1);
    sin_x.push_back(0);

    // root2
    cos_x.push_back(0);
    sin_x.push_back(1);

    // root3
    cos_x.push_back(-1);
    sin_x.push_back(0);
    return;
  }

  // no root because c > a^2 + b^2
  double sin_t = -c / vcl_sqrt(a*a+b*b);
  
  if (vnl_math::abs(sin_t) > 1) 
  {
    return; 
  }


  double cos_alpha = a/vcl_sqrt(a*a+b*b);
  double sin_alpha = b/vcl_sqrt(a*a+b*b);

  // one root
  if (vnl_math::abs(sin_t) == 1)
  {
    cos_x.push_back(sin_t * sin_alpha);
    sin_x.push_back(sin_t * cos_alpha);
    return;
  }
  
  // REGULAR CASES - 2 roots
  double cos_x_plus_alpha[2];
  double sin_x_plus_alpha[2];

  cos_x_plus_alpha[0] = vcl_sqrt(1-sin_t*sin_t);
  sin_x_plus_alpha[0] = sin_t;

  cos_x_plus_alpha[1] = -vcl_sqrt(1-sin_t*sin_t);
  sin_x_plus_alpha[1] = sin_t;

  for (int i=0; i<2; ++i)
  {
    // dot and cross product of alpha and x_plus_alpha
    double dot = cos_alpha * cos_x_plus_alpha[i] + sin_alpha * sin_x_plus_alpha[i];
    double cross = cos_alpha * sin_x_plus_alpha[i] - sin_alpha * cos_x_plus_alpha[i];

    cos_x.push_back(dot);
    sin_x.push_back(cross);
  }
  return;
}
