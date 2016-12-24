// This is basic/dbgl/algo/dbgl_closest_point.cxx
//:
// \file

#include "dbgl_closest_point.h"

#include <dbnl/dbnl_solve_quadratic_equation.h>
#include <dbnl/dbnl_solve_trigonometric_equation.h>
#include <dbgl/algo/dbgl_circ_arc.h>

#include <vcl_cmath.h>
#include <vnl/vnl_math.h>
#include <vcl_limits.h>

#include <vgl/vgl_lineseg_test.h>
#include <vgl/vgl_distance.h>
#include <vgl/vgl_closest_point.h>



//------------------------------------------------------------------------
//: minimum curvature of an arc
double dbgl_closest_point::min_curvature = dbgl_circ_arc::epsilon;
const double dbgl_closest_point_distance_epsilon = 1e-8;


//------------------------------------------------------------------------
//: Return shortest distance between two line segments.
// `ratio1' and 'ratio2' (in range [0, 1]) are normalized arclength of 
  // closest points between the two line segments
double dbgl_closest_point::
lineseg_lineseg(const vgl_point_2d<double >& line1_p1,
                const vgl_point_2d<double >& line1_p2,
                const vgl_point_2d<double >& line2_p1,
                const vgl_point_2d<double >& line2_p2,
                double& ratio1,
                double& ratio2)
{
  
  // case 1: the two line segments intersect;
  if (vgl_lineseg_test_lineseg<double >(
    line1_p1.x(), line1_p1.y(), line1_p2.x(), line1_p2.y(),
    line2_p1.x(), line2_p1.y(), line2_p2.x(), line2_p2.y()))
  {
    // compute intersecting point
    vgl_line_segment_2d<double > line1(line1_p1, line1_p2);
    vgl_line_segment_2d<double > line2(line2_p1, line2_p2);
  
    double px, py, pd;
    px = line1.b()*line2.c() - line1.c()*line2.b();
    py = line2.a()*line1.c() - line1.a()*line2.c();
    pd = line1.a()*line2.b() - line1.b()*line2.a();

    // case 1a: two lines are parallel (degenerate case). We pick either the
    // starting or ending point of line1 to be the closest point
    if (vcl_abs(pd) < 1e-12)
    {
      // check if the first point of line1 is between two end points of line2
      // if so choose as intersection point
      if (dot_product(line1_p1-line2_p1, line1_p1-line2_p2) <= 0)
      {
        px = line1_p1.x();
        py = line1_p1.y();
        vgl_point_2d<double > intersect_pt(px, py);
        ratio1 = 0;
        ratio2 = ratio<double >(line2_p1, line2_p2, intersect_pt);
      }
      // similar check for point2 of line1
      else if (dot_product(line1_p2-line2_p1, line1_p2-line2_p2) <= 0)
      {
        px = line1_p2.x();
        py = line1_p2.y();
        vgl_point_2d<double > intersect_pt(px, py);
        ratio1 = 1;
        ratio2 = ratio<double >(line2_p1, line2_p2, intersect_pt);
      }
      // if both tests above fail, that means that both end points of line2
      // are between two end points of line1 (line1 contains line2). Choose
      // starting point of line2 as intersection point
      else
      {
        px = line2_p1.x();
        py = line2_p1.y();
        vgl_point_2d<double > intersect_pt(px, py);
        ratio2 = 0;
        ratio1 = ratio<double >(line1_p1, line1_p2, intersect_pt);
      }
    }

    // case 1b: two lines actually intersect (normal case)
    else
    {
      // pd is guaranteed to be non-zero
      // px and py are the coordinate of the intersecting points
      px = px/pd;
      py = py/pd;

      // intersecting point
      vgl_point_2d<double > intersect_pt(px, py);

      // parameters in range [0, 1] of intersecting points along line1 and line2
      // line 1
      ratio1 = ratio<double >(line1_p1, line1_p2, intersect_pt);

      // line 2
      ratio2 = ratio<double >(line2_p1, line2_p2, intersect_pt);
    }
    return 0;
  }

  // case 2: two line segment don't intersect.
  // the shortest distance is the minimum among
  // distances from one line endpoint to the other line segment.
  else
  {
    // put all cases into an array so that we don't have to trace them later.
    double xy[][6] = 
    {
      { line2_p1.x(), line2_p1.y(), line2_p2.x(), 
        line2_p2.y(), line1_p1.x(), line1_p1.y() },
      { line2_p1.x(), line2_p1.y(), line2_p2.x(), 
      line2_p2.y(), line1_p2.x(), line1_p2.y() },
      { line1_p1.x(), line1_p1.y(), line1_p2.x(), 
      line1_p2.y(), line2_p1.x(), line2_p1.y() },
      { line1_p1.x(), line1_p1.y(), line1_p2.x(), 
      line1_p2.y(), line2_p2.x(), line2_p2.y() }
    };

    // find the minimum distance and the case order that min-distance occurs
    double d[4];
    for (int i=0; i<4; ++i)
    {
      d[i] = vgl_distance2_to_linesegment<double >(xy[i][0], xy[i][1],
        xy[i][2], xy[i][3], xy[i][4], xy[i][5]);
    }

    double min_dist = d[0];
    int order = 0;
    for (int i = 1; i < 4; ++i)
    {
      if (d[i] < min_dist)
      {
        min_dist = d[i];
        order = i;
      }
    }

    // one of the ratio is partial, the other is 0 or 1
    // we use `order' to trace out which one is partial
    
    // One of the closest points is an endpoint. The other is
    double xp, yp; 
    vgl_closest_point_to_linesegment<double >(xp, yp, xy[order][0], xy[order][1],
      xy[order][2], xy[order][3], xy[order][4], xy[order][5]);
    
    // length ratio from (xp,yp) to starting point of its segment
    double ratio_pt = ratio<double >(vgl_point_2d<double >(xy[order][0], xy[order][1]),
      vgl_point_2d<double >(xy[order][2], xy[order][3]),
      vgl_point_2d<double >(xp, yp));
    
    // determine which between `ratio1' and `ratio2' is partial
    switch(order)
    {
    case (0):
      ratio1 = 0;
      ratio2 = ratio_pt;
      break;
    case(1):
      ratio1 = 1;
      ratio2 = ratio_pt;
      break;
    case (2):
      ratio2 = 0;
      ratio1 = ratio_pt;
      break;
    case (3):
      ratio2 = 1;
      ratio1 = ratio_pt;
      break;
    }
    return vcl_sqrt(min_dist);
  }
}


