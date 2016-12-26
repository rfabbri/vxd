// This is basic/dbsol/dbsol_interp_curve_3d.h
#ifndef dbsol_interp_curve_3d_h_
#define dbsol_interp_curve_3d_h_
//:
// \file
// \brief  Represents a 3D interpolated curve
// \author H. Can Aras (can@lems.brown.edu)
// \date   2006-12-03
//
// Implementation is parallel to the 2D case
//    
// \verbatim
//   
// \endverbatim

#include <vcl_vector.h>
#include <vcl_iostream.h>
#include <dbgl/dbgl_param_curve_3d.h>
#include <dbgl/dbgl_eno_curve_3d.h>
#include <vsol/vsol_curve_3d.h>
#include <vsol/vsol_point_3d.h>
#include <vgl/vgl_point_3d.h>
#include <vcl_limits.h>
#include <vcl_cmath.h>  

class dbsol_interp_curve_3d : public vsol_curve_3d
{
 // PUBLIC INTERFACE----------------------------------------------------------
 public:

  // Constructors/Destructors--------------------------------------------------
  dbsol_interp_curve_3d() {};
  dbsol_interp_curve_3d(vcl_vector<dbgl_param_curve_3d *> inter);
  
  //: deletes internal storage
  ~dbsol_interp_curve_3d() {
    for (unsigned i = 0; i<ints_.size(); i++)
      delete ints_[i];
    ints_.clear();
  }

  //: same as constructor but may be called anytime
  void make(const vcl_vector<dbgl_param_curve_3d *> & inter);

  virtual vsol_spatial_object_3d* clone(void) const 
   { return new dbsol_interp_curve_3d(*this); }
   
  // Operators----------------------------------------------------------------
  virtual vsol_point_3d_sptr point_at(double s) const;

  //: Returns i-th curve sample
  vsol_point_3d_sptr point_at_sample(unsigned i) const
    { return point_at(length_at(i)); }
  
  //: Returns the tangent at s arclength away from p0_
  virtual vgl_vector_3d<double> tangent_at(double s) const;

  //: Returns the normal at s arclength away from p0_
  virtual vgl_vector_3d<double> normal_at(double s) const;

  //: Returns the tangent angles (in radian) at s arclength away from p0_
  virtual struct tangent_angles tangent_angles_at(double s) const;

  //: interpolation 3rd degree
  double curvature_at(double s) const;

  //: interpolation 3rd degree
  double torsion_at(double s) const;

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
  vcl_vector<dbgl_param_curve_3d *> const & intervals() const { return ints_; }

  void set_intervals(vcl_vector<dbgl_param_curve_3d *> const &intervals) 
     { ints_ = intervals; }

  //: easy access of intervals using an array subscript
  dbgl_param_curve_3d const & operator [] (unsigned i) const { return *(ints_[i]);}

  dbgl_param_curve_3d const * interval(unsigned i) const { return ints_[i];}

  //: Return the first point of `this'
  virtual vsol_point_3d_sptr p0() const // pure virtual of vsol_curve_3d
  {  vgl_point_3d<double> gp = ints_[0]->point_at(0);
     
     return new vsol_point_3d(gp.x(),gp.y(),gp.z());
  }

  //: Return the last point of `this'
  virtual vsol_point_3d_sptr p1() const // pure virtual of vsol_curve_3d
  {  vgl_point_3d<double> gp = (ints_.back())->point_at(1);
     
     return new vsol_point_3d(gp.x(),gp.y(),gp.z());
  }

  //: Set 1st point by recomputing 1st interpolant
  // We do nothing; we are going to propose to take off these set/get endpoints
  // from vsol_curve_2d; if you really needs this, you are welcome to code it
  virtual void set_p0(const vsol_point_3d_sptr &new_p0) 
   {vsol_point_3d_sptr p = new_p0;}

  // Not implemented: we are proposing to take off set/get endpoints methods
  // from vsol_curve_3d
  virtual void set_p1(const vsol_point_3d_sptr &new_p1)
   {vsol_point_3d_sptr p = new_p1;}
  
  //: finds index of the interval containing a given arclength of the curve, using
  // binary search.
  unsigned interval_index(double s) const;

  //: variant that also returns parameter t for the corresp. parametric curve
  unsigned interval_index(double s, double *t) const;

  virtual vcl_string is_a() const { return vcl_string("dbsol_interp_curve_3d"); }

  //: access to the lengths array
  double lengths(int index) { return lengths_[index]; };

  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const;
  //: output description to stream
  virtual void describe(vcl_ostream &strm, int blanking=0) const;

 private:

  static bool is_almost_zero(double x)
   { return (vcl_fabs(x) < vcl_numeric_limits<double>::epsilon()*1000); }
  static bool is_almost_equal(double val1, double val2, double tol)
   { return (vcl_fabs(val1 - val2) < tol); }
  static bool is_almost_equal(double val1, double val2)
   { return (vcl_fabs(val1 - val2) <vcl_numeric_limits<double>::epsilon()*1000); }

  // Data Members-------------------------------------------------------------- 
 protected:

  //: First point of the curve : just to conform to vsol_curve_2d standard
  //vsol_point_3d_sptr p0_;
  //: Last point of the curve
  //vsol_point_3d_sptr p1_;
  
  //: List of dbgl_param_curve smart pointers
  vcl_vector<dbgl_param_curve_3d *> ints_;
  
  //: Arclengths of each interval for fast access
  vcl_vector<double> lengths_;

 private:
};

#endif // dbsol_interp_curve_3d_h_
