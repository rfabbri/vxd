#include <testlib/testlib_test.h>

#include <dbsol/algo/dbsol_geno.h>
#include <dbsol/dbsol_interp_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <vnl/vnl_math.h>

#include <vcl_iostream.h> 
#include <vcl_cmath.h>
#include <vcl_limits.h>


static const double tolerance=vcl_numeric_limits<double>::epsilon()*100;

static bool almost_equal3(const vgl_point_2d<double> &a, const vgl_point_2d<double> &b) {
  if (vcl_fabs(a.x()-b.x()) < tolerance && vcl_fabs(a.y()-b.y()) < tolerance) {
    return true;
  } else {
//    vcl_cerr << "Data about differing points" << vcl_endl;
//    vcl_cerr << "Point 1: "  << a << vcl_endl;
//    vcl_cerr << "Point 2: "  << b << vcl_endl;
    return false;
  }
}

// TODO: include eulerspiral interpolation in the testing
MAIN( test_geno )
{
  START ("2D GENO Interpolation");




  {
  vcl_vector<vsol_point_2d_sptr> pts;
  //---------- Open curve ---------
  // unit circle
  pts.push_back(new vsol_point_2d(1,0));
  pts.push_back(new vsol_point_2d(0,1));
  pts.push_back(new vsol_point_2d(-1,0));
  pts.push_back(new vsol_point_2d(0,-1));

  dbsol_geno_curve_2d c;
  dbsol_geno::interpolate(&c,pts,false);

  vcl_cout << c;
  TEST("#Intervals",c.size(),3);
  TEST_NEAR("Total Length",c.length(),1.5*vnl_math::pi,
             tolerance);

  vcl_cout  << "Recovering samples from point_at" << vcl_endl;
  TEST("Point at arclength 0",almost_equal3(c.point_at(0)->get_p(),pts[0]->get_p()),true);
  TEST("Point at arclength pi/2",almost_equal3(c.point_at(vnl_math::pi/2.0)->get_p(),
                                                  pts[1]->get_p()),true);
  TEST("Point at arclength pi",almost_equal3(c.point_at(vnl_math::pi)->get_p(),
                                                  pts[2]->get_p()),true);

  TEST("Point at arclength 1.5pi",almost_equal3(c.point_at(1.5*vnl_math::pi)->get_p(),
                                                  pts[3]->get_p()),true);

  vgl_point_2d<double> p1(vcl_sqrt(2.0)/2.0,vcl_sqrt(2.0)/2.0);
  TEST("Point at arclength pi/4", almost_equal3(c.point_at(vnl_math::pi/4.0)->get_p(),p1),true);

  TEST("Point at total length",almost_equal3(c.point_at(c.length())->get_p(),
                                             pts.back()->get_p()),true);

  TEST("Point at sample 0",almost_equal3(c.point_at_sample(0)->get_p(),pts[0]->get_p()),true);
  TEST("Point at sample 1",almost_equal3(c.point_at_sample(1)->get_p(),pts[1]->get_p()),true);
  TEST("Point at sample 2",almost_equal3(c.point_at_sample(2)->get_p(),pts[2]->get_p()),true);
  TEST("Point at sample 3",almost_equal3(c.point_at_sample(3)->get_p(),pts[3]->get_p()),true);

  // Fine sample test, specially important for left/right interval consistency
  double step=0.0001, s;
  double error=false;
  vsol_point_2d_sptr pt;
  for (double prct=0; prct<=1 && !error; prct+=step) {
    s = prct*c.length();
    pt = c.point_at(s);
    if (!almost_equal3(pt->get_p(),vgl_point_2d<double>(cos(s),sin(s))))
      error=true;
  }

  TEST("Uniform sampling and interval point_at consistency",error,false);

  if (error) {
    vcl_cout << "Data regarding the error:" << vcl_endl;
    vcl_cout << "    Arc length: " << s/vnl_math::pi << "*pi" << vcl_endl;
    vcl_cout << "Analytic point: " << "(" << cos(s) << "," << sin(s) << ")" << vcl_endl;
    vcl_cout << "    Geno point: " << "(" << pt->x() << "," << pt->y() << ")" << vcl_endl;
  }

  } //< end block

  {
  //---------- Straight line samples ---------
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(0,0));
  pts.push_back(new vsol_point_2d(1,1));
  pts.push_back(new vsol_point_2d(2,2));
  pts.push_back(new vsol_point_2d(3,3));

  dbsol_geno_curve_2d c;
  dbsol_geno::interpolate(&c,pts,false);

  vcl_cout << c;

  const double sqrt2 = vcl_sqrt(2.0);
  TEST("#Intervals",c.size(),3);
  TEST_NEAR("Total Length",c.length(),3*sqrt2, tolerance);

  vcl_cout  << "Recovering samples from point_at" << vcl_endl;
  TEST("Point at arclength 0",almost_equal3(c.point_at(0)->get_p(),pts[0]->get_p()),true);
  TEST("Point at arclength sqrt2",almost_equal3(c.point_at(sqrt2)->get_p(),
                                                  pts[1]->get_p()),true);
  TEST("Point at arclength 2sqrt2",almost_equal3(c.point_at(2*sqrt2)->get_p(),
                                                  pts[2]->get_p()),true);

  TEST("Point at arclength 3sqrt2",almost_equal3(c.point_at(3*sqrt2)->get_p(),
                                                  pts[3]->get_p()),true);

  vgl_point_2d<double> p1(1.5,1.5);
  TEST("Point at midway", almost_equal3(c.point_at(1.5*sqrt2)->get_p(), p1),true);

  TEST("Point at total length",almost_equal3(c.point_at(c.length())->get_p(),
                                             pts.back()->get_p()),true);

  TEST("Point at sample 0",almost_equal3(c.point_at_sample(0)->get_p(),pts[0]->get_p()),true);
  TEST("Point at sample 1",almost_equal3(c.point_at_sample(1)->get_p(),pts[1]->get_p()),true);
  TEST("Point at sample 2",almost_equal3(c.point_at_sample(2)->get_p(),pts[2]->get_p()),true);
  TEST("Point at sample 3",almost_equal3(c.point_at_sample(3)->get_p(),pts[3]->get_p()),true);

  // Fine sample test, specially important for left/right interval consistency
  double step=0.001, s;
  double error=false;
  vsol_point_2d_sptr pt;
  for (double prct=0; prct<=1 && !error; prct+=step) {
    s = prct*c.length();
    pt = c.point_at(s);
    if (!almost_equal3(pt->get_p(),vgl_point_2d<double>(s/sqrt2,s/sqrt2)))
      error=true;
  }

  TEST("Uniform sampling and interval point_at consistency",error,false);

  if (error) {
    vcl_cout << "Data regarding the error:" << vcl_endl;
    vcl_cout << "    Arc length: " << s << vcl_endl;
    vcl_cout << "Analytic point: " << "(" << s/sqrt2 << "," << s/sqrt2 << ")" << vcl_endl;
    vcl_cout << "    Geno point: " << "(" << pt->x() << "," << pt->y() << ")" << vcl_endl;
  }
 
  
  } //< end block

  {
  //---------- Closed curve ---------
  vcl_vector<vsol_point_2d_sptr> pts;
  
  // unit circle
  pts.push_back(new vsol_point_2d(1,0));
  pts.push_back(new vsol_point_2d(0,1));
  pts.push_back(new vsol_point_2d(-1,0));
  pts.push_back(new vsol_point_2d(0,-1));

  dbsol_geno_curve_2d c;
  dbsol_geno::interpolate(&c,pts, true);

  vcl_cout << c;
  vcl_cout << "----------------------------\n";
  TEST("#Intervals",c.size(),4);
  TEST_NEAR("Total Length",c.length(),2*vnl_math::pi,
             tolerance);

  TEST("Point at 0 length",almost_equal3(c.point_at(0)->get_p(), pts[0]->get_p()),true);
  TEST("Point at total length",almost_equal3(c.point_at(c.length())->get_p(), pts[0]->get_p()),true);

  TEST_NEAR("Tangent at 0 length",c.tangent_angle_at(0),vnl_math::pi/2, tolerance);
  
  TEST_NEAR("Curvature at 0 length", ((dbgl_arc *) c.interval(0))->curvature(), 1.0, tolerance);
  } //< end block

  SUMMARY();
}
