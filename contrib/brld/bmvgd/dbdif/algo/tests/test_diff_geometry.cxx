//:
//\file Tests differential-geometric formulas as in the paper:
// "High-Order Differential geometry of Curves for Multiview Reconstruction 
// and Matching", Ricardo Fabbri & Benjamin Kimia, EMMCVPR'05
//
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//
#include <testlib/testlib_test.h>
#include <vcl_limits.h>
#include <vcl_vector.h>
#include <vcl_algorithm.h>
#include <vnl/vnl_double_3x3.h>
#include <dbdif/dbdif_util.h>
#include <dbdif/dbdif_camera.h>
#include <dbdif/dbdif_rig.h>
#include <dbdif/dbdif_analytic.h>
#include <dbdif/algo/dbdif_data.h>

static const double tolerance=vcl_numeric_limits<double>::epsilon()*100;

static void test_ellipse_differential_geometry();
static void test_project_reconstruct(vcl_vector<dbdif_camera> &cam_, vcl_vector<vcl_vector<dbdif_3rd_order_point_3d> > &crv3d);

//: tests multiview projection + reconstruction of differential geometry
MAIN( test_diff_geometry )
{
  START ("Multiview differential geometry");

  //: this implicitly tests the formulas for intrinsic parameter transformation
  test_ellipse_differential_geometry();

  //: ideal cams
  vcl_vector<dbdif_camera> cam_gt_;
  unsigned nviews_=3;
  cam_gt_.resize(nviews_);

  //: perturbed cams
  //  vcl_vector<dbdif_camera> cam_;
  //  cam_.resize(nviews_);

  /*
   * - generate cameras
   * - generate data / projections
   *    - optionally perturb data
   * - reconstruct non-degenerate ones
   * - have to get what we started with
   *    - report accuracy measures on position, tangents, etc
   */ 

  { // ---------- Digital Camera Setup
  vcl_cout << "\n\n\n ========== TESTING FOR DIGITAL CAMERA TURNTABLE ========== \n\n\n";

  unsigned  crop_origin_x_ = 450;
  unsigned  crop_origin_y_ = 1750;
  double x_max_scaled = 500;

  vnl_double_3x3 Kmatrix;
  dbdif_turntable::internal_calib_olympus(Kmatrix, x_max_scaled, crop_origin_x_, crop_origin_y_);

  vpgl_calibration_matrix<double> K(Kmatrix);

  vpgl_perspective_camera<double> *P;

  P = dbdif_turntable::camera_olympus(0, K);
  cam_gt_[0].set_p(*P);
  P = dbdif_turntable::camera_olympus(5, K);
  cam_gt_[1].set_p(*P);
  P = dbdif_turntable::camera_olympus(60, K);
  cam_gt_[2].set_p(*P);

  vcl_vector<vcl_vector<dbdif_3rd_order_point_3d> > crv3d;
  dbdif_data::space_curves_olympus_turntable( crv3d );

  //  mywritev(vcl_string("dat/synth_data3d_rec.dat"), C_rec);

  test_project_reconstruct(cam_gt_, crv3d);
  }

  SUMMARY();
}

