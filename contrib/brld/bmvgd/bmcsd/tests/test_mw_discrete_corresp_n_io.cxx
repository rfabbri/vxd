#include <testlib/testlib_test.h>
#include <mw/mw_discrete_corresp_3.h>
#include <mw/mw_discrete_corresp_n.h>
#include <mw/algo/mw_discrete_corresp_algo.h>
#include <vcl_limits.h>
#include <vcl_set.h>
#include <vnl/vnl_math.h>

#include <vcl_iterator.h>
#include <vcl_algorithm.h>

static const double tolerance=vcl_numeric_limits<double>::epsilon()*100;

static void test_discrete_corresp_n();



//: Currently tests both IO and other functions of mw_discrete_corresp_3
MAIN( test_discrete_corresp_n_io )
{
  START ("mw_discrete_corresp_n io");

  test_discrete_corresp_n();

  SUMMARY();
}

void
test_discrete_corresp_n()
{
  vcl_vector<unsigned> npts;
  npts.push_back(7);
  npts.push_back(8);
  npts.push_back(8);
  mw_discrete_corresp_n gt(npts);
  
  mw_ntuplet tup(3);

  tup[0] = 1; tup[1] = 2; tup[2] = 3; gt.l_.put(tup,mw_match_attribute());
  tup[0] = 2; tup[1] = 2; tup[2] = 2; gt.l_.put(tup,mw_match_attribute());
  tup[0] = 2; tup[1] = 3; tup[2] = 3; gt.l_.put(tup,mw_match_attribute());

  vcl_cout << gt << vcl_endl;

//  double inf = vcl_numeric_limits<double>::infinity();
  mw_discrete_corresp_n cp(npts);
  tup[0] = 1; tup[1] = 2; tup[2]= 3; cp.l_.put(tup,mw_match_attribute(false,0));
  tup[0] = 2; tup[1] = 2; tup[2]= 2; cp.l_.put(tup,mw_match_attribute(false,23.2));

  // Equality operator

  TEST("Equality operator", cp == gt, false);
  TEST("Equality operator", cp == cp, true);


  //----- BINARY I/O ----------------------------------------------------------

  vcl_cout << "Initial data:" << vcl_endl;
  vcl_cout << cp;

  // ------ Writing -------
  vsl_b_ofstream bfs_out("mw_discrete_corresp_n.tmp");
  TEST("Created mw_discrete_corresp_n.tmp for writing", (!bfs_out), false);
  vsl_b_write(bfs_out, cp);
  bfs_out.close();

  // ------ Reading -------

  mw_discrete_corresp_n cp_in;

  vsl_b_ifstream bfs_in("mw_discrete_corresp_n.tmp");
  TEST("Opened mw_discrete_corresp_n.tmp for reading", (!bfs_in), false);
  vsl_b_read(bfs_in, cp_in);
  TEST("Finished reading file successfully", (!bfs_in), false);
  bfs_in.close();

  TEST("cp_out == cp_in", cp, cp_in);

  vcl_cout << "Recovered data:" << vcl_endl;
  vcl_cout << cp_in;
}



