// This is bsold_gshock_curve_2d.h
#ifndef bsold_gshock_curve_2d_h
#define bsold_gshock_curve_2d_h
//:
//\file
//\brief Curve that has been interpolated with shock-placing GENO
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 03/02/2005 02:38:37 PM EST
//

#include <bsold/bsold_geno_curve_2d.h>
#include <bsold/bsold_geno_measures.h>
#include <dbgl/dbgl_param_curve.h>
#include <bsold/bsold_gshock_sptr.h>

//: After interpolation, this class is read-only for non-friends, except for
// the operation of unmarking shocks.
// Do not reuse this class after first interpolation. If you want to run
// interpolation again, use another instance.
class bsold_gshock_curve_2d : public bsold_geno_curve_2d {
 public:
   bsold_gshock_curve_2d() :
      bsold_geno_curve_2d(),
      nshocks_(0) { }

   ~bsold_gshock_curve_2d()
   { }

   //: return cached bsold_geno_measures for i-th interval
   const bsold_geno_measures & measure(unsigned i) const { return m_[i]; }

   //: If i-th interval has an associated shock object, 
   // return smart pointer to the shock object. Otherwise, return null
   const bsold_gshock_sptr shock(unsigned i) const {return s_[i];}

   //: was any shock placed on i-th interval?
   bool has_shock(unsigned i) const {return marked(i);}

   //: unmark interval as shock (lets the user "ignore" a shock)
   void unmark(unsigned i) { skmark_[i] = false; }

   //: number of computed shocks
   unsigned nshocks() const { return nshocks_; }
   
   //: print
   virtual void describe(vcl_ostream &strm, int blanking) const;

   //: shock-related algorithms:
   friend class bsold_geno_shock;

 protected:
   //: mark interval as shock
   void mark(unsigned i) { skmark_[i] = true; }
   //: true if interval is marked as shock
   bool marked(unsigned i) const {return skmark_[i];}
   virtual void make(const vcl_vector<dbgl_param_curve *> &inter, bool closed) 
      { bsold_geno_curve_2d::make(inter,closed); m_.clear(); cache_measures(); 
         skmark_.clear(); skmark_.resize(inter.size(),false);}

   void cache_measures();
   //: did we cache measures for each interval?
   bool computed_measures() const {return m_.size() !=0; }

   void set_nshocks(unsigned n) {nshocks_ = n;}

   //: vector of shock interpolants. shock_[i] == NULL if the interval have no
   // shock. It could be made into a list for efficient traversal of shocks if
   // this turns out to be important for some application.
   vcl_vector<bsold_gshock_sptr> s_;
 private:
   //: cache of tangent and curvature measurements of intervals
   vcl_vector<bsold_geno_measures> m_;
   //: shock mark:
   vcl_vector<bool> skmark_;
   unsigned nshocks_;
};

#endif // bsold_gshock_curve_2d_h