//------------------------------------------------------------------------
//: Return distance between a point and a circular arc.
// 'ratio' (in range [0, 1]) is the normalized arclength of closest point
// on the arc
double dbgl_closest_point::
point_to_circular_arc(const vgl_point_2d<double >& query_pt,
                      const vgl_point_2d<double >& p1, 
                      const vgl_point_2d<double >& p2, 
                      double k, double& ret_ratio)
{
  // degenerate arc: a line segment
  if (vnl_math::abs(k) < dbgl_closest_point::min_curvature)
  {
    double xm, ym;
    vgl_closest_point_to_linesegment<double >(xm, ym, p1.x(), p1.y(), 
      p2.x(), p2.y(), query_pt.x(), query_pt.y());
    
    // closest point to the line
    vgl_point_2d<double > pt(xm, ym);
    ret_ratio = ratio<double >(p1, p2, pt);
    
    // return distance between `query_pt' and closest point on line segment 
    return  vnl_math::hypot(query_pt.x()-xm, query_pt.y()-ym); 
  }

  // regular arc - an arc with curvature != 0
  dbgl_circ_arc arc(p1, p2, k);
  vgl_vector_2d<double > t1 = arc.tangent_at_start();
  // formula
  //                     k (x0-xp) - sin(psi0)   
  // tan(k*s+psi0) = - -------------------------
  //                     k (y0-yp) + cos(psi0)
  // we want to solve for k*s, the difference between psi0 and ks+psi0
  double vy = -(k*(p1.x()-query_pt.x()) - t1.y());
  double vx =  (k*(p1.y()-query_pt.y()) + t1.x());
  vgl_vector_2d<double > v(vx, vy);
  
  // k*s has two solutions, which are `pi' apart, and one corresponds to 
  // min-distance, one max-distance
  double ks[2];
  ks[0] = atan2(cross_product<double >(t1, v), dot_product<double >(t1, v));
  
  // make sure ks[1] is in range [-pi, pi]
  ks[1] = (ks[0]>0) ? (ks[0]-vnl_math::pi) : (ks[0]+vnl_math::pi);
  
  // need to find out which one gives min-distance
  double dist[2];
  for (int i=0; i<2; ++i)
  {
    vgl_point_2d<double > pt = arc.point_at_length(ks[i]/arc.k());
    dist[i] = vnl_math::hypot(pt.x()-query_pt.x(), pt.y()-query_pt.y());
  }
  // to-return distance
  double ret_distance = (dist[0] > dist[1]) ? dist[1] : dist [0]; 
  // `ks' corresponding to min-distance
  double min_dist_ks = (dist[0] > dist[1]) ? ks[1] : ks[0];

  //// normalize `min_dist_ks' to [-pi, pi] to find out actual value of `s'
  //// we know `min_dist_ks' is in the range [-pi .. 2 pi]
  //// this makes normalization easier.
  //if (min_dist_ks > vnl_math::pi) min_dist_ks -= vnl_math::pi;

  // now check range constraint. 
  // If `min_dist_s' lies within the `arc' then it is the global minimum
  // otherwise, the actual min-distance is between `query_pt' and one of the 
  // end points.
  ret_ratio = min_dist_ks / (arc.k() * arc.len());
  if (ret_ratio >=0 && ret_ratio <= 1)
  {
    return ret_distance;
  }
  else
  {
    // compare distances to two end points of the arc
    double dist_to_point[2];
    dist_to_point[0] = vnl_math::hypot(p1.x()-query_pt.x(), p1.y()-query_pt.y());
    dist_to_point[1] = vnl_math::hypot(p2.x()-query_pt.x(), p2.y()-query_pt.y());
    if (dist_to_point[0] <= dist_to_point[1])
    {
      ret_ratio = 0;
      return dist_to_point[0];
    }
    else
    {
      ret_ratio = 1;
      return dist_to_point[1];
    }
  }
  // should not get here
  assert(false);
  return 0;
}







