// This is basic/dbnl/dbnl_eno.h
#ifndef bnl_eno_shock_h
#define bnl_eno_shock_h
//:
// \file
// \brief Essential Non-Oscillatory 1D Interpolation with Shocks
// \author Ricardo Fabbri, LEMS, Brown University (rfabbri.github.io)
// \date Feb 2005
//
// This file contains class declarations regarding ENO interpolation with shocks.
//
#include <dbnl/algo/dbnl_eno.h>
#include <dbnl/algo/dbnl_eno_measures.h>

// ---------------------------------------------------------------------------
//: This class represents an interpolated interval having a shock.
// The shock interpolator is simply the continuation of the neighboring
// polynomials (assuming neighbors are not themselves shocks). We represent this
// as pointers to the neighbouring polynomial interps, together with the
// location of the shock. 
class dbnl_eno_shock_interp {
public:
   dbnl_eno_shock_interp() {}


   void print(vcl_ostream& = vcl_cerr) const;
   //: abcissa of shock
   double loc() const {return loc_;}
   //: returns true when this is a shock but neighboring interpolants don't intersect
   bool no_intercept() const {return no_intercept_;}
   const dbnl_eno_interp &next() const {return *next_;}
   const dbnl_eno_interp &prev() const {return *prev_;}
   //: makes the shock
   unsigned make(dbnl_eno_interp const *prev, dbnl_eno_interp const *curr,
                       dbnl_eno_interp const *next);

private:
   //: TAKE CARE when using this class directly; make sure these pointers are
   // valid before accessing their content
   dbnl_eno_interp const *next_, *prev_;
   double loc_;
   bool no_intercept_;
};


// ---------------------------------------------------------------------------
//: This class implements 1D shock-placing ENO interpolation. Typical usage:
//\code
// dbnl_eno_shock_1d e;
// ...
// vnl_vector<double> data(n);
// ...
// e.interpolate(data);      // plain eno
// e.place_shocks();     // eno with shocks
// ..
// vcl_cout << e[i][2];       // coefficient of x^2 at the i-th interval
// ..
// if (e.has_shock(i))
//    vcl_cout << e.shock(i).loc(); // access shock coordinate at ith interval
//
//\endcode
//
// See also the examples directory.
//
class dbnl_eno_shock_1d : public dbnl_eno_1d {
public:
   static const double default_tangent_change_threshold
      VCL_STATIC_CONST_INIT_FLOAT_DECL(20.0);
   static const double default_curvature_change_threshold
      VCL_STATIC_CONST_INIT_FLOAT_DECL(0.3);

   dbnl_eno_shock_1d(double border_value = vcl_numeric_limits<double>::max()/1e10) :
      dbnl_eno_1d(border_value),
      nshocks_(0) { }

   ~dbnl_eno_shock_1d();

   //: test if there is a shock at ith interval i
   bool test_shock(unsigned i, 
         double tt=dbnl_eno_shock_1d::default_tangent_change_threshold,
         double ct=dbnl_eno_shock_1d::default_curvature_change_threshold) const;

   //: test if there is a shock stored at ith interval i, ASSUMING place_shocks
   //was already called
   bool has_shock(unsigned i) const { return s_[i] != 0; }

   //: construct shock at ith interval regardless of any criteria
   void make_shock(unsigned i);
   //: remove shock from ith interval
   void delete_shock(unsigned i);
   //: for each interval, test for a shock, and if positive make a shock and store it
   bool place_shocks(
         double tt=dbnl_eno_shock_1d::default_tangent_change_threshold,
         double ct=dbnl_eno_shock_1d::default_curvature_change_threshold);

   //: make sure has_shock(i) is true before using this
   const dbnl_eno_shock_interp &shock(unsigned i) const {return *(s_[i]);}
   const dbnl_eno_measures& measures(unsigned i) const {return m_[i];}

   // function shock(i) returns null if s_.size() == 0

   //: did we compute and store the shocks already?
   bool shocks_computed() const { return s_.size() != 0; }
   unsigned nshocks() const {return nshocks_;}
   void print(vcl_ostream& = vcl_cerr) const;

   //: Function that tests whether an interval is a probable shock. 
   // It does not construct or store any information. It is not a member of
   // dbnl_eno_interp because I wanted that class to be totally independent of shock
   // placing concepts.
   // \relates dbnl_eno_interp
   // \param[in] prev interpolant in previous interval
   // \param[in] curr interpolant in current interval
   // \param[in] next interpolant in next interval
   // \return true if the current interval is classified as shock
   // This function may be used without actually having an instance (object) of
   // eno_shock_1d. 
   static bool test(
         dbnl_eno_interp const   *prev,
         dbnl_eno_interp const   *curr, 
         dbnl_eno_interp const   *next,
         double shock_tangent_change_threshold=default_tangent_change_threshold, 
         double shock_curvature_change_threshold=default_curvature_change_threshold
      );

   //: If you have geometric measures already computed, this version of shock_test will 
   // use them  instead of repeating the calculation.
   // This function may be used without actually having an instance (object) of
   // eno_shock_1d. 
   static bool test(
         dbnl_eno_interp const   *prev, dbnl_eno_measures const *prevm,
         dbnl_eno_interp const   *curr, dbnl_eno_measures const *currm,
         dbnl_eno_interp const   *next, dbnl_eno_measures const *nextm,
         double shock_tangent_change_threshold=default_tangent_change_threshold, 
         double shock_curvature_change_threshold=default_curvature_change_threshold
      );
   
   //: did we cache measures for each interval?
   bool computed_measures() const {return m_.size() !=0; }
protected:
   //: vector of shock interpolants. shock_[i] == NULL if the interval have no
   //shock. It could be made into a list for efficient traversal of shocks if
   //this turns out to be important for some application.
   vcl_vector<dbnl_eno_shock_interp *> s_; 
   unsigned nshocks_;
   //: cache of tangent and curvature measurements of intervals
   vcl_vector<dbnl_eno_measures> m_;
   //: used by shock functions to compute and store/cache measures for each interval 
   void compute_measures(); 
};


#endif // bnl_eno_shock_h
