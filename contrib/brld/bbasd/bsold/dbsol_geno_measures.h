// This is dbsol_geno_measures.h
#ifndef dbsol_geno_measures_h
#define dbsol_geno_measures_h
//:
//\file
//\brief Code to compute and store/cache some differential measures from
// dbgl_param_curve's 
//
// \author Ricardo Fabbri (RFabbri), Brown University
// (rfabbri.github.io)
//\date Wed Mar  2 20:04:27 EST 2005
//

#include <dbgl/dbgl_param_curve.h>
#include <vnl/vnl_math.h>
#include <vcl_iostream.h>

//: Given a dbgl_param_curve, this class computes and stores/cache some useful
// measures.  Their primary purpose is to be used in shock detection.
class dbsol_geno_measures {
public:
   dbsol_geno_measures() {}
   ~dbsol_geno_measures() {}

   void compute(const dbgl_param_curve &c);
   void print(vcl_ostream& = vcl_cerr) const;

   /// Accessors
   double start_tangent() const {return start_tangent_;} 
   double end_tangent() const {return end_tangent_;} 
   double start_curvature() const {return start_curvature_;} 
   double end_curvature() const {return end_curvature_;} 
   double delta_tangent() const {return patchspan(end_tangent() - start_tangent());}

   /// Utilities to manipulate the measures
   static const double degrees_per_radian 
      VCL_STATIC_CONST_INIT_FLOAT_DECL(180.0/3.14159265358979323846);
   static double rad2deg(double x) {return x*180 / 3.14159265358979323846;}
   //: angle in degrees
   static double patchspan(double x)
      { return (x > 180. ? x - 360. : ((x < -180.) ? x + 360. : x)); }
   static double pow_1_5(double x) { return sqrt(x*x*x); }

protected:
   double start_tangent_,
          end_tangent_,
          start_curvature_, end_curvature_;
};

#endif
