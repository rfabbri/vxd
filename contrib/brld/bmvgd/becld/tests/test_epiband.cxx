#include <testlib/testlib_test.h>
#include <vcl_limits.h>
#include <vcl_set.h>
#include <vcl_iterator.h>
#include <vnl/vnl_math.h>
#include <vgl/vgl_clip.h>
#include <vgl/vgl_distance.h>
#include <vgl/vgl_triangle_scan_iterator.h>
#include <vgl/io/vgl_io_polygon.h>
#include <dbecl/dbecl_epipole.h>
#include <dbecl/dbecl_epiband.h>
#include <dbecl/dbecl_epiband_iterator.h>
#include <vcl_iomanip.h>
#include <vcl_algorithm.h>

static const double tolerance=vcl_numeric_limits<double>::epsilon()*100;
static void print_integers(const vgl_polygon<double> &p, bool boundaryin=true);
static void test_get_bounds();
static void test_vgl_polygon_iterators();

#define remainder_div_2pi(x) ((x) - ((int)((x)/(2*vnl_math::pi)))*(x) )


struct ltpt {
  bool operator()(const vgl_point_2d<int> &s1, const vgl_point_2d<int> &s2) const
    {
       return vcl_pair<int,int> (s1.x(), s1.y()) < vcl_pair<int,int> (s2.x(),s2.y());
    }
};

typedef vcl_set<vgl_point_2d<int>,ltpt >  my_point_set;

static void 
print_integers_cover_iterator(
    const vgl_polygon<double> &p,
    vcl_set<vgl_point_2d<int>,ltpt > &s_fast, 
    vcl_set<vgl_point_2d<int>,ltpt > &s_slow);
void 
print_integers_cover_iterator(
    const vgl_polygon<double> &p,
    vcl_set<vgl_point_2d<int>,ltpt > &s_fast,
    dbecl_grid_cover_window &w
    ) ;

static void get_ground_truth(int xmin, int xmax, int ymin, int ymax, 
    my_point_set &s_truth, vgl_polygon<double> &poly, double dmax=1.5);

MAIN( test_epiband )
{
  START ("test epiband");

  test_vgl_polygon_iterators();
  test_get_bounds();

  SUMMARY();
}

void 
print_integers(const vgl_polygon<double> &p, bool boundary_in)
{
  // iterate
  vgl_polygon_scan_iterator<double> it(p,boundary_in); 

  vcl_cout << "Interior points:\n";
  for (it.reset(); it.next(); ) {
    int y = it.scany();
    for (int x = it.startx(); x <= it.endx(); ++x) {
      vcl_cout << "(" << x << "," << y << ") ";
    }
  }
  vcl_cout << vcl_endl;
}



//: prints and returns the set of points traversed by fast and slow iterators. The slow iterator is
// an implementaiton that does not require a bounding box. It is slower but uses less memory.
void 
print_integers_cover_iterator(
    const vgl_polygon<double> &p,
    vcl_set<vgl_point_2d<int>,ltpt > &s_fast, 
    vcl_set<vgl_point_2d<int>,ltpt > &s_slow)
{
  
  {
  dbecl_grid_cover_window w(p);
  // iterate
  dbecl_polygon_grid_cover_iterator it(p,w); 

  vcl_cout << "Interior points (FAST cover iterator):\n";
  for (it.reset(); it.nxt(); ) {
    vcl_cout << "(" << it.x() << "," << it.y() << ") ";
    s_fast.insert(vgl_point_2d<int>(it.x(),it.y()));
  }
  vcl_cout << vcl_endl;
  w.print();
  }
  

  {
  // iterate
  dbecl_slow_polygon_grid_cover_iterator it(p); 

  vcl_cout << "Interior points (SLOW cover iterator):\n";
  for (it.reset(); it.nxt(); ) {
    vcl_cout << "(" << it.x() << "," << it.y() << ") ";
    s_slow.insert(vgl_point_2d<int>(it.x(),it.y()));
  }
  vcl_cout << vcl_endl;
  }

  TEST("Fast and slow algorithms the same",s_fast==s_slow,true);
}