//: Return distance between a point and a circle
double dbgl_closest_point::
point_to_circle(const vgl_point_2d<double >& query_pt,
                const vgl_point_2d<double >& p0, 
                const vgl_vector_2d<double >& t0, 
                double k, 
                double & ret_arc_length)
{
  
  // degenerate arc: a line segment
  if (vnl_math::abs(k) < dbgl_closest_point::min_curvature)
  {
    vgl_vector_2d<double > tangent = normalized(t0);
    vgl_line_2d<double > line(p0, p0 + 1.0 * tangent);
    vgl_point_2d<double > closest_pt = vgl_closest_point (query_pt, line);

    ret_arc_length = dot_product(tangent, closest_pt-p0);
    return vgl_distance(query_pt, closest_pt);
  }

   



  //// regular arc - an arc with curvature != 0
  // construct a dummy arc
  double length = (vcl_abs(k)<1) ? 1 : 1 / vcl_abs(k);
  dbgl_circ_arc arc;
  arc.set_from(p0, t0, k, length);
  vgl_point_2d<double > p1 = arc.point1();

  //dbgl_circ_arc arc(p1, p2, k);
  
  vgl_vector_2d<double > t1 = arc.tangent_at_start();
  // formula
  //                     k (x0-xp) - sin(psi0)   
  // tan(k*s+psi0) = - -------------------------
  //                     k (y0-yp) + cos(psi0)
  // we want to solve for k*s, the difference between psi0 and ks+psi0
  double vy = -(k*(p1.x()-query_pt.x()) - t1.y());
  double vx =  (k*(p1.y()-query_pt.y()) + t1.x());
  vgl_vector_2d<double > v(vx, vy);
  
  // k*s has two solutions, which are `pi' apart, and one corresponds to 
  // min-distance, one max-distance
  double ks[2];
  ks[0] = atan2(cross_product<double >(t1, v), dot_product<double >(t1, v));
  
  // make sure ks[1] is in range [-pi, pi]
  ks[1] = (ks[0]>0) ? (ks[0]-vnl_math::pi) : (ks[0]+vnl_math::pi);
  
  // need to find out which one gives min-distance
  double dist[2];
  for (int i=0; i<2; ++i)
  {
    vgl_point_2d<double > pt = arc.point_at_length(ks[i]/arc.k());
    dist[i] = vnl_math::hypot(pt.x()-query_pt.x(), pt.y()-query_pt.y());
  }

  // to-return distance
  double ret_distance = (dist[0] > dist[1]) ? dist[1] : dist [0]; 
  // `ks' corresponding to min-distance
  double min_dist_ks = (dist[0] > dist[1]) ? ks[1] : ks[0];

  // returned values
  ret_arc_length = min_dist_ks / k;
  return ret_distance; 
}





