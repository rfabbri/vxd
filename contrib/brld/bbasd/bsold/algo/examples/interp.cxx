#include <bsold/algo/bsold_curve_algs.h>
#include <bsold/algo/bsold_geno.h>
#include <bsold/bsold_interp_curve_2d.h>
#include <dbgl/dbgl_arc.h>
#include <vsol/vsol_point_2d.h>
#include <vcl_iostream.h>
//: Interpolation example

int main()
{

   /*
   { // LINEAR
   vcl_vector<vsol_point_2d_sptr> pts;

   pts.push_back(new vsol_point_2d(0,0));
   pts.push_back(new vsol_point_2d(1,1));
   pts.push_back(new vsol_point_2d(1,0));


   bsold_interp_curve_2d c;

   bsold_curve_algs::interpolate_linear(&c,pts, false);

   vcl_cout << c;

   vcl_cout << "===== Sampling =====\n";
   vcl_cout << "t\t(x(t),y(t))\n";
   double t,t_interval;
   unsigned t_idx;
   for (t=0; t<c.length(); t+=0.5) {
      vcl_cout << t << "\t(" << c.point_at(t)->x() << ", " << c.point_at(t)->y() << 
         ") ";
      t_idx = c.interval_index(t,&t_interval);
      vcl_cout << "at interval #" << t_idx << "\tlocal parameter: " <<  t_interval << vcl_endl;
   }
   // do the last pt
   t = c.length();
   vcl_cout << t << "\t(" << c.point_at(t)->x() << ", " << c.point_at(t)->y() << 
      ") ";
   t_idx = c.interval_index(t,&t_interval);
   vcl_cout << "at interval #" << t_idx << "\tlocal parameter: " <<  t_interval << vcl_endl;
   } // !LINEAR

   */


   { // GENO
   vcl_vector<vsol_point_2d_sptr> pts;
   pts.push_back(new vsol_point_2d(355,68));
   pts.push_back(new vsol_point_2d(356,61));
   pts.push_back(new vsol_point_2d(359,52));
   pts.push_back(new vsol_point_2d(365,48));
   pts.push_back(new vsol_point_2d(369,40));
   pts.push_back(new vsol_point_2d(375,37));
   pts.push_back(new vsol_point_2d(382,37));
   pts.push_back(new vsol_point_2d(382,43));
   pts.push_back(new vsol_point_2d(388,43));
   pts.push_back(new vsol_point_2d(392,45));
   pts.push_back(new vsol_point_2d(394,51));
   pts.push_back(new vsol_point_2d(394,58));


   bsold_geno_curve_2d c;
   bsold_geno::interpolate(&c,pts,false);
   vcl_cout << c;
   vcl_cout << "----------------------------\n";

   /*
   vcl_vector<vsol_point_2d_sptr> pts;

   // unit circle
   pts.push_back(new vsol_point_2d(1,0));
   pts.push_back(new vsol_point_2d(0,1));
   pts.push_back(new vsol_point_2d(-1,0));
   pts.push_back(new vsol_point_2d(0,-1));

   bsold_geno_curve_2d c;
   bsold_geno::interpolate(&c,pts,true);
   vcl_cout << c;
   vcl_cout << "----------------------------\n";

   bsold_geno::interpolate(&c,pts,false);
   vcl_cout << c;

   vcl_cout << "----------------------------\n";
   pts.clear();
   pts.push_back(new vsol_point_2d(-1,0));
   pts.push_back(new vsol_point_2d(0,1));
   pts.push_back(new vsol_point_2d(1,0));
   pts.push_back(new vsol_point_2d(3.5,0.5));
   bsold_geno::interpolate(&c,pts,false);
   vcl_cout << c;
   */

   }

   return 0;
}
