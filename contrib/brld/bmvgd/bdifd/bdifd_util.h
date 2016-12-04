// This is bdifd_util.h
#ifndef bdifd_util_h
#define bdifd_util_h
//:
//\file
//\brief General utilities (probably to be moved into some vxl libs)
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 04/25/2005 10:28:03 AM EDT
//
#include <vcl_vector.h>
#include <vcl_string.h>
#include <vcl_algorithm.h>
#include <vcl_iostream.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_vector_fixed.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vpgl/vpgl_fundamental_matrix.h>
#include <vgl/vgl_point_3d.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_point_2d_sptr.h>

// These are mere shortcuts, not even new types
#define bdifd_vector_3d vnl_vector_fixed<double,3>
#define bdifd_vector_2d vnl_vector_fixed<double,2>

#define remainder_div_2pi(x) ((x) - ((int)((x)/(2*vnl_math::pi)))*(x) )

vgl_homg_point_2d<double> bdifd_epipolar_point_transfer( 
      const vgl_homg_point_2d<double> &p1, 
      const vgl_homg_point_2d<double> &p2, 
      const vpgl_fundamental_matrix<double> &f13,
      const vpgl_fundamental_matrix<double> &f23);


inline vgl_homg_line_2d<double>
bdifd_normal_correspondence_line(
  const vcl_vector<vsol_point_2d_sptr>  &con,
  unsigned k
    );


#define bdifd_tolerance 1e-7
#define MW_ROUND(X)        ((int)((X)+0.5))

class bdifd_util {
public:
  static bool near_zero(double x) { return vcl_fabs(x) < bdifd_tolerance; }
  static bool near_zero(double x,double tol) { return vcl_fabs(x) < tol; }

  //: Given two angles in the range [0, 2pi) representing the direction of a unit vector
  // \return the smallest angle between them in the range [0,pi)
  static inline double angle_difference(double angle1, double angle2); 

  static inline double angle0To2Pi (double angle);

  //: clump arg from minus 1 to 1; also assert abs(arg) not much bigger than 1
  static inline double clump_to_acos(double x);

  static inline vnl_vector_fixed<double,3> vgl_to_vnl(const vgl_point_3d<double> &p);

  //: user must ensure vector is not empty
  template <class T> static double max(const vcl_vector<T> &v, unsigned &idx);
  //: user must ensure vector is not empty
  static inline double min(const vcl_vector<double> &v, unsigned &idx);
  //: user must ensure vector is not empty
  static inline double mean(const vcl_vector<double> &v);
  //: user must ensure vector is not empty
  static inline double median(const vcl_vector<double> &v);


  //: angle between two unit vectors
  static inline double angle_unit(const bdifd_vector_3d &t1, const bdifd_vector_3d &t2);
};


//: smallest angle between two unit vectors
inline double bdifd_util::angle_unit(const bdifd_vector_3d &t1, const bdifd_vector_3d &t2)
{
  return vcl_acos(clump_to_acos(dot_product(t1,t2)));
}

inline double bdifd_util::angle_difference(double angle1, double angle2) 
{ 
   double dt_angle = vcl_fabs(angle1 - angle2);
   return (dt_angle > vnl_math::pi)? (2*vnl_math::pi - dt_angle) : dt_angle;
}

//: user must ensure vector is not empty
inline double bdifd_util::median(const vcl_vector<double> &v)
{
  vcl_vector<double> v_sorted = v;

  // sort increasing
  vcl_sort(v_sorted.begin(), v_sorted.end());
  
  unsigned median_idx = (v_sorted.size() - 1)/2;
  double median_val;


  if (v_sorted.size()%2 == 1) {
    median_val = v_sorted[median_idx];
  } else {
    median_val = (v_sorted[median_idx] + v_sorted[median_idx + 1])/2.0;
  }

  return median_val;
}

//: user must ensure vector is not empty
template <class T> double bdifd_util::max(const vcl_vector<T> &v, unsigned &idx)
{
  idx = 0;
  T maxval = v[0];
  for (unsigned i=1; i < v.size(); ++i) {
    if (maxval < v[i]) {
      maxval = v[i];
      idx = i;
    }
  }
  return maxval;
}

//: user must ensure vector is not empty
inline double bdifd_util::mean(const vcl_vector<double> &v)
{
  double meanval = 0.0;
  for (unsigned i=0; i < v.size(); ++i) {
    meanval += v[i];
  }
  meanval /= v.size();

  return meanval;
}

//: user must ensure vector is not empty
inline double bdifd_util::min(const vcl_vector<double> &v, unsigned &idx)
{
idx = 0;
double minval = v[0];
for (unsigned i=1; i < v.size(); ++i) {
  if (minval > v[i]) {
    minval = v[i];
    idx = i;
  }
}
return minval;
}

inline double bdifd_util::clump_to_acos(double x)
{ 
  if (x > 1.0 || x < -1.0) {
    assert(vcl_fabs(vcl_fabs(x)-1) < 1e-5);
    if (x > 1.0)
      return 1.0;
    if (x < -1.0)
      return -1.0;
  }
  return x;
}

//: Copied from dbdet_sel1.h
inline double bdifd_util::
angle0To2Pi (double angle)
{
  double a;
  if (angle>=2*vnl_math::pi)
    a = vcl_fmod (angle,vnl_math::pi*2);
  else if (angle < 0)
    a = (2*vnl_math::pi+ vcl_fmod (angle,2*vnl_math::pi));
  else 
    a= angle;

  // added by Nhon: these two lines of code is to fix the bug when
  // angle = -1.1721201390607859e-016
  // then after all the computation, we get
  // a = 6.2831853071795862 == 2*vnl_math::pi !!!!!!!
  // the only case this can happen is when a is very close to zero.

  if (!(a>=0 && a<2*vnl_math::pi)) {
    a = 0;
  }

  // assert (a>=0 && a<2*vnl_math::pi);
  return a;
}

inline vgl_homg_line_2d<double>
bdifd_normal_correspondence_line(
  const vcl_vector<vsol_point_2d_sptr>  &con,
  unsigned k
    )
{

  bdifd_vector_2d p0;
  
  p0[0] = con[k]->x();
  p0[1] = con[k]->y();
  bdifd_vector_2d n;

  if (k < con.size()-1) {
    n[0] = con[k]->x()-con[k+1]->x();
    n[1] = con[k]->y()-con[k+1]->y();
  } else {
    n[0] = con[k-1]->x()-con[k]->x();
    n[1] = con[k-1]->y()-con[k]->y();
  }

  double d = - dot_product(n,p0);
  
  return vgl_homg_line_2d<double> (n[0],n[1],d);
}

inline vnl_vector_fixed<double,3> bdifd_util::
vgl_to_vnl(const vgl_point_3d<double> &p)
{
  return vnl_vector_fixed<double,3> (p.x(),p.y(),p.z());
}



#endif // bdifd_util_h
