#include <testlib/testlib_test.h>
#include <dbpro/dbpro_process.h>
#include "dbpro_sample_processes.h"
#include <vcl_iostream.h>
#include <dbpro/dbpro_observer.h>


//: Print a message when notified
template <class T>
class dbpro_message : public dbpro_observer
{
  public:
    //: Transmit the data to the input of a process
    bool notify(const dbpro_storage_sptr& data, unsigned long timestamp)
    {
      if(data->info() == DBPRO_VALID)
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

  dbpro_process_sptr iq = new dbpro_input_queue<int>(data);

  dbpro_output_queue<double>* oq1_ptr = new dbpro_output_queue<double>;
  dbpro_output_queue<float>* oq2_ptr = new dbpro_output_queue<float>;
  dbpro_process_sptr oq1(oq1_ptr);
  dbpro_process_sptr oq2(oq2_ptr);

  dbpro_process_sptr conv = new dbpro_static_cast<int,double>;
  dbpro_process_sptr root = new dbpro_sqrt;
  dbpro_process_sptr sum = new dbpro_sum<double>;

  dbpro_process_sptr conv2 = new dbpro_static_cast<double,float>;

  oq1->connect_input(0,sum,0);
  root->connect_input(0,conv,0);
  sum->connect_input(0,root,0);
  sum->connect_input(1,conv,0);
  conv->connect_input(0,iq,0);

  conv2->connect_input(0,root,0);
  oq2->connect_input(0,conv2,0);

  conv->add_output_observer(0,new dbpro_message<double>);

  unsigned int count = 0;
  for(; count<100 && oq1->run(count+1) == DBPRO_VALID
                  && oq2->run(count+1) == DBPRO_VALID; ++count);

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
