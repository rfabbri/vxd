// This is basic/dbgl/algo/dbgl_intersect.cxx
//:
// \file

#include <vcl_cmath.h>
#include <vcl_iostream.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_distance.h>
#include <vgl/vgl_sphere_3d.h>
#include <vgl/vgl_lineseg_test.h>
#include <vgl/vgl_cylinder.h>
#include <vgl/vgl_intersection.h>
#include <vgl/vgl_closest_point.h>
#include <vgl/vgl_plane_3d.h>

#include <dbgl/dbgl_fuzzy_boolean.h>
#include <dbgl/algo/dbgl_intersect.h>
#include <ray_tri/ray_tri.h>




//: test if a ray intersect a triangle or not.
//  P: starting point of ray.
//  D: unit vector of ray.
//  V1, V2, V3: 3 vertices ABC of triangle
//  return true if there is an intersection.
//  t: dist(PF), (1-u-v, u, v): barycentric coord of F on triangle ABC.
//
bool dbgl_intersect_ray_tri (const vgl_point_3d<double>& P, const vgl_vector_3d<double>& D,
                             const vgl_point_3d<double>& V1, const vgl_point_3d<double>& V2, 
                             const vgl_point_3d<double>& V3,
                             double& t, double& u, double& v)
{  
  double ori[3];
  ori[0] = P.x();
  ori[1] = P.y();
  ori[2] = P.z();

  double vec[3];
  assert (dbgl_eq_m (D.sqr_length(), 1));
  vec[0] = D.x();
  vec[1] = D.y();
  vec[2] = D.z();

  double vert1[3], vert2[3], vert3[3];
  vert1[0] = V1.x();
  vert1[1] = V1.y();
  vert1[2] = V1.z();
  vert2[0] = V2.x();
  vert2[1] = V2.y();
  vert2[2] = V2.z();
  vert3[0] = V3.x();
  vert3[1] = V3.y();
  vert3[2] = V3.z();
 
  bool r = intersect_triangle (ori, vec, vert1, vert2, vert3, &t, &u, &v) != 0; 

  ////Debug: assert that dist < d(P,V1), d(P,V2), d(P,V3).
  //if (r) {
  //  double qq = vgl_distance (P, V1);
  //  double q2 = vgl_distance (P, V2);
  //  double q3 = vgl_distance (P, V3);
  //  assert (t <= vgl_distance (P, V1));
  //  assert (t <= vgl_distance (P, V2));
  //  assert (t <= vgl_distance (P, V3));
  //}
  return r;
}


// -----------------------------------------------------------------------
//: takes two points and finds the intersection of a line with a sphere if any. 
// Returns the length of the intersection. Returns 0 if no intersection is found.
// 
double
dbgl_intersect::sphere_line_intersect(vgl_line_3d_2_points<double> const& line, 
                         vgl_sphere_3d<double>& sphere)
{
    // solving for the au^2 + bu + c=0
    vgl_point_3d<double> point1 =  line.point1();
    vgl_point_3d<double> point2 =  line.point2();
    vgl_point_3d<double> center = sphere.centre();

    double x1 = point1.x();
    double x2 = point2.x();
    double x3 = center.x();
    double y1 = point1.y();
    double y2 = point2.y();
    double y3 = center.y();
    double z1 = point1.z();
    double z2 = point2.z();
    double z3 = center.z();
    double r = sphere.radius();

    // first make sure that there is an intersection
    // if (center-p).(p2-p1)=0, there is an intersection, 
    // where p is the closest point to the center on p1p2 line
    double u = ((x3-x1)*(x2-x1) + (y3-y1)*(y2-y1) + (z3-z1)*(z2-z1))/
                            ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

    if ((u < 0) || (u > 1)) // no intersection
        return 0;

    // the length of the intersection to the center should also be < radius
    vgl_point_3d<double> closest((x1 + u * (x2 - x1)), 
                                (y1 + u * (y2 - y1)), 
                                (z1 + u * (z2 - z1)));
    // the line does not intersect the sphere, if the closest point is further than r
    vgl_vector_3d<double> seg = closest - sphere.centre();
    if (seg.length() > r) 
        return 0;

    double a = (x2 - x1)*(x2-x1) +
             (y2 - y1)* (y2 - y1)+
             (z2 - z1) * (z2 - z1);

    double b = 2*((x2 - x1)*(x1 - x3) +
        (y2 - y1) * (y1 - y3) +
        (z2 - z1) * (z1 - z3));

    double c = x3*x3 + y3*y3 + z3*z3 + x1*x1 + y1*y1 + z1*z1 - 
        2*(x3*x1 + y3*y1 + z3*z1) - r*r;

    double A = (b*b - 4*a*c);
    if (A < 0) { // no intersection
        return 0;
    } else if (A == 0) { // only one intersection point
        u = (-1.0*b) / (2.0*a);
        vgl_point_3d<double> intersection((x1 + u * (x2 - x1)), 
                                          (y1 + u * (y2 - y1)), 
                                          (z1 + u * (z2 - z1)));
        return 0; // the length of the intersection
    }  else { // two intersection points
            double i = (-1.0*b) / (2.0*a);
            double j = sqrt(A) / (2.0*a);

            double u1 = i - j;
            double u2 = i + j;

            vgl_point_3d<double> intersection1((x1 + u1 * (x2 - x1)), 
                                          (y1 + u1 * (y2 - y1)), 
                                          (z1 + u1 * (z2 - z1)));
            vgl_point_3d<double> intersection2((x1 + u2 * (x2 - x1)), 
                                          (y1 + u2 * (y2 - y1)), 
                                          (z1 + u2 * (z2 - z1)));
            return (intersection2 - intersection1).length();
    }
}

