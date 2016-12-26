// This is core/vnl/algo/brent.h
#ifndef brent_h_
#define brent_h_
//:
// \file
// \author 


#ifdef __cplusplus
extern "C" {
#endif

double zeroin(double ax, double bx, double (*f)(double x), double tol, int max_iter);
double fminbr(double a, double b, double (*f)(double x), double tol);



#ifdef __cplusplus
}
#endif

#endif // brent_h_
