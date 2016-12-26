// This is basic/dbnl/dbnl_eno.h
#ifndef bnl_eno_h
#define bnl_eno_h
//:
// \file
// \brief Essentially Non-Oscillatory 1D interpolation
// \author Ricardo Fabbri, LEMS, Brown University (rfabbri.github.io)
// \date Feb 2005
// \version 1.0
//
// This file contains class declarations regarding ENO interpolation.
// Its current state is stable but we are going towards version 2.0 whith a
// major rearrangement of class structure.
//
// \todo add a sample(t) or value_at(t) method for eno_poly and eno_1d 
// \todo move plain eno to dbnl, and all the other stuff to algo
//
// \verbatim
// Modifications
// Feb 04 2005  providing operator overloading and accessor functions for
// manipulating the interpolated data.
// \endverbatim

#include <vcl_iostream.h>
#include <vcl_vector.h>
#include <vcl_algorithm.h>
#include <vcl_limits.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_vector_fixed.h>

//: Constants
// This class only defines some constants and compile-time parameters related to
// the algorithm. 
// The code looks ugly so that it works with VS 6.0
class dbnl_eno {
public:
#define DBNL_ENO_MAX_ORDER 2
#define DBNL_ENO_DATA_LENGTH (DBNL_ENO_MAX_ORDER + 2)
//: Maximum number of zero crossings in an interval.
// A shock interval has 2 polys, and, hence, twice the number of max roots
   static const double near_zero_value
      VCL_STATIC_CONST_INIT_FLOAT_DECL(1.0e-8);
   static const double interval_tolerance
      VCL_STATIC_CONST_INIT_FLOAT_DECL(1.0e-8);
};

// ---------------------------------------------------------------------------
//: This class is a very simple, static-array polynomial of order 2
// vnl_real_polynomial is not used because it can be inefficient for specific
// 2nd order ENO.
class dbnl_eno_poly {
public:
   static const unsigned zero_order_index   VCL_STATIC_CONST_INIT_INT_DECL(0);
   static const unsigned first_order_index  VCL_STATIC_CONST_INIT_INT_DECL(1);
   static const unsigned second_order_index VCL_STATIC_CONST_INIT_INT_DECL(2);

   dbnl_eno_poly() { }
   dbnl_eno_poly(unsigned degree) {order_ = degree;}
   ~dbnl_eno_poly() { }

   unsigned order() const  {return order_;}
   double coeff(unsigned i) const  {return coeffs_[i];}
   void print(vcl_ostream& = vcl_cerr) const;    

   double sample(double x) const;

   double derivative(double loc) const; 

   //: Access to the polynomial coefficients
   double& operator [] (unsigned i)       { return coeffs_[i]; }
   //: Access to the polynomial coefficients
   double  operator [] (unsigned i) const { return coeffs_[i]; }
 
protected:
   double coeffs_[DBNL_ENO_MAX_ORDER+1];
   unsigned order_;
};

//: Difference of two polynomials
//\relates dbnl_eno_poly
dbnl_eno_poly operator-(const dbnl_eno_poly &f1, const dbnl_eno_poly &f2);


inline double  dbnl_eno_poly ::
derivative(double t) const
{
   return coeffs_[dbnl_eno_poly::first_order_index] +
          2.0 * coeffs_[dbnl_eno_poly::second_order_index] * t;
}

// ---------------------------------------------------------------------------
//: This class represents an interval interpolated by a polynomial
// The additions of this class to a bare polynomial is that it stores the
// endpoints of the interval, and whether the stencil went forwards or
// backwards. It also has the method for interpolating an interval.
class dbnl_eno_interp : public dbnl_eno_poly {
public:
   dbnl_eno_interp(double start=0, double end=0) :
      // start is the index/coordinate of the first element in the interval
      // for this interp; end is the index/coordinate for the last
      // element (usually end = start + 1)
      start_ (start), end_ (end) { }
   ~dbnl_eno_interp() {}

   //: was compute_polynomial
   void interpolate (double const data[DBNL_ENO_DATA_LENGTH]);
   void interpolate(double const data[DBNL_ENO_DATA_LENGTH], 
                    double const    x[DBNL_ENO_DATA_LENGTH]);

   void interpolate(double d1, double d2, double d3, double x1, double x2, double x3);

   void set_bounds(double a, double b) {start_ = a; end_ = b;}
   double start() const {return start_;} 
   double end() const {return end_;} 
   bool forward() const {return forward_;} 
   void print(vcl_ostream& = vcl_cerr) const;    

private:
   bool forward_;
   double start_, end_;
};



// ---------------------------------------------------------------------------
//: This is the main class to use for eno interpolation of a 1D array
// It consists of an array of dbnl_eno_interp (interpolants). Example code:
// \code
// dbnl_eno1d e;
// ...
// vnl_vector<double> data(n);
// ... (add some data)
//
// e.interpolate(data);
//
// // e[i] the i-th interpolant
//
// e[i][2]; // coefficient of x^2
// e[i].print(); // print ith interval
// \endcode
//
// \sa
// This class is very easy to use, but it does not deal
// with ENO extensions such as shock-placing ENO. For that, see the child
// dbnl_eno_shock_1d class. Moreover, if you want to find the
// roots/zerocrossings of the polynomials or of the whole interpolated data, see
// the class dbnl_eno_zero_crossing.  This ENO library was also designed so you
// can work directly at each interval-level if you want. See dbnl_eno_interp for this.
// See also the examples directory.
//
// \remarks Currently the x-values (abcissas) are taken to be 0...n-1, // where n is the number of samples of the function.
//
class dbnl_eno_1d {
public: 
   dbnl_eno_1d(double border_value = vcl_numeric_limits<double>::max()/1e10) 
      : border_value_(border_value) { }

   ~dbnl_eno_1d() { }


   bool interps_been_computed() const {return interp_.size() != 0;}

   bool interpolate(double const *data, unsigned len, ptrdiff_t stride = 1); 
   bool interpolate(vnl_vector<double> const *data, ptrdiff_t stride = 1) 
      { return interpolate(data->data_block(),data->size(),stride); }

   bool interpolate(vnl_vector<double> const *data, vnl_vector<double> const *x,
         ptrdiff_t stride = 1) 
      { return interpolate(data->data_block(),x->data_block(),data->size(),stride); }

   bool interpolate(double const *data, double const *x, unsigned len, ptrdiff_t stride=1);

   const dbnl_eno_interp &operator [] (unsigned i) const { return interp_[i];}

   //: Access to the vector of interpolants
   vcl_vector<dbnl_eno_interp>& interp() {return interp_;}

   //: Access to the vector of abscissas
   double x(unsigned i) const {return abscissas_[i];}

   //: number of intervals == number of samples-1
   unsigned size() const {return interp_.size();}

   double sample(double x) const;
   bool sample(unsigned size, vnl_vector<double> &f, vnl_vector<double> &xx) const;
   unsigned interval_index(double x) const;

   void print(vcl_ostream& = vcl_cerr) const;
protected:
   vcl_vector<dbnl_eno_interp> interp_;    //:< one interpolant for each interval
   vnl_vector<double> abscissas_;
   ptrdiff_t stride_; //:< for use with images stored in big continuous memory blocks
   const double border_value_; //:< value of function outside of data bounds 
};


#endif // bnl_eno_h
