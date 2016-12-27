#include <testlib/testlib_test.h>
#include <bprod/bprod_process.h>
#include "bprod_sample_processes.h"
#include <vcl_iostream.h>
#include <bprod/bprod_observer.h>


//: Print a message when notified
template <class T>
class bprod_message : public bprod_observer
{
  public:
    //: Transmit the data to the input of a process
    bool notify(const bprod_storage_sptr& data, unsigned long timestamp)
    {
      if(data->info() == BPROD_VALID)
        vcl_cout << "Received: "<< data->template data<T>() <<" at time "<<timestamp<< vcl_endl;
      return true;
    }
};


MAIN( test_process )
{
  START ("process");

  vcl_vector<int> data;
  data.push_back(1);
  data.push_back(2);
  data.push_back(4);
  data.push_back(6);
  data.push_back(10);

  bprod_process_sptr iq = new bprod_input_queue<int>(data);

  bprod_output_queue<double>* oq1_ptr = new bprod_output_queue<double>;
  bprod_output_queue<float>* oq2_ptr = new bprod_output_queue<float>;
  bprod_process_sptr oq1(oq1_ptr);
  bprod_process_sptr oq2(oq2_ptr);

  bprod_process_sptr conv = new bprod_static_cast<int,double>;
  bprod_process_sptr root = new bprod_sqrt;
  bprod_process_sptr sum = new bprod_sum<double>;

  bprod_process_sptr conv2 = new bprod_static_cast<double,float>;

  oq1->connect_input(0,sum,0);
  root->connect_input(0,conv,0);
  sum->connect_input(0,root,0);
  sum->connect_input(1,conv,0);
  conv->connect_input(0,iq,0);

  conv2->connect_input(0,root,0);
  oq2->connect_input(0,conv2,0);

  conv->add_output_observer(0,new bprod_message<double>);

  unsigned int count = 0;
  for(; count<100 && oq1->run(count+1) == BPROD_VALID
                  && oq2->run(count+1) == BPROD_VALID; ++count);

  TEST("number of iterations", count, data.size());

  vcl_cout << "Resulting data output 1\n";
  for(unsigned int i=0; i<oq1_ptr->data.size(); ++i)
    vcl_cout << oq1_ptr->data[i] << " ";
  vcl_cout << vcl_endl;

  vcl_cout << "Resulting data output 2\n";
  for(unsigned int i=0; i<oq2_ptr->data.size(); ++i)
    vcl_cout << oq2_ptr->data[i] << " ";
  vcl_cout << vcl_endl;

  SUMMARY();
}
