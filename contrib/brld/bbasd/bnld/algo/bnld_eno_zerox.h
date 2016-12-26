// This is bbasd/bnld/bnld_eno_zerox.h
#ifndef bnl_eno_zerox_h
#define bnl_eno_zerox_h
//:
// \file
// \brief Zerocrossings from polynomial functions and interval interpolants
// \author Based on original code by  Ricardo Fabbri, LEMS, Brown University (rfabbri.github.io)
// \date Feb 2005
//

#include <bnld/algo/bnld_eno.h>
#include <bnld/algo/bnld_eno_shock.h>
//: used in bnld_eno_zero_crossing; cant be static const in all platforms
#define DBNL_ENO_MAX_ZEROX   2*DBNL_ENO_MAX_ORDER    

// ---------------------------------------------------------------------------
//: This class finds and stores the roots of polynomials(bnld_eno_poly) and
// interpolants (bnld_eno_interp)
// The roots are always stored in sorted order.
// Currently it does not distiguish roots from zero-crossings. Maybe
// zero_crossing is not appropriate name for the class.
class bnld_eno_zero_crossing {
   public:
   bnld_eno_zero_crossing () : number_(0) { };
   //: number of zeros in interval
   unsigned number() const  {return number_;}
   double  location(unsigned i) const  {return loc_[i];}
   unsigned compute(const bnld_eno_poly &poly);
   unsigned compute(const bnld_eno_poly &poly, double start, double end);
   unsigned compute(const bnld_eno_interp &ip);
   unsigned compute(const bnld_eno_shock_interp &ip);


   void print(vcl_ostream& = vcl_cerr) const;    
   bool combine(const bnld_eno_zero_crossing *z1, const bnld_eno_zero_crossing *z2);
   
   //: Sort increasing
   void sort() { vcl_sort(loc_.begin(),loc_.begin()+number_); }
   void remove_duplicates(); 

protected:
   unsigned number_;
   vnl_vector_fixed <double,DBNL_ENO_MAX_ZEROX + 1> loc_;
// double slope_[eno::max_zerox + 1];  (removed: not used)

   bool is_almost_zero(double x) const {return vcl_fabs(x) < bnld_eno::near_zero_value;}
   bool is_almost_equal(double x1, double x2, double tolerance) const
      { return vcl_fabs((double) x1 - x2) < tolerance; }
   bool is_in_interval(double val, double start, double end) const
      { return val >= start && val <= end; }
   int  check_in_interval(const bnld_eno_poly *poly, int index, double start, double end);
};

//: Compute zerocrossing vector from bnld_eno_1d and bnld_eno_shock_1d classes
// We do not want zerocrossing stuff to bloat those classes, so we made this
// compute object.
class bnld_eno_zerox_vector {
public:
   bnld_eno_zerox_vector(const bnld_eno_1d &eno);
   bnld_eno_zerox_vector(const bnld_eno_shock_1d &eno);
   ~bnld_eno_zerox_vector() {}

   const bnld_eno_zero_crossing & operator [] (unsigned i) const { return z_[i];}
   const bnld_eno_zero_crossing & zeros(unsigned i) const { return z_[i]; }

   unsigned size() const {return z_.size();};

   //: total number of zeros in all intervals
   unsigned number() const {return number_;};

   void print (vcl_ostream&) const;
   //: Prints alongside corresponding intervals
   void print (bnld_eno_1d &eno,vcl_ostream&) const;
   //: Prints alongside corresponding intervals
   void print (bnld_eno_shock_1d &eno,vcl_ostream&) const;

//   compute(bnld_eno_shock_1d &eno);
protected:
   vcl_vector<bnld_eno_zero_crossing> z_;
   unsigned number_; //:< total qty of zeros in all intervals
};

#endif // bnl_eno_zerox_h
