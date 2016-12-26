// This is bsold_gshock.h
#ifndef bsold_gshock_h
#define bsold_gshock_h
//:
//\file
//\brief GENO shock structure
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 03/09/2005 06:59:04 PM EST
//
#include <bgld/bgld_arc.h>
#include <bsold/bsold_geno_measures.h>
#include <vbl/vbl_ref_count.h>


//: Holds shock information for one interval
// See also bsold_gshock_curve_2d
// \todo write routines to measure shock tangents and speed, if ever needed
class bsold_gshock : public vbl_ref_count {
public:
   
   static const double default_shock_point_dist_threshold
      VCL_STATIC_CONST_INIT_FLOAT_DECL(1.5);
   
   //: default constructor
   bsold_gshock(const bgld_param_curve *prv, const bgld_param_curve *cur, 
                const bgld_param_curve *nxt, 
                const double st=default_shock_point_dist_threshold) 
   { make (prv, cur, nxt, st); }

   //: return intersections between neighboring interpolants
   const vgl_point_2d<double> & loc(unsigned i) const {return loc_[i];}
   //: returns the one intercept considered to be the location of the shock
   const vgl_point_2d<double> & loc() const {return loc_[0];}
   //: number of intersections btw neighboring interpolants
   double n_intercepts() const {return loc_.size();}

//   cache_measures();

   unsigned make(const bgld_param_curve *prv, const bgld_param_curve *cur, 
                 const bgld_param_curve *nxt, 
                 const double shock_point_dist_threshold=default_shock_point_dist_threshold);

protected:
//   bgld_param_curve *next_, *prev_; //< not in original kwh's GENO
   vcl_vector< vgl_point_2d<double> > loc_;
   /*
   double speed_;
   double direction_;
   double tangent1_;
   double tangent2_;
   */
private:
   static double patchspan(double);
   bool fix_outofbound_shock(const bgld_arc *prv, const bgld_arc *cur, const bgld_arc *nxt);
   void place_shock_at_endpoints(const bgld_arc *cur);
};

// ------ Inlined definitions ----------------------------------------
//: simply a shortcut to implementation in bsold_geno_measures
inline double bsold_gshock::patchspan(double x)
{ return bsold_geno_measures::patchspan(x); }

inline void bsold_gshock::place_shock_at_endpoints(const bgld_arc *cur)
{
   loc_.resize(2);
   loc_[0] = cur->point_at(0);
   loc_[1] = cur->point_at(1);
}

#endif // bsold_gshock_h

