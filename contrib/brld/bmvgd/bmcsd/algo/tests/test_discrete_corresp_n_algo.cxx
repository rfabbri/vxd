#include <testlib/testlib_test.h>
#include <bmcsd/bmcsd_discrete_corresp_3.h>
#include <bmcsd/bmcsd_discrete_corresp_n.h>
#include <bmcsd/algo/bmcsd_discrete_corresp_algo.h>
#include <vcl_limits.h>
#include <vcl_set.h>
#include <vnl/vnl_math.h>

#include <vcl_iterator.h>
#include <vcl_algorithm.h>

static const double tolerance=vcl_numeric_limits<double>::epsilon()*100;

static void test_discrete_corresp_n();
static void test_discrete_corresp();



//: Currently tests both IO and other functions of bmcsd_discrete_corresp_3
MAIN( test_discrete_corresp_n_algo )
{
  START ("bmcsd_discrete_corresp_n algo");

//  test_discrete_corresp_n();
  test_discrete_corresp();
  test_discrete_corresp_stats();

  SUMMARY();
}

void
test_discrete_corresp_n()
{
  vcl_vector<unsigned> npts;
  npts.push_back(7);
  npts.push_back(8);
  npts.push_back(8);
  bmcsd_discrete_corresp_n gt(npts);
  
  bmcsd_ntuplet tup(3);

  tup[0] = 1; tup[1] = 2; tup[2] = 3; gt.l_.put(tup,bmcsd_match_attribute());
  tup[0] = 2; tup[1] = 2; tup[2] = 2; gt.l_.put(tup,bmcsd_match_attribute());
  tup[0] = 2; tup[1] = 3; tup[2] = 3; gt.l_.put(tup,bmcsd_match_attribute());

  vcl_cout << gt << vcl_endl;

//  double inf = vcl_numeric_limits<double>::infinity();
  bmcsd_discrete_corresp_n cp(npts);
  tup[0] = 1; tup[1] = 2; tup[2]= 3; cp.l_.put(tup,bmcsd_match_attribute(false,0));
  tup[0] = 2; tup[1] = 2; tup[2]= 2; cp.l_.put(tup,bmcsd_match_attribute(false,23.2));

  // Equality operator

  TEST("Equality operator", cp == gt, false);
  TEST("Equality operator", cp == cp, true);


  //----- BINARY I/O ----------------------------------------------------------

  vcl_cout << "Initial data:" << vcl_endl;
  vcl_cout << cp;

  // ------ Writing -------
  vsl_b_ofstream bfs_out("bmcsd_discrete_corresp_n.tmp");
  TEST("Created bmcsd_discrete_corresp_n.tmp for writing", (!bfs_out), false);
  vsl_b_write(bfs_out, cp);
  bfs_out.close();

  // ------ Reading -------

  bmcsd_discrete_corresp_n cp_in;

  vsl_b_ifstream bfs_in("bmcsd_discrete_corresp_n.tmp");
  TEST("Opened bmcsd_discrete_corresp_n.tmp for reading", (!bfs_in), false);
  vsl_b_read(bfs_in, cp_in);
  TEST("Finished reading file successfully", (!bfs_in), false);
  bfs_in.close();

  TEST("cp_out == cp_in", cp, cp_in);

  vcl_cout << "Recovered data:" << vcl_endl;
  vcl_cout << cp_in;

  //----- ALGO ----------------------------------------------------------

  bmcsd_discrete_corresp_algo::print_difference(cp,gt);
}

