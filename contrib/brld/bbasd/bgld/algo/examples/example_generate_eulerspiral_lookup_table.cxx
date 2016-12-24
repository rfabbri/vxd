//---------------------------------------------------------------------
// This is basic/dbgl/algo/examples/example_generate_eulerspiral_lookup_table.cxx
//:
// \file
// \brief
//
// \author
//  Nhon Trinh
// \date 2/15/2005
// \verbatim
//  Modifications
//   <none>
// \endverbatim
//
//-------------------------------------------------------------------------

#include <dbgl/algo/dbgl_eulerspiral.h>
#include <vcl_iostream.h>
#include <vsl/vsl_binary_io.h>
#include <vsl/vsl_vector_io.h>
#include <vnl/vnl_math.h>


int main( int , char*[] )
{
  vcl_cout << "EULER SPIRAL LOOK-UP TABLE GENERATOR" << vcl_endl << vcl_endl;
  vcl_cout << "This program creates a look-up table for speeding Euler spiral computation" << vcl_endl;
  vcl_cout << "It creates three tables npts x npts of k0, gamma, and length " <<
    "of a normalized Euler spiral, i.e. start = (0, 0) and end = (1, 0)" << vcl_endl;
  vcl_cout << "The two variables of the tables are start_angle and end_angles ranging in [0, 2pi)" << vcl_endl;
  
  int npts = 100;
  vcl_string es_file("dbgl_eulerspiral_lookup_table.bvl");

  // create the table.
  vcl_vector< vcl_vector< double > > k0_table;
  vcl_vector< vcl_vector< double > > gamma_table;
  vcl_vector< vcl_vector< double > > len_table;

  double step = vnl_math::pi*2.0 / npts;
  double start_angle = 0;
  double end_angle = 0;
  
  vgl_point_2d< double > start(0, 0);
  vgl_point_2d< double > end(1, 0);
  double k0;
  double gamma;
  double len;
  vcl_cout << vcl_endl << " ----- Generate tables for k0, gamma, and length ------ " 
    << vcl_endl << vcl_endl;
  dbgl_eulerspiral es;
  vcl_cout << "(i, Start angle) = " << vcl_endl;
  int num_failed_cases = 0;
  for (int i = 0; i < npts; i ++){
    
    start_angle = i * step; // + step * 0.5;
    vcl_cout << "\t( " << i << " , " << start_angle << " )";
    vcl_vector< double > k0_row;
    vcl_vector< double > gamma_row;
    vcl_vector< double > len_row;
    for (int j = 0; j< npts; j++){
      end_angle = j * step; // + step * 0.5;
      es.set_params(start, start_angle, end, end_angle);
      if (! es.compute_es_params(false, false )){
        vcl_cout << "j = " << j << " - Euler spiral computation failed" << vcl_endl;
        num_failed_cases ++;
        
      }
      k0 = es.k0();
      gamma = es.gamma();
      len = es.length();
      
      // add to the rows
      k0_row.push_back(k0);
      gamma_row.push_back(gamma);
      len_row.push_back(len);
    }
    k0_table.push_back(k0_row);
    gamma_table.push_back(gamma_row);
    len_table.push_back(len_row);
  }

  vcl_cout << vcl_endl << "Completed generating tables" << vcl_endl;
  vcl_cout << "Number of failed cases = " << num_failed_cases << vcl_endl;
  vcl_cout << "Saving all three tables to file " << es_file << " ... ";
  vsl_b_ofstream out_stream = vsl_b_ofstream(es_file);
  // write to file

  vsl_b_write(out_stream, npts);
  // k0
  for (int i = 0; i < npts; i++ ){
    vsl_b_write(out_stream, k0_table.at(i));
  }

  // gamma
  for (int i = 0; i < npts; i++ ){
    vsl_b_write(out_stream, gamma_table.at(i));
  }

  // len
  for (int i = 0; i < npts; i++ ){
    vsl_b_write(out_stream, len_table.at(i));
  }

  out_stream.close();
  vcl_cout << "completed." << vcl_endl;
  

  // verify data
  vcl_cout << "----------- Verify output file ---------" << vcl_endl << vcl_endl ;

  vcl_vector < vcl_vector< double > > k0_table_new;
  vcl_vector < vcl_vector< double > > gamma_table_new;
  vcl_vector < vcl_vector< double > > len_table_new;

  vcl_cout << "Reading file " << es_file << vcl_endl ;
  vsl_b_ifstream in_stream(es_file);
  if (!in_stream){
    vcl_cerr<<"Failed to open " << es_file << " for input." << vcl_endl;
  }
  vcl_cout << "Opened file successfully " << vcl_endl;
  
  // k0
  vcl_cout << "Reading k0 table ... ";
  int npts_new;
  vsl_b_read(in_stream, npts_new);
  for (int i = 0; i < npts_new; i++ ){
    vcl_vector< double > k0_row_new;
    vsl_b_read(in_stream, k0_row_new);
    k0_table_new.push_back(k0_row_new);
  }
  vcl_cout << "done. " << vcl_endl;

  // gamma
  vcl_cout << "Reading gamma table ... ";
  for (int i = 0; i < npts_new; i++ ){
    vcl_vector< double > gamma_row_new;
    vsl_b_read(in_stream, gamma_row_new);
    gamma_table_new.push_back(gamma_row_new);
  }
  vcl_cout << "done. " << vcl_endl;

  // len
  vcl_cout << "Reading len table ... ";
  for (int i = 0; i < npts_new; i++ ){
    vcl_vector< double > len_row_new;
    vsl_b_read(in_stream, len_row_new);
    len_table_new.push_back(len_row_new);
  }
  vcl_cout << " done." << vcl_endl;
  in_stream.close();

  vcl_cout << "Comparing results from tables against computed results " << vcl_endl;
  double tolerance = 1e-3;
  bool verified = true;
  if (npts_new == npts){
    for (int i =0; i < npts_new; i ++){
      for (int j = 0; j < npts_new; j ++){
        double error = vcl_fabs(k0_table.at(i).at(j) - k0_table.at(i).at(j)) + 
          vcl_fabs(gamma_table.at(i).at(j) - gamma_table.at(i).at(j)) + 
          vcl_fabs(len_table.at(i).at(j) - len_table.at(i).at(j));
        if (error > 3* tolerance){
          verified = false;
          vcl_cout << "Error at (i, j) = " << i << " ,  " << j << " )" << vcl_endl;
        } 
      }
    }
  }
  else{
    vcl_cerr << "npts_new != npts" << vcl_endl;
    verified = false;
  }

  if (verified)
    vcl_cout << "Verification completed successfully - no error " << vcl_endl;
  else
    vcl_cout << "Verification failed. " << vcl_endl;
  
  // Examples
  vcl_cout << vcl_endl <<  " ----- Example -----------" << vcl_endl;
  start_angle = 1.5;
  end_angle = 1.1;
  es.compute_es_params(start, start_angle, end, end_angle);
  es.print(vcl_cout);
  int start_index = (int) (start_angle/ step);
  int end_index = (int) (end_angle/ step);
  vcl_cout << "Table result " << vcl_endl;
  vcl_cout << " k0 = " << k0_table_new.at(start_index).at(end_index) << vcl_endl;
  vcl_cout << " gamma = " << gamma_table_new.at(start_index).at(end_index) << vcl_endl;
  vcl_cout << " len = " << len_table_new.at(start_index).at(end_index) << vcl_endl;
  
  return 0;
}

