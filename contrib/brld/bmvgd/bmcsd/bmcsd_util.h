// This is bmcsd_util.h
#ifndef bmcsd_util_h
#define bmcsd_util_h
//:
//\file
//\brief General utilities (probably to be moved into some vxl libs)
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 04/25/2005 10:28:03 AM EDT
//
//\modifications:
//\Anil Usumezbas Oct 10th, 2010:
//\Added the function 'find_nearest_pt_using_double'
//
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_polyline_2d.h>
#include <vcl_vector.h>
#include <vcl_string.h>
#include <vcl_algorithm.h>

#include <vil/vil_image_view.h>
#include <bsold/algo/bsold_geno.h>

// Header files that are generally needed, but not necessarily here:

#include <vcl_iostream.h>

#include <vnl/vnl_math.h>
#include <vnl/vnl_vector_fixed.h>

#include <vpgl/vpgl_perspective_camera.h>
#include <vpgl/vpgl_fundamental_matrix.h>
#include <vgl/vgl_point_3d.h>
// #include <dvpgl/io/dvpgl_io_cameras.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_polyline_2d.h>
#include <bdifd/bdifd_frenet.h>
#include <bbld/bbld_subsequence.h>


#define bmcsd_vector_3d vnl_vector_fixed<double,3>
#define bmcsd_vector_2d vnl_vector_fixed<double,2>

// --- Globals --- 
// Defined in bmcsd_util_defs.cxx
extern int n8[8][2];
extern int n4[4][2];

#define remainder_div_2pi(x) ((x) - ((int)((x)/(2*vnl_math::pi)))*(x) )

static const double rad_to_degree_ratio = vnl_math::pi/180.0;

bool load_con_file(
      vcl_string filename, 
      vcl_vector<vsol_point_2d_sptr> &points, 
      bool *is_open);

bool con_filenames(vcl_string image_fname,vcl_vector<vcl_string> &con_fnames);

bool myread(vcl_string fname, vcl_vector<double> &pts);
bool myreadv(vcl_string fname, vcl_vector<vsol_point_2d_sptr> &pts);
bool myreadv(vcl_string fname, vcl_vector<vgl_point_3d<double> > &pts);
bool myreadv(vcl_string fname, vcl_vector<bmcsd_vector_3d> &pts);
//: Reads 3D curves to many files whose prefix and extension are given.
// The output files will be named like $prefix-3dcurve-$crv_id-{points|tangents}$ext.
// the input vcl_vector is assumed to be already resized to the correct number
// of curves.
bool myreadv(vcl_string prefix, vcl_string ext, vcl_vector<bdifd_1st_order_curve_3d> &crv_3d);

bool mywrite(vcl_string fname, const vcl_vector<double> &v);
bool mywrite_ascii(vcl_string fname, const vcl_vector<double> &v);
bool mywritev(vcl_string fname, const vcl_vector<vsol_point_2d_sptr> &pts);
bool mywritev(vcl_string fname, const vcl_vector<bmcsd_vector_3d> &crv_3d);
bool mywritev(vcl_string fname, const vcl_vector<vgl_point_3d<double> > &crv_3d);
//: Writes 3D curves to many files whose prefix and extension are given.
// The output files will be named like $prefix-3dcurve-$crv_id-{points|tangents}$ext
bool mywritev(vcl_string prefix, vcl_string ext, const vcl_vector<bdifd_1st_order_curve_3d> &crv_3d);

bool read_cam(vcl_string img_name1, vcl_string img_name2, 
      vpgl_perspective_camera <double> *P1out,
      vpgl_perspective_camera <double> *P2out);
bool read_cam( vcl_string img_name1, 
      vpgl_perspective_camera <double> *P1out);
bool read_3x4_matrix_into_cam( vcl_string img_name1, 
      vpgl_perspective_camera <double> *P1out);

bool bmcsd_get_prefix(vcl_string img_name, vcl_string *dir, vcl_string *noext);

vgl_homg_point_2d<double> bmcsd_epipolar_point_transfer( 
      const vgl_homg_point_2d<double> &p1, 
      const vgl_homg_point_2d<double> &p2, 
      const vpgl_fundamental_matrix<double> &f13,
      const vpgl_fundamental_matrix<double> &f23);

inline double
d_sqr(double x1, double y1, double x2, double y2)
{
   double dx= x1-x2, dy = y1-y2;
   return dx*dx + dy*dy;
}

void
get_normal(const bsold_geno_curve_2d &c, unsigned i, double *normal_x, double *normal_y);

