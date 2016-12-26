#include <testlib/testlib_test.h>
#include "dbpro_sample_processes.h"
#include <vcl_iostream.h>
#include <dbpro/dbpro_process_factory.h>




MAIN( test_process_factory )
{
  START ("process_factory");


  //TEST("number of iterations", count, data.size());

  typedef dbpro_process_factory::Reg_Type Reg_Type;
  const Reg_Type& reg = dbpro_process_factory::registry();
  vcl_cout << "Registered processes" << vcl_endl;
  for (Reg_Type::const_iterator i = reg.begin(); i != reg.end(); ++i)
    vcl_cout << i->first << vcl_endl;


  SUMMARY();
}
