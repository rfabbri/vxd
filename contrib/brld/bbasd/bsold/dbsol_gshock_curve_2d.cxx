//:
// \file
// \author Ricardo Fabbri
#include "dbsol_gshock_curve_2d.h"
#include <dbsol/dbsol_gshock.h>

void dbsol_gshock_curve_2d::
cache_measures()
{
   m_.resize(ints_.size());
   for (unsigned i=0; i<ints_.size(); ++i)
      m_[i].compute(*(ints_[i]));
}


//: blanking parameter not supported
void dbsol_gshock_curve_2d::
describe(vcl_ostream &strm, int blanking) const
{
  if (blanking < 0) blanking = 0; while (blanking--) strm << ' ';

  strm << "=== dbsol_geno_curve_2d ===\n";
  strm << "#intervals: " << ints_.size() << "\t length: " << length() << vcl_endl;
  for (unsigned int i=0; i<ints_.size(); ++i) {
     strm << "=== Interval " << i << " ===" << vcl_endl;
//     ints_[i]->print(strm);
     strm << "Cummulative length: " << lengths_[i] << vcl_endl;
     strm << "Stencil position: " << stencil_[i] << vcl_endl;
     strm << "Has Shock? " << (has_shock(i)? "true": "false") << vcl_endl;

     if (has_shock(i) && shock(i)) {
        strm << "#Intecepts: " << shock(i)->n_intercepts() << vcl_endl;
     }
  }
}
