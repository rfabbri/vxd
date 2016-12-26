// This is basic/dbsol/dbsol_geno_curve_2d.cxx
#include "dbsol_geno_curve_2d.h"

#include <dbgl/algo/dbgl_eulerspiral.h>
#include <dbgl/dbgl_arc.h>



//: blanking parameter not supported
void dbsol_geno_curve_2d::
describe(vcl_ostream &strm, int blanking) const
{
  if (blanking < 0) blanking = 0; while (blanking--) strm << ' ';

  strm << "=== dbsol_geno_curve_2d ===\n";
  strm << "#intervals: " << ints_.size() << "\t length: " << length() << vcl_endl;
  for (unsigned int i=0; i<ints_.size(); ++i) {
     strm << "=== Interval " << i << " ===" << vcl_endl;
  // ints_[i]->print(strm);
     strm << "Cummulative length: " << lengths_[i] << vcl_endl;
     strm << "Stencil position: " << stencil_[i] << vcl_endl; }
}

//: Returns the curvature at s arclength away from p0_
// This is an auxiliary function for high order interpolation.
double dbsol_geno_curve_2d::
curvature_at(double s) const
{
   unsigned is = interval_index(s,&s);

   return (reinterpret_cast<dbgl_arc*>(ints_[is]))->curvature_at_length(s);
}


//: Returns the curvature derivative at s arclength away from p0_
// This is an auxiliary function for high order interpolation.
// \todo Make sure this is a high enough order interpolation.
// Dynamic cast would be safer but is not used for speed purposes.
double dbsol_geno_curve_2d::
curvature_derivative_at(double s) const
{
   unsigned is = interval_index(s);
   return (reinterpret_cast<dbgl_eulerspiral*> (ints_[is]))->gamma();
}
