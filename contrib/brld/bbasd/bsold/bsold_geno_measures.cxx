//:
//\file
//\author Ricardo Fabbri
//
#include "bsold_geno_measures.h"

#if !VCL_STATIC_CONST_INIT_FLOAT_NO_DEFN
const double  bsold_geno_measures::degrees_per_radian 
      VCL_STATIC_CONST_INIT_FLOAT_DEFN(180.0/vnl_math::pi);
#endif


/*
 * Computes start_tangent/curvature, end_tangent/curvature
 * and total_curvature.
 */
void bsold_geno_measures::
compute(const dbgl_param_curve &c)
{
   start_tangent_ = c.tangent_angle_at(0);
   start_tangent_ = c.tangent_angle_at(1);
   start_curvature_ = c.curvature_at(0);
   start_curvature_ = c.curvature_at(1);
}



void bsold_geno_measures::
print(vcl_ostream& strm) const
{
   strm << "       tangents(deg): " << start_tangent_ << "  " << end_tangent_ 
        << vcl_endl
        << "      curvature(deg): " << start_curvature_ << "  " << end_curvature_
        << vcl_endl;
}
