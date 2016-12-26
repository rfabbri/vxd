// This is basic/dbsol/dbsol_interp_curve_3d.cxx
#include "dbsol_interp_curve_3d.h"
#include <vsol/vsol_point_3d.h>
#include <vcl_algorithm.h>  
#include <vcl_cstring.h>
#include <vcl_cstdio.h>
//:
// \file
// \author H. Can Aras (design/declarations) 

//: Constructor
// Caution: the user of this constructor SHALL NOT delete the dbgl_param_curve_3d objects
// pointed to by the 'inter' vector 
dbsol_interp_curve_3d::dbsol_interp_curve_3d(vcl_vector<dbgl_param_curve_3d *> inter)
    : ints_(inter), lengths_(ints_.size())
{
   lengths_[0] = ints_[0]->length();
   for (unsigned i = 1; i<ints_.size(); i++)
     lengths_[i]=lengths_[i-1] + ints_[i]->length();
}

void dbsol_interp_curve_3d::make(const vcl_vector<dbgl_param_curve_3d *> &inter)
{
   for (unsigned i = 0; i<ints_.size(); i++)
      delete ints_[i];

   ints_ = inter;
   lengths_.resize(ints_.size());

   lengths_[0] = ints_[0]->length();
   for (unsigned i = 1; i<ints_.size(); i++)
     lengths_[i]=lengths_[i-1] + ints_[i]->length();
}

//: if s is out of bounds, we extrapolate using the interpolants at the endpoints.
vsol_point_3d_sptr dbsol_interp_curve_3d::point_at(double s) const
{
   unsigned is = interval_index(s,&s);
   return new vsol_point_3d(ints_[is]->point_at_length(s));
}

vgl_vector_3d<double> dbsol_interp_curve_3d::tangent_at(double s) const
{
   unsigned is = interval_index(s,&s);
   return ints_[is]->tangent_at_length(s);
}

vgl_vector_3d<double> dbsol_interp_curve_3d::normal_at(double s) const
{
   unsigned is = interval_index(s,&s);
   return ints_[is]->normal_at_length(s);
}

struct tangent_angles dbsol_interp_curve_3d::tangent_angles_at(double s) const
{
   unsigned is = interval_index(s,&s);
   return ints_[is]->tangent_angles_at_length(s);
}

double dbsol_interp_curve_3d::curvature_at(double s) const
{
   unsigned is = interval_index(s,&s);
   return ints_[is]->curvature_at_length(s);
}

double dbsol_interp_curve_3d::torsion_at(double s) const
{
   unsigned is = interval_index(s,&s);
   return ints_[is]->torsion_at_length(s);
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
unsigned dbsol_interp_curve_3d::interval_index(double s) const
{
   if (s < 0 && is_almost_zero(s))
      s = 0.0;
   else if (s > length() && is_almost_equal(s,length()))
      s = length();
   else {
      assert (s >= 0 && s <= length());
   }

   // binary search for s in vector of arclens
   const vcl_vector<double>::const_iterator
      p = vcl_lower_bound(lengths_.begin(), lengths_.end(), s);

   // remove this
   assert (p <  lengths_.begin() + lengths_.size() );

   return p - lengths_.begin();
}

//: Same as interval_index(double s), but also returns the remaining
// arclength parameter s on the parametric curve representing the interval.
unsigned dbsol_interp_curve_3d::interval_index(double s, double *t) const
{
   unsigned is = interval_index(s);

   if (is == 0)
      *t = s;
   else
      *t = s - lengths_[is-1];

   return is;
}

//: Print an ascii summary to the stream
void dbsol_interp_curve_3d::print_summary(vcl_ostream &os) const
{
  os << *this;
}


//: blanking parameter not supported
void dbsol_interp_curve_3d::describe(vcl_ostream &strm, int blanking) const
{
  if (blanking < 0) blanking = 0; while (blanking--) strm << ' ';

  strm << "=== dbsol_interp_curve_3d ===\n";
  strm << "#intervals: " << ints_.size() << "\t length: " << length() << vcl_endl;
  for (unsigned int i=0; i<ints_.size(); ++i) 
  {
     strm << "=== Interval " << i << " ===" << vcl_endl;
     dbgl_eno_curve_3d *p = (dbgl_eno_curve_3d *) ints_[i];
     if((typeid(*ints_[i]) == typeid(dbgl_eno_curve_3d)))
     {
       vcl_cout << "t start: " << p->start_t() << " start point: " << p->point_at_t(p->start_t()) << vcl_endl;
       vcl_cout << "t end: " << p->end_t() << " end point: " << p->point_at_t(p->end_t()) << vcl_endl;
       vcl_cout << "x-Coefficients: ";
       vcl_cout << p->coef_x(0) << " " << p->coef_x(1) << " " << p->coef_x(2) << " " << p->coef_x(3) << vcl_endl;
       vcl_cout << "y-Coefficients: ";
       vcl_cout << p->coef_y(0) << " " << p->coef_y(1) << " " << p->coef_y(2) << " " << p->coef_y(3) << vcl_endl;
       vcl_cout << "z-Coefficients: ";
       vcl_cout << p->coef_z(0) << " " << p->coef_z(1) << " " << p->coef_z(2) << " " << p->coef_z(3) << vcl_endl;
     }
     vcl_cout << typeid(*ints_[i]).name() << vcl_endl;
     strm << "Cummulative length: " << lengths_[i] << vcl_endl;
  }
}