void 
print_integers_cover_iterator(
    const vgl_polygon<double> &p,
    vcl_set<vgl_point_2d<int>,ltpt > &s_fast,
    dbecl_grid_cover_window &w
    ) 
{
  // iterate
  dbecl_polygon_grid_cover_iterator it(p,w); 

  vcl_cout << "Interior points (FAST cover iterator):\n";
  for (it.reset(); it.nxt(); ) {
    vcl_cout << "(" << it.x() << "," << it.y() << ") ";
    s_fast.insert(vgl_point_2d<int>(it.x(),it.y()));
  }
  vcl_cout << vcl_endl;
  w.print();
}


void 
test_vgl_polygon_iterators()
{
  void get_double_squares_truth_1(my_point_set &s_truth);
  void get_double_triangles_truth_1(my_point_set &s_truth, vgl_polygon<double> &poly, double dmax=1.5);

  void print_difference_info(const my_point_set &set1, const my_point_set &s_truth, const vgl_polygon<double> &poly);
  // Following case fails with vgl_scan_iterator:
  // (400,178.034) (0,173.683) (0,177.67) (400,182.046)

  // These test cases are listed in my Oct 3rd 2006 notes
  { // Test on vgl_polygon: two disjoint squares
    vgl_polygon<double> p1(1);

    p1.push_back(1,0);
    p1.push_back(3,0);
    p1.push_back(3,2);
    p1.push_back(1,2);

    my_point_set s_truth;

    // set of integer points at distance <= 1.5 
    for (int ix=0; ix <=4; ++ix)
      for (int iy=-1; iy <=3; ++iy)
        s_truth.insert(vgl_point_2d<int>(ix,iy));

    TEST("square: containment",
    p1.contains(2,1)&& !p1.contains(4.5,1)&& !p1.contains(7,1) ,true);

    TEST("square: vertex containment",
    p1.contains(1,0)&& p1.contains(3,0)&& p1.contains(3,2) && p1.contains(1,2) ,true);

    TEST("square: edge containment",
    p1.contains(2,0)&& p1.contains(3,1)&& p1.contains(2,2) && p1.contains(1,1) ,true);

    print_integers(p1);
    {
    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
    print_integers_cover_iterator(p1,s_fast,s_slow);
    TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);
    }

    p1.new_sheet();
    p1.push_back(6,2);
    p1.push_back(8,2);
    p1.push_back(8,0);
    p1.push_back(6,0);

    TEST("Two sheet square: containment", 
        p1.contains(2,1) && !p1.contains(4.5,1) && p1.contains(7,1),true);

    // distance
//    double d = vgl_distance(p1, vgl_point_2d<double>(2,1));
    // TODO test on distance

    print_integers(p1);
    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
    print_integers_cover_iterator(p1,s_fast,s_slow);
  }
  
  { // Test on vgl_polygon: two squares, one of them being the hole

    vgl_polygon<double> p1(1);
    p1.push_back(0,0);
    p1.push_back(7,0);
    p1.push_back(7,4);
    p1.push_back(0,4);

    p1.new_sheet(); //: ccw or cw doesn't matter
    p1.push_back(5,1);
    p1.push_back(5,3); 
    p1.push_back(3,3);
    p1.push_back(3,1);

    // it also doesn't matter if hole is defined before the larger region

    TEST("Polygon with hole: containment", 
        p1.contains(1.5,2) && !p1.contains(4,2) && p1.contains(6,2),true);

//    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
//    print_integers_cover_iterator(p1,s_fast,s_slow);
  }

  { // Test on vgl_polygon: double-squares represented by two sheets
    vgl_polygon<double> p1(1);

    p1.push_back(2,3);
    p1.push_back(2,6);
    p1.push_back(5,6);
    p1.push_back(5,3);

    p1.new_sheet(); //: ccw or cw doesn't matter
    p1.push_back(10,0);
    p1.push_back(5,0); 
    p1.push_back(5,3);
    p1.push_back(10,3);

    TEST("Double-polygon with two sheets 1: containment", 
        p1.contains(6,2) && !p1.contains(6,4) && p1.contains(4,4),true);

    vgl_polygon<double> p2(1);

    p2.push_back(0,4);
    p2.push_back(0,7);
    p2.push_back(3,7);
    p2.push_back(3,4);

    p2.new_sheet(); //: ccw or cw doesn't matter
    p2.push_back(3,4);
    p2.push_back(3,2); 
    p2.push_back(6,2);
    p2.push_back(6,4);

    TEST("Double-polygon with two sheets 2: containment", 
        p2.contains(4,3) && !p2.contains(2,3) && p2.contains(2,5),true);
    
    // Q: will it report duplicate integer positions? (3,4)? 
    // A: NO!
    print_integers(p2);

    // Intersect

    vgl_polygon<double> p_int = vgl_clip(p1, p2, vgl_clip_type_intersect);
    p_int.print(vcl_cout);
    // Result is given as one sheet!
  }

  // Example double_squares;
  { // Test on vgl_polygon: double-squares represented by one sheet
    vgl_polygon<double> p1(1);

    p1.push_back(2,3);  // A
    p1.push_back(5,3);  // B
    p1.push_back(5,0);  // C
    p1.push_back(10,0); // D
    p1.push_back(10,3); // E
    p1.push_back(5,3);  // F
    p1.push_back(5,6);  // G
    p1.push_back(2,6);  // H

    my_point_set s_truth;

    get_double_squares_truth_1(s_truth);

    vcl_cout << "Here!\n";
    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
    print_integers_cover_iterator(p1,s_fast,s_slow);

    TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);



    print_integers(p1);
    TEST("Double-polygon with one sheet 1: containment", 
        p1.contains(6,2) && !p1.contains(6,4) && p1.contains(4,4),true);

    TEST("Double-polygon with one sheet 1: containment - special case after central vertex", 
        p1.contains(8,3),true);
    // Note that (8,3) does not appear in the list given by the vgl_scan_iterator.

    vgl_polygon<double> p2(1);

    p2.push_back(0,4); // A
    p2.push_back(0,7); // B
    p2.push_back(3,7); // C
    //:The following vertex is not needed but doesn't alter the results
