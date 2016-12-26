#include <vcl_cassert.h>
#include "dbnl_eno_zerox.h"

//:
// \file
// \author Ricardo Fabbri


//: Find zeros of polynomial
//\return number of zeros
unsigned dbnl_eno_zero_crossing::
compute(const dbnl_eno_poly &poly)
{
    double const
      a = poly[dbnl_eno_poly::second_order_index], 
      b = poly[dbnl_eno_poly::first_order_index], 
      c = poly[dbnl_eno_poly::zero_order_index]; 
    double
      discriminant;
    
    discriminant = (b*b) - (4.0 * a*c);
    
    number_ = 0;

    if ( discriminant < 0.0 ){
        /* roots are imaginary, nothing to do */

    } else if (is_almost_zero(a)){
        /* the polynomial is a straight line, so at most one root */
        if (!is_almost_zero(b)) {
          double intercept =  -c / b;
          number_ = 1;
          loc_[0] = intercept;
        }
    } else {
        /* two real roots */
       double root_minus = (-b - sqrt(discriminant))/(2.0 * a);
       double root_plus  = (-b + sqrt(discriminant))/(2.0 * a);
       number_ = 2;
       
       loc_[0] = root_minus;
       loc_[1] = root_plus;
    }
    return number_;
}

//: Find zeros of polynomial, but restrict them only btween start and end
//\return number of zeros
unsigned dbnl_eno_zero_crossing::
compute(const dbnl_eno_poly &poly, double start, double end)
{
   int valid_root_cnt = 0,
       root_cnt = compute(poly);

   /* if (root_cnt == 0) { // only imaginary roots } */

   if (root_cnt >= 1 && check_in_interval(&poly,0,start,end))
       valid_root_cnt++;

   if (root_cnt >= 2 && check_in_interval(&poly,1,start,end)) {
      /* if this the first valid root, store it in loc[0] */
      if (++valid_root_cnt == 1)
          loc_[0] = loc_[1];
   }
   return number_ = valid_root_cnt;
}

unsigned dbnl_eno_zero_crossing::
compute(const dbnl_eno_interp &ip)
{
   compute(ip, ip.start(), ip.end());
    
   // the original code did the following, but I didn't figure out a good reason
   // why it does it
   if (number() > 0) { 
      sort();
      remove_duplicates();
   }
   return number();
}

unsigned  dbnl_eno_zero_crossing::
compute(const dbnl_eno_shock_interp &ip)
{
   double shock_loc = ip.loc();
   dbnl_eno_zero_crossing prev_zeros, next_zeros;

   // When there is a shock, Kaleem originally was checking the
   // ENTIRE current interval for zerox of the neighboring intervals.
   //
   // I'm restricting it to search only PART of the interval for
   // zero crossings:
   // zerox(prev_interpolant) within (cur_interval_start,shock_loc)
   // zerox(next_interpolant) within (shock_loc,cur_interval_end)
   if (ip.no_intercept()) {
      prev_zeros.compute(ip.prev(),ip.prev().end(),ip.next().start());
      next_zeros.compute(ip.next(),ip.prev().end(),ip.next().start());
   } else {
      prev_zeros.compute(ip.prev(), ip.prev().end(), shock_loc);
      next_zeros.compute(ip.next(), shock_loc, ip.next().start());
   }
   combine(&prev_zeros,&next_zeros);

   if (number() > 0) { 
      sort();
      remove_duplicates();
   }
   return number();
}

//: Tests if zerocrossing in loc_[index] is in between start and end.
int dbnl_eno_zero_crossing::
check_in_interval(const dbnl_eno_poly *poly, int index, double start, double end)
{
   int in_interval = 0;
   double loc_idx = loc_[index];
 
   /* If we ignored the leading term of the polynomial, I'm going to be
       a little lenient about the end points. This is necessary because
       the zero crossings shift slightly when you ignore the leading
       polynomial term, possibly placing them outside the interval by a
       very small amount. If this is the case, I'm clamping the return
       location to the appropriate end point */
   if (is_almost_zero( (*poly)[dbnl_eno_poly::second_order_index] )) {
      if (is_almost_equal(loc_idx, start, dbnl_eno::interval_tolerance)) {
        in_interval++;
        loc_[index] = start;
      }
      else if (is_almost_equal(loc_idx, end, dbnl_eno::interval_tolerance)) {
        in_interval++;
        loc_[index] = end; /*was start;  Seth made this change, 7-3-97*/
      }
   }
   
   /* haven't classified location as in interval, is it strictly
      in the interval?*/
   if (!in_interval && is_in_interval(loc_idx,start,end))
      in_interval++;

   return in_interval;
}