//: takes a homg line and a sphere finds the intersection of a line with a sphere if any. 
// Returns the length of the intersection. returns 0 if no intersection is found.
double
dbgl_intersect::sphere_homg_line_intersect(vgl_homg_line_3d_2_points<double> const& line, 
                         vgl_sphere_3d<double>& sphere)
{
    // solving for the au^2 + bu + c=0
    vgl_point_3d<double> ray_start =(line.point_finite());
    vgl_vector_3d<double> ray_direction(line.point_infinite().x(), 
        line.point_infinite().y(), 
        line.point_infinite().z());
    ray_direction = normalize(ray_direction);
    vgl_point_3d<double> centre = sphere.centre();

    double x1 = ray_start.x();
    double x2 = ray_direction.x();
    double x3 = centre.x();
    double y1 = ray_start.y();
    double y2 = ray_direction.y();
    double y3 = centre.y();
    double z1 = ray_start.z();
    double z2 = ray_direction.z();
    double z3 = centre.z();
    double r = sphere.radius();

    // since the direction vector is a unit vector, a is set to 1; 
    //unused double a = 1;

    double b = 2*(x2*(x1 - x3) +
        y2* (y1 - y3) +
        z2* (z1 - z3));

    double c = x3*x3 + y3*y3 + z3*z3 + x1*x1 + y1*y1 + z1*z1 - 
        2*(x3*x1 + y3*y1 + z3*z1) - r*r;

    double A = (b*b - 4*c);
    if (A < 0) { // no intersection
        return 0;
    } else if (A == 0) { // only one intersection point
        double u = (-1.0*b) / 2.0;
        vgl_point_3d<double> intersection(x1 + u * x2, 
                                          y1 + u * y2, 
                                          z1 + u * z2);
        return r; // the length of the intersection
    }  else { // two intersection points
            double i = (-1.0*b) / 2.0;
            double j = sqrt(A) / 2.0;

            double u1 = i - j;
            double u2 = i + j;

            vgl_point_3d<double> intersection1((x1 + u1 * x2), 
                                          (y1 + u1 * y2), 
                                          (z1 + u1 * z2));
            vgl_point_3d<double> intersection2((x1 + u2 * x2), 
                                          (y1 + u2 * y2), 
                                          (z1 + u2 * z2));
            double l = (intersection2 - intersection1).length();
            return l;
    }
}

