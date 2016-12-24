//---------------------------------------------------------------------
// This is basic/dbgl/algo/example_generate_eulerspiral_num_iter_table.cxx
//:
// \file
// \brief This program generates a table of number of iterations used for 
// different Euler spiral computation. The result table has the structure
//                  start_angle_1       start_angle_1       num_iter_1
//                  start_angle_2       start_angle_2       num_iter_2
//                  start_angle_3       start_angle_3       num_iter_3
//                      ...                 ...                 ...
//                  start_angle_n       start_angle_n       num_iter_n
//
// \author
//  Nhon Trinh
// \date 2/24/2005
// \verbatim
//  Modifications
//   <none>
// \endverbatim
//
//-------------------------------------------------------------------------

#include <dbgl/algo/dbgl_eulerspiral.h>
#include <vcl_iostream.h>
#include <vnl/vnl_math.h>
#include <vcl_fstream.h>


int main( int argc, char* argv[] )
{
  vcl_cout << "This program generates a table of number of iterations " <<
    "used for different computation" << vcl_endl;
  vcl_cout << "The result table has the structure" << vcl_endl;
  vcl_cout << "     start_angle_1       start_angle_1       num_iter_1 " << vcl_endl;
  vcl_cout << "     start_angle_2       start_angle_2       num_iter_2 " << vcl_endl;
  vcl_cout << "     start_angle_3       start_angle_3       num_iter_3 " << vcl_endl;
  vcl_cout << "         ...                 ...                 ...    " << vcl_endl;
  vcl_cout << "     start_angle_n       start_angle_n       num_iter_n " << vcl_endl;
  
  vgl_point_2d< double > start(0, 0);
  vgl_point_2d< double > end(1, 0);
  double start_angle = 0;
  double end_angle = 0;
  dbgl_eulerspiral es;
  int npts = 100;
  double dt = vnl_math::pi*2 / npts;

  bool success = false;
  vcl_ofstream ofs("dbgl_eulerspiral_num_iter_table_using_lookup_table_minimization.txt");
  int num_failed_cases = 0;
  for (int i = 0; i < npts; i++ ){
    
    start_angle = (i + 1.0/(i+2)) * dt;
    vcl_cout << "i = " << i << " , start_angle = " << start_angle << vcl_endl;
    for (int j = 0; j < npts; j++){
      end_angle = (j+ 1.0/(j + 2)) * dt;
      es.set_params(start, start_angle, end, end_angle);
      success = es.compute_es_params(); //(start, start_angle, end, end_angle );
      ofs << start_angle << "\t" << end_angle << "\t" << es.num_iterations() << "\t" ; 
      ofs << success << vcl_endl;
      if (! success) num_failed_cases++;
      
    }
  }
  vcl_cout << "Number of failed cases = " << num_failed_cases << vcl_endl;
  ofs.close();
  return 0;
}

