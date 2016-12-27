#include <testlib/testlib_test.h>
#include "bprod_sample_processes.h"
#include <vcl_iostream.h>
#include <bprod/bprod_process_factory.h>




MAIN( test_process_factory )
{
  START ("process_factory");


  //TEST("number of iterations", count, data.size());

  typedef bprod_process_factory::Reg_Type Reg_Type;
  const Reg_Type& reg = bprod_process_factory::registry();
  vcl_cout << "Registered processes" << vcl_endl;
  for (Reg_Type::const_iterator i = reg.begin(); i != reg.end(); ++i)
    vcl_cout << i->first << vcl_endl;


  SUMMARY();
}
