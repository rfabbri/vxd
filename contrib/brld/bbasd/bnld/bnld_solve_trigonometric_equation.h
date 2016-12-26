// This is basic/dbnl/dbnl_solve_trigonometric_equation.h

#ifndef dbnl_solve_trigonometric_equation_h_
#define dbnl_solve_trigonometric_equation_h_

//:
// \file
// \brief Functions to solve various trigonomtric equations
// 
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date Nov 19, 2009
//
// \verbatim
//  Modifications:
// \endverbatim


#include <vcl_vector.h>

//: Solve trigonometry equation a sin(x) + b cos(x) + c = 0
// returned roots have format: root(cos(x), sin(x))
void dbnl_solve_1st_order_trig_equation(double a, double b, double c,
                                   vcl_vector<double >& sin_x,
                                   vcl_vector<double >& cos_x);



#endif // basic/dbnl/dbnl_solve_trigonometric_equation.h
