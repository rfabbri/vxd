#include "dbnl_eno.h"
//:
// \file
// \author Ricardo Fabbri

// Constants
#if !VCL_STATIC_CONST_INIT_INT_NO_DEFN
const unsigned dbnl_eno_poly::zero_order_index 
      VCL_STATIC_CONST_INIT_INT_DEFN(0);
const unsigned  dbnl_eno_poly::first_order_index 
      VCL_STATIC_CONST_INIT_INT_DEFN(1);
const unsigned  dbnl_eno_poly::second_order_index 
      VCL_STATIC_CONST_INIT_INT_DEFN(2);
#endif

void dbnl_eno_poly::
print(vcl_ostream &strm) const
{
   strm << "==== Polynomial ====" << vcl_endl
        << "order: " << order() << vcl_endl
        << "coefficients: ";

   for (unsigned i=0; i<=order(); ++i)
      strm << coeff(i) << " ";
   strm << vcl_endl;
}

dbnl_eno_poly operator-(const dbnl_eno_poly &f1, const dbnl_eno_poly &f2)
{
   dbnl_eno_poly diff_poly(2);

   diff_poly[f1.second_order_index] = f1[f1.second_order_index] - f2[f1.second_order_index];
   diff_poly[f1.first_order_index]  = f1[f1.first_order_index] - f2[f1.first_order_index];
   diff_poly[f1.zero_order_index]   = f1[f1.zero_order_index] - f2[f1.zero_order_index];

   return diff_poly;
}

double dbnl_eno_poly::
sample(double x) const
{ 
   return x*(coeffs_[second_order_index]*x + coeffs_[first_order_index]) 
          + coeffs_[zero_order_index];
}

// TODO
// - an eval method would be useful for testing...
// - use a class from vnl?
