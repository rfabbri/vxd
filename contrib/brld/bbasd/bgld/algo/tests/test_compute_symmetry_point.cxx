// This is basic/dbgl/algo/tests/test_compute_symmetry_point.cxx

//:
// \file


#include <testlib/testlib_test.h>

#include <dbgl/algo/dbgl_compute_symmetry_point.h>
#include <vnl/vnl_math.h>


//------------------------------------------------------------------------------
//: test functions of dbgl_circ_arc class
void test_compute_symmetry_point_on_line()
{
  // Case 1: degenerate - two tangents are parallel
  vgl_point_2d<double > left_bnd_pt(5, 6);
  vgl_vector_2d<double > left_bnd_tangent(-1, 0);
  vgl_point_2d<double > right_line_start(1, 0);
  vgl_point_2d<double > right_line_end(11, 0);

  // the symmetry point on line is (5, 0) and has s = 4;

  vcl_vector<double > s_along_line;

  dbgl_compute_symmetry_point_on_line(right_line_start, right_line_end,
                                      left_bnd_pt, left_bnd_tangent,
                                      s_along_line);

  TEST("Symmetry point on a line - parallel case - test #point", s_along_line.size(), 1);

  if (s_along_line.size() == 1)
  {
    TEST_NEAR("Symmetry point on a line -parallel case - test position of point", s_along_line[0], 4, 1e-12);
  }

  // Case 2: typical case
  left_bnd_pt.set( 5 - 6 * vcl_cos(vnl_math::pi/6), 9);
  double angle = -2*vnl_math::pi /3;
  left_bnd_tangent.set(vcl_cos(angle), vcl_sin(angle));

  s_along_line.clear();
  dbgl_compute_symmetry_point_on_line(right_line_start, right_line_end,
                                      left_bnd_pt, left_bnd_tangent,
                                      s_along_line);
  TEST("Symmetry point on a line - typical case - test #point", s_along_line.size(), 1);

  if (s_along_line.size() == 1)
  {
    TEST_NEAR("Symmetry point on a line - typical case - test position of point", s_along_line[0], 4, 1e-12);
  }
}





//------------------------------------------------------------------------------
//: test functions of dbgl_circ_arc class
void test_compute_symmetry_point_on_circ_arc()
{
  // Case 1: degenerate - two tangents are parallel
  vgl_point_2d<double > left_bnd_pt(5, 6);
  vgl_vector_2d<double > left_bnd_tangent(-1, 0);


  // arc: radius = 8
  // angle = 2 x 30o
  double radius = 8;
  vgl_point_2d<double > pt1(5-radius * vcl_cos(vnl_math::pi/6) , -4);
  vgl_point_2d<double > pt2(5+ radius * vcl_cos(vnl_math::pi/6) , -4);
  double k = -1.0 / radius;
  dbgl_circ_arc right_bnd_arc(pt1, pt2, k);

  // the symmetry point on line is (5, 0) and has s = 1/2 length of arc;
  vcl_vector<double > s_along_line;
  dbgl_compute_symmetry_point_on_circ_arc(right_bnd_arc,
                                      left_bnd_pt, left_bnd_tangent,
                                      s_along_line);

  TEST("Symmetry point on a circular arc -parallel case- test #point", s_along_line.size(), 1);

  if (s_along_line.size() == 1)
  {
    TEST_NEAR("Symmetry point on a circular arc -parallel case- test position of point", 
      s_along_line[0], right_bnd_arc.length()/2, 1e-12);
  }

  // Case 2: typical case
  left_bnd_pt.set( 5 - 6 * vcl_cos(vnl_math::pi/6), 9);
  double angle = -2*vnl_math::pi /3;
  left_bnd_tangent.set(vcl_cos(angle), vcl_sin(angle));

  s_along_line.clear();
  dbgl_compute_symmetry_point_on_circ_arc(right_bnd_arc,
                                      left_bnd_pt, left_bnd_tangent,
                                      s_along_line);
  TEST("Symmetry point on a circular arc - mid point - test #point", s_along_line.size(), 1);

  if (s_along_line.size() == 1)
  {
    TEST_NEAR("Symmetry point on a circular arc - mid point - test position of point", 
      s_along_line[0], right_bnd_arc.length()/2, 1e-12);
  }

  // Case 3: Move the start and end point of the arc
  double len = right_bnd_arc.length();
  vgl_point_2d<double > p1 = right_bnd_arc.point_at_length(-len/4);
  vgl_point_2d<double > p2 = right_bnd_arc.point_at_length(3*len/4);
  dbgl_circ_arc shifted_arc(p1, p2, right_bnd_arc.k());

  s_along_line.clear();
  dbgl_compute_symmetry_point_on_circ_arc(shifted_arc,
                                      left_bnd_pt, left_bnd_tangent,
                                      s_along_line);
  TEST("Symmetry point on a circular arc - typical case - test #point", s_along_line.size(), 1);

  if (s_along_line.size() == 1)
  {
    TEST_NEAR("Symmetry point on a circular arc - typical case - test position of point", 
      s_along_line[0], 3*len/4, 1e-12);
  }

}




//: Test functions of biarc class
MAIN( test_compute_symmetry_point )
{
  START ("Test compute symmetry point");
  test_compute_symmetry_point_on_line();
  test_compute_symmetry_point_on_circ_arc();
  SUMMARY();
}

