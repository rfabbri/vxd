// This is basic/dbgl/algo/tests/test_intersect.cxx

#include <testlib/testlib_test.h>
#include <dbgl/algo/dbgl_intersect.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vgl/vgl_line_2d.h>
#include <vgl/vgl_distance.h>
#include <vgl/vgl_cylinder.h>
#include <vgl/vgl_plane_3d.h>



void test_compute_line_sphere_intersect() {

    vcl_cout << "Test intersection between a sphere and a line" << vcl_endl;
    vcl_cout << "Case1: There is only one intersection point" << vcl_endl;
    vgl_point_3d<double> center(10, 0, 0);
    vgl_sphere_3d<double> sphere(center, 5);
    vgl_homg_line_3d_2_points<double> line(vgl_homg_point_3d<double> (0,0,5,1), vgl_homg_point_3d<double> (10,0,5,1));
    double inters_length = dbgl_intersect::sphere_homg_line_intersect(line, sphere);
    TEST_NEAR("Sphere line intersection", inters_length, 5, 1e-8);

    vcl_cout << "Case2: There are two intersection points" << vcl_endl;
    vgl_homg_line_3d_2_points<double> line2(vgl_homg_point_3d<double> (0,0,0,1), vgl_homg_point_3d<double> (10,0,3,1));
    inters_length = dbgl_intersect::sphere_homg_line_intersect(line2, sphere);
    vcl_cout << "length intersc-->" << inters_length << vcl_endl;
    TEST_NEAR("Sphere line intersection", inters_length, 8, 1);

    vcl_cout << "Case3: There are two intersection points" << vcl_endl;
    vgl_homg_line_3d_2_points<double> line3(vgl_homg_point_3d<double> (0,0,0,1), vgl_homg_point_3d<double> (20,0,0,1));
    inters_length = dbgl_intersect::sphere_homg_line_intersect(line3, sphere);
    vcl_cout << "length intersc-->" << inters_length << vcl_endl;
    TEST_NEAR("Sphere line intersection", inters_length, 10, 1e-8);

    vcl_cout << "Case3: There are two intersection points" << vcl_endl;
    vgl_homg_line_3d_2_points<double> line4(vgl_homg_point_3d<double> (0,0,0,1), vgl_homg_point_3d<double> (10,0,10,1));
    inters_length = dbgl_intersect::sphere_homg_line_intersect(line4, sphere);
    vcl_cout << "length intersc-->" << inters_length << vcl_endl;
    TEST_NEAR("Sphere line intersection", inters_length, 0, 1e-8);
}

void 
test_line_lineseg_intersect() 
{
  {
  vgl_line_segment_2d<double> seg(vgl_point_2d<double>(-1,1), vgl_point_2d<double>(-4,-2));
  vgl_line_2d<double> line(0,1,0); //: x-axis

  vgl_point_2d<double> intr;

  bool has_intr;
  has_intr = dbgl_intersect::line_lineseg_intersect(line,seg, intr);

  TEST("Line lineseg intersection",has_intr,true);
  TEST("Line lineseg intersection",vgl_distance<double>(intr,vgl_point_2d<double> (-2,0)) < 1e-8, true);
  }

  { // case where the lines intersect but the segment doesn't
  vgl_line_segment_2d<double> seg(vgl_point_2d<double>(2,4), vgl_point_2d<double>(-1,1));
  vgl_line_2d<double> line(0,1,0); //: x-axis

  vgl_point_2d<double> intr;

  bool has_intr;
  has_intr = dbgl_intersect::line_lineseg_intersect(line,seg, intr);

  TEST("Line lineseg intersection",has_intr,false);
  }
}


void test_plane_infinite_cylinder_intersect()
{
  vcl_cout << "Intersect a plane and an infinite cylinder\n";
  // define a right cylinder
  vgl_point_3d<double > cyl_center (1, 1, 0);
  vgl_vector_3d<double > cyl_orient(0, 0, 1);
  double cyl_radius = 2;
  vgl_cylinder<double > cylinder(cyl_center, cyl_radius, 100, cyl_orient);

  // define a plane that goes thru (1, 1, 1) and has normal = (1, 1, 1)
  vgl_plane_3d<double > plane(vgl_vector_3d<double >(0, 1, 1), vgl_point_3d<double >(1, 1, 1));

  vgl_point_3d<double > ellipse_center;
  vgl_vector_3d<double > ellipse_major;
  vgl_vector_3d<double > ellipse_minor;
  bool intersection_exist = 
    dbgl_intersect::plane_infinite_cylinder(plane, cylinder, ellipse_center, ellipse_major, ellipse_minor);
  TEST("Intersection existence", intersection_exist, true);

  // true intersection axis
  vgl_point_3d<double > true_ellipse_center(1, 1, 1);
  vgl_vector_3d<double > true_ellipse_major(0, -2, 2);
  vgl_vector_3d<double > true_ellipse_minor(2, 0, 0);

  vcl_cout << "Intersection ellipse: "
    << "\n  ellipse_center = " << ellipse_center
    << "\n  ellipse_major = " << ellipse_major
    << "\n  ellipse_minor = " << ellipse_minor << vcl_endl;

  TEST_NEAR("ellipse_center", (ellipse_center-true_ellipse_center).length(), 0, 1e-8);
  
  TEST_NEAR("ellipse_major_axis - direction", 
    angle(ellipse_major, true_ellipse_major), 0, 1e-8);
  TEST_NEAR("ellipse_major_axis - length", 
    ellipse_major.length()-true_ellipse_major.length(), 0, 1e-8);

  TEST_NEAR("ellipse_minor_axis - direction", 
    angle(ellipse_minor, true_ellipse_minor), 0, 1e-8);
    
  TEST_NEAR("ellipse_minor_axis - length", 
    ellipse_minor.length()-true_ellipse_minor.length(), 0, 1e-8);

  vgl_plane_3d<double > plane1(1, 0, 0, -3);
  vgl_plane_3d<double > plane2(0, 1, 0, 8);
  vgl_plane_3d<double > plane3(0, 0, 1, -18);

  vgl_point_3d<double > point(plane1, plane2, plane3);
  vcl_cout << "\nplane 1 = " << plane1
    << "\nplane 2 = " << plane2
    << "\nplane 3 = " << plane3
    << "\nintersection point = " << point << vcl_endl;

  



  return;
}


//: Test functions of biarc class
MAIN( test_intersect )
{
  START (" Test dbgl_intersect class");
  test_compute_line_sphere_intersect();
  test_line_lineseg_intersect();
  test_plane_infinite_cylinder_intersect();
  SUMMARY();
}
