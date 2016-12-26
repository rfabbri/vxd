// This is basic/dbsol/dbsol_interp_curve_2d.h
#ifndef dbsol_interp_curve_2d_h_
#define dbsol_interp_curve_2d_h_
//:
// \file
// \brief Represents a 2D interpolated curve
// \author Ozge Can Ozcanli
// \date 02/23/05
//
//  An interpolated curve is a series of intervals, each of which is a
//  parametric curve (from dbgl_param_curve). 
//  There are N intervals, which are given to the constructor, together with
//  arclengths of each interval
//  --> lengths_[0] for first and lengths_[N-1] for last interval, etc.
//  so length of the whole curve is in lengths_[N-1]
//  It is possible to continuously
//  sample from an interpolated curve.  The parameter for each interval is in
//  the [0-li] range, where li is the arclength of the i-th interval. This class
//  shall also provide a [0-N] non-uniform parametrization scheme, which is
//  linear in terms of the natural parameter of the curve, which may not be
//  arclength. This should be faster than arclength-based queries for certain
//  curves.
//
//  An arclength vector is kept for fast location of the intervals. Given
//  arclenth s, the interval is found and the sample from that interval is
//  fetched by supplying the remaining arclength  s - lengths_[i-1] to the
//  point_at_length method of the parametric curve.
//
//  In a meeting of August 8 2005, it was conventioned that the [0-N] parameter
//  interface will be designed to work as follows. Sample at parameter t ==
//  integer will be a original sample point. Samples at t == non-integer double
//  value will be somewhere between interval  [floor(t), ceil(t)].  And it will
//  use the natural [0,1] parametrization of the parametric curve, which might
//  not be uniform, but should be efficient. 
//
// \remarks
//  No need for a notion of openness or closeness, it is the responsibility of
//  constructor of intervals (interpolator) to maintain such a property during
//  construction
//
// \todo Do we have to add cast_to_interp_curve in vsol_curve_2d?
//
//    
// \verbatim Modifications
//   
// \endverbatim

#include <vcl_vector.h>
#include <vcl_iostream.h>
#include <dbgl/dbgl_param_curve.h>
#include <vsol/vsol_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vgl/vgl_point_2d.h>
#include <vcl_limits.h>
#include <vcl_cmath.h>  

class dbsol_interp_curve_2d : public vsol_curve_2d
{
 // PUBLIC INTERFACE----------------------------------------------------------
 public:
  // Constructors/Destructors--------------------------------------------------
  dbsol_interp_curve_2d() {};
  dbsol_interp_curve_2d(vcl_vector<dbgl_param_curve *> inter);
  dbsol_interp_curve_2d( const dbsol_interp_curve_2d& that): vsol_curve_2d(that) { *this = that; }
  
  //: deletes internal storage
  ~dbsol_interp_curve_2d() {
    for (unsigned i = 0; i<ints_.size(); i++)
      delete ints_[i];
    ints_.clear();
  }

  //: Assignment operator
  dbsol_interp_curve_2d& operator=( dbsol_interp_curve_2d const& that)
   {
     lengths_ = that.lengths_;
     ints_.resize(that.ints_.size());
     for (unsigned i=0; i<that.ints_.size(); ++i)
       ints_[i] = (that.ints_[i])->clone();
     return *this;
   }

  //: same as constructor but may be called anytime
  void make(const vcl_vector<dbgl_param_curve *> & inter);
   
  virtual vsol_spatial_object_2d* clone(void) const 
   { return new dbsol_interp_curve_2d(*this); }

  // Operators----------------------------------------------------------------

  virtual vsol_point_2d_sptr point_at(double s) const;

  //: Returns i-th curve sample
  vsol_point_2d_sptr point_at_sample(unsigned i) const
    { return point_at(length_at(i)); }
  
  //: Returns the tangent at s arclength away from p0_
  virtual vgl_vector_2d<double> tangent_at(double s) const;

  //: Returns the angle (in radian) in [0, 2PI) at s arclength away from p0_
  virtual double tangent_angle_at(double s) const;

  //: interpolation 2nd degree
  //double curvature_at(double s);

  //: Arc length at i-th _sample_
  double length_at(unsigned i) const
    { return (i==0)? 0 : lengths_[i-1]; }

  //: length of interpolated curve from p0_ to p1_
  virtual double length() const
     { return lengths_.back(); }
  
  //: number of intervals == number of samples minus 1
  unsigned size() const
     { return ints_.size(); }

  // Data Access---------------------------------------------------------------

  //: return vector of intervals (parametric curves)
  vcl_vector<dbgl_param_curve *> const & intervals() const { return ints_; }

  void set_intervals(vcl_vector<dbgl_param_curve *> const &intervals) 
     { ints_ = intervals; }

  //: easy access of intervals using an array subscript
  dbgl_param_curve const & operator [] (unsigned i) const { return *(ints_[i]);}

  dbgl_param_curve const * interval(unsigned i) const { return ints_[i];}

  //: Return the first point of `this'
  virtual vsol_point_2d_sptr p0() const // pure virtual of vsol_curve_2d
  {  vgl_point_2d<double> gp = ints_[0]->point_at(0);
     
     return new vsol_point_2d(gp.x(),gp.y());
  }

  //: Return the last point of `this'
  virtual vsol_point_2d_sptr p1() const // pure virtual of vsol_curve_2d
  {  vgl_point_2d<double> gp = (ints_.back())->point_at(1);
     
     return new vsol_point_2d(gp.x(),gp.y());
  }

  //: Set 1st point by recomputing 1st interpolant
  // We do nothing; we are going to propose to take off these set/get endpoints
  // from vsol_curve_2d; if you really needs this, you are welcome to code it
  virtual void set_p0(const vsol_point_2d_sptr &new_p0) 
   {vsol_point_2d_sptr p = new_p0;}

  // Not implemented: we are proposing to take off set/get endpoints methods
  // from vsol_curve_2d
  virtual void set_p1(const vsol_point_2d_sptr &new_p1)
   {vsol_point_2d_sptr p = new_p1;}
  
  //: finds index of the interval containing a given arclength of the curve, using
  // binary search.
  unsigned interval_index(double s) const;

  //: variant that also returns parameter t for the corresp. parametric curve
  unsigned interval_index(double s, double *t) const;


  virtual vcl_string is_a() const { return vcl_string("dbsol_interp_curve_2d"); }


  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const;
  //: output description to stream
  virtual void describe(vcl_ostream &strm, int blanking=0) const;

 private:

  static bool is_almost_zero(double x)
   { return (vcl_fabs(x) < vcl_numeric_limits<double>::epsilon()*10000); }
  static bool is_almost_equal(double val1, double val2, double tol)
   { return (vcl_fabs(val1 - val2) < tol); }
  static bool is_almost_equal(double val1, double val2)
   { return (vcl_fabs(val1 - val2) <vcl_numeric_limits<double>::epsilon()*10000); }

  // Data Members-------------------------------------------------------------- 
 protected:

  //: First point of the curve : just to conform to vsol_curve_2d standard
  //vsol_point_2d_sptr p0_;
  //: Last point of the curve
  //vsol_point_2d_sptr p1_;
  
  //: List of dbgl_param_curve smart pointers
  vcl_vector<dbgl_param_curve *> ints_;
  
  //: Arclengths of each interval for fast access
  vcl_vector<double> lengths_;

 private:
};

#endif // dbsol_interp_curve_2d_h_