//------------------------------------------------------------------------
//: Return distance between a line segment and a circular arc
// 'line_ratios' and 'arc_ratios' are normalized arclength (in range [0, 1])
// of the closest points
double dbgl_closest_point::
lineseg_to_circular_arc(const vgl_point_2d<double >& line_p1,
                        const vgl_point_2d<double >& line_p2,
                        const vgl_point_2d<double >& arc_p1, 
                        const vgl_point_2d<double > arc_p2,
                        double arc_k,
                        vcl_vector<double >& line_ratios, 
                        vcl_vector<double >& arc_ratios)
{
  // See ultraserver.lems.brown.edu/vision/docs/kimia/reports/notes/
  // circular-arc-formulae/paper.tex for detailed description of algorithm

  // clear old stuffs
  line_ratios.clear();
  arc_ratios.clear();

  // Case 1: degenerate arc (k = 0)
  if (vnl_math::abs(arc_k) < dbgl_closest_point::min_curvature)
  {
    double line_ratio, arc_ratio;
    double dist = dbgl_closest_point::lineseg_lineseg(
      line_p1, line_p2, arc_p1, arc_p2, line_ratio, arc_ratio);
    line_ratios.push_back(line_ratio);
    arc_ratios.push_back(arc_ratio);
    return dist;
  }

  // Case 2: regular arc (k != 0)
  dbgl_circ_arc arc(arc_p1, arc_p2, arc_k);
  vgl_vector_2d<double > lineseg = line_p2 - line_p1;

  // tangent of arc at `arc_p1'
  vgl_vector_2d<double > psi0 = arc.tangent_at_start();
  
  // tangent along the line segment, pointing from `line_p1' to `line_p2'
  vgl_vector_2d<double > phi0 = normalized(lineseg);

  // equations
  // sin(ks + psi0) = a1*t + b1
  // cos(ks + psi0) = a2*t + b2
  double a1 =  arc_k * phi0.x();
  double a2 = -arc_k * phi0.y();
  double b1 =  arc_k * (line_p1.x()-arc_p1.x()) + psi0.y();
  double b2 = -arc_k * (line_p1.y()-arc_p1.y()) + psi0.x();

  // solve quadratic equation for t
  // (a1^2 + a2^2) t^2 + 2(a1*b1 + a2*b2)t + (b1^2+b2^2-1) = 0, or
  //             a t^2 + 2 b + c = 0
  // note a1^2 + a2^2 = k^2
  double a = arc_k*arc_k;
  double b = 2*(a1*b1 + a2*b2);
  double c = b1*b1 + b2*b2 - 1;
  vcl_vector<double > roots;
  dbnl_solve_quadratic_equation(a, b, c, roots);

  double ret_distance = -1;
  // if there are real roots then the CIRCLE and the LINE intersect
  if (roots.size() > 0)
  {
    // compute solution pairs (ti, k*si) for the intersection points
    // then perform range check
    for (unsigned int i=0; i<roots.size(); ++i)
    {
      double ti = roots[i];
      double ratio_ti = ti / lineseg.length();
      // check if this point lies within the line segment
      if (!(0 <= ratio_ti && ratio_ti <= 1)) continue;
      
      // angle psi0 + k*s
      vgl_vector_2d<double > psi0_plus_ks(a2*ti+b2, a1*ti+b1);

      // `psi0_plus_ks' should already be normalized
      // find k*s = (psi0 + ks) - psi0
      double ksi = vcl_atan2(cross_product(psi0, psi0_plus_ks), 
        dot_product(psi0, psi0_plus_ks));
      double ratio_si = ksi / (arc_k*arc.len()) ;
      
      // check if this point lies within the circular arc
      if (!(0 <= ratio_si && ratio_si <= 1)) continue;

      // range check passed. This is a valid intersection point. Record it.
      line_ratios.push_back(ratio_ti);
      arc_ratios.push_back(ratio_si);
      ret_distance = 0;
    }
  }
  // there are no real roots - the circle does not intersect the line
  // need to find closest point between the CIRCLE and the LINE, the perfor range check
  else
  {
    // parameter (t) of closest point on line is the average of two imaginary roots
    double t = -b/(2*a);
    double ratio_t = t / lineseg.length();

    // only continue if this point lies within the line segment
    if (0 <= ratio_t && ratio_t <= 1)
    {
      // closest point on  line
      vgl_point_2d<double > line_foot = line_p1+ t*phi0;

      // compute parameter of closest point on arc
      // there are two candidates, which are half-circle apart and one
      // corresponds to min-distance, one max-distance
      vgl_vector_2d<double > psi0_plus_ks(a2*t+b2, a1*t+b1);
      double ks[2];
      ks[0] = atan2(cross_product<double >(psi0, psi0_plus_ks), 
        dot_product<double >(psi0, psi0_plus_ks));
      
      // make sure ks[1] is in range [-pi, pi]
      ks[1] = (ks[0]>0) ? (ks[0]-vnl_math::pi) : (ks[0]+vnl_math::pi);

      // need to find out which one gives min-distance
      double dist[2];
      for (int i=0; i<2; ++i)
      {
        vgl_point_2d<double > pt = arc.point_at_length(ks[i]/arc.k());
        dist[i] = vnl_math::hypot(pt.x()-line_foot.x(), pt.y()-line_foot.y());
      }
      double min_dist_ks = (dist[0] > dist[1]) ? ks[1] : ks[0];

      // not necessary any more
      //// normalized `min_dist_ks'
      //if (min_dist_ks > vnl_math::pi) min_dist_ks -= vnl_math::pi;

      double ratio_s = min_dist_ks / (arc.k()*arc.len());

      // only record the point when it lies within the arc
      if (0 <= ratio_s && ratio_s <= 1)
      {
        line_ratios.push_back(ratio_t);
        arc_ratios.push_back(ratio_s);
        ret_distance = (dist[0] > dist[1]) ? dist[1] : dist[0];
      }
    }
  }

  if (line_ratios.size() > 0) return ret_distance;

  // if non of the local minima satisfy range constraints, then min-distance 
  // is between one end point and the other object (line segment or arc)
  double distances[4];
  double line_candidate_ratios[4];
  double arc_candidate_ratios[4];

  // 1st set: line_p1 and arc
  line_candidate_ratios[0] = 0;
  distances[0] = dbgl_closest_point::point_to_circular_arc(
    line_p1, arc_p1, arc_p2, arc_k, arc_candidate_ratios[0]);

  // 2nd set: line_p2 and arc
  line_candidate_ratios[1] = 1;
  distances[1] = dbgl_closest_point::point_to_circular_arc(
    line_p2, arc_p1, arc_p2, arc_k, arc_candidate_ratios[1]);

  // 3rd set: arc_p1 and line segment
  double ret_x, ret_y;

  arc_candidate_ratios[2] = 0;
  vgl_closest_point_to_linesegment(ret_x, ret_y, line_p1.x(), line_p1.y(), 
    line_p2.x(), line_p2.y(), arc_p1.x(), arc_p1.y());
  distances[2] = vnl_math::hypot(ret_x - arc_p1.x(), ret_y - arc_p1.y());
  line_candidate_ratios[2] = ratio<double >(line_p1, line_p2, 
    vgl_point_2d<double >(ret_x, ret_y));

  // 4th set: arc_p2 and line segment
  arc_candidate_ratios[3] = 1;
  vgl_closest_point_to_linesegment(ret_x, ret_y, line_p1.x(), line_p1.y(), 
    line_p2.x(), line_p2.y(), arc_p2.x(), arc_p2.y());
  distances[3] = vnl_math::hypot(ret_x - arc_p2.x(), ret_y - arc_p2.y());
  line_candidate_ratios[3] = ratio<double >(line_p1, line_p2, 
    vgl_point_2d<double >(ret_x, ret_y));
  
  // now we compare the distances of 4 sets to find the min-distance
  ret_distance = distances[0];
  int order = 0;
  for (int i=1; i<4; ++i)
  {
    if (distances[i] < ret_distance)
    {
     ret_distance = distances[i];
     order = i;
    }
  }

  line_ratios.push_back(line_candidate_ratios[order]);
  arc_ratios.push_back(arc_candidate_ratios[order]);
  return ret_distance;
}




