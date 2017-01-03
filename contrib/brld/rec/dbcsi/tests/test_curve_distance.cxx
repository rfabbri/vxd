#include <testlib/testlib_test.h>

#include <vsol/vsol_point_2d.h>
#include <dbcsi/dbcsi_curve_distance.h>
#include <vil/algo/vil_exact_distance_transform.h>
#include <dbdet/algo/dbdet_convert_edgemap.h>

void
test_num_inliers(
    const vcl_vector<double> &d_threshold, 
    const vcl_vector<double> &dtheta_threshold, 
    const vcl_vector<unsigned> &num_inliers, 
    dbdet_edgemap &em, 
    const vcl_vector<dbdet_edgel> &curve, 
    bool verbose)
{
  assert(d_threshold.size() == num_inliers.size());
  assert(dtheta_threshold.size() == num_inliers.size());

  // rasterize edge map

  vil_image_view<vxl_byte > feature_map;
  dbdet_convert_edgemap_to_image(em, feature_map);

  vil_image_view<vxl_uint_32> edt(feature_map.ni(), feature_map.nj(), 1);

  for(unsigned i=0; i < edt.ni(); i++)
    for(unsigned j=0; j < edt.nj(); j++)
      if (feature_map(i,j))
        edt(i,j)=0; // features have distance 0
      else
        edt(i,j)=1;

  // compute dt + label
  vil_image_view <vxl_uint_32> label(edt.ni(), edt.nj(), 1);
  vil_exact_distance_transform_maurer_label(edt, label);

  for (unsigned i=0; i < d_threshold.size(); ++i) {
    if (verbose) {
      vcl_cout << "distance threshold " << d_threshold[i] << vcl_endl;
      vcl_cout << "orientation difference threshold (deg) " << dtheta_threshold[i]*180./vnl_math::pi << vcl_endl;
    }

    unsigned num_inliers_computed = 
      dbcsi_curve_distance::num_inliers_dt_oriented(curve, 
          d_threshold[i], dtheta_threshold[i], 
          edt, label, em);

    TEST("Number inliers equals ground truth", num_inliers_computed, num_inliers[i]);

    if (verbose)
      vcl_cout  << "num_inliers = " << num_inliers_computed 
        << "  num_inliers ground truth = " << num_inliers[i] << vcl_endl << vcl_endl;
  }
}

