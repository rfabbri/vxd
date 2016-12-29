#include "mw_epi_interceptor.h"
#include <dbgl/algo/dbgl_intersect.h>
#include <vgl/vgl_distance.h>


#include <mw/mw_util.h>

void mw_epi_interceptor_brute::
points_intercepting_epipolar(
    const vgl_homg_line_2d<double> *ep_l,
    const vcl_vector<vsol_point_2d_sptr> &p,
    vcl_list<unsigned> &p_idx
    ) const
{
  vcl_vector<bool> indices; 

  compute(ep_l,p, indices);

  for (unsigned i=0; i < indices.size(); ++i) {
    if (indices[i])
      p_idx.push_back(i);
  }
}

/*
unsigned long mw_epi_interceptor_fast_using_angle::
npoints() const
{
  return npts_;
}

mw_epi_interceptor::
mw_epi_interceptor_fast_using_angle(n_points, dist)
  :
  npts_(n_points),
  d_err_(dist)
{
}

void mw_epi_interceptor_fast_using_angle::
print() const
{
  vcl_cout << "==== mw_epi_interceptor ====" << vcl_endl;
  vcl_cout << "npts: " << npts _ << ", angles.size(): " << angles.size() << vcl_endl;
  vcl_cout << "==============================" << vcl_endl;
}
*/

void mw_epi_interceptor::
curve_line_intersection_simple(
    const vsol_polyline_2d &crv, 
    const vgl_line_2d<double> &l, 
    vgl_point_2d<double> *pt_img1,
    unsigned *nearest_sample_id)
{
  // TODO use this: intersect_line_fast(crv, vgl_line_2d<double>(l), pt_img1);
  // cross-check with very simple implementation:
  unsigned &lmin = *nearest_sample_id;
  lmin = 0;

  { // determine point of this iset minimizing epipolar distance (assume accurate calib)
    // this is just to get the segment with which to intersect.
    double cost_min = vcl_numeric_limits<double>::infinity(); 
    double cost;

    for (unsigned ls=0; ls< crv.size(); ++ls) {
      const vsol_point_2d_sptr pt2 = crv.vertex(ls);

      vgl_homg_point_2d<double> homg_pt(pt2->x(),pt2->y());
      cost = vgl_distance(vgl_homg_line_2d<double>(l), homg_pt);
      if ( cost < cost_min ) {
        cost_min = cost;
        lmin = ls;
      }
    }
  }

  *pt_img1 = crv.vertex(lmin)->get_p();

  bool intersects=false;

  if (lmin != 0) {
    vgl_line_segment_2d<double> seg(crv.vertex(lmin-1)->get_p(), 
                                    crv.vertex(lmin)->get_p());
    vgl_point_2d<double> ipt;

    intersects=dbgl_intersect::line_lineseg_intersect(l,seg,ipt);
    if (intersects)
      *pt_img1 = ipt;
  }

  if (!intersects && lmin+1 < crv.size()) {
    vgl_line_segment_2d<double> seg(crv.vertex(lmin)->get_p(), 
                                    crv.vertex(lmin+1)->get_p());
    vgl_point_2d<double> ipt;

    intersects=dbgl_intersect::line_lineseg_intersect(l,seg,ipt);
    if (intersects)
      *pt_img1 = ipt;
  }
}