//: Return distance between a line segment and a circular arc
// 'line_ratios' and 'arc_ratios' are normalized arclength (in range [0, 1])
// of the closest points
double dbgl_closest_point::
lineseg_to_circular_arc(const vgl_line_segment_2d<double >& lineseg,
                        const dbgl_circ_arc& arc,
                        vcl_vector<double >& line_ratios, vcl_vector<double >& arc_ratios)
{
  return dbgl_closest_point::lineseg_to_circular_arc(
    lineseg.point1(), lineseg.point2(), arc.start(), arc.end(), arc.k(), 
    line_ratios,  arc_ratios);
}







//: Return distance between two circular arcs
// 'arc1_ratios' and 'arc2_ratios' are normalized arclength (in range [0, 1])
// of the closest points
double dbgl_closest_point::
circular_arc_to_circular_arc(const vgl_point_2d<double >& arc1_p1,
                             const vgl_point_2d<double >& arc1_p2,
                             double arc1_k,
                             const vgl_point_2d<double >& arc2_p1,
                             const vgl_point_2d<double >& arc2_p2,
                             double arc2_k,
                             vcl_vector<double >& arc1_ratios, 
                             vcl_vector<double >& arc2_ratios)
{
  // See ultraserver.lems.brown.edu/vision/docs/kimia/reports/notes/
  // circular-arc-formulae/paper.tex for detailed description of algorithm

  arc1_ratios.clear();
  arc2_ratios.clear();

  // --------- cases (arc1_k == 0 or arc2_k ==0) ------------------
  if (vnl_math::abs(arc1_k) < dbgl_closest_point::min_curvature)
    return dbgl_closest_point::lineseg_to_circular_arc(arc1_p1, arc1_p2,
    arc2_p1, arc2_p2, arc2_k, arc1_ratios, arc2_ratios);

  if (vnl_math::abs(arc2_k) < dbgl_closest_point::min_curvature)
    return dbgl_closest_point::lineseg_to_circular_arc(arc2_p1, arc2_p2,
    arc1_p1, arc1_p2, arc1_k, arc2_ratios, arc1_ratios);

  // -------  regular arcs (arc1_k != 0 and arc2_k !=0) ------------

  // construct two arcs, 
  // making the first one's curvature less than the second one's
  dbgl_circ_arc arc1(arc1_p1, arc1_p2, arc1_k);
  dbgl_circ_arc arc2(arc2_p1, arc2_p2, arc2_k);
  
  vcl_vector<double >* ret_ratios1 = &arc1_ratios;
  vcl_vector<double >* ret_ratios2 = &arc2_ratios;

  // swap when |k2| < |k1| so that the term (k1/k2), involed in some computation,
  // is always in the range [-1, 1], 
  // This allows us avoiding large number computation (when k1 >> k2)
  if (vnl_math::abs(arc1_k) > vnl_math::abs(arc2_k))
  {
    arc1.set(arc2_p1, arc2_p2, arc2_k);
    ret_ratios1 = &arc2_ratios;

    arc2.set(arc1_p1, arc1_p2, arc1_k);
    ret_ratios2 = &arc1_ratios;
  }
  

  // from this point on, all codes should refer ONLY to arc1, arc1, ret_ratios1,
  // and ret_ratios2. DO NOT use arc1_p1, arc1_p2, ...

  vgl_vector_2d<double > psi0 = arc1.tangent_at_start();
  vgl_vector_2d<double > phi0 = arc2.tangent_at_start();

  // equations for intersection
  // sin(k1*s + psi0) = a sin(k2*t + phi0) + b1
  // cos(k1*s + psi0) = a cos(k2*t + phi0) + b2
  double a = arc1.k()/arc2.k();
  double b1 =  arc1.k()*(arc2.point1().x()-arc1.point1().x()) - 
    arc1.k()/arc2.k() * phi0.y() + psi0.y();
  double b2 = -arc1.k()*(arc2.point1().y()-arc1.point1().y()) - 
    arc1.k()/arc2.k() * phi0.x() + psi0.x();

  // solve trignometry equation to find t
  // equation (1)
  // 2a*b1 sin(k2*t + phi0) + 2a*b2 cos(k2*t + phi0) + (a^2+b1^2+b2^2-1)=0
  vcl_vector<vgl_vector_2d<double > > roots;
  double eq_a = 2*a*b1; 
  double eq_b = 2*a*b2;
  double eq_c = a*a+b1*b1+b2*b2-1;
  dbgl_closest_point::solve_1st_order_trig_equation(eq_a, eq_b, eq_c, roots);
  
  // if there is a root, the two CIRCLES intersect
  // the intersection points need to satisfy range constraints for the 
  // two circular arcs to intersect
  double ret_distance = -1;

  // NO intersection check when when two arcs lie on the SAME circle
  if (roots.size()==1 || roots.size()==2)
  {
    for (unsigned int i=0; i<roots.size(); ++i)
    {
      vgl_vector_2d<double > phi0_plus_k2xt = roots[i];
      double k2xt = vcl_atan2(cross_product(phi0, phi0_plus_k2xt), 
        dot_product(phi0, phi0_plus_k2xt));
      double ratio_t = k2xt/arc2.k() / arc2.len();

      // only continue if this intersection point lies within arc2
      if (0 <= ratio_t && ratio_t <= 1)
      {
        vgl_vector_2d<double > psi0_plus_k1xs(a*phi0_plus_k2xt.x()+b2,
          a*phi0_plus_k2xt.y()+b1);
        double k1xs =  vcl_atan2(cross_product(psi0, psi0_plus_k1xs),
          dot_product(psi0, psi0_plus_k1xs));
        double ratio_s = k1xs / arc1.k() / arc1.len();
        
        // record this intersection point if it also lies within arc1 
        if (0<= ratio_s && ratio_s <= 1)
        {
          ret_ratios1->push_back(ratio_s);
          ret_ratios2->push_back(ratio_t);
          ret_distance = 0;
        }
      }
    }
  }
  // if the two circles do NOT intersect, find their closest point
  else if (roots.size() == 0)
  {
    // the closest point is the midpoint of two `intersection point'
    // if there were,
    // The midpoint of two solutions of equation is
    // a sin(x) + b cos(x) + c = 0
    // (cos(midpoint), sin(midpoint)) = (b/sqrt(a^2+b^2), a/sqrt(a^2, b^2))
    // applying this to equation (1) to find midpoint of its two solutions

    vgl_vector_2d<double > phi0_plus_k2xt(b2, b1);
    double k2xt[2];
    k2xt[0] = vcl_atan2(cross_product(phi0, phi0_plus_k2xt),
        dot_product(phi0, phi0_plus_k2xt));

    // make sure k2xt[1] is in [-pi, pi]
    k2xt[1] = (k2xt[0] > 0) ? k2xt[0]-vnl_math::pi : k2xt[0]+vnl_math::pi;

    double t[2];
    vgl_point_2d<double > arc2_pts[2];
    for (int i=0; i<2; ++i)
    {
      t[i] = k2xt[i] / arc2.k();
      arc2_pts[i] = arc2.point_at_length(t[i]);
    }

    vgl_vector_2d<double > psi0_plus_k1xs(b2, b1);
    double k1xs[2];
    k1xs[0] = vcl_atan2(cross_product(psi0, psi0_plus_k1xs),
        dot_product(psi0, psi0_plus_k1xs));

    // make sure k1xs[1] is in [-pi, pi]
    k1xs[1] = (k1xs[0] > 0) ? (k1xs[0]-vnl_math::pi) : (k1xs[0]+vnl_math::pi);

    double s[2];
    vgl_point_2d<double > arc1_pts[2];
    for (int i=0; i<2; ++i)
    {
      s[i] = k1xs[i] / arc1.k();
      arc1_pts[i] = arc1.point_at_length(s[i]);
    }

    // now comparing distances of 4 pair of points to find minimum
    // even though some of the distances may involve very large number
    // and the final min-distance is not large, hence accurate
    double min_dist_t=0, min_dist_s=0;
    double min_dist = vcl_numeric_limits<double >::max();
    for (int i=0; i<2; ++i)
    {
      for (int j=0; j<2; ++j)
      {
        double d = vnl_math::hypot(arc1_pts[i].x()-arc2_pts[j].x(),
          arc1_pts[i].y()-arc2_pts[j].y());
        // update when a smaller distance is found
        if (d < min_dist)
        {
          min_dist = d;
          min_dist_s = s[i];
          min_dist_t = t[j];
        }
      }
    }

    // range check
    double ratio_s = min_dist_s / arc1.len();
    double ratio_t = min_dist_t / arc2.len();
    if (0<= ratio_s && ratio_s <= 1 && 0<= ratio_t && ratio_t <= 1)
    {
      // record the pair of local minimum points
      ret_ratios1->push_back(ratio_s);
      ret_ratios2->push_back(ratio_t);
      ret_distance = min_dist;
    }
  }
  // if closest point has been found, then return the distance
  if (ret_ratios1->size() > 0) return ret_distance;

  // if none of the local minima satisfy range constraints, then
  // the global minimum is between one's end point and the other arc
  
  double distances[4];
  double arc1_candidate_ratios[4];
  double arc2_candidate_ratios[4];

  // 1st set: arc1.point1() and arc2
  arc1_candidate_ratios[0] = 0;
  distances[0] = dbgl_closest_point::point_to_circular_arc(arc1.point1(), 
    arc2.point1(), arc2.point2(), arc2.k(), arc2_candidate_ratios[0]);

  // 2nd set: arc1.point2() and arc2
  arc1_candidate_ratios[1] = 1;
  distances[1] = dbgl_closest_point::point_to_circular_arc(arc1.point2(), 
    arc2.point1(), arc2.point2(), arc2.k(), arc2_candidate_ratios[1]);

  // 3rd set: arc2.point1() and arc1
  arc2_candidate_ratios[2] = 0;
  distances[2] = dbgl_closest_point::point_to_circular_arc(arc2.point1(), 
    arc1.point1(), arc1.point2(), arc1.k(), arc1_candidate_ratios[2]);

  // 4th set: arc2.point2() and arc1
  arc2_candidate_ratios[3] = 1;
  distances[3] = dbgl_closest_point::point_to_circular_arc(arc2.point2(), 
    arc1.point1(), arc1.point2(), arc1.k(), arc1_candidate_ratios[3]);

  // now we compare the distances of 4 sets to find the min-distance
  ret_distance = distances[0];
  int order = 0;
  for (int i=1; i<4; ++i)
  {
    if (distances[i] < ret_distance)
    {
     ret_distance = distances[i];
     order = i;
    }
  }

  ret_ratios1->push_back(arc1_candidate_ratios[order]);
  ret_ratios2->push_back(arc2_candidate_ratios[order]);
  return ret_distance;
}




