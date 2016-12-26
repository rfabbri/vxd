#include <testlib/testlib_test.h>

#include <dbsol/algo/dbsol_curve_algs.h>
#include <dbsol/dbsol_interp_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vgl/vgl_vector_2d.h>
#include <vnl/vnl_math.h>

#include <vcl_iostream.h> 
#include <vcl_cmath.h>
#include <vcl_limits.h>


MAIN( test_interp_eno )
{
   START ("2D ENO Interpolation");
  
   vcl_vector<vsol_point_2d_sptr> pts;

   pts.push_back(new vsol_point_2d(0, vcl_pow(0, 2.0)));
   pts.push_back(new vsol_point_2d(0.2, vcl_pow(0.2, 2.0)));
   pts.push_back(new vsol_point_2d(1.5, vcl_pow(1.5, 2.0)));
   pts.push_back(new vsol_point_2d(2.2, vcl_pow(2.2, 2.0)));
   pts.push_back(new vsol_point_2d(2.3, vcl_pow(2.3, 2.0)));
   pts.push_back(new vsol_point_2d(3.6, vcl_pow(3.6, 2.0)));
   pts.push_back(new vsol_point_2d(4.0, vcl_pow(4.0, 2.0)));

   dbsol_interp_curve_2d c;
   vnl_vector<double> samples;
   dbsol_curve_algs::interpolate_eno(&c,pts,samples);
   vcl_cout << c;

   vcl_vector<vgl_point_2d<double> > pts2;
   double s1 = 1; 
   double s2 = 2; 
   double s3 = 4;
   pts2.push_back(vgl_point_2d<double>(s1, 2*vcl_pow(s1,2.0) - 3*s1 + 1));
   pts2.push_back(vgl_point_2d<double>(s2, 2*vcl_pow(s2,2.0) - 3*s2 + 1));
   pts2.push_back(vgl_point_2d<double>(s3, 2*vcl_pow(s3,2.0) - 3*s3 + 1));

   dbsol_interp_curve_2d c2;
   vnl_vector<double> samples2;
   samples2.set_size(3);
   samples2(0) = 1; samples2(1) = 2; samples2(2) = 4;
   dbsol_curve_algs::interpolate_eno(&c2,pts2,samples2);
   vcl_cout << c2;


   vcl_vector<vgl_point_2d<double> > pts3;
   s1 = 1; 
   s2 = 3; 
   pts3.push_back(vgl_point_2d<double>(s1, -3*s1 + 1));
   pts3.push_back(vgl_point_2d<double>(s2, -3*s2 + 1));

   dbsol_interp_curve_2d c3;
   vnl_vector<double> samples3;
   samples3.set_size(2);
   samples3(0) = 1; samples3(1) = 3;
   dbsol_curve_algs::interpolate_eno(&c3,pts3,samples3);
   vcl_cout << c3;

   SUMMARY();
}
