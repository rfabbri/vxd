// This is basic/dbgl/algo/tests/test_fit_circ_arc_spline.cxx

// \author Nhon Trinh
// \date April 16, 2008

#include <testlib/testlib_test.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_numeric_traits.h>
#include <dbgl/algo/dbgl_fit_circ_arc_spline.h>
#include <dbgl/algo/dbgl_closest_point.h>

//: test functions of dbgl_circ_arc class
void test_dbgl_fit_circ_arc_spline()
{

  ////////////////////////////////////////////////////////
  // INTERPOLATE A SINE CURVE WITH A CIRCULAR ARC SPLINE
  ////////////////////////////////////////////////////////

  {
  // Draw a sine curve and circular arc interpolation
  // y = 100* sin(2*pi * t);
  // t = x / 100;

  vcl_vector<vgl_point_2d<double > > pts;
  for (unsigned i =0; i <= 50; ++i)
  {
    double t = i * (1.0/50);
    double x = 100 * t;
    double y = 100 * vcl_sin(2 * vnl_math::pi * t);
    pts.push_back(vgl_point_2d<double > (x, y));
  }

  // Interpolate these curves with a polyarc
  
  //: Interpolate a set of points with a circular arc spline
  vcl_vector<dbgl_circ_arc > arc_list;
  double tol = 1;

  bool success = dbgl_fit_circ_arc_spline_to_polyline(arc_list, pts, tol);

  // Compute the closest distance from each point to the arc spline
  double max_error = 0; 

  for (unsigned i =0; i < pts.size(); ++i)
  {
    vgl_point_2d<double > pt = pts[i];
    // find the closest point
    double min_distance = vnl_numeric_traits<double >::maxval;
    for (unsigned k =0; k < arc_list.size(); ++k)
    {
      dbgl_circ_arc arc = arc_list[k];
      double ratio = 2;
      double distance = dbgl_closest_point::point_to_circular_arc(pt, 
        arc.start(), arc.end(), arc.k(), ratio);

      min_distance = (min_distance <= distance) ? min_distance : distance;
    }

    max_error = (max_error >= min_distance) ? max_error : min_distance;
  }

  vcl_cout << "tol = " << tol << "\n"
    << "max_error = " << max_error << "\n"
    << "Number of arc segments = " << arc_list.size() << "\n";

    // allow for 10% buffer range
  TEST("Interp sine curve with arc spline", max_error <= 1.1*tol, true);
  }




  ////////////////////////////////////////////////////////
  // INTERPOLATE HALF AN ELLIPSE WITH A CIRCULAR ARC SPLINE
  ////////////////////////////////////////////////////////

  {
  double a = 200;
  double b = 50;


  vcl_vector<vgl_point_2d<double > > pts2;
  for (unsigned i =0; i <= 50; ++i)
  {
    double t = i * (vnl_math::pi/50) - vnl_math::pi_over_2;
    double x = a * vcl_cos(t);
    double y = b * vcl_sin(t);
    pts2.push_back(vgl_point_2d<double > (x, y));
  }

  // Interpolate these curves with a polyarc
  
  //: Interpolate a set of points with a circular arc spline
  vcl_vector<dbgl_circ_arc > arc_list2;
  double tol2 = 2;
  
  bool success2 = dbgl_fit_circ_arc_spline_to_polyline(arc_list2, pts2, tol2);

  // Compute the closest distance from each point to the arc spline
  double max_error2 = 0; 

  for (unsigned i =0; i < pts2.size(); ++i)
  {
    vgl_point_2d<double > pt = pts2[i];
    // find the closest point
    double min_distance = vnl_numeric_traits<double >::maxval;
    for (unsigned k =0; k < arc_list2.size(); ++k)
    {
      dbgl_circ_arc arc = arc_list2[k];
      double ratio = 2;
      double distance = dbgl_closest_point::point_to_circular_arc(pt, 
        arc.start(), arc.end(), arc.k(), ratio);

      min_distance = (min_distance <= distance) ? min_distance : distance;
    }

    max_error2 = (max_error2 >= min_distance) ? max_error2 : min_distance;
  }

  vcl_cout << "tol2 = " << tol2 << "\n"
    << "max_error2 = " << max_error2 << "\n"
    << "Number of arc segments2 = " << arc_list2.size() << "\n";

  // allow for 10% buffer range
  TEST("Interp half an ellipse with arc spline", max_error2 <= 1.1*tol2, true);

  }







}


//: Test functions of biarc class
MAIN( test_fit_circ_arc_spline )
{
  START ("Test functions of dbgl_interp_arc_spline");
  test_dbgl_fit_circ_arc_spline();
  SUMMARY();
}

