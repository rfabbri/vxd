#include "dbnl_eno_shock.h"
#include "dbnl_eno_zerox.h"
#include <vcl_cassert.h>


#if !VCL_STATIC_CONST_INIT_FLOAT_NO_DEFN
const double dbnl_eno_shock_1d::default_tangent_change_threshold
      VCL_STATIC_CONST_INIT_FLOAT_DEFN(20.0);
const double dbnl_eno_shock_1d::default_curvature_change_threshold
      VCL_STATIC_CONST_INIT_FLOAT_DEFN(0.3);
#endif



// ----------------------------------------------------------------------------
// dbnl_eno_shock_interp
// ----------------------------------------------------------------------------

void dbnl_eno_shock_interp::
print(vcl_ostream& strm) const
{
   strm << "==== Shock ====\n";
   if (no_intercept())
      strm << "Shock with no intersection" << vcl_endl;
   else
      strm << "Shock location: " << loc() << vcl_endl;
}


//:\return number of intersections of the continuations of neighboring intervals
unsigned dbnl_eno_shock_interp::
make(dbnl_eno_interp const *prev, dbnl_eno_interp const *curr,
      dbnl_eno_interp const *next) 

{
   dbnl_eno_zero_crossing zeros;
   dbnl_eno_poly sub = *prev - *next;
   unsigned valid_root_cnt = zeros.compute(sub, curr->start(), curr->end());

   /* if we found valid roots, create a new shock structure */
   if (valid_root_cnt != 0) {
      /* it doesn't seem like we should have shocks in successive
         intervals - will see if this ever occurrs. */


      // we are only using the first root found. Maybe we should do
      // something different if there are two?  TODO
#ifndef NDEBUG 
      if (valid_root_cnt == 2)
         vcl_cerr << "two roots/intersections, ignoring root at " << zeros.location(1) << vcl_endl;
#endif
      loc_ = zeros.location(0);
      no_intercept_=false;
   } else
      no_intercept_=true;
   prev_ = prev;
   next_ = next;

   return valid_root_cnt;
}

// ----------------------------------------------------------------------------
// dbnl_eno_shock_1d
// ----------------------------------------------------------------------------

