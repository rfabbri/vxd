#include <testlib/testlib_test.h>

#include <dbsol/algo/dbsol_curve_algs.h>
#include <dbsol/dbsol_interp_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <vnl/vnl_math.h>

#include <vcl_iostream.h> 
#include <vcl_cmath.h>
#include <vcl_limits.h>



static bool almost_equal3(const vgl_point_2d<double> &a, const vgl_point_2d<double> &b) {
  return (vcl_fabs(a.x()-b.x()) < vcl_numeric_limits<double>::epsilon()*100 &&
          vcl_fabs(a.y()-b.y()) < vcl_numeric_limits<double>::epsilon()*100);
}

MAIN( test_interp_linear )
{
   START ("2D Linear Interpolation");
  
   vcl_vector<vsol_point_2d_sptr> pts;

   pts.push_back(new vsol_point_2d(0,0));
   pts.push_back(new vsol_point_2d(1,1));
   pts.push_back(new vsol_point_2d(1,0));

   dbsol_interp_curve_2d c;
   dbsol_curve_algs::interpolate_linear(&c,pts, false);
   vcl_cout << c;

   TEST("#Intervals",c.size(),2);

   TEST_NEAR("Total Length",c.length(),1+vcl_sqrt(2.0), 
              vcl_numeric_limits<double>::epsilon()*100);

   vcl_cout  << "Recovering samples from point_at" << vcl_endl;
   TEST("Point at arclength 0",almost_equal3(c.point_at(0)->get_p(),pts[0]->get_p()),true);
   vgl_point_2d<double> p1(vcl_sqrt(2.0)/2.0, vcl_sqrt(2.0)/2.0);
   TEST("Point at arclength 1",almost_equal3(c.point_at(1)->get_p(),p1),true);
   TEST("Point at arclength sqrt(2)",almost_equal3(c.point_at(vcl_sqrt(2.0))->get_p(),
                                                   pts[1]->get_p()),true);
   TEST("Point at total length",almost_equal3(c.point_at(c.length())->get_p(),
                                              pts[2]->get_p()),true);

   // Closed 
   vcl_cout << "-------------------\n";
   vcl_cout << "Closed curve:\n";
   dbsol_curve_algs::interpolate_linear(&c,pts, true);
   vcl_cout << c;
   TEST("#Intervals",c.size(),3);
   TEST_NEAR("Total Length",c.length(),2+vcl_sqrt(2.0), 
              vcl_numeric_limits<double>::epsilon()*100);

   TEST("Point at 0 length",almost_equal3(c.point_at(0)->get_p(),
                                              pts[0]->get_p()),true);
   TEST("Point at total length",almost_equal3(c.point_at(c.length())->get_p(),
                                              pts[0]->get_p()),true);

   TEST_NEAR("Tangent at 0 length",c.tangent_angle_at(0),vnl_math::pi/4,
              vcl_numeric_limits<double>::epsilon()*100);
   TEST_NEAR("Tangent at total length",c.tangent_angle_at(c.length()),vnl_math::pi,
              vcl_numeric_limits<double>::epsilon()*100);

   SUMMARY();
}
