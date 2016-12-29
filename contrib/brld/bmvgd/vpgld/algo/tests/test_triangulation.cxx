#include <testlib/testlib_test.h>
#include <vgl/vgl_distance.h>
#include <dbdif/algo/dbdif_data.h>
#include <brct/brct_algos.h>
#include <dvpgl/algo/dvpgl_triangulation.h>
#include <vcl_algorithm.h>

static void test_two_view_triangulation();

// Test multiple triangulation algorithms using synthetic data
static void 
test_triangulation()
{
  test_two_view_triangulation();
}

//
// \param[in] formal_test: true if should use TEST_MACRO or just output the error.
//
static double
test_reconstruction(
    const vcl_vector<vgl_point_3d<double> > rec_world_pts,
    const vcl_vector<vgl_point_3d<double> > gt_world_pts,
    bool formal_test
    )
{
  assert (rec_world_pts.size() == gt_world_pts.size());
  double reproj_error=0; // avereage reprojection error
  for (unsigned i=0; i < gt_world_pts.size(); ++i) {
    reproj_error += vgl_distance(rec_world_pts[i], gt_world_pts[i]);
     // TEST_NEAR("Reproj. error", vgl_distance(rec_world_pts[i], gt_world_pts[i]), 0, 0.0001);
  }
  reproj_error /= gt_world_pts.size();
  if (formal_test) {
    TEST_NEAR("Average reprojection error", reproj_error, 0, 0.0001);
  } else
    vcl_cout << "Average reprojection error: " << reproj_error << vcl_endl;

  return reproj_error;
}

void 
test_two_view_triangulation_on_gt_data(
  const vcl_vector<vpgl_perspective_camera<double> > &gt_cams,
  const vcl_vector<vcl_vector<vgl_point_2d<double> > > &gt_image_pts,
  const vcl_vector<vgl_point_3d<double> > &gt_world_pts,
  bool formal_test)
{

  // Triangulate using linear least squares.
  double linear_reproj_error = -1;
  {
  vcl_cout << "Linear least squares:\n";
  vcl_vector<vgl_point_3d<double> > linear_world_pts(gt_world_pts.size());

  {
  vcl_vector<vnl_double_3x4> projs;
  projs.push_back(gt_cams[0].get_matrix());
  projs.push_back(gt_cams[1].get_matrix());

  for (unsigned ip=0; ip < gt_world_pts.size(); ++ip) {
    vcl_vector<vnl_double_2> corresp_pts;

    corresp_pts.push_back(vnl_double_2(gt_image_pts[0][ip].x(), gt_image_pts[0][ip].y()));
    corresp_pts.push_back(vnl_double_2(gt_image_pts[1][ip].x(), gt_image_pts[1][ip].y()));

    linear_world_pts[ip] = brct_algos::bundle_reconstruct_3d_point(corresp_pts, projs);
  }
  }

  linear_reproj_error = test_reconstruction(linear_world_pts, gt_world_pts, formal_test);
  }
  

  double kanatani_fast_reproj_error = -1;
  // Triangulate using Kanatani et. al's BMVC08 optimal triangulation fast version.
  {
  vcl_cout << "Kanatani et. al.'s method:\n";
  vcl_vector<vgl_point_3d<double> > kanatani_world_pts(gt_world_pts.size());

  {
  for (unsigned ip=0; ip < gt_world_pts.size(); ++ip) {
    kanatani_world_pts[ip] = triangulate_3d_point_optimal_kanatani_fast(
        gt_image_pts[0][ip], gt_image_pts[1][ip],
        gt_cams[0], gt_cams[1]);
  }
  }

  kanatani_fast_reproj_error = test_reconstruction(kanatani_world_pts, gt_world_pts, formal_test);
  }

  TEST("Kanatani is really optimal", 
      kanatani_fast_reproj_error < linear_reproj_error || 
      vcl_fabs(linear_reproj_error - kanatani_fast_reproj_error) < 0.0000001, true);

  // Triangulate using Kanatani et. al's BMVC08 optimal triangulation slow version.
//  {
//  vcl_cout << "Kanatani et. al.'s method:\n";
//  vcl_vector<vgl_point_3d<double> > kanatani_world_pts(gt_world_pts.size());

//  {
//  for (unsigned ip=0; ip < gt_world_pts.size(); ++ip) {
//     optimal_2view_triangulation_kanatani_fast(
//        gt_image_pts[0][ip], gt_image_pts[1][ip],
//        gt_cams[0], gt_cams[1],
//        &(kanatani_world_pts[ip]));
//  }
//  }

//  test_reconstruction(kanatani_world_pts, gt_world_pts, formal_test);
//  }
}

//: Test some optimal algorithms against linear triangulation.
// \todo eliminate dependence on dbdif_algo
void 
test_two_view_triangulation()
{
  // --- Perfect data ---
  vcl_vector<vpgl_perspective_camera<double> > gt_cams;
  vcl_vector<vcl_vector<vgl_point_2d<double> > > gt_image_pts;
  vcl_vector<vgl_point_3d<double> > gt_world_pts;

  {
  vcl_vector<double> angles;
  angles.push_back(30);
  angles.push_back(60);

  dbdif_data::get_digital_camera_point_dataset(&gt_cams, &gt_image_pts, &gt_world_pts, angles);
  gt_image_pts.resize(vcl_min((size_t)500, gt_image_pts.size()));
  gt_world_pts.resize(vcl_min((size_t)500, gt_image_pts.size()));
  }

  test_two_view_triangulation_on_gt_data(gt_cams, gt_image_pts, gt_world_pts, true);

  
  // --- Perturbed cams ---
  vcl_vector<vpgl_perspective_camera<double> > perturbed_cams;

  {
  vcl_vector<double> angles;
  angles.push_back(37);
  angles.push_back(50);

  vcl_vector<vcl_vector<vgl_point_2d<double> > > gt_image_pts_dummy;
  vcl_vector<vgl_point_3d<double> > gt_world_pts_dummy;
  dbdif_data::get_digital_camera_point_dataset(&perturbed_cams, &gt_image_pts_dummy, &gt_world_pts_dummy, angles);
  gt_image_pts.resize(vcl_min((size_t)500, gt_image_pts.size()));
  gt_world_pts.resize(vcl_min((size_t)500, gt_image_pts.size()));
  }

  test_two_view_triangulation_on_gt_data(perturbed_cams, gt_image_pts, gt_world_pts, false);
}


TESTMAIN(test_triangulation);

