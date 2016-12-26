// This is basic/dbsol/dbsol_interp_curve_2d.cxx
#include "dbsol_interp_curve_2d.h"
#include <vsol/vsol_point_2d.h>
#include <vcl_algorithm.h>  
//:
// \file
// \author Ozge Can Ozcanli (design/declarations) 
// \author Ricardo Fabbri (definitions)
 

//: Constructor
// Caution: the user of this constructor SHALL NOT delete the dbgl_param_curve objects
// pointed to by the 'inter' vector 
dbsol_interp_curve_2d::
dbsol_interp_curve_2d(vcl_vector<dbgl_param_curve *> inter)
    : ints_(inter), lengths_(ints_.size())
{
   lengths_[0] = ints_[0]->length();
   for (unsigned i = 1; i<ints_.size(); i++)
     lengths_[i]=lengths_[i-1] + ints_[i]->length();
}

void dbsol_interp_curve_2d::
make(const vcl_vector<dbgl_param_curve *> &inter)
{
  for (unsigned i = 0; i<ints_.size(); i++) {
    delete ints_[i];
  }

   ints_ = inter;
   lengths_.resize(ints_.size());

   if (ints_.size() > 0) {
     lengths_[0] = ints_[0]->length();
     for (unsigned i = 1; i<ints_.size(); i++) {
       lengths_[i]=lengths_[i-1] + ints_[i]->length();
     }
   }
}


//: if s is out of bounds, we extrapolate using the interpolants at the endpoints.
vsol_point_2d_sptr dbsol_interp_curve_2d::
point_at(double s) const
{
  double s_from_beginning_of_interval;
  //unsigned is = interval_index(s,&s);
  unsigned is = interval_index(s,&s_from_beginning_of_interval);

  //vsol_point_2d_sptr p = new vsol_point_2d(ints_[is]->point_at_length(s));
  vsol_point_2d_sptr p = new vsol_point_2d(ints_[is]->point_at_length(s_from_beginning_of_interval));

  return p;
}

/*
vgl_point_2d<double> dbsol_interp_curve_2d::
point_at(double s) const
{
   vsol_point_2d_sptr sp = point_at(s);

   vgl_point_2d<double> p(sp->x(),sp->y());
   return p;
}
*/

vgl_vector_2d<double> dbsol_interp_curve_2d:: 
tangent_at(double s) const
{
   unsigned is = interval_index(s,&s);
   return ints_[is]->tangent_at_length(s);
}

double dbsol_interp_curve_2d:: 
tangent_angle_at(double s) const
{
   unsigned is = interval_index(s,&s);
   return ints_[is]->tangent_angle_at_length(s);
}


//: Finds index of the interval containing a given arclength of the curve, using
// binary search.
// If s correspond to one of the endpoints/samples, we return the index of the
// interval to the "left" of it. 
// If s is 0, returns the index of the first interval; if s is equal to
// the length of the curve, returns the index of the last interval. If you are
// working with closed curves, your program is responsible for dealing with this
// convention, as it may imply two different measures for s==0 and s==length,
// even though they both correspond to the same point.
// If s is out of bounds, the result is undefined (or halt if macro NDEBUG is not
// defined).
unsigned dbsol_interp_curve_2d::
interval_index(double s) const
{
  if (s < 0 || is_almost_zero(s)) {
      s = 0.0;
  } else {
    
    if (s > length() || is_almost_equal(s,length())) {
      s = length();
    
    } else {

      assert (s >= 0 && s <= length());
    }

  }
   

   // binary search for s in vector of arclens
   const vcl_vector<double>::const_iterator
      p = vcl_lower_bound(lengths_.begin(), lengths_.end(), s);

   // remove this
   //vcl_cout << "WARNING assertion on: p: lenth size: " << lengths_.size() << "\n";
   //vcl_cout.flush();  

   assert (p <  lengths_.begin() + lengths_.size() );

   return p - lengths_.begin();
}


//: Same as interval_index(double s), but also returns the remaining
// arclength parameter s on the parametric curve representing the interval.
unsigned dbsol_interp_curve_2d::
interval_index(double s, double *t) const
{
   unsigned is = interval_index(s);

   if (is == 0)
      *t = s;
   else
      *t = s - lengths_[is-1];

   return is;
}

//: Print an ascii summary to the stream
void dbsol_interp_curve_2d::
print_summary(vcl_ostream &os) const
{
  os << *this;
}


//: blanking parameter not supported
void dbsol_interp_curve_2d::
describe(vcl_ostream &strm, int blanking) const
{
  if (blanking < 0) blanking = 0; while (blanking--) strm << ' ';

  strm << "=== dbsol_interp_curve_2d ===\n";
  strm << "#intervals: " << ints_.size() << "\t length: " << length() << vcl_endl;
  for (unsigned int i=0; i<ints_.size(); ++i) {
     strm << "=== Interval " << i << " ===" << vcl_endl;
  // ints_[i]->print(strm);
     strm << "Cummulative length: " << lengths_[i] << vcl_endl;
  }

}
