#include <testlib/testlib_test.h>
#include <dbpro/dbpro_storage.h>
#include <vcl_iostream.h>


MAIN( test_storage )
{
  START ("storage");

  dbpro_storage_sptr ds_int = new dbpro_storage_type<int>(10);
  int test = ds_int->data<int>();


  TEST("get data (int)", test, 10);

  SUMMARY();
}
