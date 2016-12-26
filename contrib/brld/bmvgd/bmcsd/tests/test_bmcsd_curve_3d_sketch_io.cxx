#include <testlib/testlib_test.h>
#include <vcl_algorithm.h>
#include <mw/mw_util.h>
#include <mw/dbmcs_view_set.h>
#include <mw/dbmcs_curve_3d_attributes.h>
#include <mw/dbmcs_curve_3d_sketch.h>

static void test_dbmcs_curve_3d_sketch_io();
static void test_stereo_views_io();
static void test_attributes();

//: tests multiview projection + reconstruction of differential geometry
MAIN( test_dbmcs_curve_3d_sketch_io )
{
  START ("3D curve sketch");

  test_stereo_views_io();
  test_attributes();
  test_dbmcs_curve_3d_sketch_io();

  SUMMARY();
}

void 
test_dbmcs_curve_3d_sketch_io()
{
  vcl_vector< dbdif_1st_order_curve_3d > curves(2);

  dbdif_1st_order_point_3d p;
  p.Gama[0] = 123.999;
  p.Gama[1] = 83.12;
  p.Gama[2] = 83.21;
  p.T[0] = 0;
  p.T[1] = 0;
  p.T[2] = 1;
  p.valid = true;
  curves[0].push_back(p);

  p.Gama[0] = 13.999;
  p.Gama[1] = 8.12;
  p.Gama[2] = 8.21;
  p.T[0] = 1;
  p.T[1] = 0;
  p.T[2] = 0;
  curves[0].push_back(p);

  p.Gama[0] = 23.998;
  p.Gama[1] = 0;
  p.Gama[2] = 3.21;
  p.T[0] = 0;
  p.T[1] = 1;
  p.T[2] = 0;
  curves[1].push_back(p);

  // Fill up the data with 2 curves and 2 attribs

  vcl_vector< dbmcs_curve_3d_attributes > attr(2);
  {
  dbmcs_stereo_views_sptr vp = new dbmcs_stereo_views();
  dbmcs_stereo_views &v = *vp;
  v.set_stereo0(4);
  v.set_stereo1(9123);
  v.add_confirmation_view(5);
  v.add_confirmation_view(10982);
  v.add_confirmation_view(88);

  dbmcs_curve_3d_attributes &a = attr[0];

  a.set_views(vp);
  a.inlier_views_.push_back(1);
  a.inlier_views_.push_back(12);
  a.inlier_views_.push_back(3000);
  a.inlier_views_.push_back(4);
  a.set_i0_i1(122, 43);
  a.total_support_ = 79;
  }
  {
  dbmcs_stereo_views_sptr vp = new dbmcs_stereo_views();
  dbmcs_stereo_views &v = *vp;
  v.set_stereo1(932);
  v.add_confirmation_view(25);
  v.add_confirmation_view(8);

  dbmcs_curve_3d_attributes &a = attr[1];

  a.set_views(vp);
  a.inlier_views_.push_back(91);
  a.set_i0_i1(22, 8);
  a.total_support_ = 0;
  }

  dbmcs_curve_3d_sketch cs(curves, attr);

  bool ret = cs.write_dir_format("test-curve-sketch-tmp/");
  TEST("Write dir", ret, true);
  dbmcs_curve_3d_sketch cs_in;
  ret = cs_in.read_dir_format("test-curve-sketch-tmp/");
  TEST("Read dir", ret, true);
  TEST("cs_in == cs_out", cs_in, cs);
}

void
test_stereo_views_io()
{
  dbmcs_stereo_views v;
  v.set_stereo0(4);
  v.set_stereo1(9123);
  v.add_confirmation_view(5);
  v.add_confirmation_view(10982);
  v.add_confirmation_view(88);

  {
  vsl_b_ofstream bfs_out("dbmcs_stereo_views_io.tmp");
  vsl_b_write(bfs_out, v);
  }

  dbmcs_stereo_views v_in;
  {
  vsl_b_ifstream bfs_in("dbmcs_stereo_views_io.tmp");
  vsl_b_read(bfs_in, v_in);
  }

  TEST("v_out == v_in", v, v_in);
}

void
test_attributes()
{
  dbmcs_stereo_views_sptr vp = new dbmcs_stereo_views();
  dbmcs_stereo_views &v = *vp;
  v.set_stereo0(4);
  v.set_stereo1(9123);
  v.add_confirmation_view(5);
  v.add_confirmation_view(10982);
  v.add_confirmation_view(88);

  dbmcs_curve_3d_attributes a;

  a.set_views(vp);
  a.inlier_views_.push_back(1);
  a.inlier_views_.push_back(12);
  a.inlier_views_.push_back(3000);
  a.inlier_views_.push_back(4);
  a.set_i0_i1(123,747);
  a.total_support_ = 99;

  {
  vsl_b_ofstream bfs_out("dbmcs_attribs.tmp");
  vsl_b_write(bfs_out, a);
  }

  dbmcs_curve_3d_attributes a_in;
  {
  vsl_b_ifstream bfs_in("dbmcs_attribs.tmp");
  vsl_b_read(bfs_in, a_in);
  }

  TEST("a_out == a_in", a, a_in);
}
