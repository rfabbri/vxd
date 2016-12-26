// This is basic/dbnl/dbnl_eno_zerox.h
#ifndef bnl_eno_zerox_h
#define bnl_eno_zerox_h
//:
// \file
// \brief Zerocrossings from polynomial functions and interval interpolants
// \author Ricardo Fabbri, LEMS, Brown University (rfabbri.github.io)
// \date Feb 2005
//

#include <dbnl/algo/dbnl_eno.h>
#include <dbnl/algo/dbnl_eno_shock.h>
//: used in dbnl_eno_zero_crossing; cant be static const in all platforms
#define DBNL_ENO_MAX_ZEROX   2*DBNL_ENO_MAX_ORDER    

// ---------------------------------------------------------------------------
//: This class finds and stores the roots of polynomials(dbnl_eno_poly) and
// interpolants (dbnl_eno_interp)
// The roots are always stored in sorted order.
// Currently it does not distiguish roots from zero-crossings. Maybe
// zero_crossing is not appropriate name for the class.
class dbnl_eno_zero_crossing {
   public:
   dbnl_eno_zero_crossing () : number_(0) { };
   //: number of zeros in interval
   unsigned number() const  {return number_;}
   double  location(unsigned i) const  {return loc_[i];}
   unsigned compute(const dbnl_eno_poly &poly);
   unsigned compute(const dbnl_eno_poly &poly, double start, double end);
   unsigned compute(const dbnl_eno_interp &ip);
   unsigned compute(const dbnl_eno_shock_interp &ip);


   void print(vcl_ostream& = vcl_cerr) const;    
   bool combine(const dbnl_eno_zero_crossing *z1, const dbnl_eno_zero_crossing *z2);
   
   //: Sort increasing
   void sort() { vcl_sort(loc_.begin(),loc_.begin()+number_); }
   void remove_duplicates(); 

protected:
   unsigned number_;
   vnl_vector_fixed <double,DBNL_ENO_MAX_ZEROX + 1> loc_;
// double slope_[eno::max_zerox + 1];  (removed: not used)

   bool is_almost_zero(double x) const {return vcl_fabs(x) < dbnl_eno::near_zero_value;}
   bool is_almost_equal(double x1, double x2, double tolerance) const
      { return vcl_fabs((double) x1 - x2) < tolerance; }
   bool is_in_interval(double val, double start, double end) const
      { return val >= start && val <= end; }
   int  check_in_interval(const dbnl_eno_poly *poly, int index, double start, double end);
};

//: Compute zerocrossing vector from dbnl_eno_1d and dbnl_eno_shock_1d classes
// We do not want zerocrossing stuff to bloat those classes, so we made this
// compute object.
class dbnl_eno_zerox_vector {
public:
   dbnl_eno_zerox_vector(const dbnl_eno_1d &eno);
   dbnl_eno_zerox_vector(const dbnl_eno_shock_1d &eno);
   ~dbnl_eno_zerox_vector() {}

   const dbnl_eno_zero_crossing & operator [] (unsigned i) const { return z_[i];}
   const dbnl_eno_zero_crossing & zeros(unsigned i) const { return z_[i]; }

   unsigned size() const {return z_.size();};

   //: total number of zeros in all intervals
   unsigned number() const {return number_;};

   void print (vcl_ostream&) const;
   //: Prints alongside corresponding intervals
   void print (dbnl_eno_1d &eno,vcl_ostream&) const;
   //: Prints alongside corresponding intervals
   void print (dbnl_eno_shock_1d &eno,vcl_ostream&) const;

//   compute(dbnl_eno_shock_1d &eno);
protected:
   vcl_vector<dbnl_eno_zero_crossing> z_;
   unsigned number_; //:< total qty of zeros in all intervals
};

#endif // bnl_eno_zerox_h