void 
test_project_reconstruct(vcl_vector<dbdif_camera> &cam_, vcl_vector<vcl_vector<dbdif_3rd_order_point_3d> > &crv3d)
{

  unsigned nviews_=cam_.size();
  { // Test without intrinsics (do everything in world coordinates)
        vcl_vector< vcl_vector<vcl_vector<dbdif_3rd_order_point_2d> > > crv2d_gt_;
    
      //  dbdif_data::project_into_cams(crv3d, cam_, crv2d_gt_);

        crv2d_gt_.resize(nviews_);
        for (unsigned i=0; i < nviews_; ++i) { // nviews
          crv2d_gt_[i].resize(crv3d.size());
          for (unsigned  k=0; k < crv3d.size(); ++k) { // ncurves
            crv2d_gt_[i][k].resize(crv3d[k].size());
            for (unsigned  jj=0; jj < crv3d[k].size(); ++jj) {
              bool not_degenerate;
              not_degenerate = cam_[i].project_3rd(crv3d[k][jj], &(crv2d_gt_[i][k][jj]));
            }
          }
        }
        
        vcl_vector<vcl_vector<dbdif_3rd_order_point_3d> > C_rec;

        dbdif_rig rig(cam_[0].Pr_,cam_[1].Pr_);

        C_rec.resize(crv2d_gt_[0].size());
        for (unsigned  ic=0; ic < crv2d_gt_[0].size(); ++ic) {
          C_rec[ic].resize(crv2d_gt_[0][ic].size());
          for (unsigned  i=0; i < crv2d_gt_[0][ic].size(); ++i) {
            if (!crv2d_gt_[0][ic][i].valid || !crv2d_gt_[1][ic][i].valid) {
              C_rec[ic][i].valid = false;
            } else {

              dbdif_3rd_order_point_2d p1_w, p2_w;

              p1_w = crv2d_gt_[0][ic][i];
              p2_w = crv2d_gt_[1][ic][i];

              rig.reconstruct_3rd_order(p1_w, p2_w, &(C_rec[ic][i]));
            }
          }
        }


        // statistics


        //: error for each curve 
        vcl_vector<double> err_pos, err_tan, err_k, err_kdot, err_tau, err_n, err_b, min_err_k, max_err_k,
          min_err_k_angle, max_err_k_angle, max_err_speed, min_err_speed, max_err_tau, max_err_tau_angle;

        err_pos.resize(C_rec.size(),0);
        err_tan.resize(C_rec.size(),0);
        err_n.resize(C_rec.size(),0);
        err_b.resize(C_rec.size(),0);
        err_k.resize(C_rec.size(),0);
        min_err_k.resize(C_rec.size(),vcl_numeric_limits<double>::infinity());
        max_err_k.resize(C_rec.size(),0);
        max_err_tau.resize(C_rec.size(),0);
        max_err_speed.resize(C_rec.size(),0);
        min_err_speed.resize(C_rec.size(),0);
        min_err_k_angle.resize(C_rec.size(),0);
        max_err_k_angle.resize(C_rec.size(),0);
        max_err_tau_angle.resize(C_rec.size(),0);
        err_kdot.resize(C_rec.size(),0);
        err_tau.resize(C_rec.size(),0);

        assert(C_rec.size() == crv3d.size());
        for (unsigned  ic=0; ic < C_rec.size(); ++ic) {
          assert(C_rec[ic].size() == crv3d[ic].size());
          vcl_cout << "=========================================\n";
          vcl_cout << "Curve #" << ic+1 << " " << " npoints: " << C_rec[ic].size();
          unsigned  nvalid=0;
          unsigned  nvalid2=0;
          for (unsigned  i=0; i < C_rec[ic].size(); ++i) {
            if (!C_rec[ic][i].valid)
              continue;

            ++nvalid;
            err_pos[ic]  += (C_rec[ic][i].Gama - crv3d[ic][i].Gama).two_norm();
            err_tan[ic]  += (C_rec[ic][i].T - crv3d[ic][i].T).two_norm();
            double err_k_current = vcl_fabs(C_rec[ic][i].K - crv3d[ic][i].K);
            err_k[ic]    += err_k_current;
            if (err_k_current < min_err_k[ic]) {
              min_err_k[ic] = err_k_current;

              dbdif_vector_3d t_world; dbdif_vector_3d gama_world;
              t_world    = crv2d_gt_[0][ic][i].t;
              gama_world = crv2d_gt_[0][ic][i].gama;

              min_err_k_angle[ic] = rig.angle_with_epipolar_plane(t_world, gama_world);
              min_err_speed[ic] = cam_[0].speed(crv3d[ic][i], dot_product(crv3d[ic][i].Gama - cam_[0].c,cam_[0].F));
            }
            if (err_k_current > max_err_k[ic]) {
              max_err_k[ic] = err_k_current;

              dbdif_vector_3d t_world; dbdif_vector_3d gama_world;
              t_world    = crv2d_gt_[0][ic][i].t;
              gama_world = crv2d_gt_[0][ic][i].gama;

              max_err_k_angle[ic] = rig.angle_with_epipolar_plane(t_world, gama_world);
              max_err_speed[ic] = cam_[0].speed(crv3d[ic][i], dot_product(crv3d[ic][i].Gama - cam_[0].c,cam_[0].F));
            }

            if (!dbdif_util::near_zero(C_rec[ic][i].K)) {
              ++nvalid2;
              err_n[ic]    += (C_rec[ic][i].N - crv3d[ic][i].N).two_norm();
              err_b[ic]    += (C_rec[ic][i].B - crv3d[ic][i].B).two_norm();
              err_kdot[ic] += vcl_fabs(C_rec[ic][i].Kdot - crv3d[ic][i].Kdot);
              err_tau[ic]  += vcl_fabs(C_rec[ic][i].Tau - crv3d[ic][i].Tau);
              double err_tau_current = vcl_fabs(C_rec[ic][i].Tau - crv3d[ic][i].Tau);
              if (err_tau_current > max_err_tau[ic]) {
                max_err_tau[ic] = err_tau_current;

                dbdif_vector_3d t_world; dbdif_vector_3d gama_world;
                t_world    = crv2d_gt_[0][ic][i].t;
                gama_world = crv2d_gt_[0][ic][i].gama;

                max_err_tau_angle[ic] = rig.angle_with_epipolar_plane(t_world, gama_world);
              }
            } else {
              if (!dbdif_util::near_zero(C_rec[ic][i].Kdot) || !dbdif_util::near_zero(C_rec[ic][i].Tau)) {
                TEST("Kdot and Tau !=0 while K = 0",0,1);
                vcl_cout << "Kdot: " << C_rec[ic][i].Kdot << " Tau: " << C_rec[ic][i].Tau << vcl_endl;
              }
            }
            // OBS: should have 0 Torsion and 0 Kdot if K == 0, although N is
            // undefined
          }
          vcl_cout << " nvalid: " << nvalid << " nvalid2: " << nvalid2;
          if (nvalid) {
            err_pos[ic] /= nvalid;
            err_tan[ic] /= nvalid;
            err_k[ic] /= nvalid;

            vcl_cout << " err_tan: " << err_tan[ic] << vcl_endl;
            vcl_cout << " err_k: " << err_k[ic] << " max_err_k: " << max_err_k[ic] << 
              "(theta " << max_err_k_angle[ic]*(180.0/vnl_math::pi) << ")" <<
              " max_err_speed: " << max_err_speed[ic] << 
              " min_err_k: " << min_err_k[ic] << 
              "(theta " << min_err_k_angle[ic]*(180.0/vnl_math::pi) << ")" <<
              " min_err_speed: " << min_err_speed[ic] << 
              vcl_endl;

            TEST_NEAR("Pos error", err_pos[ic], 0, 1e-5);
            TEST_NEAR("Tan error", err_tan[ic], 0, 1e-5);
            TEST_NEAR("K error", err_k[ic], 0, 1e-5);

            if (nvalid2) {
              err_kdot[ic] /= nvalid2;
              err_tau[ic] /= nvalid2;
              err_n[ic] /= nvalid2;
              err_b[ic] /= nvalid2;

              vcl_cout << " err_n: " << err_n[ic];
              vcl_cout << " err_b: " << err_b[ic];
              vcl_cout << " err_kdot: " << err_kdot[ic];
              vcl_cout << " err_tau: " << err_tau[ic] << " max_err_tau: " << max_err_tau[ic] << 
              "(theta " << max_err_tau_angle[ic]*(180.0/vnl_math::pi) << ")" << vcl_endl;
              TEST_NEAR("Normal error", err_n[ic], 0, 1e-5);
              TEST_NEAR("Binormal error", err_b[ic], 0, 1e-5);
              TEST_NEAR("Kdot error", err_kdot[ic], 0, 1e-5);
              TEST_NEAR("Torsion error", err_tau[ic], 0, 1e-4);
            }
          } else {
            vcl_cout << vcl_endl;
          }
        }
  }// ! block: test without intrinsics


  { // Test with intrinsics

    vcl_vector< vcl_vector<vcl_vector<dbdif_3rd_order_point_2d> > > crv2d_gt_;
      //  dbdif_data::project_into_cams(crv3d, cam_, crv2d_gt_);

        vcl_cout << "Projecting curves (with intrinsics) \n";
        crv2d_gt_.resize(nviews_);
        for (unsigned i=0; i < nviews_; ++i) { // nviews
          crv2d_gt_[i].resize(crv3d.size());
          for (unsigned  k=0; k < crv3d.size(); ++k) { // ncurves
            crv2d_gt_[i][k].resize(crv3d[k].size());
            for (unsigned  jj=0; jj < crv3d[k].size(); ++jj) {
              bool not_degenerate;
              crv2d_gt_[i][k][jj] = cam_[i].project_to_image(crv3d[k][jj], &not_degenerate);
            }
          }
        }


        vcl_cout << "Reconstructing curves (with intrinsics) \n";

        vcl_vector<vcl_vector<dbdif_3rd_order_point_3d> > C_rec;

        dbdif_rig rig(cam_[0].Pr_,cam_[1].Pr_);

        C_rec.resize(crv2d_gt_[0].size());
        for (unsigned  ic=0; ic < crv2d_gt_[0].size(); ++ic) {
          C_rec[ic].resize(crv2d_gt_[0][ic].size());
          for (unsigned  i=0; i < crv2d_gt_[0][ic].size(); ++i) {
            if (!crv2d_gt_[0][ic][i].valid || !crv2d_gt_[1][ic][i].valid) {
              C_rec[ic][i].valid = false;
            } else {

              dbdif_3rd_order_point_2d p1_w, p2_w;

              rig.cam[0].img_to_world(&(crv2d_gt_[0][ic][i]),&p1_w);
              rig.cam[1].img_to_world(&(crv2d_gt_[1][ic][i]),&p2_w);

              assert(p1_w.valid && p2_w.valid);

              rig.reconstruct_3rd_order(p1_w, p2_w, &(C_rec[ic][i]));
            }
          }
          vcl_cout << vcl_endl;
        }

        // statistics


        //: error for each curve 
        vcl_vector<double> err_pos, err_tan, err_k, err_kdot, err_tau, err_n, err_b, min_err_k, max_err_k,
          min_err_k_angle, max_err_k_angle;

        err_pos.resize(C_rec.size(),0);
        err_tan.resize(C_rec.size(),0);
        err_n.resize(C_rec.size(),0);
        err_b.resize(C_rec.size(),0);
        err_k.resize(C_rec.size(),0);
        min_err_k.resize(C_rec.size(),vcl_numeric_limits<double>::infinity());
        max_err_k.resize(C_rec.size(),0);
        min_err_k_angle.resize(C_rec.size(),0);
        max_err_k_angle.resize(C_rec.size(),0);
        err_kdot.resize(C_rec.size(),0);
        err_tau.resize(C_rec.size(),0);

        assert(C_rec.size() == crv3d.size());
        for (unsigned  ic=0; ic < C_rec.size(); ++ic) {
          assert(C_rec[ic].size() == crv3d[ic].size());
          vcl_cout << "=========================================\n";
          vcl_cout << "Curve #" << ic+1 << " " << " npoints: " << C_rec[ic].size();
          unsigned  nvalid=0;
          unsigned  nvalid2=0;
          for (unsigned  i=0; i < C_rec[ic].size(); ++i) {
            if (!C_rec[ic][i].valid)
              continue;

            ++nvalid;
            err_pos[ic]  += (C_rec[ic][i].Gama - crv3d[ic][i].Gama).two_norm();
            err_tan[ic]  += (C_rec[ic][i].T - crv3d[ic][i].T).two_norm();
            double err_k_current = vcl_fabs(C_rec[ic][i].K - crv3d[ic][i].K);
            err_k[ic]    += err_k_current;
            if (err_k_current < min_err_k[ic]) {
              min_err_k[ic] = err_k_current;

              dbdif_vector_3d t_world; dbdif_vector_3d gama_world;
              cam_[0].img_to_world_vector(crv2d_gt_[0][ic][i].t[0],crv2d_gt_[0][ic][i].t[1], &t_world);
              cam_[0].get_gama(crv2d_gt_[0][ic][i].gama[0],crv2d_gt_[0][ic][i].gama[1], &gama_world);

              min_err_k_angle[ic] = rig.angle_with_epipolar_plane(t_world, gama_world);
            }
            if (err_k_current > max_err_k[ic]) {
              max_err_k[ic] = err_k_current;

              dbdif_vector_3d t_world; dbdif_vector_3d gama_world;
              cam_[0].img_to_world_vector(crv2d_gt_[0][ic][i].t[0],crv2d_gt_[0][ic][i].t[1], &t_world);
              cam_[0].get_gama(crv2d_gt_[0][ic][i].gama[0],crv2d_gt_[0][ic][i].gama[1], &gama_world);

              max_err_k_angle[ic] = rig.angle_with_epipolar_plane(t_world, gama_world);
            }

            if (!dbdif_util::near_zero(C_rec[ic][i].K)) {
              ++nvalid2;
              err_n[ic]    += (C_rec[ic][i].N - crv3d[ic][i].N).two_norm();
              err_b[ic]    += (C_rec[ic][i].B - crv3d[ic][i].B).two_norm();
              err_kdot[ic] += vcl_fabs(C_rec[ic][i].Kdot - crv3d[ic][i].Kdot);
              err_tau[ic]  += vcl_fabs(C_rec[ic][i].Tau - crv3d[ic][i].Tau);
            }
            // OBS: should have 0 Torsion and 0 Kdot if K == 0, although N is
            // undefined
          }
          vcl_cout << " nvalid: " << nvalid << " nvalid2: " << nvalid2;
          if (nvalid) {
            err_pos[ic] /= nvalid;
            err_tan[ic] /= nvalid;
            err_k[ic] /= nvalid;

            vcl_cout << " err_tan: " << err_tan[ic] << vcl_endl;
            vcl_cout << " err_k: " << err_k[ic] << " max_err_k: " << max_err_k[ic] << 
              "(theta " << max_err_k_angle[ic]*(180.0/vnl_math::pi) << ")" <<
              " min_err_k: " << min_err_k[ic] << 
              "(theta " << min_err_k_angle[ic]*(180.0/vnl_math::pi) << ")" <<
              vcl_endl;

            TEST_NEAR("Pos error", err_pos[ic], 0, 1e-5);
            TEST_NEAR("Tan error", err_tan[ic], 0, 1e-5);
            TEST_NEAR("K error", err_k[ic], 0, 1e-5);

            if (nvalid2) {
              err_kdot[ic] /= nvalid2;
              err_tau[ic] /= nvalid2;
              err_n[ic] /= nvalid2;
              err_b[ic] /= nvalid2;

              vcl_cout << " err_n: " << err_n[ic];
              vcl_cout << " err_b: " << err_b[ic];
              vcl_cout << " err_kdot: " << err_kdot[ic];
              vcl_cout << " err_tau: " << err_tau[ic] << vcl_endl;
              TEST_NEAR("Normal error", err_n[ic], 0, 1e-5);
              TEST_NEAR("Binormal error", err_b[ic], 0, 1e-5);
              TEST_NEAR("Kdot error", err_kdot[ic], 0, 1e-5);
              TEST_NEAR("Torsion error", err_tau[ic], 0, 1e-4);
            }
          } else {
            vcl_cout << vcl_endl;
          }
        }
  }// ! block: test with intrinsics
}

