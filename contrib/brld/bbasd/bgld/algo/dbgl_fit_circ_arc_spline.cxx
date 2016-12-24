// This is file basic/dbgl/algo/dbgl_fit_circ_arc_spline.cxx

//:
// \file


#include "dbgl_fit_circ_arc_spline.h"
#include "internals/arcspline.h"
#include <vbl/vbl_array_1d.h>
#include <vnl/vnl_math.h>
#include <dbgl/algo/dbgl_biarc.h>

//: Interpolate a set of points with a circular arc spline
bool dbgl_fit_circ_arc_spline_to_polyline(vcl_vector<dbgl_circ_arc >& arc_list,
                                 const vcl_vector<vgl_point_2d<double > >& pts,
                                 double tolerance)
{
  // Preliminary check
  arc_list.clear();
  if (pts.size() < 3)
    return false;

  // Construct a vector of Point3D
  vbl_array_1d<Point3D > Point3D_list(pts.size(), Point3D());
  for (unsigned i =0; i < pts.size(); ++i)
  {
    Point3D_list[i].x = pts[i].x();
    Point3D_list[i].y = pts[i].y();
    Point3D_list[i].z = 0;
  }
  int n = pts.size();
  Point3D* P = Point3D_list.begin();



  // estimate alpha1 and betan;
  double alpha1 = 0;
  double betan = 0;


  dbgl_circ_arc arc_first;
  if (arc_first.set_from(pts[0], pts[1], pts[2]))
  {
    alpha1 = signed_angle(arc_first.tangent_at_start(), pts[1]-pts[0]);
  }

  dbgl_circ_arc arc_last;
  if (arc_last.set_from(pts[n-3], pts[n-2], pts[n-1]))
  {
    betan = signed_angle(pts[n-1]-pts[n-2], arc_last.tangent_at_end());
  }



  // Construct an arc spline interpolating the points
  Profile * profile = FairCurReconstruct(n, P, alpha1, betan);


  // Now optimize the arc spline to reduce the number of arcs
  // while keeping the curve within a predefined tolerance
  
  UnfArcSplineReduction(tolerance, profile);


  // Return the list of circular arc, no optimization
  CurveSmt* cur_smt = profile->curve_smt;
  while (cur_smt != NULL)
  {
    if (cur_smt->lnac == 0) // curve segment is a line segment
    {
      Point3D s_point = cur_smt->line_smt->s_point;
      Point3D e_point = cur_smt->line_smt->e_point;

      // convert to vxl-type
      vgl_point_2d<double > start(s_point.x, s_point.y);
      vgl_point_2d<double > end(e_point.x, e_point.y);

      // construct the arc
      dbgl_circ_arc circ_arc(start, end, 0);
      arc_list.push_back(circ_arc);
    }
    else
    {
      Point3D s_point = cur_smt->arc_smt->s_point;
      Point3D e_point = cur_smt->arc_smt->e_point;
      double radius = cur_smt->arc_smt->radius;
      int sign_k = (cur_smt->arc_smt->wise == 0) ? 1 : -1;

      // Convert to vxl-type
      vgl_point_2d<double > start(s_point.x, s_point.y);
      vgl_point_2d<double > end(e_point.x, e_point.y);

      // Construct the arc
      dbgl_circ_arc circ_arc(start, end, (1/radius) * sign_k);

      // add to the list
      arc_list.push_back(circ_arc);
    }
    
    cur_smt = cur_smt->nextc;
  }
  return true;
}


// -----------------------------------------------------------------------------
//: Interpolate a polygon with circular-arc polygon, i.e., a closed smooth contour
// consisting circular arc segments
bool dbgl_fit_circ_arc_spline_to_polygon(vcl_vector<dbgl_circ_arc >& arc_list,
                                 const vcl_vector<vgl_point_2d<double > >& pts,
                                 double tolerance)
{
  // Algorithm:
  // We will cut open the polygon and fit it with circular arcs using Yang's 
  // circular arc fitting. But this may cause unsmoothness at the end points
  // We will manually fix the end point problem by 
  // i. Remove the longest line-segment in the polygon
  // ii. Fit the remaining polygline with circular arc
  // iii. Fit the gap between two end-points of the poly-arc with a biarc

  // identify the longest line segment
  double huge_val = 1e10;
  vcl_vector<double > sqr_lengths(pts.size(), 0);
  for (unsigned i =0; i < pts.size(); ++i)
  {
    vgl_point_2d<double > start = pts[i];
    vgl_point_2d<double > end = pts[(i+1) % pts.size()];
    sqr_lengths[i] = (end-start).sqr_length();
  }

  double max_sqr_length = -1;
  int max_sqr_length_index = -1;
  for (unsigned i =0; i < sqr_lengths.size(); ++i)
  {
    if (max_sqr_length < sqr_lengths[i])
    {
      max_sqr_length = sqr_lengths[i];
      max_sqr_length_index = i;
    }
  }

  // cut-open the polygon at the longest line segment
  vcl_vector<vgl_point_2d<double > > polyline(pts.size(), vgl_point_2d<double >());
  for (unsigned i =0; i < pts.size(); ++i)
  {
    polyline[i] = pts[(i+max_sqr_length_index+1) % pts.size()];
  }

  // fit the polyline with circular arc spline
  dbgl_fit_circ_arc_spline_to_polyline(arc_list, polyline, tolerance);

  // interpolate the gap with a biarc
  vgl_point_2d<double > gap_start_pt = arc_list.back().end();
  vgl_vector_2d<double > gap_start_tangent = arc_list.back().tangent_at_end();

  vgl_point_2d<double > gap_end_pt = arc_list.front().start();
  vgl_vector_2d<double > gap_end_tangent = arc_list.front().tangent_at_start();

  dbgl_biarc gap_biarc(gap_start_pt, gap_start_tangent, gap_end_pt, gap_end_tangent);
  if (gap_biarc.is_consistent())
  {
    dbgl_circ_arc arc1(gap_biarc.start(), gap_biarc.mid_pt(), gap_biarc.k1());
    dbgl_circ_arc arc2(gap_biarc.mid_pt(), gap_biarc.end(), gap_biarc.k2());

    arc_list.push_back(arc1);
    arc_list.push_back(arc2);
    return true;
  }
  else
  {
    // just add a line connecting the two end points
    dbgl_circ_arc arc1(gap_start_pt, gap_end_pt, 0);
    arc_list.push_back(arc1);
    return false;
  }
}



