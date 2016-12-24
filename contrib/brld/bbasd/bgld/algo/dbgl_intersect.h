// This is basic/dbgl/algo/dbgl_intersect.h

#ifndef dbgl_intersect_h_
#define dbgl_intersect_h_

//:
// \file
// \brief Algorithms to compute intersection of geometry objects
// vgl already has function to detect intersection of two line segments
// this class will add more to it, i.e. intersection of line-arc, arc-arc, etc
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date 07/14/05
//
// \verbatim  
//  Modifications:
//    Nhon Trinh        7/14/2005     Initial version
//    Ming-Ching Chang  Mar 07, 2007  Add ray-triangle intersection (db3p/ray_tri).
//  
// \endverbatim

#include <vgl/vgl_line_segment_2d.h>
#include <vgl/vgl_line_3d_2_points.h>
#include <vgl/vgl_homg_line_3d_2_points.h>
#include <vgl/vgl_sphere_3d.h>



//: test if a ray intersect a triangle or not.
//  P: starting point of ray.
//  D: unit vector of ray.
//  V1, V2, V3: 3 vertices ABC of triangle
//  return true if there is an intersection.
//  t: dist(PF), (1-u-v, u, v): barycentric coord of F on triangle ABC.
//
bool dbgl_intersect_ray_tri (const vgl_point_3d<double>& P, 
                             const vgl_vector_3d<double>& D,
                             const vgl_point_3d<double>& V1, 
                             const vgl_point_3d<double>& V2, 
                             const vgl_point_3d<double>& V3,
                             double& t, double& u, double& v);

// ###############################################################

//: A class to compute intersection of geometry objects
// Reference for computing arc-arc intersection
// Agarwal, P., et al. "Counting Circular Arc Intersections." 2002.
class dbgl_intersect
{
protected:
public:
 
  ~dbgl_intersect(){};
  
  //: finds the intersection of a line with a circle and returns the length of 
  // the intersection segment
  static double sphere_line_intersect(vgl_line_3d_2_points<double> const& line, 
                         vgl_sphere_3d<double>& sphere);    

  static double sphere_homg_line_intersect(vgl_homg_line_3d_2_points<double> const& line, 
    vgl_sphere_3d<double>& sphere);

  //: Intersects line segment with line; returns false if no intersection
  static bool line_lineseg_intersect(
      const vgl_line_2d<double> &line, 
      const vgl_line_segment_2d<double> &seg,
      vgl_point_2d<double> &point);

  //: Intersect a plane and an infinite cylinder, which results in an ellipse
  // Return false if the cylinder is parallel to the plane
  // An arbitrary point on the ellipse can be described as
  // c(t) = ellipse_center + cos(t) * ellipse_major_axis + sin(t) * ellipse_minor_axis
  // where t is in [0, 2pi)
  // \relates vgl_plane_3d
  // \relates vgl_cylinder
  static bool plane_infinite_cylinder(const vgl_plane_3d<double >& plane, 
    const vgl_cylinder<double >& cylinder,
    vgl_point_3d<double >& ellipse_center,
    vgl_vector_3d<double >& ellipse_major_axis,
    vgl_vector_3d<double >& ellipse_minor_axis);


private:
     dbgl_intersect(){};  
};



#endif // dbgl_intersect_h_
