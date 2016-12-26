#include <dbnl/algo/dbnl_eno.h>
#include <dbnl/algo/dbnl_eno_shock.h>
#include <dbnl/algo/dbnl_eno_zerox.h>

int main()
{
   
   /*
   vcl_cout
      << "------------------------------------" << vcl_endl
      << "EXAMPLE 0" << vcl_endl;
   {
   dbnl_eno_shock_1d myeno;

   vnl_vector<double> data(4);

   // these are samples of  p(x) = (x-1.2)(x-1.8)
   data(0) = 2.16;
   data(1) = 0.16;
   data(2) = 0.16;
   data(3) = 2.16;

   myeno.interpolate(&data);
   myeno.place_shocks(); 
   dbnl_eno_zerox_vector z(myeno);
   z.print(myeno, vcl_cout);
   }
   */

   /*
   vcl_cout
      << "------------------------------------" << vcl_endl
      << "EXAMPLE 0" << vcl_endl;
   {
   dbnl_eno_shock_1d myeno;

   vnl_vector<double> data(4);

   data(0) = 5.04;
   data(1) = 1.54;
   data(2) = 0.04;
   data(3) = 3;
   data(4) = 18.32;
   data(5) = 56.1733333;

   myeno.interpolate(&data);
   myeno.place_shocks(); 
   dbnl_eno_zerox_vector z(myeno);
   z.print(myeno, vcl_cout);
   }
   */

   // this is one case where 2nd order interpolation cannot caputure an
   // inflection point and causes a shock.
   vcl_cout << vcl_endl 
      << "------------------------------------" << vcl_endl
      << "EXAMPLE 1" << vcl_endl;
   {
   dbnl_eno_shock_1d myeno;

   double data[4];
   data[0] = 1;
   data[1] = 2;
   data[2] = 1;
   data[3] = 100;

   myeno.interpolate(data, 4);
   myeno.place_shocks();
   dbnl_eno_zerox_vector z(myeno);
   z.print(myeno, vcl_cout);
   }

   /*
   vcl_cout << vcl_endl 
      << "------------------------------------" << vcl_endl
      << "EXAMPLE 2" << vcl_endl;
   {
   dbnl_eno_shock_1d myeno;

   double data[4];
   data[0] = 1;
   data[1] = 2;
   data[2] = 2;
   data[3] = 1;

   myeno.interpolate(data, 4);


   myeno.place_shocks();

//   for (int i=0; i<myeno.size(); ++i)
//      if(myeno.test_shock(i))
//         vcl_cerr << "shock at " << i << vcl_endl; 
//   myeno.place_zerox();
   myeno.print(vcl_cout);
   }

   vcl_cout << vcl_endl 
      << "------------------------------------" << vcl_endl
      << "EXAMPLE: 6 points" << vcl_endl;
   {
   dbnl_eno_shock_1d myeno;

   double data[6];
   data[0] = 0;
   data[1] = 1;
   data[2] = 100;    // no shock is placed!
   data[3] = 100;
   data[4] = 1;
   data[5] = 0;

   myeno.interpolate(data, 6);
   myeno.place_shocks();
//   myeno.place_zerox();
   myeno.print(vcl_cout);
   }
*/
   return 0;
}