void
get_normal_arc(const bsold_geno_curve_2d &c, unsigned i, double *normal_x, double *normal_y);

inline vgl_homg_line_2d<double>
bmcsd_normal_correspondence_line(
  const vcl_vector<vsol_point_2d_sptr>  &con,
  unsigned k
    );


#define bmcsd_tolerance 1e-7
#define MW_ROUND(X)        ((int)((X)+0.5))

class bmcsd_util {
public:

  typedef enum {MW_INTRINSIC_EXTRINSIC, MW_3X4} camera_file_type;

  static bool near_zero(double x) { return vcl_fabs(x) < bmcsd_tolerance; }
  static bool near_zero(double x,double tol) { return vcl_fabs(x) < tol; }

  //: takes two angles and return the smallest angle between them in the range [0,2pi)
  static inline double angle_difference(double angle1, double angle2); 

  static inline double angle0To2Pi (double angle);

  //: clump arg from minus 1 to 1; also assert abs(arg) not much bigger than 1
  static inline double clump_to_acos(double x);

  static inline vnl_vector_fixed<double,3> vgl_to_vnl(const vgl_point_3d<double> &p);

  //: convert from vsol polyline to vcl_vector of vsol points
  static void get_vsol_point_vector(const vsol_polyline_2d &crv, vcl_vector<vsol_point_2d_sptr> *pts);

  //: user must ensure vector is not empty
  template <class T> static double max(const vcl_vector<T> &v, unsigned &idx);
  //: user must ensure vector is not empty
  static inline double min(const vcl_vector<double> &v, unsigned &idx);
  //: user must ensure vector is not empty
  static inline double mean(const vcl_vector<double> &v);
  //: user must ensure vector is not empty
  static inline double median(const vcl_vector<double> &v);

  //: angle between two unit vectors
  static inline double angle_unit(const bmcsd_vector_3d &t1, const bmcsd_vector_3d &t2);

  //: Returns only the points of the curve that fall within bounds of the image.
  // After this operation, ordering along the curve doesn't matter anymore as
  // there might be multiple connected components.
  static void clip_to_img_bounds(
      const vil_image_view<vxl_uint_32> &img,
      vcl_vector<vsol_point_2d_sptr> *curve); 

  //: Returns only the points of the curve that fall within bounds of the image.
  // After this operation, ordering along the curve doesn't matter anymore as
  // there might be multiple connected components.
  static void clip_to_img_bounds(
      const vil_image_view<vxl_uint_32> &img,
      bdifd_1st_order_curve_2d *curve); 

  //: \return true if all points of the curve are within bounds.
  static bool
  in_img_bounds( const vcl_vector<vsol_point_2d_sptr> &curve, 
      const vil_image_view<vxl_uint_32> &img);

  //: \return true if all points of the curve are within bounds.
  static bool 
  in_img_bounds(const vsol_polyline_2d &curve,
      const vil_image_view<vxl_uint_32> &img);

  //: \return true if all points of the curve are within a rectangle 
  // with distance \p radius from the borders of img
  static bool 
  in_img_bounds(const vsol_polyline_2d &curve,
      const vil_image_view<vxl_uint_32> &img, unsigned radius);

  //: \return true if all points of the curve are within bounds.
  static bool 
  in_img_bounds(const bdifd_1st_order_curve_2d &curve,
      const vil_image_view<vxl_uint_32> &img);

  //: \return true if all points of the curve are within a rectangle 
  // with distance \p radius from the borders of img
  static bool 
  in_img_bounds(const bdifd_1st_order_curve_2d &curve,
      const vil_image_view<vxl_uint_32> &img, unsigned radius);

  //: return sub-polyline from c(ini) to c(end); ini can be greater or smaller
  // than end; 
  static inline vsol_polyline_2d_sptr 
  get_subcurve(const vsol_polyline_2d_sptr &c, unsigned ini, unsigned end);

  static inline vsol_polyline_2d_sptr 
  reverse_order(const vsol_polyline_2d_sptr &c);

  //: returns index of nearest point to pt in crv and also the minimum distance
  //(mindist param)
  static unsigned
  find_nearest_pt(const vsol_point_2d_sptr &pt, const vsol_polyline_2d_sptr &crv, unsigned &mindist);

  static unsigned
  find_nearest_pt_using_double(const vsol_point_2d_sptr &pt, const vsol_polyline_2d_sptr &crv, double &mindist);

  static void prune_curves(
      unsigned min_num_samples, vcl_vector< vsol_polyline_2d_sptr > *pcurves,
      bbld_subsequence_set *ss);

