#include <testlib/testlib_test.h>

#include <vcl_limits.h>
#include <vcl_iostream.h>

#include <vgl/vgl_distance.h>
#include <vgl/algo/vgl_norm_trans_2d.h>

#include <vpgl/algo/vpgl_fm_compute_8_point.h>
#include <vpgl/algo/vpgl_fm_compute_7_point.h>

#include <mvl/FMatrix.h>
#include <mvl/FMatrixComputeLinear.h>
#include <mvl/FMatrixComputeNonLinear.h>
#include <mvl/FMatrixCompute7Point.h>
#include <mvl/FMatrixComputeRANSAC.h>
#include <mvl/HomgInterestPointSet.h>
#include <mvl/HomgNorm2D.h>




//static const double tolerance=vcl_numeric_limits<double>::epsilon()*10000;
static const double tolerance=1e-5;


// More extensive tests for fundamental matrix estimation. The goal is to:
//
// - provide more tests with real data
// - compare output to the old mvl library
// - study the stability of each method in the long-run.
// - study the benefits of iterative optimization
// - study if and how robust estimation can help not only in weeding out of outliers, but also in
// selecting subset of correspondences that provides the best conditioning for estimation.
//
//
//
// Findings 9/27/07
//
//  - vpgl and mvl don't agree when normalization is on
//      - vpgl doesn't change results if normalization is on/off. This is a bug!
//
//  - when normalization is off, vpgl and mvl agree for 8 points, but disagree for more than 8
//  points!
//
MAIN( test_fm_estimation )
{
  START ("Fundamental Matrix Estimation - Extra Tests");


  // Corresponding points picked by hand from GE Jeep data set.
  // There are no outliers, but the error in pixels has standard deviation of about 2px.


  vcl_vector<vgl_point_2d<double> > vp0,vp1;
  vcl_vector<vgl_homg_point_2d<double> > vp0_homg,vp1_homg;


  // Points with same index correspond.
  
  vp0.push_back(vgl_point_2d<double>( 647,627  ));  //1
  vp0.push_back(vgl_point_2d<double>( 96,268   ));  //2
  vp0.push_back(vgl_point_2d<double>( 947,516  ));  //3
  vp0.push_back(vgl_point_2d<double>( 605,275  ));  //4
  vp0.push_back(vgl_point_2d<double>( 776,779  ));  //5
  vp0.push_back(vgl_point_2d<double>( 989,155  ));  //6
  vp0.push_back(vgl_point_2d<double>( 1244,837 ));  //7
  vp0.push_back(vgl_point_2d<double>( 417,792  ));  //8
//  vp0.push_back(vgl_point_2d<double>( 755,443  ));  //9
//  vp0.push_back(vgl_point_2d<double>( 72,20    ));  //10
//  vp0.push_back(vgl_point_2d<double>( 108,22   ));  //11
//  vp0.push_back(vgl_point_2d<double>( 1077,774 ));  //12
//  vp0.push_back(vgl_point_2d<double>( 550,902  ));  //13
//  vp0.push_back(vgl_point_2d<double>( 1249,590 ));  //14
//  vp0.push_back(vgl_point_2d<double>( 535,574  ));  //15

  vp1.push_back(vgl_point_2d<double>( 651,562  ));  //1
  vp1.push_back(vgl_point_2d<double>( 741,346  ));  //2
  vp1.push_back(vgl_point_2d<double>( 955,534  ));  //3
  vp1.push_back(vgl_point_2d<double>( 1023,378 ));  //4
  vp1.push_back(vgl_point_2d<double>( 544,638  ));  //5
  vp1.push_back(vgl_point_2d<double>( 1103,279 ));  //6
  vp1.push_back(vgl_point_2d<double>( 744,699  ));  //7
  vp1.push_back(vgl_point_2d<double>( 351,618  ));  //8
//  vp1.push_back(vgl_point_2d<double>( 850,466  ));  //9
//  vp1.push_back(vgl_point_2d<double>( 739,155  ));  //10
//  vp1.push_back(vgl_point_2d<double>( 1134,209 ));  //11
//  vp1.push_back(vgl_point_2d<double>( 761,649  ));  //12
//  vp1.push_back(vgl_point_2d<double>( 240,650  ));  //13
//  vp1.push_back(vgl_point_2d<double>( 1088,604 ));  //14
//  vp1.push_back(vgl_point_2d<double>( 599,521  ));  //15



  unsigned n=vp0.size();
  vp0_homg.resize(n);
  vp1_homg.resize(n);
  for (unsigned i=0; i < (unsigned)n; ++i) {
    vp0_homg[i] = vgl_homg_point_2d<double>(vp0[i]);
    vp1_homg[i] = vgl_homg_point_2d<double>(vp1[i]);
  }
  

  //  ---- VPGL algorithms ----

  vpgl_fundamental_matrix<double> f_vpgl;
  
  // -- VPGL 8 point Code *not* using RANSAC
  {
    vpgl_fm_compute_8_point fmc8(true); // normalization on
    bool retval = fmc8.compute(vp0_homg,vp1_homg,f_vpgl);

    if (!retval) {
      vcl_cerr << "Traditional 8-point fundamental matrix estimation was NOT successful\n";
    }
    TEST("Traditional 8-point fundamental matrix method returned ok",retval,true);

    vcl_cout << "Fundamental Matrix from VPGL 8 point linear method:\n" << f_vpgl.get_matrix() << vcl_endl;
  }

  FMatrix f_mvl;

  // Perform the fit using the normalised 8-point estimator.
  {
    FMatrixComputeLinear computor(true,true);
    f_mvl = computor.compute(vp0_homg, vp1_homg);

    vcl_cout << "Fundamental matrix from MVL 8 Point linear method:\n" << f_mvl << vcl_endl;
  }



  // ----- Compare the results -----
  

  // Position of epipoles

  vgl_homg_point_2d<double> e0_mvl,e0_vpgl; // epipole in image_0
  vgl_homg_point_2d<double> e1_mvl,e1_vpgl; // epipole in image_1
  
  f_mvl.get_epipoles(e0_mvl,e1_mvl);
  f_vpgl.get_epipoles(e0_vpgl,e1_vpgl);

  vcl_cout << "vpgl epipoles: e0 = " << vgl_point_2d<double>(e0_vpgl) << "\te1 = " << vgl_point_2d<double>(e1_vpgl) << vcl_endl;
  vcl_cout << " mvl epipoles: e0 = " << vgl_point_2d<double>(e0_mvl)  << "\te1 = " << vgl_point_2d<double>(e1_mvl)  << vcl_endl;


  TEST_NEAR("Distance between epipole e0 from vpgl and mvl normalized 8 point",vgl_distance<double>(e0_vpgl,e0_mvl),0,tolerance);
  TEST_NEAR("Distance between epipole e1 from vpgl and mvl normalized 8 point",vgl_distance<double>(e1_vpgl,e1_mvl),0,tolerance);


  // ----- Compare vgl and mvl normalizing tranformations -----

  vcl_vector<vgl_homg_point_2d<double> > p0_norm_vgl;

  { // vgl
    p0_norm_vgl.reserve(n);

    vgl_norm_trans_2d<double> p0nt;

    p0nt.compute_from_points(vp0_homg);
    for ( unsigned i = 0; i < n; i++ ) {
      p0_norm_vgl.push_back( p0nt*vp0_homg[i] );
    }
  }

  vcl_vector<vgl_homg_point_2d<double> > p0_norm_mvl;

  { // mvl
    p0_norm_mvl.reserve(n);

    HomgNorm2D cond0(vp0_homg);

    for (unsigned i=0; i < n; i++ ) {
      p0_norm_mvl.push_back(vgl_homg_point_2d<double>(cond0[i].x(),cond0[i].y(),cond0[i].w()));
    }
  }

  vcl_cout << "\nvgl normalized p0" << vcl_endl;
  for (unsigned i=0; i < n; ++i) {
    vcl_cout << "\t" << p0_norm_vgl[i] << vcl_endl;
  }

  vcl_cout << "\nmvl normalized p0" << vcl_endl;
  for (unsigned i=0; i < n; ++i) {
    vcl_cout << "\t" << p0_norm_mvl[i] << vcl_endl;
  }
  vcl_cout << vcl_endl;

  for (unsigned i=0; i < n; ++i) {
    TEST_NEAR("Distance between vpgl and mvl normalized p0",vgl_distance<double>(p0_norm_vgl[i],p0_norm_mvl[i]),0,tolerance);
  }

  SUMMARY();
}

