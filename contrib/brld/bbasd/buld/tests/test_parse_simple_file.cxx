// \file
// \author Ricardo Fabbri
//
#include <testlib/testlib_test.h>
#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <dbul/dbul_parse_simple_file.h>

void test_parse_number_lists()
{

  {
  vcl_ofstream ofp("number_lists_tmp.txt", vcl_ios::out);

  if (!ofp) {
    vcl_cerr << "Error in test, unable to open file" << vcl_endl;
    return;
  }

  ofp << "# Example of a file\n"
    << "0 2 1 3 4 5\n"
    << "0 2 1 3 5\n";
  }

  vcl_vector<vcl_vector<int> > nl;
  bool retval = dbul_parse_number_lists("number_lists_tmp.txt", nl);

  TEST("retval", retval, true);
  TEST("num lines", nl.size(), 2u);

  int num_lines_gt[2] = { 6, 5 };

  for (unsigned i=0; i < nl.size(); ++i) {
    vcl_cout << "Line [" << i << "]:\n" << nl[i].size() << vcl_endl;
    TEST("num element in line", nl[i].size(), num_lines_gt[i]);
    for (unsigned k=0; k < nl[i].size(); ++k) {
      vcl_cout << nl[i][k] << ":";
    }
    vcl_cout << vcl_endl;
  }
}

MAIN( test_parse_simple_file )
{
   START ("Parse Simple File");

   test_parse_number_lists();
   
   SUMMARY();
}
