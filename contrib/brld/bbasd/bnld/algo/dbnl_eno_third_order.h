// This is basic/dbnl/dbnl_eno_third_order.h
#ifndef bnl_eno_third_order_h
#define bnl_eno_third_order_h
//:
// \file
// \brief Essentially Non-Oscillatory 1D interpolation resulting in third order polynomial
// \author H. Can Aras, aras@brown.edu
// \date 2006-11-23
//
// The algorithm uses ENO idea to fit third order polynomials between each point sample pair.
// The code is mainly written to be used for space curve projects (curvature and
// torsion computations are more stable using ENO).
//
// The coefficients are stored in the "coeffs_" array starting with the coefficient of the 
// lowest degree.
//
// For an interval [x(i), x(i+1)], it is always true that
// p(x(i)) = f(x(i)) and p(x(i+1)) = f(x(i+1)) where p is the fitting polynomial.
// In other words, the fitting polynomial always passes through the start and end points of the interval.
//
// Remark: The user is responsible for ensuring that x samples of (x, f(x)) pairs must be different 
// from each other. This is not checked in the code for speed efficiency.
//
// \verbatim
//
// \endverbatim

#include <vcl_vector.h>

class dbnl_eno_third_order
{
  // FUNCTIONS
public:
  //: empty default constructor
  dbnl_eno_third_order();
  //: destructor
  virtual ~dbnl_eno_third_order();
  //: interpolator with f(x_i) values only, the sampling is assumed 
  //   to be uniform in this case, i.e. x_i = 0,1,2,...
  void interpolate(vcl_vector<double> fxi);
  //: interpolator with both x_i and f(x_i) values
  void interpolate(vcl_vector<double> xi, vcl_vector<double> fxi);
  //: get the specified coefficient of the specified interval
  double coefficient(int interval, int coeff_order);
  //: get starting index of the fitted polynomial for an interval
  int polynomial_start_index(int interval);
  //: print interval information
  void print();
protected:

private:
  // given 4 sample points, this function fits a cubic through these points
  void find_parameters(vcl_vector<double> xi, vcl_vector<double> fxi, int start_index, double *coeffs);

  // VARIABLES
public:

protected:
  //: number of intervals (one less than the number of points)
  int num_intervals_;
  //: number of points
  int num_points_;
  //: coefficients of all intervals, the first array index is the interval number
  double **coeffs_;
  //: index of the start point for the fitted polynomial at an interval
  int *poly_start_index_;

private:

};

#endif
