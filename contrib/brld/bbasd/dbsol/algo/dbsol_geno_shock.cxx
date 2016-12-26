//:
// \file
// \author Ricardo Fabbri
//

#include "dbsol_geno_shock.h"

#if !VCL_STATIC_CONST_INIT_FLOAT_NO_DEFN
const double dbsol_geno_shock::default_tangent_threshold
   VCL_STATIC_CONST_INIT_FLOAT_DEFN(30.0);
const double dbsol_geno_shock::default_curvature_threshold
   VCL_STATIC_CONST_INIT_FLOAT_DEFN(0.3);
#endif


//: Tests whether an interval is a probable shock based on
// abrupt variation of measures at the endpoints.
// It does not construct or store any information. 
// \param[in] prev interpolant in previous interval
// \param[in] curr interpolant in current interval
// \param[in] next interpolant in next interval
// \return true if the current interval is classified as shock
// This function may be used without actually having an instance (object) of
// eno_shock_1d. 
bool dbsol_geno_shock::
test_change( dbgl_param_curve const   &prv,
      dbgl_param_curve const   &cur, 
      dbgl_param_curve const   &nxt,
      double tangent_threshold, 
      double curvature_threshold)
{

   dbsol_geno_measures prvm, curm, nxtm;

   prvm.compute(prv);
   curm.compute(cur);
   nxtm.compute(nxt);

   return test_change(prv, prvm, cur, curm, nxt, nxtm, 
               tangent_threshold, curvature_threshold);
}

bool dbsol_geno_shock::
test_change( dbgl_param_curve const   &, dbsol_geno_measures const &prevm,
      dbgl_param_curve const   &, dbsol_geno_measures const &currm,
      dbgl_param_curve const   &, dbsol_geno_measures const &nextm,
      double tangent_threshold, 
      double curv_threshold )
{
   // OBS: criterion slightly different than the one used in ENO
   // TODO: compare with dbnl/algo/dbnl_eno_shock.cxx
   
   double delta_tan, delta_curv;

   delta_tan  = currm.patchspan(
         currm.rad2deg( prevm.end_tangent() - nextm.start_tangent() ));

   delta_curv = vcl_fabs(prevm.end_curvature() - nextm.start_curvature());

   if (delta_tan >= tangent_threshold) {
      if (delta_curv < curv_threshold) {
#ifndef NDEBUG
      vcl_cerr << "Classified as shock due solely to large tangent change\n";
#endif
      }
      return true;
   } else 
      return delta_curv >= curv_threshold;
}

bool dbsol_geno_shock::
test_change(dbsol_gshock_curve_2d const &c, unsigned iprv, unsigned icur, unsigned inxt,
      double tangent_threshold, 
      double curv_threshold )
{
   return test_change(c[iprv],c.measure(iprv), c[icur],c.measure(icur),
                      c[inxt],c.measure(inxt), 
                      tangent_threshold, curv_threshold);
}

//: Try to mark c[icur] as shock, but if previous is already marked, we
// keep only the one with larger curvature.
void dbsol_geno_shock::
mark_and_untie( dbsol_gshock_curve_2d &c, unsigned iprv, unsigned i)
{
    double tc, prev_tc;

    if (!c.marked(iprv)) {
       c.mark(i);
    } else {
       tc      = fabs(c.measure(i).delta_tangent());
       prev_tc = fabs(c.measure(iprv).delta_tangent());

       if(tc <= prev_tc) {
           c.unmark(i);
       } else {
           c.unmark(iprv);
           c.mark(i);
       }
    }
}

//: mark the intervals that pass a combination of tests as shocks
void dbsol_geno_shock::
mark_all_shocks(dbsol_gshock_curve_2d &c, double tt, double ct)
{
   unsigned i;
   if (!c.closed()) {
      for (i=1; i < c.size()-1; ++i)
         if ( combined_tests(c,i-1,i,i+1,tt,ct) )
            mark_and_untie(c,i-1,i);
   } else {

      if ( combined_tests(c, c.size()-1, 0, 1, tt,ct) )
         c.mark(0);

      for (i=1; i < c.size()-1; ++i)
         if ( combined_tests(c,i-1,i,i+1,tt,ct) )
            mark_and_untie(c,i-1,i);

      if ( combined_tests(c,i-1,i,0,tt,ct) ) {
         mark_and_untie(c,i-1,i);
         mark_and_untie(c,i,0);
      }
   }
}

bool dbsol_geno_shock::
test_stencil(dbsol_geno_curve_2d const &c, unsigned i)
{
   unsigned iprv, inxt;
   if (c.closed()) {
      iprv= c.prev_circular(i);
      inxt= c.next_circular(i);
   } else {
      if (i == 0 || i >= c.size())
         return false;
      iprv= i-1;
      inxt= i+1;
   }
   
   return test_stencil(c,iprv,i,inxt);
}

/* TODO
inline bool dbsol_geno_shock::
test_stencil(dbsol_gshock_curve const &c,
      unsigned i)
{
   if (!c.closed() && i == 0 || i == c.size())
      return false;

   if !c.closed()
      return c.is_upper(i-1) && c.is_lower(i+1);
   else {

      return c.is_upper(i-1) && c.is_lower(i+1);
   }

}
*/

bool dbsol_geno_shock::
test_change(dbsol_gshock_curve_2d const &c, unsigned i, double tt, double ct)
{
   unsigned iprv,inxt;
   if (c.closed()) {
      iprv= c.prev_circular(i);
      inxt= c.next_circular(i);
   } else {
      if (i == 0 || i >= c.size())
         return false;
      iprv= i-1;
      inxt= i+1;
   }

   return test_change(c,iprv,i,inxt,tt,ct);
}



bool dbsol_geno_shock::
test_change(dbsol_geno_curve_2d const &c, unsigned i, double tt, double ct)
{
   unsigned iprv,inxt;
   if (c.closed()) {
      iprv= c.prev_circular(i);
      inxt= c.next_circular(i);
   } else {
      if (i == 0 || i >= c.size())
         return false;
      iprv= i-1;
      inxt= i+1;
   }

   return test_change(c[iprv],c[i],c[inxt],tt,ct);
}

void  dbsol_geno_shock::
make_all_shocks(dbsol_gshock_curve_2d &c, const double shk_dist_thresh)
{
   unsigned i;

   c.s_.resize(c.size(),0);
   
   if (!c.closed()) {
      for (i=1; i < c.size()-1; ++i)
         if (c.has_shock(i)) {
            c.s_[i] = new dbsol_gshock(c.interval(i-1), c.interval(i), c.interval(i+1),
                                       shk_dist_thresh);
         }
   } else {

      if ( c.has_shock(0) )
            c.s_[0] = new dbsol_gshock(c.interval(c.size()-1), c.interval(0),
                                       c.interval(1), shk_dist_thresh);
      for (i=1; i < c.size()-1; ++i)
         if ( c.has_shock(i) )
            c.s_[i] = new dbsol_gshock(c.interval(i-1), c.interval(i), c.interval(i+1),
                                       shk_dist_thresh);
      if ( c.has_shock(i) )
         c.s_[i] = new dbsol_gshock(c.interval(i-1), c.interval(i), c.interval(0),
                                       shk_dist_thresh);
   }
}

void dbsol_geno_shock::
place_all_shocks( dbsol_gshock_curve_2d &c,
                  double tangent_threshold, double curvature_threshold, 
                  double shock_threshold)
{
   mark_all_shocks(c,tangent_threshold,curvature_threshold);
   make_all_shocks(c,shock_threshold);
}