void dbnl_eno_zero_crossing::
remove_duplicates()
{
   if (number() > 1 && 
       is_almost_equal(location(0),location(1),dbnl_eno::near_zero_value))
      --number_;
}

//: Merges/concatenates objects z1 and z2 into the current object. More
//precisely, it first combines z1 with the current object, checking if there are
//no repeated zeros in the two. It then combines z2 into the result by the same
//procedure.
//\section Remarks
//The original code is broken.
// \return true on success
bool dbnl_eno_zero_crossing::
combine(const dbnl_eno_zero_crossing *z1, const dbnl_eno_zero_crossing *z2)
{

   unsigned i,k; bool found;

   for (i=0; i < z1->number(); ++i) {
      found = false;
      for (k=number()-1; k != (unsigned)-1 && !found; --k)
         found = is_almost_equal(loc_[k],z1->location(i),dbnl_eno::near_zero_value);

      if (!found)
         loc_[number_++] = z1->location(i);

      // these asserts here attest that the original authors didn't tought it was
      // barely possible to have more than two intersections in a shock
      // interpolant, although we are merging zeros of two degree 2 polynomials
      assert (number_ <= DBNL_ENO_MAX_ZEROX);
   }

   for (i=0; i < z2->number(); ++i) {
      found = false;
      for (k=number()-1; k != (unsigned)-1 && !found; --k)
         found = is_almost_equal(loc_[k],z2->location(i),dbnl_eno::near_zero_value);

      if (!found)
         loc_[number_++] = z2->location(i);

      assert (number_ <= DBNL_ENO_MAX_ZEROX);
   }

   return true;
}

//: Detects zero-crossings in each interval
// \return true on success
dbnl_eno_zerox_vector::
dbnl_eno_zerox_vector(const dbnl_eno_1d &eno)
{
#ifndef NDEBUG
   if (!eno.interps_been_computed()) {
      vcl_cerr << "eno object with no interpolation data" << vcl_endl;
      // TODO: improve error treatment
      return;
   }
#endif

   z_.resize(eno.size());

   for (unsigned i=0; i<z_.size(); ++i)
      z_[i].compute(eno[i]);
}

dbnl_eno_zerox_vector::
dbnl_eno_zerox_vector(const dbnl_eno_shock_1d &eno)
  :
  number_(0)
{
   if (!eno.shocks_computed())  {
      z_.resize(eno.size());

      for (unsigned i=0; i<z_.size(); ++i) {
         z_[i].compute(eno[i]);
         number_ += z_[i].number();
      }
   } else {
      z_.resize(eno.size());

      for (unsigned i=0; i<z_.size(); ++i) {
         if (eno.has_shock(i))
            z_[i].compute(eno.shock(i));
         else
            z_[i].compute(eno[i]);
         number_ += z_[i].number();
      }
   }
}

void dbnl_eno_zero_crossing::
print(vcl_ostream &strm) const
{
   strm << "==== Zeros ====" << vcl_endl
        << "quantity: " << number() << vcl_endl;

   if (number() != 0)
      strm << "location: " << vcl_endl;

   for (unsigned i=0; i<number(); ++i)
      strm << location(i) << "  ";
   strm << vcl_endl;
}

void dbnl_eno_zerox_vector::
print(vcl_ostream& strm) const
{

   strm << "==== Zerox Vector ====" << vcl_endl
        << "len: " << z_.size() << vcl_endl;

   unsigned i;
   for (i=0; i < z_.size(); ++i)
      z_.at(i).print(strm);
}

void dbnl_eno_zerox_vector::
print(dbnl_eno_1d &eno, vcl_ostream& strm) const
{
   strm << "==== Eno 1D with Zerocrossing ====" << vcl_endl
        << "len: " << eno.size() << vcl_endl;

   unsigned i;
   for (i=0; i < eno.size(); ++i) {
      eno[i].print(strm);
      z_[i].print(strm);
   }
}

void dbnl_eno_zerox_vector::
print(dbnl_eno_shock_1d &eno, vcl_ostream& strm) const
{
   strm << "==== Eno 1D with Shocks ====" << vcl_endl
        << "len: " << eno.size() << vcl_endl;

   unsigned i;
   for (i=0; i < eno.size(); ++i) {
      strm << "--------------------------------------------\n";
      eno[i].print(strm);
      if (eno.nshocks() && eno.has_shock(i))
         eno.shock(i).print(strm);
      if (eno.computed_measures())
         eno.measures(i).print(strm);
      if (z_.size() != 0)
         z_.at(i).print(strm);
      else
         strm << "No zeros\n";
   }
}