//: Affine-transfrom 2D differential geometry of a unit circle using generic
// formulas, and compare with analytic formulas specific for the circle.
void
test_ellipse_differential_geometry()
{

  vcl_vector<double> cv;
  cv.push_back(0);
  cv.push_back(4);
  for (unsigned i_cv=0; i_cv < cv.size(); ++i_cv) {
    long double b=2945.29, a=2895.22, c=cv[i_cv];

    vcl_vector<dbdif_3rd_order_point_2d > p;
    dbdif_vector_2d translation(0,0);
    vcl_vector<double> v_theta;
    dbdif_analytic::circle_curve( 1, translation, p, v_theta, 0., 5., 90.);

    vcl_vector<double> v_theta_2;
    vcl_vector<dbdif_3rd_order_point_2d > ell;
    dbdif_analytic::ellipse(a,b, translation, ell, v_theta_2, 0., 5., 90.);

    assert(v_theta.size() == v_theta_2.size());
    /*
    // check vtheta and vtheta 2 similar
    for (unsigned  i=0; i < v_theta_2.size(); ++i)
      TEST_NEAR("Ellipse and circle thetas", v_theta[i], v_theta_2[i], 1e-9);
    */

    vnl_matrix_fixed<double,2,2> L;

    L(0,0) = a;
    L(0,1) = c;
    L(1,1) = b;
    L(1,0) = 0;

    vnl_matrix_fixed<double,2,2> Linv;
    Linv(0,0) = 1./a;
    Linv(0,1) = -c/(a*b);
    Linv(1,1) = 1./b;
    Linv(1,0) = 0;

    //  assert(dbdif_util::near_zero( (L*L_inv).two_norm()) );
    for (unsigned i=0; i < p.size(); ++i) {
      vcl_cout << "Sample point idx: " << i << " angle: " << v_theta[i]*(180.0/vnl_math::pi) << "(deg)" << vcl_endl;
      // - linear transform p[i]
      // - compare k and kdot with values listed bellow

      // 1) transform gama, t, n
      dbdif_3rd_order_point_2d qq;

      dbdif_vector_2d pp_gama2;

      pp_gama2[0] = p[i].gama[0];
      pp_gama2[1] = p[i].gama[1];

      dbdif_vector_2d qq_gama2;
      qq_gama2 = L*(pp_gama2);

      qq.gama[0] = qq_gama2[0];
      qq.gama[1] = qq_gama2[1];
      qq.gama[2] = 0;

      if (c == 0)
        TEST("Ellipse == linear tr", dbdif_util::near_zero((qq.gama - ell[i].gama).two_norm()),true);

      dbdif_vector_2d pp_n_2;
      pp_n_2[0] = p[i].n[0];
      pp_n_2[1] = p[i].n[1];

      dbdif_vector_2d pp_t_2;
      pp_t_2[0] = p[i].t[0];
      pp_t_2[1] = p[i].t[1];

      dbdif_vector_2d qq_t_2;
      dbdif_vector_2d qq_n_2;
      bool stat = dbdif_frenet::linear_transform(pp_n_2, pp_t_2, p[i].k, p[i].kdot, &(qq.k), &(qq.kdot), &(qq_t_2), &(qq_n_2), L);
      if (!stat)
        qq.valid = false;

      qq.t[0] = qq_t_2[0];
      qq.t[1] = qq_t_2[1];
      qq.t[2] = 0;

      qq.n[0] = qq_n_2[0];
      qq.n[1] = qq_n_2[1];
      qq.n[2] = 0;


      if (c == 0) {
        TEST("Ellipse_T == linear tr T", dbdif_util::near_zero((qq.t - ell[i].t).two_norm()),true);
        TEST("Ellipse_N == linear tr N", dbdif_util::near_zero((qq.n - ell[i].n).two_norm()),true);
        if (!dbdif_util::near_zero((qq.n - ell[i].n).two_norm()))
          vcl_cout << "Ellipse n: " << ell[i].n << "   transf n: " << qq.n << vcl_endl;
      }

      long double theta = v_theta[i];

      // now get back

      dbdif_3rd_order_point_2d rr;

      dbdif_vector_2d rr_t_2, rr_n_2;

      bool stat_rr = dbdif_frenet::linear_transform(qq_n_2, qq_t_2, qq.k, qq.kdot, &(rr.k), &(rr.kdot), &(rr_t_2), &(rr_n_2), Linv);

      TEST("linear_transform inversion", qq.valid && stat_rr && !dbdif_util::near_zero(p[i].k- rr.k, 1e-7), false);
      if (qq.valid && stat_rr && !dbdif_util::near_zero(p[i].k- rr.k, 1e-7))  {
        vcl_cout << "INVERSION TEST: FAIL!  k_orig: " << p[i].k << " k_inverted: " << rr.k << vcl_endl;
      } 

      long double k_q = a * b * sqrtl(0.1e1L / (powl(cosl(theta), 0.4e1L) *
          powl(c, 0.4e1L) + 0.2e1L * powl(cosl(theta), 0.4e1L) * powl(c, 0.2e1L)
          * powl(b, 0.2e1L) - 0.2e1L * powl(b, 0.2e1L) * powl(cosl(theta),
            0.4e1L) * powl(a, 0.2e1L) + powl(a, 0.4e1L) * powl(cosl(theta),
              0.4e1L) + powl(b, 0.4e1L) * powl(cosl(theta), 0.4e1L) - 0.6e1L *
          powl(cosl(theta), 0.4e1L) * powl(a, 0.2e1L) * powl(c, 0.2e1L) + 0.4e1L
          * powl(a, 0.3e1L) * sinl(theta) * c * powl(cosl(theta), 0.3e1L) -
          0.4e1L * powl(cosl(theta), 0.3e1L) * sinl(theta) * a * powl(c, 0.3e1L)
          - 0.4e1L * powl(cosl(theta), 0.3e1L) * sinl(theta) * a * powl(b,
            0.2e1L) * c + 0.6e1L * powl(cosl(theta), 0.2e1L) * powl(a, 0.2e1L) *
          powl(c, 0.2e1L) + 0.2e1L * powl(b, 0.2e1L) * powl(a, 0.2e1L) *
          powl(cosl(theta), 0.2e1L) - 0.2e1L * powl(a, 0.4e1L) *
          powl(cosl(theta), 0.2e1L) - 0.4e1L * powl(a, 0.3e1L) * sinl(theta) * c
          * cosl(theta) + powl(a, 0.4e1L))) * powl(-0.2e1L * a * sinl(theta) * c
        * cosl(theta) + powl(c, 0.2e1L) * powl(cosl(theta), 0.2e1L) + powl(b,
          0.2e1L) * powl(cosl(theta), 0.2e1L) + powl(a, 0.2e1L) - powl(a,
            0.2e1L) * powl(cosl(theta), 0.2e1L), -0.1e1L / 0.2e1L);

      //: Huuuuuuuge formula from Maple - 120 lines!!!!!!!!!!!
      long double kdot_q = -0.3e1L * a * b * (powl(a, 0.5e1L) * c - 0.3e1L * powl(c, 0.4e1L) *
        powl(cosl(theta), 0.5e1L) * sinl(theta) * powl(b, 0.2e1L) - 0.15e2 * powl(a, 0.4e1L)
        * powl(cosl(theta), 0.5e1L) * powl(c, 0.2e1L) * sinl(theta) - 0.3e1L * powl(a,
          0.4e1L) * powl(b, 0.2e1L) * powl(cosl(theta), 0.5e1L) * sinl(theta) + 0.20e2 *
        powl(a, 0.4e1L) * powl(cosl(theta), 0.3e1L) * powl(c, 0.2e1L) * sinl(theta) + 0.4e1L
        * powl(a, 0.4e1L) * powl(b, 0.2e1L) * powl(cosl(theta), 0.3e1L) * sinl(theta) -
        powl(a, 0.4e1L) * powl(b, 0.2e1L) * cosl(theta) * sinl(theta) - 0.5e1L * powl(a,
          0.4e1L) * powl(c, 0.2e1L) * cosl(theta) * sinl(theta) + 0.12e2 * powl(a, 0.3e1L)
        * powl(cosl(theta), 0.6e1L) * c * powl(b, 0.2e1L) - 0.18e2 * powl(a, 0.3e1L) *
        powl(cosl(theta), 0.4e1L) * c * powl(b, 0.2e1L) - powl(c, 0.6e1L) *
        powl(cosl(theta), 0.5e1L) * sinl(theta) - powl(b, 0.6e1L) * powl(cosl(theta), 0.5e1L) *
        sinl(theta) + powl(a, 0.6e1L) * powl(cosl(theta), 0.5e1L) * sinl(theta) - 0.2e1L *
        powl(a, 0.6e1L) * powl(cosl(theta), 0.3e1L) * sinl(theta) + powl(a, 0.6e1L) *
        cosl(theta) * sinl(theta) - 0.6e1L * powl(a, 0.5e1L) * powl(cosl(theta), 0.6e1L) * c +
        0.13e2 * powl(a, 0.5e1L) * powl(cosl(theta), 0.4e1L) * c - 0.8e1L * powl(a,
          0.5e1L) * powl(cosl(theta), 0.2e1L) * c + 0.20e2 * powl(a, 0.3e1L) *
        powl(cosl(theta), 0.6e1L) * powl(c, 0.3e1L) - 0.30e2 * powl(a, 0.3e1L) *
        powl(cosl(theta), 0.4e1L) * powl(c, 0.3e1L) - 0.6e1L * a * powl(cosl(theta), 0.6e1L)
        * powl(c, 0.5e1L) + 0.5e1L * a * powl(c, 0.5e1L) * powl(cosl(theta), 0.4e1L) +
        0.10e2 * powl(a, 0.3e1L) * powl(c, 0.3e1L) * powl(cosl(theta), 0.2e1L) + 0.6e1L *
        powl(a, 0.3e1L) * c * powl(b, 0.2e1L) * powl(cosl(theta), 0.2e1L) + 0.5e1L * a
        * c * powl(b, 0.4e1L) * powl(cosl(theta), 0.4e1L) + 0.15e2 * powl(a, 0.2e1L) *
        powl(cosl(theta), 0.5e1L) * powl(c, 0.4e1L) * sinl(theta) + 0.3e1L * powl(a, 0.2e1L)
        * powl(b, 0.4e1L) * powl(cosl(theta), 0.5e1L) * sinl(theta) - 0.10e2 * powl(a,
            0.2e1L) * powl(cosl(theta), 0.3e1L) * powl(c, 0.4e1L) * sinl(theta) - 0.2e1L *
        powl(a, 0.2e1L) * powl(b, 0.4e1L) * powl(cosl(theta), 0.3e1L) * sinl(theta) - 0.12e2
        * a * powl(cosl(theta), 0.6e1L) * powl(c, 0.3e1L) * powl(b, 0.2e1L) - 0.6e1L *
        a * powl(cosl(theta), 0.6e1L) * c * powl(b, 0.4e1L) + 0.10e2 * a * powl(c,
            0.3e1L) * powl(cosl(theta), 0.4e1L) * powl(b, 0.2e1L) + 0.18e2 * powl(a,
              0.2e1L) * powl(cosl(theta), 0.5e1L) * powl(c, 0.2e1L) * sinl(theta) * powl(b,
                0.2e1L) - 0.12e2 * powl(a, 0.2e1L) * powl(cosl(theta), 0.3e1L) * powl(b,
                  0.2e1L) * powl(c, 0.2e1L) * sinl(theta) - 0.3e1L * powl(c, 0.2e1L) *
                  powl(cosl(theta), 0.5e1L) * sinl(theta) * powl(b, 0.4e1L)) * powl(-0.2e1L *
                    a * sinl(theta) * c * cosl(theta) + powl(c, 0.2e1L) *
                    powl(cosl(theta), 0.2e1L) + powl(b, 0.2e1L) * powl(cosl(theta), 0.2e1L) +
                    powl(a, 0.2e1L) - powl(a, 0.2e1L) * powl(cosl(theta), 0.2e1L), -0.3e1L
                    / 0.2e1L) * powl(0.1e1L / (-0.6e1L * powl(cosl(theta), 0.4e1L) * powl(a,
                          0.2e1L) * powl(c, 0.2e1L) + powl(b, 0.4e1L) * powl(cosl(theta),
                            0.4e1L) - 0.2e1L * powl(b, 0.2e1L) * powl(cosl(theta), 0.4e1L)
                        * powl(a, 0.2e1L) + 0.2e1L * powl(cosl(theta), 0.4e1L) * powl(b,
                          0.2e1L) * powl(c, 0.2e1L) + powl(a, 0.4e1L) * powl(cosl(theta),
                            0.4e1L) + powl(cosl(theta), 0.4e1L) * powl(c, 0.4e1L) + 0.4e1L
                        * powl(a, 0.3e1L) * sinl(theta) * c * powl(cosl(theta), 0.3e1L) -
                        0.4e1L * powl(cosl(theta), 0.3e1L) * sinl(theta) * a * c *
                        powl(b, 0.2e1L) - 0.4e1L * powl(cosl(theta), 0.3e1L) * sinl(theta) *
                        a * powl(c, 0.3e1L) - 0.2e1L * powl(a, 0.4e1L) *
                        powl(cosl(theta), 0.2e1L) + 0.2e1L * powl(a, 0.2e1L) * powl(b,
                          0.2e1L) * powl(cosl(theta), 0.2e1L) + 0.6e1L * powl(cosl(theta),
                            0.2e1L) * powl(a, 0.2e1L) * powl(c, 0.2e1L) - 0.4e1L *
                        powl(a, 0.3e1L) * sinl(theta) * c * cosl(theta) + powl(a,
                          0.4e1L)), -0.1e1L / 0.2e1L) / (powl(a, 0.8e1L) - 0.12e2 *
                        powl(b, 0.4e1L) * powl(cosl(theta), 0.6e1L) * powl(a, 0.4e1L) +
                        0.6e1L * powl(b, 0.4e1L) * powl(cosl(theta), 0.8e1L) * powl(c,
                          0.4e1L) + 0.6e1L * powl(b, 0.4e1L) * powl(cosl(theta), 0.8e1L) *
                        powl(a, 0.4e1L) + 0.4e1L * powl(b, 0.6e1L) * powl(cosl(theta),
                          0.8e1L) * powl(c, 0.2e1L) - 0.36e2 * powl(cosl(theta), 0.8e1L) *
                        powl(a, 0.2e1L) * powl(c, 0.2e1L) * powl(b, 0.4e1L) + 0.60e2
                        * powl(cosl(theta), 0.8e1L) * powl(a, 0.4e1L) * powl(c, 0.2e1L) *
                        powl(b, 0.2e1L) + 0.112e3 * powl(cosl(theta), 0.5e1L) * powl(a,
                          0.5e1L) * powl(c, 0.3e1L) * sinl(theta) + 0.56e2 *
                        powl(cosl(theta), 0.7e1L) * powl(a, 0.3e1L) * powl(c, 0.5e1L) *
                        sinl(theta) - 0.120e3 * powl(cosl(theta), 0.6e1L) * powl(a, 0.4e1L)
                        * powl(c, 0.2e1L) * powl(b, 0.2e1L) - 0.60e2 * powl(cosl(theta),
                          0.8e1L) * powl(a, 0.2e1L) * powl(c, 0.4e1L) * powl(b,
                            0.2e1L) - 0.56e2 * powl(cosl(theta), 0.7e1L) * powl(a, 0.5e1L)
                        * powl(c, 0.3e1L) * sinl(theta) + 0.36e2 * powl(b, 0.4e1L) *
                        powl(cosl(theta), 0.6e1L) * powl(a, 0.2e1L) * powl(c, 0.2e1L) -
                        0.24e2 * powl(a, 0.7e1L) * powl(cosl(theta), 0.5e1L) * sinl(theta) *
                        c + 0.8e1L * powl(a, 0.7e1L) * powl(cosl(theta), 0.7e1L) *
                        sinl(theta) * c + 0.60e2 * powl(cosl(theta), 0.6e1L) * powl(b,
                          0.2e1L) * powl(c, 0.4e1L) * powl(a, 0.2e1L) + 0.60e2 *
                        powl(cosl(theta), 0.4e1L) * powl(b, 0.2e1L) * powl(c, 0.2e1L) *
                        powl(a, 0.4e1L) - 0.56e2 * powl(cosl(theta), 0.5e1L) * powl(c,
                            0.5e1L) * powl(a, 0.3e1L) * sinl(theta) - 0.8e1L *
                        powl(cosl(theta), 0.7e1L) * powl(c, 0.7e1L) * sinl(theta) * a -
                        0.24e2 * powl(b, 0.4e1L) * powl(cosl(theta), 0.7e1L) * sinl(theta) *
                        a * powl(c, 0.3e1L) + 0.24e2 * powl(b, 0.4e1L) *
                        powl(cosl(theta), 0.7e1L) * powl(a, 0.3e1L) * sinl(theta) * c -
                        0.8e1L * powl(b, 0.6e1L) * powl(cosl(theta), 0.7e1L) * sinl(theta) *
                        a * c + 0.80e2 * powl(cosl(theta), 0.7e1L) * powl(a, 0.3e1L)
                        * powl(c, 0.3e1L) * sinl(theta) * powl(b, 0.2e1L) + 0.48e2 *
                        powl(b, 0.2e1L) * powl(cosl(theta), 0.5e1L) * powl(a, 0.5e1L) *
                        sinl(theta) * c - 0.24e2 * powl(b, 0.2e1L) * powl(cosl(theta),
                            0.7e1L) * powl(a, 0.5e1L) * sinl(theta) * c - 0.24e2 *
                        powl(b, 0.4e1L) * powl(cosl(theta), 0.5e1L) * powl(a, 0.3e1L) *
                        sinl(theta) * c - 0.24e2 * powl(cosl(theta), 0.7e1L) * powl(b,
                            0.2e1L) * powl(c, 0.5e1L) * sinl(theta) * a - 0.80e2 *
                        powl(cosl(theta), 0.5e1L) * powl(b, 0.2e1L) * powl(c, 0.3e1L) *
                        powl(a, 0.3e1L) * sinl(theta) + 0.24e2 * powl(a, 0.7e1L) *
                        sinl(theta) * c * powl(cosl(theta), 0.3e1L) - 0.56e2 *
                        powl(cosl(theta), 0.3e1L) * sinl(theta) * powl(a, 0.5e1L) * powl(c,
                            0.3e1L) - 0.8e1L * powl(a, 0.7e1L) * sinl(theta) * c *
                        cosl(theta) - 0.4e1L * powl(a, 0.8e1L) * powl(cosl(theta), 0.2e1L) +
                        0.6e1L * powl(a, 0.8e1L) * powl(cosl(theta), 0.4e1L) +
                        powl(cosl(theta), 0.8e1L) * powl(c, 0.8e1L) + powl(a, 0.8e1L) *
                        powl(cosl(theta), 0.8e1L) - 0.4e1L * powl(a, 0.8e1L) *
                        powl(cosl(theta), 0.6e1L) + powl(b, 0.8e1L) * powl(cosl(theta),
                            0.8e1L) - 0.24e2 * powl(cosl(theta), 0.3e1L) * sinl(theta) *
                        powl(a, 0.5e1L) * c * powl(b, 0.2e1L) - 0.4e1L * powl(b,
                            0.6e1L) * powl(cosl(theta), 0.8e1L) * powl(a, 0.2e1L) - 0.84e2
                        * powl(cosl(theta), 0.4e1L) * powl(a, 0.6e1L) * powl(c, 0.2e1L) -
                        0.140e3 * powl(cosl(theta), 0.6e1L) * powl(a, 0.4e1L) * powl(c,
                            0.4e1L) + 0.84e2 * powl(cosl(theta), 0.6e1L) * powl(a, 0.6e1L)
                        * powl(c, 0.2e1L) - 0.28e2 * powl(cosl(theta), 0.8e1L) * powl(a,
                            0.2e1L) * powl(c, 0.6e1L) - 0.28e2 * powl(cosl(theta), 0.8e1L)
                        * powl(a, 0.6e1L) * powl(c, 0.2e1L) + 0.70e2 * powl(cosl(theta),
                            0.8e1L) * powl(a, 0.4e1L) * powl(c, 0.4e1L) + 0.28e2 *
                        powl(cosl(theta), 0.2e1L) * powl(a, 0.6e1L) * powl(c, 0.2e1L) +
                        0.12e2 * powl(b, 0.2e1L) * powl(cosl(theta), 0.6e1L) * powl(a,
                            0.6e1L) + 0.4e1L * powl(a, 0.6e1L) * powl(b, 0.2e1L) *
                        powl(cosl(theta), 0.2e1L) + 0.70e2 * powl(cosl(theta), 0.4e1L) *
                        powl(c, 0.4e1L) * powl(a, 0.4e1L) + 0.28e2 * powl(cosl(theta),
                            0.6e1L) * powl(c, 0.6e1L) * powl(a, 0.2e1L) + 0.4e1L *
                        powl(cosl(theta), 0.8e1L) * powl(b, 0.2e1L) * powl(c, 0.6e1L) -
                        0.12e2 * powl(b, 0.2e1L) * powl(cosl(theta), 0.4e1L) * powl(a,
                            0.6e1L) - 0.4e1L * powl(b, 0.2e1L) * powl(cosl(theta), 0.8e1L)
                        * powl(a, 0.6e1L) + 0.6e1L * powl(b, 0.4e1L) * powl(cosl(theta),
                            0.4e1L) * powl(a, 0.4e1L) + 0.4e1L * powl(b, 0.6e1L) *
                        powl(cosl(theta), 0.6e1L) * powl(a, 0.2e1L)) * powl(powl(-a *
                            sinl(theta) + c * cosl(theta), 0.2e1L) + powl(b, 0.2e1L) *
                          powl(cosl(theta), 0.2e1L), -0.1e1L / 0.2e1L); 




      //: curvature of a unit circle after point-transform by 
      // L = [a c
      //      0 b]

      if (qq.valid) {
        TEST_NEAR("linear_transform K Transf Equals Analytic", (long double)(qq.k), k_q, 1e-8);
        TEST_NEAR("linear_transform Kdot Transf Equals Analytic", (long double)(qq.kdot), kdot_q, 1e-8);
      }

      if (qq.valid && !dbdif_util::near_zero((long double)(qq.k)- k_q, 1e-7))  {
        vcl_cout << "Not equal! k_maple: " << k_q << " k_linear: " << qq.k << vcl_endl;
      } 

      // Another test: use the inverse transform on the value from maple, and see what
      // we get.

      bool stat_maple = dbdif_frenet::linear_transform(qq_n_2, qq_t_2, k_q, qq.kdot, &(rr.k), &(rr.kdot), &(rr_t_2), &(rr_n_2), Linv);

        TEST_NEAR("linear_transform Maple inversion", rr.k, p[i].k, 1e-6);

      if (qq.valid && stat_rr && stat_maple)
        vcl_cout << "Ok\n";
      else 
        vcl_cout << "Invalid!\n";

      if (qq.valid && stat_rr && !dbdif_util::near_zero(p[i].k- rr.k, 1e-7))  {
        vcl_cout << "INVERSION TEST: FAIL!  k_orig: " << p[i].k << " k_inverted: " << rr.k << vcl_endl;
      }
    }
  }
}