  static void prune_curves_by_length(
      double min_length, vcl_vector< vsol_polyline_2d_sptr > *pcurves,
      bbld_subsequence_set *ss);

  //: parses a vcl_string of ther form "listname val1 val2 val3..." into a vcl_string
  // with the listname, and a list of numbers of type T.
  template <typename T> static void parse_num_list(const vcl_string &stringlist, vcl_vector<T> *values);

  static bool read_cam_anytype(vcl_string fname, camera_file_type type, 
    vpgl_perspective_camera<double> *cam);

  //: writes a vcl_vector of cameras out.
  static bool write_cams(vcl_string dir, vcl_string fname_prefix, camera_file_type type, 
    const vcl_vector<vpgl_perspective_camera<double> > &cam);

  //: variant that accepts a name for each camera; the extension will be
  // appended by this function depending on the camera type.
  static bool write_cams(
      vcl_string dir, 
      vcl_vector<vcl_string> cam_fname_noexts, 
      camera_file_type type, 
      const vcl_vector<vpgl_perspective_camera<double> > &cam);
};

//: smallest angle between two unit vectors
inline double bmcsd_util::angle_unit(const bmcsd_vector_3d &t1, const bmcsd_vector_3d &t2)
{
  return vcl_acos(clump_to_acos(dot_product(t1,t2)));
}


//: takes two angles and return the smallest angle between them in the range
//[0,2pi)
inline double bmcsd_util::angle_difference(double angle1, double angle2) 
{ 
   double dt_angle = vcl_fabs(angle1 - angle2);
   return (dt_angle > vnl_math::pi)? (2*vnl_math::pi - dt_angle) : dt_angle;
}

//: user must ensure vector is not empty
inline double bmcsd_util::median(const vcl_vector<double> &v)
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
template <class T> double bmcsd_util::max(const vcl_vector<T> &v, unsigned &idx)
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
inline double bmcsd_util::mean(const vcl_vector<double> &v)
{
  double meanval = 0.0;
  for (unsigned i=0; i < v.size(); ++i) {
    meanval += v[i];
  }
  meanval /= v.size();

  return meanval;
}

//: user must ensure vector is not empty
inline double bmcsd_util::min(const vcl_vector<double> &v, unsigned &idx)
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

inline double bmcsd_util::clump_to_acos(double x)
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
inline double bmcsd_util::
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
bmcsd_normal_correspondence_line(
  const vcl_vector<vsol_point_2d_sptr>  &con,
  unsigned k
    )
{
  bmcsd_vector_2d p0;
  
  p0[0] = con[k]->x();
  p0[1] = con[k]->y();
  bmcsd_vector_2d n;

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

inline vnl_vector_fixed<double,3> bmcsd_util::
vgl_to_vnl(const vgl_point_3d<double> &p)
{
  return vnl_vector_fixed<double,3> (p.x(),p.y(),p.z());
}

inline vsol_polyline_2d_sptr bmcsd_util::
get_subcurve(const vsol_polyline_2d_sptr &c, unsigned ini, unsigned end)
{
  vsol_polyline_2d *c_short = new vsol_polyline_2d();

  if (ini > end)
    for (unsigned i=ini; i >= end && i != (unsigned)-1; --i)
      c_short->add_vertex(c->vertex(i));
  else
    for (unsigned i=ini; i<=end; ++i)
      c_short->add_vertex(c->vertex(i));

  return c_short;
}

inline vsol_polyline_2d_sptr  bmcsd_util::
reverse_order(const vsol_polyline_2d_sptr &c)
{
  vsol_polyline_2d *c_rev = new vsol_polyline_2d();

  for (unsigned i=c->size()-1; i!=(unsigned)-1; --i)
    c_rev->add_vertex(c->vertex(i));

  return c_rev;
}

//: For use in for_each
struct bmcsd_delete_object {
public:
  template <typename T>
  void operator()(const T *ptr) const {
    delete ptr;
  }
};

struct bmcsd_dereference_equal {
  template <typename PtrType> bool
  operator()(PtrType pt1, PtrType pt2) const
  {
    return *pt1 == *pt2;
  }
};

struct bmcsd_dereference_less {
  template <typename PtrType> bool
  operator()(PtrType pt1, PtrType pt2) const
  {
    return *pt1 < *pt2;
  }
};


#define MW_UTIL_INSTANTIATE(T) extern "please include bmcsd/bmcsd_util.hxx first"

#endif // bmcsd_util_h
