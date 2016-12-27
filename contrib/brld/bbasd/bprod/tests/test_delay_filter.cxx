#include <testlib/testlib_test.h>
#include <bprod/bprod_process.h>
#include <bprod/bprod_delay.h>
#include "bprod_sample_processes.h"
#include <vcl_iostream.h>



MAIN( test_delay_filter )
{
  START ("delay_filter");


  bprod_process_sptr delay = new bprod_delay(2,1.0);

  bprod_output_queue<double>* oq_ptr = new bprod_output_queue<double>;
  bprod_process_sptr oq(oq_ptr);

  bprod_process_sptr sum = new bprod_sum<double>;


  oq->connect_input(0,delay,1);
  sum->connect_input(0,delay,0);
  sum->connect_input(1,delay,1);
  delay->connect_input(0,sum,0);


  for(unsigned int count = 0; count<10; ++count){
    bprod_signal s1 = oq->run(count);
    bprod_signal s2 = delay->run(count);
    if(s1 == BPROD_INVALID || s2 == BPROD_INVALID){
      vcl_cerr << "Error stream failure" << vcl_endl;
      break;
    }
    if(s1 == BPROD_EOS || s2 == BPROD_EOS){
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
