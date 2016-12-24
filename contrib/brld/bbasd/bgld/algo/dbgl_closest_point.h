// This is basic/dbgl/algo/dbgl_closest_point.h

#ifndef dbgl_closest_point_h_
#define dbgl_closest_point_h_

//:
// \file
// \brief Algorithms to compute closest point between geometry objects
// vgl already has functions to compute closest point from a point to other 
// objects such as line, polygon, etc
// This class will add to the function list, i.e. compute closest points 
// between line segments, line-arc, arc-arc, etc.
//
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date 08/08/2005
//
// \verbatim  
//  Modifications:
//    Nhon Trinh   08/08/2005    Initial version
//  
// \endverbatim

#include <vgl/vgl_line_segment_2d.h>
#include <vcl_complex.h>
#include <dbgl/algo/dbgl_biarc.h>
#include <dbgl/algo/dbgl_circ_arc.h>

//: A set of static functions to compute closest points of geometry objects
class dbgl_closest_point
{
protected:
public:
  ~dbgl_closest_point(){};

  //: minimum curvature of an arc
  static double min_curvature;

  //: Return shortest distance between two line segments.
  // `ratio1' and 'ratio2' (in range [0, 1]) are normalized arclength of 
  // closest points between the two line segments
  static double lineseg_lineseg(const vgl_point_2d<double >& line1_p1,
    const vgl_point_2d<double >& line1_p2,
    const vgl_point_2d<double >& line2_p1,
    const vgl_point_2d<double >& line2_p2,
    double& ratio1, double& ratio2);

  //: Return distance between a point and a circular arc.
  // 'ret_ratio' (in range [0, 1]) is the normalized arclength of closest point
  // on the arc
  static double point_to_circular_arc(const vgl_point_2d<double >& query_pt,
    const vgl_point_2d<double >& p1, const vgl_point_2d<double >& p2, double k, 
    double & ret_ratio);

  //: Return distance between a point and a circle
  static double point_to_circle(const vgl_point_2d<double >& query_pt,
    const vgl_point_2d<double >& p0, 
    const vgl_vector_2d<double >& t0, 
    double k, 
    double & ret_arc_length);

  //: Return distance between a line segment and a circular arc
  // 'line_ratios' and 'arc_ratios' are normalized arclength (in range [0, 1])
  // of the closest points
  static double lineseg_to_circular_arc(const vgl_point_2d<double >& line_p1,
    const vgl_point_2d<double >& line_p2,
    const vgl_point_2d<double >& arc_p1, 
    const vgl_point_2d<double > arc_p2,
    double arc_k,
    vcl_vector<double >& line_ratios, vcl_vector<double >& arc_ratios);

  //: Return distance between a line segment and a circular arc
  // 'line_ratios' and 'arc_ratios' are normalized arclength (in range [0, 1])
  // of the closest points
  static double lineseg_to_circular_arc(const vgl_line_segment_2d<double >& lineseg,
    const dbgl_circ_arc& arc,
    vcl_vector<double >& line_ratios, vcl_vector<double >& arc_ratios);

  //: Return distance between two circular arcs
  // 'arc1_ratios' and 'arc2_ratios' are normalized arclength (in range [0, 1])
  // of the closest points
  static double circular_arc_to_circular_arc(
    const vgl_point_2d<double >& arc1_p1,
    const vgl_point_2d<double >& arc1_p2,
    double arc1_k,
    const vgl_point_2d<double >& arc2_p1,
    const vgl_point_2d<double >& arc2_p2,
    double arc2_k,
    vcl_vector<double >& arc1_ratios, 
    vcl_vector<double >& arc2_ratios);


  // --------------------------------------------------------------------------
  // Closest point x-to-biarc
  // --------------------------------------------------------------------------
  //: Compute closest between a point and a biarc
  // Return the shortest distance and saved the arc_length of the closest point
  // at `ret_arclength'
  static double point_to_biarc(const vgl_point_2d<double > query,
    const dbgl_biarc& biarc, double& ret_arclength);


protected:
  ////: Solve quadratic equation ax^2 + bx + c = 0, a != 0
  //static void solve_quadratic_equation(double a, double b, double c, 
  //  vcl_vector<double >& roots);

  //: Solve trigonometry equation a sin(x) + b cos(x) + c = 0
  // returned roots have format: root(cos(x), sin(x))
  static void solve_1st_order_trig_equation(double a, double b, double c,
    vcl_vector<vgl_vector_2d<double > >& roots);

private:
  dbgl_closest_point(){};
};

#endif // dbgl_closest_point_h_
