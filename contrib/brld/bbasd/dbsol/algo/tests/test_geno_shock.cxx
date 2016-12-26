#include <testlib/testlib_test.h>

#include <dbsol/algo/dbsol_geno_shock.h>
#include <dbsol/dbsol_interp_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <vnl/vnl_math.h>

#include <vcl_iostream.h> 
#include <vcl_cmath.h>
#include <vcl_limits.h>



//static bool almost_equal3(const vgl_point_2d<double> &a, const vgl_point_2d<double> &b) {
//  return (vcl_fabs(a.x()-b.x()) < vcl_numeric_limits<double>::epsilon()*100 &&
//          vcl_fabs(a.y()-b.y()) < vcl_numeric_limits<double>::epsilon()*100);
//}

MAIN( test_geno_shock )
{
   START ("2D Shock-placing GENO Interpolation");


   vcl_vector<vsol_point_2d_sptr> pts;

   // unit circle
   pts.push_back(new vsol_point_2d(1,0));
   pts.push_back(new vsol_point_2d(0,1));
   pts.push_back(new vsol_point_2d(-1,0));
   pts.push_back(new vsol_point_2d(0,-1));

   {
   // Open curve:
   dbsol_gshock_curve_2d c;
   dbsol_geno::interpolate(&c,pts,false);  // plain geno

   dbsol_geno_shock::place_all_shocks(c); // shocks

   vcl_cout << c << vcl_endl;
   }


   // Closed Curve:
   dbsol_gshock_curve_2d c;
   dbsol_geno::interpolate(&c,pts, true);

   dbsol_geno_shock:: place_all_shocks(c); // shocks

   vcl_cout << c << vcl_endl;

   SUMMARY();
}
