// This is bbasd/bnld/bnld_solve_quadratic_equation.h

#ifndef bnld_solve_quadratic_equation_h_
#define bnld_solve_quadratic_equation_h_

//:
// \file
// \brief Functions to solve quadratic equation
// 
// \author Based on original code by  Nhon Trinh ( @lems.brown.edu)
// \date Nov 19, 2009
//
// \verbatim
//  Modifications:
// \endverbatim

//#include <vcl_cmath.h>
//#include <vcl_limits.h>
//#include <vnl/vnl_math.h>

#include <vcl_vector.h>

//: Solve quadratic equation ax^2 + bx + c = 0, a != 0
void bnld_solve_quadratic_equation(double a, double b, double c, 
                                     vcl_vector<double >& roots);


#endif // bbasd/bnld/bnld_solve_quadratic_equation.h