//    p2.push_back(3,4); // D 
    p2.push_back(3,2); // E 
    p2.push_back(6,2); // F
    p2.push_back(6,4); // G
    //:The following vertex is not needed
//    p2.push_back(3,4); // H

    TEST("Double-polygon with one sheet 2: containment", 
        p2.contains(4,3) && !p2.contains(2,3) && p2.contains(2,5),true);
    
    // Q: will it report duplicate integer positions? (3,4)? 
    // A: NO!
    print_integers(p2);

    // Intersect

    vgl_polygon<double> p_int = vgl_clip(p1, p2, vgl_clip_type_intersect);
    p_int.print(vcl_cout);
    // Result is given as one sheet!
  }

  { // Test on vgl_polygon: double-triangle (like epipolar sector)
    vgl_polygon<double> p1(1);

    p1.push_back(10,4);  // A (not really needed)
    p1.push_back(3,2);   // B
    p1.push_back(2,6);   // C
    p1.push_back(18,2);  // D
    p1.push_back(17,6);  // E
// If you define it differently, the results are the same..
//    p1.push_back(10,4);  // A (not really needed)
//    p1.push_back(3,2);   // B
//    p1.push_back(2,6);   // C
//    p1.push_back(10,4);  // A (not really needed)
//    p1.push_back(17,6);  // E
//    p1.push_back(18,2);  // D
    my_point_set s_truth;

    get_double_triangles_truth_1(s_truth,p1);

    TEST("Double-triangle with one sheet 1: containment", 
        p1.contains(5,4) && !p1.contains(9,6) && p1.contains(17,3),true);

    vcl_cout << " =========== Double triangle with one sheet ============\n";
    print_integers(p1,true);
    print_integers(p1,false);
    // Notice how the vertex (10,4) is repeated in both the above lists.

    // Also, the second list of interior points provide vertex B and D, but not C and E.

    // same, with polygon_grid_cover_iterator
    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
    print_integers_cover_iterator(p1,s_fast,s_slow);
    TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);
    if (s_fast != s_truth) {
      print_difference_info(s_fast,s_truth,p1);
    }
  }
  { // Test on vgl_polygon: double-triangle (like epipolar sector)
    vgl_polygon<double> p1(1);

    // If you define it differently, the results are the same..
    p1.push_back(10,4);  // A (not really needed)
    p1.push_back(3,2);   // B
    p1.push_back(2,6);   // C
    p1.push_back(10,4);  // A (not really needed)
    p1.push_back(17,6);  // E
    p1.push_back(18,2);  // D
    my_point_set s_truth;

    get_double_triangles_truth_1(s_truth,p1);

    TEST("Double-triangle with one sheet 1: containment", 
        p1.contains(5,4) && !p1.contains(9,6) && p1.contains(17,3),true);

    vcl_cout << " =========== Double triangle with one sheet ============\n";
    print_integers(p1,true);
    print_integers(p1,false);
    // Notice how the vertex (10,4) is repeated in both the above lists.

    // Also, the second list of interior points provide vertex B and D, but not C and E.

    // same, with polygon_grid_cover_iterator
    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
    print_integers_cover_iterator(p1,s_fast,s_slow);
    TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);
    if (s_fast != s_truth) {
      print_difference_info(s_fast,s_truth,p1);
    }
  }

  { // Test on vgl_polygon: sliver (very thin polygon)
    vgl_polygon<double> p1(1);

    p1.push_back(0.5,0.5);  // A 
    p1.push_back(12,0.75);  // B
    p1.push_back(12,0.25);  // C

    my_point_set s_truth;
    get_ground_truth(-1,14,-1,2,s_truth,p1);

    TEST("Sliver : containment", 
        p1.contains(10,0.5) && !p1.contains(1,0.1) && p1.contains(1,0.5),true);

    vcl_cout << " =========== Sliver 1 ============\n";
    print_integers(p1,true);
    print_integers(p1,false);
    // Notice how the vertex (10,4) is repeated in both the above lists.

    // Also, the second list of interior points provide vertex B and D, but not C and E.

    // Let us try with a triangle iterator

    vgl_triangle_scan_iterator<double> it;

    vcl_cout << "Triangle iterator points:\n";
    for (it.reset(); it.next(); ) {
      int y = it.scany();
      for (int x = it.startx(); x <= it.endx(); ++x) {
        vcl_cout << "(" << x << "," << y << ") ";
      }
    }
    vcl_cout << vcl_endl;

    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
    print_integers_cover_iterator(p1,s_fast,s_slow);
    TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);
    if (s_fast != s_truth) {
      print_difference_info(s_fast,s_truth,p1);
    }
  }

  { // Test on vgl_polygon: rotated sliver (very thin polygon).
    vgl_polygon<double> p1(1);

    p1.push_back(0.5,0.5);  // A 
    p1.push_back(0.75, 12);  // B
    p1.push_back(0.25, 12);  // C

    my_point_set s_truth;
    get_ground_truth(-1,2,-1,14,s_truth,p1);

    TEST("Sliver : containment", 
        p1.contains(0.5,10) && !p1.contains(0.1, 1) && p1.contains(0.5, 1),true);

    vcl_cout << " =========== Sliver 1 rotated 90 degrees ============\n";
    print_integers(p1,true);
    print_integers(p1,false);
    // Notice how the vertex (10,4) is repeated in both the above lists.
    // Also, the second list of interior points provide vertex B and D, but not C and E.

    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
    print_integers_cover_iterator(p1,s_fast,s_slow);
    TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);
    if (s_fast != s_truth) {
      print_difference_info(s_fast,s_truth,p1);
    }
  }

  { // Test on vgl_polygon: sliver 2 (very thin polygon)
    vgl_polygon<double> p1(1);

    p1.push_back(21,1);  // A 
    p1.push_back(0,1);   // B
    p1.push_back(0,3);   // C

    my_point_set s_truth;
    get_ground_truth(-1,22,-1,4,s_truth,p1);

    vcl_cout << " =========== Sliver 2 ============\n";
    print_integers(p1,true);
    print_integers(p1,false);
    // Notice how the vertex (10,4) is repeated in both the above lists.

    // Also, the second list of interior points provide vertex B and D, but not C and E.

    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
    print_integers_cover_iterator(p1,s_fast,s_slow);
    TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);
    if (s_fast != s_truth) {
      print_difference_info(s_fast,s_truth,p1);
    }
  }

  { // area of an empty polygon 

    vgl_polygon<double> p1;

    double A = vgl_area<double>(p1);
    vcl_cout << "Area: " << A << vcl_endl;
    TEST("Empty polygon area:", A, 0);

    // distance to empty polygon 

    vgl_point_2d<double> pt(2,3);
    double d = vgl_distance(p1, pt);
    vcl_cout << "d:" << d << vcl_endl;


    my_point_set s_truth; // empty

    vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;
    print_integers_cover_iterator(p1,s_fast,s_slow);
    TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);
    if (s_fast != s_truth) {
      print_difference_info(s_fast,s_truth,p1);
    }
  }

    /*
  { // Test on vgl_polygon: degenerate case - coredump 10/15/07
    vgl_polygon<double> p1,p2;

    vsl_b_ifstream bfs_in("polygon_bug_test.bvl");
    assert(!(bfs_in) == false);
    vsl_b_read(bfs_in, p1);
    assert(!(bfs_in) == false);
    vsl_b_read(bfs_in, p2);
    assert(!(bfs_in) == false);
    bfs_in.close();

    vcl_cout << vcl_setprecision(20) << vcl_scientific;
    vcl_cout << "Poly A:\n" << p1<< vcl_endl;
    vcl_cout << "Poly B:\n" << p2<< vcl_endl;


    // Intersect

    int retval;
    vgl_polygon<double> p_int = vgl_clip(p1, p2, vgl_clip_type_intersect,&retval);
    if (retval == 0)
      vcl_cout << "Polygon clipper failed\n";
    TEST("Clipper still has that old bug",retval,1);
    p_int.print(vcl_cout);
  }
    */

  { // Test on vgl_polygon: degenerate case - coredump 10/15/07
    vgl_polygon<double> p1(1);




    p1.push_back(9.06500000000000000000e+02,2.45000000000000000000e+01 );
    p1.push_back(5.00000000000000000000e+03,-4.80010077624948280572e+02);
    p1.push_back(5.00000000000000000000e+03,-2.83924406595397795172e+03);
//    p1.push_back(9.06500000000000000000e+02,2.45000000000000000000e+01 );
//    p1.push_back(-5.00000000000000000000e+03,7.52456216805119879609e+02);
//    p1.push_back(-5.00000000000000000000e+03,4.15658851241166939872e+03);

    vgl_polygon<double> p2(1);

//    p2.push_back(3.54600000000000022737e+02,4.10600000000000022737e+02  );
//    p2.push_back(7.49559060392885585316e+04,1.21310523603145280504e+05  );
//    p2.push_back(-1.16051452365805569571e+05,8.18463253459640109213e+04 );
    p2.push_back(3.54600000000000022737e+02,4.10600000000000022737e+02  );
    p2.push_back(-7.42467060392885468900e+04,-1.20489323603145268862e+05);
    p2.push_back(1.16760652365805581212e+05,-8.10251253459639992798e+04 );

    // Intersect

    int retval;
    vgl_polygon<double> p_int = vgl_clip(p1, p2, vgl_clip_type_intersect,&retval);
    if (retval == 0)
      vcl_cout << "Polygon clipper failed\n";
    p_int.print(vcl_cout);
  }

    // ANOTHER TEST:
    //  - after each iteration, window's label buffer should be in the right state
    //  write  a function test_in_initial_state();
    
  dbecl_grid_cover_window w(vgl_box_2d<double>(0,6,0,4),0);
  {
    { // Test on vgl_polygon
      vgl_polygon<double> p1(1);

      p1.push_back(0,0);
      p1.push_back(0,3);
      p1.push_back(2,3);
      p1.push_back(2,0);

      my_point_set s_truth;


      vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;

      print_integers_cover_iterator(p1,s_fast,w);

      get_ground_truth(0, 6, 0, 4, s_truth, p1);

      TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);
      if (s_fast != s_truth) {
        print_difference_info(s_fast,s_truth,p1);
      }
    }

    { // Test on vgl_polygon
      vgl_polygon<double> p1(1);

      p1.push_back(4,1);
      p1.push_back(6,1);
      p1.push_back(6,4);
      p1.push_back(4,4);

      my_point_set s_truth;


      vcl_set<vgl_point_2d<int>,ltpt > s_fast,s_slow;

      TEST("Is window clean for re-iteration",w.is_label_buf_clean(),true);
      print_integers_cover_iterator(p1,s_fast,w);

      get_ground_truth(0, 6, 0, 4, s_truth, p1);

      TEST("Fast iterator conforms to ground truth",s_fast==s_truth,true);
      if (s_fast != s_truth) {
        print_difference_info(s_fast,s_truth,p1);
      }
    }
  } //:< tests with same window w 
}

