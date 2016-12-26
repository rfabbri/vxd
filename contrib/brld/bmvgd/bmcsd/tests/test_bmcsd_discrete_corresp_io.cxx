#include <iostream>
#include <testlib/testlib_test.h>
#include <bmcsd/bmcsd_discrete_corresp.h>
#include <vcl_limits.h>
#include <vnl/vnl_math.h>


static const double tolerance=vcl_numeric_limits<double>::epsilon()*100;

//: Currently tests both IO and other functions of bmcsd_discrete_corresp
MAIN( test_discrete_corresp_io )
{
  START ("bmcsd_discrete_corresp io");

  {

  //----- BINARY I/O ----------------------------------------------------------
  bmcsd_discrete_corresp cp_out(4,7);

  cp_out.corresp_[0].push_back(bmcsd_attributed_object(13,0,1));

  cp_out.corresp_[1].push_back(bmcsd_attributed_object(0,0,1));
  cp_out.corresp_[1].push_back(bmcsd_attributed_object(69,0,33.1425));
  cp_out.corresp_[1].push_back(bmcsd_attributed_object(69,0,2.24789));

  // null match
  cp_out.corresp_[2].push_back(bmcsd_attributed_object(0,1,1));

  // empty list 
  // cp_out.corresp_[3]

  vcl_cout << "Initial data:" << vcl_endl;
  vcl_cout << cp_out;

  // ------ Writing -------
  vsl_b_ofstream bfs_out("bmcsd_discrete_corresp.tmp");
  TEST("Created bmcsd_discrete_corresp.tmp for writing", (!bfs_out), false);
  vsl_b_write(bfs_out, cp_out);
  bfs_out.close();

  // ------ Reading -------

  bmcsd_discrete_corresp cp_in;

  vsl_b_ifstream bfs_in("bmcsd_discrete_corresp.tmp");
  TEST("Opened bmcsd_discrete_corresp.tmp for reading", (!bfs_in), false);
  vsl_b_read(bfs_in, cp_in);
  TEST("Finished reading file successfully", (!bfs_in), false);
  bfs_in.close();
  
  TEST("cp_out == cp_in", cp_out, cp_in);

  vcl_cout << "Recovered data:" << vcl_endl;
  vcl_cout << cp_in;


  //----- OTHER TESTS ----------------------------------------------------------
  
  cp_out.sort();
  vcl_cout << vcl_endl;
  vcl_cout << "Sorted:\n " << cp_out << vcl_endl;

  cp_out.threshold_by_cost(5);
  vcl_cout << vcl_endl;
  vcl_cout << "Remove all costs > 5:\n " << cp_out << vcl_endl;

  }

  SUMMARY();
}
