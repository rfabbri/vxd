// This is basic/dbnl/dbnl_fresnel.h

#ifndef dbnl_fresnel_h_
#define dbnl_fresnel_h_

//:
// \file
// \brief Fresnel integrals - sine and cosine
// 
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date 2/03/05
//
// \verbatim
//  Modifications:
// \endverbatim

// Reference: book "Numerical Recipes in C" by William Press et.al. Section 6.9, page 255


//: Fresnel cosine integral
// FresnelC = int(cos(pi/2 * t^2), t = 0..x);
// FresnelS = int(sin(pi/2 * t^2), t = 0..x);
// Return: fs is Fresnel cosin integral, fc is Frenel sine integral
void dbnl_fresnel_integral(double x, double* fresnel_cos, double* fresnel_sin);

#endif // basic/dbnl/dbnl_fresnel.h