bool dbnl_eno_shock_1d::
test( dbnl_eno_interp const *prv, dbnl_eno_measures const *prvm,
      dbnl_eno_interp const *cur, dbnl_eno_measures const *curm,
      dbnl_eno_interp const *nxt, dbnl_eno_measures const *nxtm,
      double shock_tangent_change_threshold, 
      double shock_curvature_change_threshold
      )
{

   // TODO: in the future we will probably use these parameters for the stencil
   // criterion: 
   // "the current interval is probably a shock if:
   // prv->forward == false && nxt->forward == true
   prv = cur = nxt = NULL; // just remove warning


   double delta_tan,            /* tangent change within interval */
    delta_tan_fwd,          /* holds difference of tangent between
                                   next and current interval in degrees.  */
    delta_tan_back,         /* same as above for cur and previous */
    delta_curv,                 /* curvature change within interval */
    delta_curv_fwd,         /* curvature change between next and
                                   cur interval.  */
    delta_curv_back;        /* same as above for cur and previous */
   bool large_tangent_change_p, /* true if tangent change is large */
    large_curvature_change_p,   /* true if curvature change is large */
    need_shock_p;           /* true if shock detected.  */

   /* compute changes in tangents and curvatures between successive
      intervals. large changes indicate a shock should be placed. */

   // Oddly, the criterium of neighboring stencils (described in
   // the paper) is ignored. - rfabbri

   /** Why doesn't Kaleem's original code consider the change within
    an interval? This seems to be the only way to capture shocks in
      certain configurations...i.e.
      
      0.   1.   4.   9.  16.  16.   9.   4.   1.   0.
      
      To place a shock correctly between the 16s, you need to look
      at the change in tangent _within_ an interval. I've added it
      here.  -Perry
      **/

   // only look to place shocks in regions of locally maximum curvature
   // I guess the orig authors meant && here
   if ( !(vcl_fabs(curm->total_curvature()) > vcl_fabs(prvm->total_curvature()) || 
       vcl_fabs(curm->total_curvature()) > vcl_fabs(nxtm->total_curvature())) )
      return false;

   delta_tan = 
      curm->angle_difference(curm->end_tangent(), curm->start_tangent());
   delta_tan_back = 
      curm->angle_difference(curm->start_tangent(), prvm->end_tangent());
   delta_tan_fwd = 
      curm->angle_difference(nxtm->start_tangent(), curm->end_tangent());

   /* set flag if either tangent exceeds the threshold */
   large_tangent_change_p = 
        (vcl_fabs(delta_tan_back) > shock_tangent_change_threshold || 
         vcl_fabs(delta_tan_fwd) > shock_tangent_change_threshold);

   delta_curv =
       curm->end_curvature() - prvm->start_curvature();
   delta_curv_back =
       curm->start_curvature() - prvm->end_curvature();
   delta_curv_fwd =
       nxtm->start_curvature() - curm->end_curvature();

   /* set flag if either curvature exceeds the threshold */
   large_curvature_change_p = 
      (vcl_fabs(delta_curv_back) > shock_curvature_change_threshold || 
       vcl_fabs(delta_curv_fwd)  > shock_curvature_change_threshold);


   /* if only delta_tan would cause this to be marked a shock,
      print out for testing... */
   if (!large_tangent_change_p && !large_curvature_change_p
       && vcl_fabs(delta_tan) > shock_tangent_change_threshold) {
      large_tangent_change_p = true;
      #ifndef NDEBUG
         vcl_cerr << "Classified as Shock because of internal interval tangent change.\n";
      #endif
   }

   /* try to place a shock if tangent or curvature change is large */
   need_shock_p = (large_tangent_change_p || large_curvature_change_p);

   //   if (need_shock_p) {

   //#ifdef DEBUG
   //         fprintf(stderr, "interval(%g,%g)\n", interp_.start_,
   //                 interp_.end_);
   //         fprintf(stderr, "\tdelta tang =: (%5.5g,%5.5g,%5.5g)\n",
   //                 delta_tan_back, delta_tan, delta_tan_fwd);
   //
   //         fprintf(stderr, "\tdelta curv =: (%5.5g,%5.5g,%5.5g)\n",
   //                 delta_curv_back, delta_curv, delta_curv_fwd);
   //#endif
   //         need_shock_p = this->make_shock(prev, next) > 0;
   //#ifdef DEBUG
   //         fprintf(stderr, "\n");
   //#endif
   //      } 
   //#ifdef DEBUG
   //   else {
   //         vcl_cout << " ***** no shock needed" << vcl_endl;
   //         this->print();
   //         vcl_cout << "*********"<<vcl_endl;
   //   }
   //#endif 
   //   }
   //#ifdef DEBUG
   //   vcl_cout << large_tangent_change_p  << " x " << large_curvature_change_p <<vcl_endl;
   //#endif

   return need_shock_p;
}

bool dbnl_eno_shock_1d::
test( dbnl_eno_interp const   *prv, 
      dbnl_eno_interp const   *cur, 
      dbnl_eno_interp const   *nxt, 
      double shock_tangent_change_threshold, 
      double shock_curvature_change_threshold
      )
{
   dbnl_eno_measures prvm, curm, nxtm;

   prvm.compute(*prv);
   curm.compute(*cur);
   nxtm.compute(*nxt);

   return test(prv, &prvm, cur, &curm, nxt, &nxtm, 
         shock_tangent_change_threshold, shock_curvature_change_threshold);
}