void
test_discrete_corresp()
{
  vgl_point_2d<double> a(1,23);
  vgl_point_2d<double> b(1.123,203);
  vgl_point_2d<double> c(888,0);
  vgl_point_2d<double> d(1,1);
  vgl_point_2d<double> e(121,1);
  vgl_point_2d<double> f(831,-98341);

  // ------
  vcl_vector<vsol_polyline_2d_sptr> a0;
  // ------
  {  // 0
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(d));
  a0.push_back(new vsol_polyline_2d(pts));
  }

  {  // 1
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(e));
  pts.push_back(new vsol_point_2d(a));
  a0.push_back(new vsol_polyline_2d(pts));
  }

  {  // 2
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(f));
  a0.push_back(new vsol_polyline_2d(pts));
  }

  {  // 3
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(c));
  pts.push_back(new vsol_point_2d(d));
  a0.push_back(new vsol_polyline_2d(pts));
  }

  // ------
  vcl_vector<vsol_polyline_2d_sptr> a1;
  // ------
  {  // 0
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(a));
  pts.push_back(new vsol_point_2d(b));
  a1.push_back(new vsol_polyline_2d(pts));
  }

  {  // 1
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(c));
  a1.push_back(new vsol_polyline_2d(pts));
  }

  {  // 2
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(c));
  pts.push_back(new vsol_point_2d(c));
  pts.push_back(new vsol_point_2d(d));
  pts.push_back(new vsol_point_2d(b));
  a1.push_back(new vsol_polyline_2d(pts));
  }


  // Correspond a0 and a1

  bmcsd_discrete_corresp acorr(a0.size(), a1.size());
  acorr[0].push_back(bmcsd_attributed_object(2));
  acorr[1].push_back(bmcsd_attributed_object(0));
  acorr[1].push_back(bmcsd_attributed_object(1));
  // acorr[2] empty
  acorr[3].push_back(bmcsd_attributed_object(0));
  acorr[3].push_back(bmcsd_attributed_object(2));

  // Define b0 subset of a0, b1 subset of a1

  // ------
  vcl_vector<vsol_polyline_2d_sptr> b0;
  // ------
  {  // 0 == a0[1]
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(e));
  pts.push_back(new vsol_point_2d(a));
  b0.push_back(new vsol_polyline_2d(pts));
  }
  {  // 1 == a0[3]
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(c));
  pts.push_back(new vsol_point_2d(d));
  b0.push_back(new vsol_polyline_2d(pts));
  }

  // ------
  vcl_vector<vsol_polyline_2d_sptr> b1;
  // ------
  {  // 0 == a1[1]
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(c));
  b1.push_back(new vsol_polyline_2d(pts));
  }

  {  // 1 == a1[2]
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(c));
  pts.push_back(new vsol_point_2d(c));
  pts.push_back(new vsol_point_2d(d));
  pts.push_back(new vsol_point_2d(b));
  b1.push_back(new vsol_polyline_2d(pts));
  }

  {  // 3 == not in a1
  vcl_vector<vsol_point_2d_sptr> pts;
  pts.push_back(new vsol_point_2d(f));
  b1.push_back(new vsol_polyline_2d(pts));
  }

  // Test subset corresps
  {
  bmcsd_discrete_corresp eq;
  bmcsd_discrete_corresp_algo::match_equal(b0, a0, &eq);
  bmcsd_discrete_corresp eq_gt(b0.size(), a0.size());
  eq_gt[0].push_back(bmcsd_attributed_object(1));
  eq_gt[1].push_back(bmcsd_attributed_object(3));
  TEST("Computed subset corresp equals truth", eq, eq_gt);
  }

  // Make sure induced corresp is as expected.

  bmcsd_discrete_corresp bcorr_gt(b0.size(), b1.size());
  bcorr_gt[0].push_back(bmcsd_attributed_object(0));
  bcorr_gt[1].push_back(bmcsd_attributed_object(1));
  vcl_cout << "Bcorr_gt:\n" << bcorr_gt;

  bmcsd_discrete_corresp bcorr;
  bmcsd_discrete_corresp_algo::extend(a0, a1, b0, b1, acorr, &bcorr);
  vcl_cout << "Bcorr:\n" << bcorr;

  TEST("Computed induced corresp equals truth", bcorr, bcorr_gt);
}