void 
test_get_bounds()
{
  // Test cases in my notes of October 13

  { // Test A

    dbecl_epipole ep(vgl_point_2d<double>(0,0));
    vgl_polygon<double> poly(1);
    poly.push_back(1,0);
    poly.push_back(1,1);
    poly.push_back(0,1);
    
    double theta_plus;
    double theta_minus;
    double theta_mean;
    dbecl_epiband::get_bounds(poly, ep, theta_plus, theta_minus, theta_mean);

    TEST_NEAR("dbecl_epiband testcase A:",theta_minus,0,tolerance);
    TEST_NEAR("dbecl_epiband testcase A:",theta_plus,vnl_math::pi/2.0,tolerance);
    TEST_NEAR("dbecl_epiband testcase A:",theta_mean,vnl_math::pi/4.0,tolerance);
  }

  { // Test B
    dbecl_epipole ep(vgl_point_2d<double>(0,0));
    vgl_polygon<double> poly(1);
    poly.push_back(2.5,2.5);
    poly.push_back(0,3);
    poly.push_back(-0.5,0.5);
    poly.push_back(0,1);
    poly.push_back(1,1);
    poly.push_back(2,0);
    
    double theta_plus;
    double theta_minus;
    double theta_mean;
    dbecl_epiband::get_bounds(poly, ep, theta_plus, theta_minus, theta_mean);
    TEST_NEAR("dbecl_epiband testcase B:",theta_minus,0,tolerance);
    TEST_NEAR("dbecl_epiband testcase B:",theta_plus,3.0*vnl_math::pi/4.0,tolerance);
    TEST_NEAR("dbecl_epiband testcase B:",theta_mean,3.0*vnl_math::pi/8.0,tolerance);
  }

  { // Test C
    dbecl_epipole ep(vgl_point_2d<double>(0,0));
    vgl_polygon<double> poly(1);
    poly.push_back(0,2);  // A
    poly.push_back(-2,2); // B
    poly.push_back(-2,-2); // C
    
    double theta_plus;
    double theta_minus;
    double theta_mean;
    dbecl_epiband::get_bounds(poly, ep, theta_plus, theta_minus, theta_mean);


    vcl_cout << "theta_plus: " << (theta_plus/vnl_math::pi)*180.0 << " theta_minus: " << (theta_minus/vnl_math::pi)*180.0 << " theta_mean: " << (theta_mean/vnl_math::pi)*180.0 << vcl_endl;

    TEST("dbecl_epiband testcase C:",remainder_div_2pi(theta_minus - vnl_math::pi/2.0) < tolerance, true);
    TEST("dbecl_epiband testcase C:",remainder_div_2pi(theta_plus - 5.0*vnl_math::pi/4.0) < tolerance, true);
    TEST("dbecl_epiband testcase C:",remainder_div_2pi(theta_mean - (3.0*vnl_math::pi/8.0 + vnl_math::pi/2.0)) < tolerance, true);
  }

  { // Test C other direction
    dbecl_epipole ep(vgl_point_2d<double>(0,0));
    vgl_polygon<double> poly(1);
    poly.push_back(-2,-2); // C
    poly.push_back(-2,2); // B
    poly.push_back(0,2);  // A
    
    double theta_plus;
    double theta_minus;
    double theta_mean;
    dbecl_epiband::get_bounds(poly, ep, theta_plus, theta_minus, theta_mean);

    TEST("dbecl_epiband testcase C:",remainder_div_2pi(theta_minus - vnl_math::pi/2.0) < tolerance, true);
    TEST("dbecl_epiband testcase C:",remainder_div_2pi(theta_plus - 5.0*vnl_math::pi/4.0) < tolerance, true);
    TEST("dbecl_epiband testcase C:",remainder_div_2pi(theta_mean - (3.0*vnl_math::pi/8.0 + vnl_math::pi/2.0)) < tolerance, true);

  }

  { // Test D 
    dbecl_epipole ep(vgl_point_2d<double>(0,0));
    vgl_polygon<double> poly(1);
    poly.push_back(-1,1); 
    poly.push_back(-2,-2); 
    poly.push_back(1,-2); 
    
    double theta_plus;
    double theta_minus;
    double theta_mean;
    dbecl_epiband::get_bounds(poly, ep, theta_plus, theta_minus, theta_mean);

    vcl_cout << "theta_plus: " << (theta_plus/vnl_math::pi)*180.0 << " theta_minus: " << (theta_minus/vnl_math::pi)*180.0 << " theta_mean: " << (theta_mean/vnl_math::pi)*180.0 << vcl_endl;
  }

  { // Test E 
    dbecl_epipole ep(vgl_point_2d<double>(0,0));
    vgl_polygon<double> poly(1);
    poly.push_back(3,-3); 
    poly.push_back(4,4); 
    poly.push_back(-2,2); 
    poly.push_back(-2.5,-2.5); 
    poly.push_back(0,1); 
    poly.push_back(3,-3); 
    
    double theta_plus;
    double theta_minus;
    double theta_mean;
    dbecl_epiband::get_bounds(poly, ep, theta_plus, theta_minus, theta_mean);

    TEST("dbecl_epiband testcase E:",remainder_div_2pi(theta_minus - (-vnl_math::pi/4.0)) < tolerance, true);
    TEST("dbecl_epiband testcase E:",remainder_div_2pi(theta_plus - 5.0*vnl_math::pi/4.0) < tolerance, true);
    TEST("dbecl_epiband testcase E:",remainder_div_2pi(theta_mean - (vnl_math::pi/2.0)) < tolerance, true);

  }
  { // Test Whole plane degeneracy  - a polygon around the angle-discontinuity half-axis
    dbecl_epipole ep(vgl_point_2d<double>(3731.58,422.944));
    vgl_polygon<double> poly(1);
    poly.push_back(800.45,423.44); 
    poly.push_back(800.45,421.85); 
    poly.push_back(-1.0334,421.55); 
    poly.push_back(-1.0334,423.57); 
    
    double theta_plus;
    double theta_minus;
    double theta_mean;
    dbecl_epiband::get_bounds(poly, ep, theta_plus, theta_minus, theta_mean);

    vcl_cout << "\n\n=============================================\n";
    vcl_cout << "theta_minus : " <<  theta_minus
             << "theta_plus : " <<  theta_plus
             << "theta_med : " <<  theta_mean;
    vcl_cout << "\n=============================================\n";

    TEST("dbecl_epiband testcase degen:",theta_plus - theta_minus < vnl_math::pi/8, true);
  }

  { // Same as Test E  but with epipole not in origion

    double t[2] = {1000,-38900};
    dbecl_epipole ep(vgl_point_2d<double>(t[0],t[1]));
    vgl_polygon<double> poly(1);
    poly.push_back(3+t[0],t[1]-3); 
    poly.push_back(4+t[0],t[1]+4); 
    poly.push_back(-2+t[0],t[1]+2); 
    poly.push_back(-2.5+t[0],t[1]-2.5); 
    poly.push_back(0+t[0],t[1]+1); 
    poly.push_back(3+t[0],t[1]-3); 
    
    double theta_plus;
    double theta_minus;
    double theta_mean;
    dbecl_epiband::get_bounds(poly, ep, theta_plus, theta_minus, theta_mean);

    TEST("dbecl_epiband testcase E:",remainder_div_2pi(theta_minus - (-vnl_math::pi/4.0)) < tolerance, true);
    TEST("dbecl_epiband testcase E:",remainder_div_2pi(theta_plus - 5.0*vnl_math::pi/4.0) < tolerance, true);
    TEST("dbecl_epiband testcase E:",remainder_div_2pi(theta_mean - (vnl_math::pi/2.0)) < tolerance, true);
  }
}

