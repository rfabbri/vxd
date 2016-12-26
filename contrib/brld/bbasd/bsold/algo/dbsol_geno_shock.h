// This is dbsol_geno_shock.h
#ifndef dbsol_geno_shock_h
#define dbsol_geno_shock_h
//:
//\file
//\brief Shock-placing GENO interpolation and related classes 
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 03/02/2005 03:10:08 PM EST
//
#include <dbsol/algo/dbsol_geno.h> 
#include <vnl/vnl_math.h>
#include <dbsol/dbsol_geno_measures.h>
#include <dbsol/dbsol_gshock.h>
#include <dbsol/dbsol_gshock_curve_2d.h>

//: Shock-placing GENO interpolation and related routines
// 
class dbsol_geno_shock
{
 public:


   //--- Constants/Parameters  ---------------------------------------
    
   static const double default_tangent_threshold
      VCL_STATIC_CONST_INIT_FLOAT_DECL(30.0);
   static const double default_curvature_threshold
      VCL_STATIC_CONST_INIT_FLOAT_DECL(0.3);

   //--- Shock Tests -----------------------------------------------
   
   static bool
   test_change(dbsol_geno_curve_2d const &c, unsigned i,
         double tt=default_tangent_threshold, 
         double ct=default_curvature_threshold );

   static bool 
   test_change(dbsol_gshock_curve_2d const &c, unsigned i,
         double tt=default_tangent_threshold, 
         double ct=default_curvature_threshold );


   //: Tests whether an interval is a probable shock based on
   // abrupt variation of measures at the endpoints.
   static bool test_change(
         dbgl_param_curve const   &prev,
         dbgl_param_curve const   &curr, 
         dbgl_param_curve const   &next,
         double tt=default_tangent_threshold, 
         double ct=default_curvature_threshold );

   //: If you have geometric measures already computed, this version of shock_test will 
   // use them  instead of repeating the calculation.
   static bool test_change(
         dbgl_param_curve const   &prev, dbsol_geno_measures const &prevm,
         dbgl_param_curve const   &curr, dbsol_geno_measures const &currm,
         dbgl_param_curve const   &next, dbsol_geno_measures const &nextm,
         double tt=default_tangent_threshold, 
         double ct=default_curvature_threshold );

   static bool
   test_change(dbsol_gshock_curve_2d const &c, 
         unsigned iprv, unsigned icur, unsigned inxt,
         double tt=default_tangent_threshold, 
         double ct=default_curvature_threshold );


   //: Tests whether the i-th interval is a probable shock, based on the direction of
   // neighboring stencils
   static bool test_stencil(dbsol_geno_curve_2d const &c, unsigned i);

   //: Tests whether an interval is a probable shock, based on the direction of
   // neighboring stencils
   static bool test_stencil(dbsol_geno_curve_2d const &c, 
                            unsigned iprv, unsigned icur, unsigned inxt);

   //: This codes the combination of tests that ultimately says if an interval is 
   // a shock. It is this combination that is used by place_shocks.
   // \return true = shock, false = no shock
   static bool combined_tests(
         dbsol_gshock_curve_2d const &c, 
         unsigned iprv, unsigned icur, unsigned inxt,
         double tt=default_tangent_threshold, 
         double ct=default_curvature_threshold)
   { return test_stencil(c,iprv,icur,inxt) && test_change(c,iprv,icur,inxt,tt,ct); }

   //: mark intervals that pass a combination of tests as shocks
   static void mark_all_shocks( dbsol_gshock_curve_2d &c,
                                double tt=default_tangent_threshold, 
                                double ct=default_curvature_threshold);
   
   //: construct shock structures for each interval that is marked as a shock
   static void make_all_shocks( dbsol_gshock_curve_2d &c, 
          double st=dbsol_gshock::default_shock_point_dist_threshold);

   //: test, mark, and make all shocks
   static void place_all_shocks( dbsol_gshock_curve_2d &c,
                            double tt=default_tangent_threshold, 
                            double ct=default_curvature_threshold,
                            double st=dbsol_gshock::default_shock_point_dist_threshold);

   
   //: Try to mark c[icur] as shock, but if previous is already marked, we
   // keep only the one with larger curvature.
   static void mark_and_untie(dbsol_gshock_curve_2d &c, unsigned iprv, unsigned i);

 protected:
};

inline bool dbsol_geno_shock::
test_stencil(dbsol_geno_curve_2d const &c, 
             unsigned iprv, unsigned /*icur warning suppression*/, unsigned inxt)
{ return c.is_upper(iprv) && c.is_lower(inxt);
// that is slightly different from the semantics of the original GENO code:
//    return !c.is_upper(iprv) && c.is_upper(inxt);
// which seems wrong -- rfabbri
// TODO: test this!
}

#endif // dbsol_geno_shock_h
