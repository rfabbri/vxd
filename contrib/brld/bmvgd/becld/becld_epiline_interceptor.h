// This is becld_epiline_interceptor.h
#ifndef becld_epiline_interceptor_h
#define becld_epiline_interceptor_h
//:
//\file
//\brief Epipolar line-to-pointset intersection
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date Mon Apr 24 20:47:25 EDT 2006
//

#include <vcl_vector.h>
#include <vcl_list.h>

#include <vgl/vgl_line_2d.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vgl/vgl_homg_line_2d.h>
#include <vgl/vgl_distance.h>

#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_polyline_2d.h>


//: A general class specifying an uniform interface to algorithms for epipolar
// line intersection 
class becld_epiline_interceptor_base {
public:
  becld_epiline_interceptor_base() {};
  virtual ~becld_epiline_interceptor_base() {};

  virtual void points_intercepting_epipolar(
      const vgl_homg_line_2d<double> *ep_l,
      const vcl_vector<vsol_point_2d_sptr> &p,
      vcl_list<unsigned> &p_idx
      ) const = 0;
};

//: Bruteforce algo
class becld_epiline_interceptor_brute : public becld_epiline_interceptor_base {
public:
  virtual ~becld_epiline_interceptor_brute() {}
  
  virtual void points_intercepting_epipolar(
      const vgl_homg_line_2d<double> *ep_l,
      const vcl_vector<vsol_point_2d_sptr> &p,
      vcl_list<unsigned> &p_idx
      ) const;

  static inline bool
  compute(
      const vgl_homg_line_2d<double> *l, 
      const vcl_vector<vsol_point_2d_sptr> &p,
      vcl_vector<bool> &indices, 
      double maxdist=1.4);
};

//: General static utilities
class becld_epiline_interceptor {
public:
  //: returns only one intersection of the line and the curve. This assumes that
  // the curve actually intersects the line only once. If there are multiple
  // intersections, this function returns the intersection neighboring the sample
  // that is closest to the epipolar line.
  static void curve_line_intersection_simple(
      const vsol_polyline_2d &crv, const vgl_line_2d<double> &l, 
      vgl_point_2d<double> *pt_img1, unsigned *nearest_sample_id);

  //: convenience version, without a nearest_sample_id parameter
  static void curve_line_intersection_simple(
      const vsol_polyline_2d &crv, const vgl_line_2d<double> &l, 
      vgl_point_2d<double> *pt_img1)
  {
    unsigned nearest_sample_id;
    curve_line_intersection_simple(crv, l, pt_img1, &nearest_sample_id);
  }
};

/*
//:
//  Class for efficient intercection of epipolar-line and pointset 
//
// TODO
//  - XXX still under construction XXX
class becld_epiline_interceptor_fast_using_angle : becld_epiline_interceptor_base {
  public:

  becld_epiline_interceptor(unsigned long n_points, double maxdist=1.4);

  ~becld_epiline_interceptor() {}

  //: Standard interface to intecection algorithm. This fn. will call the
  // current morst efficient algorithm

  void points_intercepting_epipolar(
      const vgl_homg_line_2d<double> *ep_l,
      const vcl_vector<vsol_point_2d_sptr> &p,
      vcl_vector<vcl_list<unsigned> > &p_idx
      ) const;

  void set_distance_error(double dist) { d_err_ = dist; }
  
  // intersect(angle)
  // intersect (ep_line)
  // set_epsilon_distance(dist double)
  // set_epsolin_angle
  // cache_angle() // private
  // has_angles_been_cached()
  //

  void print() const;
  unsigned long npoints() const;


  protected:
    double d_err;
    unsigned long npts_;
    vcl_vector<double> angles_;
};
*/

//--------------------------------------------------------------------------------
// INLINERS
//--------------------------------------------------------------------------------

//: Computes intersecting interval of a line with a point set whose vertices are
// within the range of a grid (possibly being subpixel). This means pointset
// coordinates are positive.
//
// \param[out] indices: indices[i] = true for the points i (in point set) 
// that are sufficiently close to l to be considered a "fuzzy" intersection 
// of l and p.
//
inline bool
becld_epiline_interceptor_brute::
compute(
    const vgl_homg_line_2d<double> *l, 
    const vcl_vector<vsol_point_2d_sptr> &p,
    vcl_vector<bool> &indices, 
    double maxdist)
{
//    - currently, an intersection is simply a point whose distance to the line
//    is less than sqrt(2).

  indices.clear();
  indices.resize(p.size(),false);
  bool has_intersection=false;
  for (unsigned i=0; i<p.size(); ++i) {
    const vsol_point_2d_sptr pt = p[i];
    vgl_homg_point_2d<double> homg_pt(pt->x(),pt->y());
    if (vgl_distance(*l,homg_pt) <= maxdist) {
      indices[i]=true;
      has_intersection=true;
    }
  }

  return has_intersection;
}

#endif // becld_epiline_interceptor_h
