// This is basic/dbnl/dbnl_brent_root.cxx
//:
// \file

#include "dbnl_brent_root.h"
#include <brent/brent.h>
#include <vnl/vnl_math.h>
#include <vcl_iostream.h>


// to remove
#include <vcl_string.h>
#include <vnl/vnl_matrix.h>
/////////////////////////////


void *vnl_cost_function_obj;



double dbnl_brent_root::
f(double x)
{
  vnl_vector<double > v(1, x);
  return this->f_->f(v);
}




// ----------------------------------------------------------------------------
double dbnl_brent_root::
zeroin_f(double x)
{
  vnl_cost_function* f0 = (vnl_cost_function*) vnl_cost_function_obj;
  vnl_vector<double > v(1, x);
  return f0->f(v);
}


// ----------------------------------------------------------------------------
//: Find a root of the function in the range [ax, bx]
// Required: f(ax) . f(bx) < 0
bool dbnl_brent_root::
solve(double ax, double bx, double& root)
{

  // Simple checks
  // is ``ax'' a root?
  double f_ax = this->f(ax);

  // is ``bx'' a root?
  double f_bx = this->f(bx);
  

  // for brent algo to work, the function evaluation on the two bounds must have
  // opposite sign
  if (f_ax * f_bx >= 0)
  {
    if (vnl_math::abs(f_ax) <= this->tol_)
    {
      root = ax;
      return true;
    }
    if (vnl_math::abs(f_bx) <= this->tol_)
    {
      root = bx;
      return true;
    }
    
    // best guess for the errorneous case
    root = (ax + bx)/2;
    //vcl_cerr << "ERROR: f(ax) and f(bx) must have opposite signs.\n";
    return false;
  }

  // swap the bounds to the correct order
  if (ax > bx)
  {
    double temp = ax;
    ax = bx;
    bx = temp;
  }

  vnl_cost_function_obj = (void*)(this->f_);

  // An abuse in using the tol_ variable. It is meant for the tolerance of the
  // function f. But in bren't algorithm, the tolerance is used to check the 
  // increment of the 'x' variable.
  root = zeroin(ax, bx, &dbnl_brent_root::zeroin_f, this->tol_, this->max_iter_);

  return (vnl_math::abs(this->f(root)) <= this->tol_);
}