// ----------------------------------------------------------------------------
bool dbgl_intersect::
line_lineseg_intersect(
    const vgl_line_2d<double> &line, 
    const vgl_line_segment_2d<double> &seg,
    vgl_point_2d<double> &point)
{
  vgl_point_2d<double> pl1, pl2, ps1, ps2;
  ps1 = seg.point1();
  ps2 = seg.point2();
  line.get_two_points(pl1, pl2);

  if (!vgl_lineseg_test_line<double>(
        pl1.x(), pl1.y(), 
        pl2.x(), pl2.y(), 
        ps1.x(), ps1.y(),
        ps2.x(), ps2.y()))

    return false;

  point = vgl_point_2d<double>(line, vgl_line_2d<double>(seg.a(), seg.b(), seg.c()));
  return true;
}


// ----------------------------------------------------------------------------
//: Intersect a plane and an infinite cylinder, which results in an ellipse
// Return false if the cylinder is parallel to the plane
// An arbitrary point on the ellipse can be described as
// c(t) = ellipse_center + cos(t) * ellipse_major_axis + sin(t) * ellipse_minor_axis
// where t is in [0, 2pi)
bool dbgl_intersect::
plane_infinite_cylinder(const vgl_plane_3d<double >& plane, 
                        const vgl_cylinder<double >& cylinder,
                        vgl_point_3d<double >& ellipse_center,
                        vgl_vector_3d<double >& ellipse_major_axis,
                        vgl_vector_3d<double >& ellipse_minor_axis)
{
  vgl_vector_3d<double > plane_n = plane.normal();
  vgl_vector_3d<double > cylinder_t = normalized(cylinder.orientation());

  // first thing first: check whether the plane and the cylinder are parallel
  if (vcl_abs(inner_product(plane_n, cylinder_t)) < 1e-10)
  {
    ellipse_center = vgl_closest_point(plane, cylinder.center());
    ellipse_major_axis = normalized(cylinder.orientation());
    ellipse_minor_axis = cross_product(plane.normal(), ellipse_major_axis); 
    return false;
  }

  // Determine center of ellipse: intersection of the center line of the cylinder and the plane
  vgl_line_3d_2_points<double > cyl_center_line(cylinder.center(), 
    cylinder.center()+cylinder_t); 
  ellipse_center = vgl_intersection(cyl_center_line, plane);

  // Determine the major axis direction

  // find a point on the major axis by projecting a point (unit length away 
  // from the center) onto the plane
  vgl_point_3d<double > proj = vgl_closest_point(ellipse_center + cylinder_t, plane);
  vgl_vector_3d<double > major = proj - ellipse_center;
  // let `a' be the angle between the center line and the major axis
  double sin_a = vcl_sqrt(1 - major.sqr_length());
  double length_major = cylinder.radius() / sin_a;

  // degenerate case: cylinder is orthogonal to the plane, the major axis is arbitrary
  if (major.sqr_length() < 1e-12)
  {
    // find a point that is not on the center line
    vgl_point_3d<double > p = ellipse_center + cylinder_t;
    // at least one of the components squared is less than 0.4
    if (cylinder_t.x() * cylinder_t.x() < 0.4)
      p = ellipse_center + vgl_vector_3d<double >(1+cylinder_t.x(), cylinder_t.y(), cylinder_t.z());
    else if (cylinder_t.y() * cylinder_t.y() < 0.4)
      p = ellipse_center + vgl_vector_3d<double >(cylinder_t.x(), 1+cylinder_t.y(), cylinder_t.z());
    else
      p = ellipse_center + vgl_vector_3d<double >(cylinder_t.x(), cylinder_t.y(), 1+cylinder_t.z());

    vgl_point_3d<double > proj_p = vgl_closest_point(p, plane);
    major = normalized(proj_p - ellipse_center);
  }
  // regular case
  else
  {
    normalize(major);
  }
  ellipse_major_axis = major * length_major;

  // Determine the minor axis
  // x-axis = major, y-axis = minor, z-axis = plane normal
  // y = z cross x
  vgl_vector_3d<double > minor = cross_product(plane_n, major);

  // let `b' be the angle between the center line and the minor axis
  double cos_b = dot_product(cylinder_t, minor);
  double sin_b = vcl_sqrt(1 - cos_b*cos_b);

  double length_minor = cylinder.radius() / sin_b;
  ellipse_minor_axis = length_minor * minor;
  return true;
}