MAIN( test_curve_distance )
{
  START ("dbcsi curve distance aggregation");

  { 
  vcl_cout << "2x2 image with two features at different distances from the curve,\n";
  vcl_cout << "but the nearest edgel has a very different orientation.\n";
  // -- Data
  vcl_vector<dbdet_edgel *> edgels;

  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(0.,0.), vnl_math::pi/4.0));
  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(0.,1.), 2*vnl_math::pi/3.0));

  unsigned r=2,c=2;
  dbdet_edgemap em(c,r, edgels); // this destroys the newed edgels.

  vcl_vector<dbdet_edgel> curve;
  curve.push_back(dbdet_edgel(vgl_point_2d<double>(1,0), 3*vnl_math::pi/4));
  
  // -- Ground truth
  vcl_vector <double> d_threshold;
  vcl_vector <double> dtheta_threshold;
  vcl_vector <unsigned> num_inliers;

  d_threshold.push_back(1.01); dtheta_threshold.push_back(vnl_math::pi/2.);
  num_inliers.push_back(0);

  d_threshold.push_back(1.01); dtheta_threshold.push_back(vnl_math::pi/6. + 1e-5);
  num_inliers.push_back(0);

  // only the closest edgel's orientation can count. If we have an edgel within
  // the d_threshold but which is not the closest to the curve edgel, it won't
  // count under this strategy. This aproximation makes the code much faster.
  d_threshold.push_back(300); dtheta_threshold.push_back(vnl_math::pi/6. + 1e-5);
  num_inliers.push_back(0);

  d_threshold.push_back(0); dtheta_threshold.push_back(0);
  num_inliers.push_back(0);   
  
  // -- Test
  bool verbose=true;
  test_num_inliers(d_threshold, dtheta_threshold, num_inliers, em, curve, 
      verbose);
  }

  { 
  vcl_cout << "2x2 image with two features at different distances from the curve\n";
  // -- Data
  vcl_vector<dbdet_edgel *> edgels;

  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(0.,0.), vnl_math::pi/4.0));
  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(0.,1.), 2*vnl_math::pi/3.0));

  unsigned r=2,c=2;
  dbdet_edgemap em(c,r, edgels); // this destroys the newed edgels.

  vcl_vector<dbdet_edgel> curve;
  curve.push_back(dbdet_edgel(vgl_point_2d<double>(1,0), 3*vnl_math::pi/4));
  
  // -- Ground truth
  vcl_vector <double> d_threshold;
  vcl_vector <double> dtheta_threshold;
  vcl_vector <unsigned> num_inliers;

  d_threshold.push_back(1.01); dtheta_threshold.push_back(vnl_math::pi/2.);
  num_inliers.push_back(0);

  d_threshold.push_back(1.01); dtheta_threshold.push_back(vnl_math::pi/6. + 1e-5);
  num_inliers.push_back(0);

  // only the closest edgel's orientation can count. If we have an edgel within
  // the d_threshold but which is not the closest to the curve edgel, it won't
  // count under this strategy. This aproximation makes the code much faster.
  d_threshold.push_back(300); dtheta_threshold.push_back(vnl_math::pi/6. + 1e-5);
  num_inliers.push_back(0);

  d_threshold.push_back(0); dtheta_threshold.push_back(0);
  num_inliers.push_back(0);   
  
  // -- Test
  bool verbose=true;
  test_num_inliers(d_threshold, dtheta_threshold, num_inliers, em, curve, 
      verbose);
  }

  { 
  vcl_cout << "2x2 image with two equidistant features from the curve and with \n"
    << "similar orientations. One of them will count (but _which_ one is undefined).\n";
  // -- Data
  vcl_vector<dbdet_edgel *> edgels;

  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(0.,0.), 60.*vnl_math::pi/180.));
  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(1.,1.), 30.*vnl_math::pi/180.));

  unsigned r=2,c=2;
  dbdet_edgemap em(c,r, edgels); // this destroys the newed edgels.

  vcl_vector<dbdet_edgel> curve;
  curve.push_back(dbdet_edgel(vgl_point_2d<double>(1,0), vnl_math::pi/4));
  
  // -- Ground truth
  vcl_vector <double> d_threshold;
  vcl_vector <double> dtheta_threshold;
  vcl_vector <unsigned> num_inliers;

  d_threshold.push_back(1.01); dtheta_threshold.push_back(vnl_math::pi/2.);
  num_inliers.push_back(1);

  d_threshold.push_back(1.01); dtheta_threshold.push_back(vnl_math::pi/6.);
  num_inliers.push_back(1);

  // only the closest edgel's orientation can count. If we have an edgel within
  // the d_threshold but which is not the closest to the curve edgel, it won't
  // count under this strategy. This aproximation makes the code much faster.
  d_threshold.push_back(300); dtheta_threshold.push_back(vnl_math::pi/6.);
  num_inliers.push_back(1);

  d_threshold.push_back(300); dtheta_threshold.push_back(0);
  num_inliers.push_back(0);   

  d_threshold.push_back(0); dtheta_threshold.push_back(0);
  num_inliers.push_back(0);   
  
  // -- Test
  bool verbose=true;
  test_num_inliers(d_threshold, dtheta_threshold, num_inliers, em, curve, 
      verbose);
  }

  { 
  vcl_cout << "4x5 image\n";
  // -- Data
  vcl_vector<dbdet_edgel *> edgels;

  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(2.,1.4), 210.*vnl_math::pi/180.));
  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(2.3,0.7), 70.*vnl_math::pi/180.));
  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(0.,2.), 2.*vnl_math::pi/3.));
  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(0.,4.), 100.*vnl_math::pi/180.));
  edgels.push_back(new dbdet_edgel(vgl_point_2d<double>(3.,3.), 150.*vnl_math::pi/180.));

  unsigned r=5, c=4;
  dbdet_edgemap em(c,r, edgels); // this destroys the newed edgels.

  vcl_vector<dbdet_edgel> curve;
  curve.push_back(dbdet_edgel(vgl_point_2d<double>(3,0), 0.));
  curve.push_back(dbdet_edgel(vgl_point_2d<double>(2,0), 0.01));
  curve.push_back(dbdet_edgel(vgl_point_2d<double>(1,1), vnl_math::pi/3.0));
  curve.push_back(dbdet_edgel(vgl_point_2d<double>(1,2), vnl_math::pi/2.0));
  curve.push_back(dbdet_edgel(vgl_point_2d<double>(2.2,3.4), 160.*vnl_math::pi/180.0));
  
  // -- Ground truth
  vcl_vector <double> d_threshold;
  vcl_vector <double> dtheta_threshold;
  vcl_vector <unsigned> num_inliers;

  // one would think that this would yield 4 inliers, but don't forget that we
  // don't count the same inlier edgel at bucket 2,1 twice.
  // This case is actually accidental because the same edgel from this bucket is
  // picket when evaluating two different curve edgels.
  d_threshold.push_back(1.01); dtheta_threshold.push_back(vnl_math::pi/2.);
  num_inliers.push_back(3);

  // The edgel at (0,4) never counts, because it is not closest.
  d_threshold.push_back(300); dtheta_threshold.push_back(vnl_math::pi/2);
  num_inliers.push_back(3);   

  d_threshold.push_back(1.01); dtheta_threshold.push_back(vnl_math::pi/6 + 1e-5);
  num_inliers.push_back(3);

  d_threshold.push_back(1.01); dtheta_threshold.push_back(11.0*vnl_math::pi/180.0);
  num_inliers.push_back(2);

  d_threshold.push_back(0); dtheta_threshold.push_back(0);
  num_inliers.push_back(0);   
  
  // -- Test
  bool verbose=true;
  test_num_inliers(d_threshold, dtheta_threshold, num_inliers, em, curve, 
      verbose);
  }

  SUMMARY();
}
