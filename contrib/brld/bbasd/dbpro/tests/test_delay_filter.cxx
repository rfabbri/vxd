#include <testlib/testlib_test.h>
#include <dbpro/dbpro_process.h>
#include <dbpro/dbpro_delay.h>
#include "dbpro_sample_processes.h"
#include <vcl_iostream.h>



MAIN( test_delay_filter )
{
  START ("delay_filter");


  dbpro_process_sptr delay = new dbpro_delay(2,1.0);

  dbpro_output_queue<double>* oq_ptr = new dbpro_output_queue<double>;
  dbpro_process_sptr oq(oq_ptr);

  dbpro_process_sptr sum = new dbpro_sum<double>;


  oq->connect_input(0,delay,1);
  sum->connect_input(0,delay,0);
  sum->connect_input(1,delay,1);
  delay->connect_input(0,sum,0);


  for(unsigned int count = 0; count<10; ++count){
    dbpro_signal s1 = oq->run(count);
    dbpro_signal s2 = delay->run(count);
    if(s1 == DBPRO_INVALID || s2 == DBPRO_INVALID){
      vcl_cerr << "Error stream failure" << vcl_endl;
      break;
    }
    if(s1 == DBPRO_EOS || s2 == DBPRO_EOS){
      break;
    }
  }

  //TEST("number of iterations", count, data.size());

  vcl_cout << "Resulting data output\n";
  for(unsigned int i=0; i<oq_ptr->data.size(); ++i)
    vcl_cout << oq_ptr->data[i] << " ";
  vcl_cout << vcl_endl;


  SUMMARY();
}
