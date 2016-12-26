// This is basic/dbnl/dbnl_eno_third_order.cxx
#include "dbnl_eno_third_order.h"
#include <vcl_iostream.h>
#include <vcl_cfloat.h>
#include <vcl_cmath.h>
#include <assert.h>

dbnl_eno_third_order::dbnl_eno_third_order()
{
}

dbnl_eno_third_order::~dbnl_eno_third_order()
{
  for(int i=0; i < num_intervals_; i++)
    delete [] coeffs_[i];

  delete [] coeffs_;
}

void dbnl_eno_third_order::interpolate(vcl_vector<double> fxi)
{
  vcl_vector<double> xi;

  int size = fxi.size();
  for(int i=0; i<size; i++)
    xi.push_back(double(i));

  return interpolate(xi, fxi);
}

void dbnl_eno_third_order::interpolate(vcl_vector<double> xi, vcl_vector<double> fxi)
{
  assert(xi.size() > 3);
  num_intervals_ = xi.size()-1;
  num_points_ = xi.size();
  // allocate space for variables
  poly_start_index_ = new int [num_intervals_];
  coeffs_ = new double* [num_intervals_];
  for(int i=0; i < num_intervals_; i++)
    coeffs_[i] = new double [4];

  // find the amoothest fitting polynomial for the interval (x(i), x(i+1))
  for(int i=0; i < num_intervals_; i++)
  {
    // There are three possible fitting polynomials for each interval.
    // First and last two intervals do not have all three possible, so they are checked

    double temp_coeffs[4];
    double smoothest_poly_coeffs[4] = {DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX};

    if(i <= num_intervals_-3)
    {
      // using {x(i), x(i+1), x(i+2), x(i+3)} set
      find_parameters(xi, fxi, i, temp_coeffs);
      if( vcl_fabs(temp_coeffs[3]) < vcl_fabs(smoothest_poly_coeffs[3]) )
      {
        for(int j=0; j<4; j++)
          smoothest_poly_coeffs[j] = temp_coeffs[j];
        poly_start_index_[i] = i;
      }
    }
    if(i <= num_intervals_-2 && i >= 1)
    {
      // using {x(i-1), x(i), x(i+1), x(i+2)} set
      find_parameters(xi, fxi, i-1, temp_coeffs);
      if( vcl_fabs(temp_coeffs[3]) < vcl_fabs(smoothest_poly_coeffs[3]) )
      {
        for(int j=0; j<4; j++)
          smoothest_poly_coeffs[j] = temp_coeffs[j];
        poly_start_index_[i] = i-1;
      }
    }
    if(i >= 2)
    {
      // using {x(i-2), x(i-1), x(i), x(i+1)} set
      find_parameters(xi, fxi, i-2, temp_coeffs);
      if( vcl_fabs(temp_coeffs[3]) < vcl_fabs(smoothest_poly_coeffs[3]) )
      {
        for(int j=0; j<4; j++)
          smoothest_poly_coeffs[j] = temp_coeffs[j];
        poly_start_index_[i] = i-2;
      }
    }
   
    for(int j=0; j<4; j++)
      coeffs_[i][j] = smoothest_poly_coeffs[j];
  }
}

double dbnl_eno_third_order::coefficient(int interval, int coeff_order)
{
  assert(interval >= 0 && interval < num_intervals_);
  assert(coeff_order >= 0 && coeff_order < 4);
  return coeffs_[interval][coeff_order];
}

int dbnl_eno_third_order::polynomial_start_index(int interval)
{
  assert(interval >= 0 && interval < num_intervals_);
  return poly_start_index_[interval];
}

void dbnl_eno_third_order::print()
{
  for(int i=0; i<num_intervals_; i++)
  {
    vcl_cout << "interval " << i << vcl_endl << "---" << vcl_endl;
    vcl_cout << "starting point index: " << poly_start_index_[i] << vcl_endl;
    vcl_cout << coeffs_[i][0] << " " << coeffs_[i][1] << " " << 
                coeffs_[i][2] << " " << coeffs_[i][3] << vcl_endl;
  }
}

void dbnl_eno_third_order::find_parameters(vcl_vector<double> xi, vcl_vector<double> fxi, int start_index, double *coeffs)
{
  double a0, a1, a2, a3;
  
  double x0 = xi[start_index];
  double x1 = xi[start_index+1];
  double x2 = xi[start_index+2];
  double x3 = xi[start_index+3];

  double f0 = fxi[start_index];
  double f1 = fxi[start_index+1];
  double f2 = fxi[start_index+2];
  double f3 = fxi[start_index+3];

  // Standard cubic polynomial fitting procedure
  a0 = f0;
  a1 = (f1-a0) / (x1-x0);
  a2 = (f2-a0-a1*(x2-x0)) / ((x2-x1)*(x2-x0));
  a3 = (f3-a0-a1*(x3-x0)-a2*(x3-x1)*(x3-x0)) / ((x3-x2)*(x3-x1)*(x3-x0));

  coeffs[0] = a0 - a1*x0 + a2*x1*x0 - a3*x2*x1*x0;
  coeffs[1] = a1 - a2*x0 - a2*x1 + a3*x1*x0 + a3*x2*x0 + a3*x2*x1;
  coeffs[2] = a2 - a3*x0 - a3*x1 - a3*x2;
  coeffs[3] = a3;
}