#if 0
void
test_discrete_corresp_3()
{
  vcl_cout << "\nTesting statistics ----------------------------------------\n";
  mw_discrete_corresp_3 gt(7,8,8);

  gt.l_.put(1,2,3,mw_match_attribute());
  gt.l_.put(2,2,2,mw_match_attribute());
  gt.l_.put(2,3,3,mw_match_attribute());
  gt.l_.put(5,3,7,mw_match_attribute());
  gt.l_.put(4,4,4,mw_match_attribute());
  gt.l_.put(3,4,4,mw_match_attribute());


  double inf = vcl_numeric_limits<double>::infinity();
  mw_discrete_corresp_3 cp(7,8,8);
  cp.l_.put(1,2,3,mw_match_attribute(false,0));
  cp.l_.put(2,2,2,mw_match_attribute(false,23.2));
  cp.l_.put(4,5,6,mw_match_attribute(false,4.45));
  cp.l_.put(4,4,4,mw_match_attribute(false,29.8));
  cp.l_.put(4,7,1,mw_match_attribute(false,10.33));

  //: Infinite cost doesn't count.
  cp.l_.put(5,3,7,mw_match_attribute(false,inf));

  //: empty g-t doesn't count.
  cp.l_.put(6,7,4,mw_match_attribute(false,10.33));

  // compare
  cp.compare_and_print(&gt);

  /* 
     # Points in image 1 having correct candidates ranked among top 50% matches:
        71.4286% (5 out of 7).
     # Points in image 1 having correct candidates in top 5 matches:
        85.7143% (6 out of 7).
  */


  //----------------------------------------------
  // Test for new function that gives % correspondences having cost above right
  // ones


  unsigned n_correct, n_valid;
  cp.number_of_correct_triplets(n_correct, n_valid, &gt);

  vcl_cout << "n_correct: " << n_correct << " n_valid: " << n_valid << vcl_endl;
  TEST("number_of_correct_triplets:  n_correct",n_correct,3);
  TEST("number_of_correct_triplets:  n_valid",n_valid,5);

  vcl_vector<bool> p0s, p1s, p2s;
  gt.participating_points(p0s,p1s,p2s);

  vcl_cout << "[ ";
  for (unsigned i=0; i <p0s.size(); ++i)
    vcl_cout << p0s[i] << " "; 
  vcl_cout << "]\n";



  //: All triplets are correct if we compare ground-truth against itself
  gt.number_of_correct_triplets(n_correct, n_valid, &gt);
  TEST("number_of_correct_triplets ground-truth must be 100%:",n_correct,n_valid);

  // Equality operator

  TEST("Equality operator", cp == gt, false);
  TEST("Equality operator", cp == cp, true);

  //----- Hashed fast access ---------------------------------------------------


  TEST("Isnt Hashed",cp.is_hashed(),false);
  cp.hash();
  TEST("Is Hashed",cp.is_hashed(),true);

  const vcl_set<triplet_uuu> &s4xx = cp.triplets(0,4);

  vcl_cout << "All triplets in cp whose first index is 4: ";
  vcl_copy(s4xx.begin(), s4xx.end(), vcl_ostream_iterator<trip_uuu>(vcl_cout, " "));
  vcl_cout << vcl_endl;


  gt.hash();
  vcl_set<triplet_uuu> sx44;

  gt.triplets(1,4,2,4,sx44);

  vcl_cout << "All triplets in gt whose second and third indices are both 4: ";
  vcl_copy(sx44.begin(), sx44.end(), vcl_ostream_iterator<trip_uuu>(vcl_cout, " "));
  vcl_cout << vcl_endl;


  //----- BINARY I/O ----------------------------------------------------------

  vcl_cout << "Initial data:" << vcl_endl;
  vcl_cout << cp;

  // ------ Writing -------
  vsl_b_ofstream bfs_out("mw_discrete_corresp_3.tmp");
  TEST("Created mw_discrete_corresp_3.tmp for writing", (!bfs_out), false);
  vsl_b_write(bfs_out, cp);
  bfs_out.close();

  // ------ Reading -------

  mw_discrete_corresp_3 cp_in;

  vsl_b_ifstream bfs_in("mw_discrete_corresp_3.tmp");
  TEST("Opened mw_discrete_corresp_3.tmp for reading", (!bfs_in), false);
  vsl_b_read(bfs_in, cp_in);
  TEST("Finished reading file successfully", (!bfs_in), false);
  bfs_in.close();

  TEST("cp_out == cp_in", cp, cp_in);

  vcl_cout << "Recovered data:" << vcl_endl;
  vcl_cout << cp_in;

}

void 
test_set_triplet()
{ // A brief sets refresher:
  typedef vbl_triple<unsigned,unsigned,unsigned> trip_uuu;
  vcl_set<trip_uuu > s1;

  s1.insert(trip_uuu(1,2,3));
  s1.insert(trip_uuu(3,2,1));
  s1.insert(trip_uuu(1,2,4));

  vcl_cout << "Set 1: ";
  vcl_copy(s1.begin(), s1.end(), vcl_ostream_iterator<trip_uuu>(vcl_cout, " "));
  vcl_cout << vcl_endl;

  vcl_set<trip_uuu > s2;
  s2.insert(trip_uuu(4,1,1));
  s2.insert(trip_uuu(3,2,1));
  s2.insert(trip_uuu(69,1,1));
  s2.insert(trip_uuu(1,2,3));

  vcl_cout << "Set 2: ";
  vcl_copy(s2.begin(), s2.end(), vcl_ostream_iterator<trip_uuu>(vcl_cout, " "));
  vcl_cout << vcl_endl;

  vcl_set<trip_uuu> s3;
  vcl_set_intersection(s1.begin(),s1.end(), s2.begin(), s2.end(), inserter(s3, s3.begin()));

  vcl_cout << "Set 3 (Set 1 intersection with Set 2): ";
  vcl_copy(s3.begin(), s3.end(), vcl_ostream_iterator<trip_uuu>(vcl_cout, " "));
  vcl_cout << vcl_endl;
}
#endif
