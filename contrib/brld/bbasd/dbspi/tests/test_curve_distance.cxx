#include <testlib/testlib_test.h>

#include <vsol/vsol_point_2d.h>
#include <dbspi/dbspi_curve_distance.h>
#include <dbil/algo/dbil_exact_distance_transform.h>

void
test_num_inliers(
    const vcl_vector<double> &d_threshold, 
    const vcl_vector<unsigned> &num_inliers, 
    vil_image_view<vxl_uint_32> &feature_map, 
    const vsol_digital_curve_2d &curve, 
    bool verbose)
{
  assert(d_threshold.size() == num_inliers.size());
  // compute dt + label
  vil_image_view <vxl_uint_32> label(feature_map.ni(), feature_map.nj(), 1);
  dbil_exact_distance_transform_maurer_label(feature_map, label);

  for (unsigned i=0; i < d_threshold.size(); ++i) {
    if (verbose)
      vcl_cout << "distance threshold " << d_threshold[i] << vcl_endl;

    unsigned num_inliers_computed = 
      dbspi_curve_distance::num_inliers_dt(curve, d_threshold[i], feature_map, label);

    TEST("Number inliers equals ground truth", num_inliers_computed, num_inliers[i]);

    if (verbose)
      vcl_cout  << "num_inliers = " << num_inliers_computed 
        << "  num_inliers ground truth = " << num_inliers[i] << vcl_endl;
  }
}

MAIN( test_curve_distance )
{
  START ("dbspi curve distance aggregation");


  { 
  vcl_cout << "2x2 image with two features at different distances from the curve\n";
  // -- Data
  unsigned r=2,c=2;
  vil_image_view <vxl_uint_32> feature_map(c,r,1);
  feature_map.fill(1);
  feature_map(0,0) = 0;
  feature_map(0,1) = 0;

  vsol_digital_curve_2d curve;
  curve.add_vertex(new vsol_point_2d(1,0));
  
  // -- Ground truth
  vcl_vector <double> d_threshold;
  vcl_vector <unsigned> num_inliers;

  // equidistant edgels count as 1
  d_threshold.push_back(1.01); num_inliers.push_back(1);
  d_threshold.push_back(300); num_inliers.push_back(1);
  d_threshold.push_back(0); num_inliers.push_back(0);   
  
  // -- Test
  bool verbose=true;
  test_num_inliers(d_threshold, num_inliers, feature_map, curve, verbose);
  }

  { 
  vcl_cout << "2x2 image with equidistant features from the curve\n";
  // -- Data
  unsigned r=2,c=2;
  vil_image_view <vxl_uint_32> feature_map(c,r,1);
  feature_map.fill(1);
  feature_map(0,0) = 0;
  feature_map(1,1) = 0;

  vsol_digital_curve_2d curve;
  curve.add_vertex(new vsol_point_2d(1,0));
  
  // -- Ground truth
  vcl_vector <double> d_threshold;
  vcl_vector <unsigned> num_inliers;

  // equidistant edgels count as 1
  d_threshold.push_back(1.01); num_inliers.push_back(1);
  d_threshold.push_back(2); num_inliers.push_back(1);
  d_threshold.push_back(0); num_inliers.push_back(0);   
  
  // -- Test
  bool verbose=true;
  test_num_inliers(d_threshold, num_inliers, feature_map, curve, verbose);
  }

  { 
  vcl_cout << "2x2 image with features on all pixels, one of them on the curve\n";
  // -- Data
  unsigned r=2,c=2;
  vil_image_view <vxl_uint_32> feature_map(c,r,1);
  feature_map.fill(0);

  vsol_digital_curve_2d curve;
  curve.add_vertex(new vsol_point_2d(1,0));
  
  // -- Ground truth
  vcl_vector <double> d_threshold;
  vcl_vector <unsigned> num_inliers;

  d_threshold.push_back(1.01); num_inliers.push_back(1);
  d_threshold.push_back(3); num_inliers.push_back(1);
  d_threshold.push_back(0.01); num_inliers.push_back(1);   
  
  // -- Test
  bool verbose=true;
  test_num_inliers(d_threshold, num_inliers, feature_map, curve, verbose);
  }

  { 
  vcl_cout << "4x5 image\n";
  // -- Data
  unsigned r=5,c=4;
  vil_image_view <vxl_uint_32> feature_map(c,r,1);
  feature_map.fill(1);
  feature_map(2,1) = 0;
  feature_map(0,4) = 0;
  feature_map(0,2) = 0;
  feature_map(3,4) = 0;

  vsol_digital_curve_2d curve;
  curve.add_vertex(new vsol_point_2d(3,0));
  curve.add_vertex(new vsol_point_2d(2,0));
  curve.add_vertex(new vsol_point_2d(1,1));
  curve.add_vertex(new vsol_point_2d(1,2));
  curve.add_vertex(new vsol_point_2d(2,3));
  
  // -- Ground truth
  vcl_vector <double> d_threshold;
  vcl_vector <unsigned> num_inliers;

  d_threshold.push_back(0); num_inliers.push_back(0);   
  d_threshold.push_back(1.01); num_inliers.push_back(2);
  d_threshold.push_back(2.01); num_inliers.push_back(3);

  // only nearest edgels count:
  d_threshold.push_back(5.01); num_inliers.push_back(3);
  
  // -- Test
  bool verbose=true;
  test_num_inliers(d_threshold, num_inliers, feature_map, curve, verbose);
  }


  SUMMARY();
}