//: place shocks on all intervals
// \return true on success
bool dbnl_eno_shock_1d::
place_shocks( double tangent_change_thresh, 
                  double curvature_change_thresh)
{
#ifndef NDEBUG
   if (!interps_been_computed()) {
      vcl_cerr << "no interpolants\n";
      return false;
   }
#endif

   compute_measures();

   if (s_.size() == 0)
      s_.resize(interp_.size(),0);

   // TODO: better use stride_ stuff (special iterator??)
   for (unsigned i=1; i+1 < size() && !has_shock(i); ++i) {
      // test for shock presence
      if ( test(&interp_[i-1],&m_[i-1], &interp_[i], &m_[i], &interp_[i+1], &m_[i+1],
                tangent_change_thresh, curvature_change_thresh) ) {
         // make shock
         s_[i] = new dbnl_eno_shock_interp;

         s_[i]->make(&interp_[i-1],&interp_[i],&interp_[i+1]);
         nshocks_++;
      }

#ifndef NDEBUG
      if (has_shock(i-1) || has_shock(i+1)) {
        vcl_cerr << "Neighbor already has a shock.\n**** does that make sense?***\n";
        // TODO from this print/debug code, it is clear that the
        // shock-placing criteria have never been really solid. - rfabbri
        //======= I commented out the following code, as it is useful
        // just for testing; many useless tests were being done -- fabbri
        //
        // FIXME: keep only the shock with the greater curvature 
        //if (prev->interp.poly.coeff[kSecondOrderIndex] < 
//          cur->interp.poly.coeff[kSecondOrderIndex]){
//          enoDEL(prev->shock);
//          prev->shock = NULL;
//          printf("removing shock from interval (%f,%f)",
//          prev->interp.start,prev->interp.end);
//          }

        //  printf("**********placing shock at interval (%f,%f)",
        //         cur->interp.start, cur->interp.end);
      }
#endif
   }
   return true;
}

void dbnl_eno_shock_1d::
delete_shock(unsigned i)
{
   assert (s_.size()!=0 && has_shock(i));

   delete s_[i];
   nshocks_--;
}

// -----------------------------------------------------------------------------
// dbnl_eno_shock_1d
// -----------------------------------------------------------------------------

bool dbnl_eno_shock_1d::
test_shock( unsigned i, 
            double tangent_change_thresh, 
            double curvature_change_thresh) const
{
   if (i==0 || i+1 >=s_.size())
      return false;

   if (computed_measures())
      return test(&interp_[i-1], &m_[i-1], &interp_[i], &m_[i], &interp_[i+1], &m_[i+1],
         tangent_change_thresh, curvature_change_thresh); 

   return test(&interp_[i-1], &interp_[i], &interp_[i+1],
         tangent_change_thresh, curvature_change_thresh); 
}

void dbnl_eno_shock_1d::
compute_measures()
{
#ifndef NDEBUG
   if (!interps_been_computed()) {
      vcl_cerr << "error: interpolants not computed." << vcl_endl;
      return;
   }
#endif
   m_.resize(size());
   for (unsigned i=0; i<size(); ++i)
      m_[i].compute(interp_[i]);
}

void dbnl_eno_shock_1d::
make_shock(unsigned i)
{
#ifndef NDEBUG
   if (i==0 || i+1 >= interp_.size()) {
      vcl_cerr << "msg: we dont compute shock for boundary elements" << vcl_endl; 
      return;
   }
#endif

   if (s_.size() == 0)
      s_.resize(interp_.size(),0);
#ifndef NDEBUG
   else if (has_shock(i)) {
      vcl_cerr << "error: this interval already have a shock" << vcl_endl;
      return;
   }
#endif
   s_[i] = new dbnl_eno_shock_interp;
   s_[i]->make(&interp_[i-1],&interp_[i],&interp_[i+1]); 

   nshocks_++;
}

//:Destructor
dbnl_eno_shock_1d::
~dbnl_eno_shock_1d()
{
   for (unsigned i=0; i<s_.size(); ++i)
      if (has_shock(i))
         delete s_[i];
}


void dbnl_eno_shock_1d::
print(vcl_ostream& strm) const
{
   strm << "==== Eno 1D with Shocks ====" << vcl_endl
        << "len: " << size() << vcl_endl;

   unsigned i;
   for (i=0; i < size(); ++i) {
      interp_.at(i).print(strm);
      if (nshocks() && has_shock(i))
         s_.at(i)->print(strm);
      if (computed_measures())
         m_.at(i).print(strm);
   }
}