void
test_discrete_corresp_stats()
{
  //---- STATISTICS ----------------------------------------------------------

  {
  vcl_cout << "\nTesting statistics ----------------------------------------\n";
  bmcsd_discrete_corresp gt(10,10);

  gt.corresp_[0].push_back(bmcsd_attributed_object(7,0,0));
  gt.corresp_[1].push_back(bmcsd_attributed_object(2));
  gt.corresp_[1].push_back(bmcsd_attributed_object(3));
  gt.corresp_[2].push_back(bmcsd_attributed_object(3));
  gt.corresp_[3].push_back(bmcsd_attributed_object(6));
  gt.corresp_[4].push_back(bmcsd_attributed_object(4));
  gt.corresp_[4].push_back(bmcsd_attributed_object(5));
  // 5 : empty
  gt.corresp_[6].push_back(bmcsd_attributed_object(3));
  // 7 : empty
  gt.corresp_[8].push_back(bmcsd_attributed_object(8));
  gt.corresp_[8].push_back(bmcsd_attributed_object(7));
  gt.corresp_[8].push_back(bmcsd_attributed_object(6));

  gt.corresp_[9].push_back(bmcsd_attributed_object(6));

  TEST("#empty (including dummy)", gt.count_empty(), 3);


  double inf = vcl_numeric_limits<double>::infinity();
  bmcsd_discrete_corresp cp(10,10);
  cp.corresp_[0].push_back(bmcsd_attributed_object(4,0,inf));
  cp.corresp_[0].push_back(bmcsd_attributed_object(5,0,inf));
  cp.corresp_[0].push_back(bmcsd_attributed_object(7,0,1e-2));  // T

  //: Infinite cost doesn't count.
  cp.corresp_[1].push_back(bmcsd_attributed_object(5,0,0));  // F
  cp.corresp_[1].push_back(bmcsd_attributed_object(7,0,inf));
  cp.corresp_[1].push_back(bmcsd_attributed_object(4,0,inf));
  cp.corresp_[1].push_back(bmcsd_attributed_object(6,0,inf));
  cp.corresp_[1].push_back(bmcsd_attributed_object(1,0,inf));
  cp.corresp_[1].push_back(bmcsd_attributed_object(2,0,inf));

  cp.corresp_[2].push_back(bmcsd_attributed_object(3));  // T
  cp.corresp_[2].push_back(bmcsd_attributed_object(4));  // F
  cp.corresp_[2].push_back(bmcsd_attributed_object(5));  // F
  cp.corresp_[2].push_back(bmcsd_attributed_object(6));  // F


  cp.corresp_[3].push_back(bmcsd_attributed_object(1,0,0.1));  // F
  cp.corresp_[3].push_back(bmcsd_attributed_object(2,0,0.2));  // F
  cp.corresp_[3].push_back(bmcsd_attributed_object(3,0,0.3));  // F
  cp.corresp_[3].push_back(bmcsd_attributed_object(4,0,0.4));  // F
  cp.corresp_[3].push_back(bmcsd_attributed_object(6,0,0.6));  // T

  cp.corresp_[4].push_back(bmcsd_attributed_object(5,0,0));  // T

  cp.corresp_[6].push_back(bmcsd_attributed_object(4,0,inf));
  cp.corresp_[6].push_back(bmcsd_attributed_object(2,0,inf));
  cp.corresp_[6].push_back(bmcsd_attributed_object(7,0,inf));
  cp.corresp_[6].push_back(bmcsd_attributed_object(3,0,0));  // T
  cp.corresp_[6].push_back(bmcsd_attributed_object(0,0,inf));
  cp.corresp_[6].push_back(bmcsd_attributed_object(1,0,inf));

  //: empty g-t doesn't count.
  cp.corresp_[7].push_back(bmcsd_attributed_object(1,0,inf));
  cp.corresp_[7].push_back(bmcsd_attributed_object(2,0,inf));
  cp.corresp_[7].push_back(bmcsd_attributed_object(3,0,inf));

  cp.corresp_[8].push_back(bmcsd_attributed_object(8,0,inf));
  cp.corresp_[8].push_back(bmcsd_attributed_object(7,0,2.3));  // T
  cp.corresp_[8].push_back(bmcsd_attributed_object(6,0,0.1));  // T
  cp.corresp_[8].push_back(bmcsd_attributed_object(5,0,0));  // F


  cp.corresp_[9].push_back(bmcsd_attributed_object(1,0,0.1));  // F
  cp.corresp_[9].push_back(bmcsd_attributed_object(2,0,0.2));  // F
  cp.corresp_[9].push_back(bmcsd_attributed_object(3,0,0.3));  // F
  cp.corresp_[9].push_back(bmcsd_attributed_object(4,0,0.4));  // F
  cp.corresp_[9].push_back(bmcsd_attributed_object(5,0,0.5));  // F
  cp.corresp_[9].push_back(bmcsd_attributed_object(6,0,0.6));  // T
  

  // sort & print

  cp.sort();

  // compare
  cp.compare_and_print(&gt);

  /* 
     # objects in image 1 having correct candidates ranked among top 50% matches:
        71.4286% (5 out of 7).
     # objects in image 1 having correct candidates in top 5 matches:
        85.7143% (6 out of 7).
  */

  unsigned p1_idx = 0;
  vcl_list<bmcsd_attributed_object>::const_iterator  itr;
  itr = cp.find_right_corresp_mincost(p1_idx,&gt);

  TEST("find_right_corresp_mincost idx 0",itr->obj_,7);

  p1_idx = 4;
  itr = cp.find_right_corresp_mincost(p1_idx,&gt);

  TEST("find_right_corresp_mincost idx 4",itr->obj_,5);

  //: empty
  p1_idx = 5; 
  itr = cp.find_right_corresp_mincost(p1_idx,&gt);

  TEST("find_right_corresp_mincost idx 5",itr,cp.corresp_[p1_idx].end());

  //: if many ground-truth possibilities, then have to get minimum cost:
  p1_idx = 8; 
  itr = cp.find_right_corresp_mincost(p1_idx,&gt);

  TEST("find_right_corresp_mincost idx 8",itr->obj_,6);

  //----------------------------------------------
  // Test for new function that gives % correspondences having cost above right
  // ones


//  unsigned n, n_valid;
//  cp.percentage_of_matches_above_truth(n,n_valid,&gt);

//  vcl_cout << "n: " << n << " n_valid: " << n_valid << vcl_endl;
//  TEST("percentage_of_matches_above_truth:  nmatches",n,1);
//  TEST("percentage_of_matches_above_truth:  n_valid",n_valid,15);

  unsigned n_w_gt;
  cp.number_of_pts1_with_gt_among_any_candidates(n_w_gt,&gt);
  vcl_cout << "# of objects in image 1 having ground-truth among any candidates:\n\t" 
    << 100.0*(float)n_w_gt/(float)(cp.size()) << "% ("  << n_w_gt << " out of " << cp.size()<< ")." << vcl_endl;
  TEST("# of objects in image 1 having ground-truth among any candidates: n_w_gt", n_w_gt,9);
  // remember the policy that if both gt and candidates are empty, then it counts.

  vcl_vector<bool> expected_result;
  expected_result.resize(cp.size());
  expected_result[0] = true;
  expected_result[1] = false;
  expected_result[2] = true;
  expected_result[3] = true;
  expected_result[4] = true;
  expected_result[5] = true;
  expected_result[6] = true;
  expected_result[7] = true;
  expected_result[8] = true;
  for (unsigned i=0; i < cp.size(); ++i) {
    bool result = cp.is_gt_among_top5(i, &gt);
    TEST("is_gt_among_top5",result,expected_result[i]);
  }

  {
  dborl_exp_stat s;
  bmcsd_discrete_corresp_algo::exp_stats(&gt, s, &gt);
  s.print_stats();
  TEST("consistent FP/FN/TP/TN", s.all_done(), true);
  TEST_NEAR("self-test", s.precision(), 1.0, 1e-9);
  TEST_NEAR("self-test", s.recall(), 1.0, 1e-9);
  TEST_EQUAL("n0", gt.n0(), 11);
  TEST_EQUAL("n1", gt.n1(), 11);
  TEST_EQUAL("#positives == num corresps", s.positive_cnt_, 12);
  TEST_EQUAL("#negatives == possible corresps - num corresps", s.negative_cnt_, 11*11 - 12);
  }

  {
  dborl_exp_stat s;
  bmcsd_discrete_corresp_algo::exp_stats(&cp, s, &gt);
  s.print_stats();
  TEST("consistent FP/FN/TP/TN", s.all_done(), true);
  TEST_EQUAL("TP", s.TP_, 8);
  TEST_EQUAL("FP", s.FP_, 14);
  TEST_EQUAL("TN", s.TN_, s.negative_cnt_ - s.FP_);
  TEST_EQUAL("FN", s.FN_, s.positive_cnt_ - s.TP_);
  }

  {
  dborl_exp_stat s;
  bmcsd_discrete_corresp_algo::exp_stats_hitmiss(&gt, s, &gt);
  s.print_stats();
  TEST("consistent FP/FN/TP/TN", s.all_done(), true);
  TEST_NEAR("self-test", s.precision(), 1.0, 1e-9);
  TEST_NEAR("self-test", s.recall(), 1.0, 1e-9);
  TEST_EQUAL("#positives", s.positive_cnt_, 12);
  TEST_EQUAL("#negatives", s.negative_cnt_, 11*11-s.positive_cnt_);
  }

  {
  dborl_exp_stat s;
  bmcsd_discrete_corresp_algo::exp_stats_hitmiss(&cp, s, &gt);
  vcl_cout << vcl_endl;
  s.print_stats();
  TEST("consistent FP/FN/TP/TN", s.all_done(), true);
  // cp[0]: TP
  // cp[1]: FP
  // cp[2]: FP FP FP FP
  // cp[3]: FP FP FP FP FP
  // cp[4]: TP TP
  // cp[5]:
  // cp[6]: TP
  // cp[7]: 
  // cp[8]: FP FP FP
  // cp[9]: FP FP FP FP FP FP
  // cp[10]:
  TEST_EQUAL("TP", s.TP_, 4);
  TEST_EQUAL("FP", s.FP_, 19);
  TEST_EQUAL("TN", s.TN_, s.negative_cnt_ - s.FP_);
  TEST_EQUAL("FN", s.FN_, s.positive_cnt_ - s.TP_);
  }

  { 
  bmcsd_discrete_corresp cp_copy = cp;
  cp_copy.threshold_by_cost_lteq(0);
  cp_copy.threshold_by_cost(1e6);
  bmcsd_discrete_corresp cp_copy2 = cp_copy;

  cp_copy.keep_only_extreme_cost(true);
  cp_copy2.keep_only_unambiguous_max(0);
  TEST("keep_only_unambiguous_max degenerates to max", cp_copy == cp_copy2, true);
  }

  }

}