void
get_double_squares_truth_1(my_point_set &s_truth)
{
  for (int y=-1; y <= 4; ++y)
    for (int x=4; x <=11; ++x)
      s_truth.insert(vgl_point_2d<int>(x,y));

  for (int y=2; y <= 7; ++y)
    for (int x=1; x <=6; ++x)
      s_truth.insert(vgl_point_2d<int>(x,y));
}

void
get_double_triangles_truth_1(my_point_set &s_truth, vgl_polygon<double> &poly, double dmax)
{
  get_ground_truth(1,19,1,7,s_truth,poly,dmax);
}

void
get_ground_truth(int xmin, int xmax, int ymin, int ymax, my_point_set &s_truth,
  vgl_polygon<double> &poly, double dmax) 
{
  for (int x=xmin; x <=xmax; ++x)
    for (int y=ymin; y <= ymax; ++y) {
      if (vgl_distance(poly,vgl_point_2d<double>(x,y)) < dmax)
        s_truth.insert(vgl_point_2d<int>(x,y));
    }
}

void
print_difference_info(const my_point_set &set1, const my_point_set &s_truth, const vgl_polygon<double> &poly)
{
    my_point_set C;

    vcl_set_symmetric_difference(set1.begin(), set1.end(), s_truth.begin(), s_truth.end(),
               vcl_inserter(C, C.begin()),
               ltpt());
    vcl_cout << "Set C (difference of set1 and s_truth): ";
//      vcl_copy(C.begin(), C.end(), vcl_ostream_iterator<const vgl_point_2d<int> >(vcl_cout, " "));
//      vcl_cout << vcl_endl;

    my_point_set::const_iterator it;
    for (it = C.begin(); it != C.end(); ++it) {
      vgl_point_2d<double> p_double(it->x(),it->y());
      double d = vgl_distance(poly,p_double);
      printf("(%d,%d) dist = %g\n",it->x(),it->y(),d);
    }
}