// --------------------------------------------------------------------------
// Closest point x-to-biarc
// --------------------------------------------------------------------------
//: Compute closest between a point and a biarc
// Return the shortest distance and saved the arc_length of the closest point
// at `ret_arc_length'
double dbgl_closest_point::
point_to_biarc(const vgl_point_2d<double > query,
               const dbgl_biarc& biarc, double& ret_arclength)
{
  
  double min_dist = vgl_distance(query, biarc.start());
  ret_arclength = 0;

  // only compute for meaning arc segment
  if (biarc.len1() > dbgl_closest_point_distance_epsilon)
  {
    dbgl_circ_arc arc1(biarc.start(), biarc.mid_pt(), biarc.k1());
    double ratio1 = -1;
    double d1 = dbgl_closest_point::point_to_circular_arc(query, 
    arc1.point1(), arc1.point2(), arc1.k(), ratio1);

    if (d1 < min_dist)
    {
      min_dist = d1;
      ret_arclength = ratio1 * biarc.len1();
    }
  }

  if (biarc.len2() > dbgl_closest_point_distance_epsilon)
  {
    dbgl_circ_arc arc2(biarc.mid_pt(), biarc.point_at(biarc.len()), biarc.k2());
    double ratio2 = -1;
    double d2 = dbgl_closest_point::point_to_circular_arc(query, 
      arc2.point1(), arc2.point2(), arc2.k(), ratio2);

    if (d2 < min_dist)
    {
      min_dist = d2;
      ret_arclength = biarc.len1() + ratio2 * biarc.len2();
    }
  }

  return min_dist;

}


// -----------------------------------------------------------------------
//: Solve trigonometry equation a sin(x) + b cos(x) + c = 0
void dbgl_closest_point::
solve_1st_order_trig_equation(double a, double b, double c,
  vcl_vector<vgl_vector_2d<double > >& roots)
{
  roots.clear();

  vcl_vector<double > cos_x;
  vcl_vector<double > sin_x;
  dbnl_solve_1st_order_trig_equation(a, b, c, sin_x, cos_x);

  for (unsigned i =0; i < cos_x.size(); ++i)
  {
    roots.push_back(vgl_vector_2d<double >(cos_x[i], sin_x[i]));
  }
  
  return;
}



