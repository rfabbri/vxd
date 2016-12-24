// This is basic/dbgl/algo/dbgl_distance.cxx
//:
// \file

#include "dbgl_distance.h"

#include <vcl_cmath.h>
#include <vcl_algorithm.h>
#include <vgl/vgl_lineseg_test.h>
#include <vgl/vgl_distance.h>
//#include <vgl/vgl_closest_point.h>


////------------------------------------------------------------------------
////: Return distance between two points
//double dbgl_distance::point_point(const vgl_point_2d<double >& p1,
//  vgl_point_2d<double >& p2)
//{
//  return vcl_sqrt((p2.x()-p1.x())*(p2.x()-p1.x())+
//    (p2.y()-p1.y())*(p2.y()-p1.y()) );
//}


//------------------------------------------------------------------------
//: Return shortest distance between two line segments
double dbgl_distance::
lineseg_lineseg(const vgl_point_2d<double >& line1_p1,
                const vgl_point_2d<double >& line1_p2,
                const vgl_point_2d<double >& line2_p1,
                const vgl_point_2d<double >& line2_p2)
{
  // case 1: the two line segments intersect;
  if (vgl_lineseg_test_lineseg<double >(
    line1_p1.x(), line1_p1.y(), line1_p2.x(), line1_p2.y(), 
    line2_p1.x(), line2_p1.y(), line2_p2.x(), line2_p2.y()))
  {
    return 0;
  }

  // case 2: two line segment don't intersect.
  // the shortest distance is the minimum among
  // distances from one line endpoint to the other line segment.
  else
  {
    double d[4];
    // line2_p1 to line1
    d[0] = vgl_distance2_to_linesegment<double >(line1_p1.x(), line1_p1.y(),
      line1_p2.x(), line1_p2.y(), line2_p1.x(), line2_p1.y());
    // line2_p2 to line1
    d[1] = vgl_distance2_to_linesegment<double >(line1_p1.x(), line1_p1.y(),
      line1_p2.x(), line1_p2.y(), line2_p2.x(), line2_p2.y());
    // line1_p1 to line2
    d[2] = vgl_distance2_to_linesegment<double >(line2_p1.x(), line2_p1.y(),
      line2_p2.x(), line2_p2.y(), line1_p1.x(), line1_p1.y());
    // line1_p2 to line2
    d[3] = vgl_distance2_to_linesegment<double >(line2_p1.x(), line2_p1.y(),
      line2_p2.x(), line2_p2.y(), line1_p2.x(), line1_p2.y());

    // find the min among 4 distances
    double min_dist = d[0];
    for (int i=1; i<4; ++i)
    {
      if (d[i] < min_dist)
        min_dist = d[i];
    }
    return vcl_sqrt(min_dist);
  }
}

