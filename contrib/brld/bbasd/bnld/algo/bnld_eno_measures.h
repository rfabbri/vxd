// This is bnld_eno_measures.h
#ifndef bnld_eno_measures_h
#define bnld_eno_measures_h
//:
//\file
//\brief Code to compute some differential measures from bnld_eno_intep's
//\author Based on original code by  Ricardo Fabbri (RFabbri), Brown University  (rfabbri.github.io)
//\date 02/11/2005 05:21:36 PM EST
//

#include <bnld/algo/bnld_eno.h>

//: Given an eno_interp, this class computes and stores some useful measures.
// Their primary purpose is to be used in shock detection
class bnld_eno_measures {
public:
   bnld_eno_measures() {}
   ~bnld_eno_measures() {}

   void compute(const bnld_eno_interp &I);
   void print(vcl_ostream& = vcl_cerr) const;

   /// Accessors
   double start_tangent() const {return start_tangent_;} 
   double end_tangent() const {return end_tangent_;} 
   double start_curvature() const {return start_curvature_;} 
   double end_curvature() const {return end_curvature_;} 
   double total_curvature() const {return total_curvature_;}

   static const double degrees_per_radian 
      VCL_STATIC_CONST_INIT_FLOAT_DECL(180.0/3.14159265358979323846);
   static double angle_difference(double a,double b);

protected:
   double start_tangent_, end_tangent_,
          start_curvature_, end_curvature_,
          total_curvature_;
   double pow_1_5(double x) const { return sqrt(x*x*x); }
};

inline double bnld_eno_measures::
angle_difference(double a,double b)
{
// angles are in degrees
// OBS: made more efficient than orig enolib
   double const diff = fmod(a,360.0) - fmod(b,360.0);
 
   if (vcl_fabs(diff) < 180.0)
      return diff;
   if (diff > 0)
      return diff - 360.0;
   return diff + 360.0;
}
#endif // bnld_eno_measures_h
