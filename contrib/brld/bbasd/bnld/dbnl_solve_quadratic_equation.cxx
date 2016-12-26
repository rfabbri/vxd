// This is basic/dbnl/dbnl_solve_quadratic_equation.cxx
//:
// \file

#include "dbnl_solve_quadratic_equation.h"

#include <vcl_cmath.h>
#include <vcl_cassert.h>

//: Solve quadratic equation ax^2 + bx + c = 0, a != 0
void dbnl_solve_quadratic_equation(double a, double b, double c, 
                                     vcl_vector<double >& roots)
{
  // require
  assert(a != 0);
  roots.clear();

  double delta = b*b - 4*a*c;
  if (delta > 0) // two distinct roots
  {
    roots.push_back((-b-vcl_sqrt(delta))/(2*a));
    roots.push_back((-b+vcl_sqrt(delta))/(2*a));
    return;
  }
  else if (delta == 0)  // one root
  {
    roots.push_back(-b/(2*a));
  }

  // delta < 0: no root
  return;
}