#if 0
void
test_discrete_corresp_3()
{
  vcl_cout << "\nTesting statistics ----------------------------------------\n";
  bmcsd_discrete_corresp_3 gt(7,8,8);

  gt.l_.put(1,2,3,bmcsd_match_attribute());
  gt.l_.put(2,2,2,bmcsd_match_attribute());
  gt.l_.put(2,3,3,bmcsd_match_attribute());
  gt.l_.put(5,3,7,bmcsd_match_attribute());
  gt.l_.put(4,4,4,bmcsd_match_attribute());
  gt.l_.put(3,4,4,bmcsd_match_attribute());


  double inf = vcl_numeric_limits<double>::infinity();
  bmcsd_discrete_corresp_3 cp(7,8,8);
  cp.l_.put(1,2,3,bmcsd_match_attribute(false,0));
  cp.l_.put(2,2,2,bmcsd_match_attribute(false,23.2));
  cp.l_.put(4,5,6,bmcsd_match_attribute(false,4.45));
  cp.l_.put(4,4,4,bmcsd_match_attribute(false,29.8));
  cp.l_.put(4,7,1,bmcsd_match_attribute(false,10.33));

  //: Infinite cost doesn't count.
  cp.l_.put(5,3,7,bmcsd_match_attribute(false,inf));

  //: empty g-t doesn't count.
  cp.l_.put(6,7,4,bmcsd_match_attribute(false,10.33));

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
  vsl_b_ofstream bfs_out("bmcsd_discrete_corresp_3.tmp");
  TEST("Created bmcsd_discrete_corresp_3.tmp for writing", (!bfs_out), false);
  vsl_b_write(bfs_out, cp);
  bfs_out.close();

  // ------ Reading -------

  bmcsd_discrete_corresp_3 cp_in;

  vsl_b_ifstream bfs_in("bmcsd_discrete_corresp_3.tmp");
  TEST("Opened bmcsd_discrete_corresp_3.tmp for reading", (!bfs_in), false);
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
