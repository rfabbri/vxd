#include "dbnl_eno_measures.h"

#if !VCL_STATIC_CONST_INIT_FLOAT_NO_DEFN
const double  dbnl_eno_measures::degrees_per_radian 
      VCL_STATIC_CONST_INIT_FLOAT_DEFN(180.0/vnl_math::pi);
#endif


/*
 * Computes start_tangent/curvature, end_tangent/curvature
 * and total_curvature.
 */
void dbnl_eno_measures::
compute(const dbnl_eno_interp &I)
{
  double
    dy_dx;      // derivative of polynomial at point loc 

  // second derivative of polynomial
  double const d2 = 2*I[dbnl_eno_poly::second_order_index];

  // do values at start of interval  
  dy_dx =  I.derivative(I.start());
  start_tangent_   = atan2(1.0,dy_dx) * dbnl_eno_measures::degrees_per_radian;
  start_curvature_ = d2 / pow_1_5(1.0 + dy_dx*dy_dx);

  // do values at end of interval 
  dy_dx =  I.derivative(I.end());
  end_tangent_   = atan2(1.0,dy_dx) * dbnl_eno_measures::degrees_per_radian;
  end_curvature_ = d2 / pow_1_5(1.0 + dy_dx*dy_dx);

  // compute total curvature across this interpolant interval 
  total_curvature_ = angle_difference(end_tangent_, start_tangent_);
}



void dbnl_eno_measures::
print(vcl_ostream& strm) const
{
   strm << "       tangents(deg): " << start_tangent_ << "  " << end_tangent_ 
        << vcl_endl
        << "      curvature(deg): " << start_curvature_ << "  " << end_curvature_
        << vcl_endl
        << "total_curvature(deg): " << total_curvature_ << vcl_endl;
}

