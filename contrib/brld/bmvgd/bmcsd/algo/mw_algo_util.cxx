#include "mw_algo_util.h"

#include <vnl/vnl_double_2.h>
#include <brct/brct_algos.h>
#include <dbgl/algo/dbgl_distance.h>
#include <dbgl/algo/dbgl_curve_smoothing.h>
#include <dbsol/algo/dbsol_geno.h>
#include <dbdif/algo/dbdif_transfer.h>
#include <mw/mw_util.h>

void mw_algo_util::
move_world_to_1st_cam(vcl_vector<vpgl_perspective_camera<double> *> &cam)
{
  // just pass empty vcl_vector
  vcl_vector<vgl_point_3d<double> > pts;

  move_world_to_1st_cam(cam,pts);
}

void mw_algo_util::
move_world_to_1st_cam(vcl_vector<vpgl_perspective_camera<double> *> &cam,vcl_vector<vgl_point_3d<double> > &pts)
{
  vgl_h_matrix_3d<double> R_0;
  R_0 = cam[0]->get_rotation().as_h_matrix_3d();

  vgl_point_3d<double> c_0_pt;
  c_0_pt = cam[0]->get_camera_center();
//  vnl_vector_fixed<double,3> c_0(c_0_pt.x(),c_0_pt.y(),c_0_pt.z());
//  vnl_vector<double> T_0 = -R_0*c_0;


  vnl_matrix_fixed<double,4,4> tr_0_inv;
  tr_0_inv.fill(0);

  tr_0_inv[0][3] = c_0_pt.x();
  tr_0_inv[1][3] = c_0_pt.y();
  tr_0_inv[2][3] = c_0_pt.z();
  tr_0_inv[3][3] = 1;

  for (int c = 0; c<3; c++)
    for (int r = 0; r<3; r++)
      tr_0_inv[r][c]=R_0.get_matrix()[c][r]; //: inverse/transpose

  vgl_h_matrix_3d<double> tr_0_inv_hmg(tr_0_inv);


  vgl_h_matrix_3d<double> R_id;
  R_id.set_identity();

  { // sanity check

    vpgl_perspective_camera<double> ctmp;

    ctmp = vpgl_perspective_camera<double>::postmultiply(*(cam[0]),tr_0_inv_hmg);

    assert(mw_util::near_zero((ctmp.get_calibration().get_matrix()-cam[0]->get_calibration().get_matrix()).array_two_norm()));
    assert(mw_util::near_zero(mw_util::vgl_to_vnl(ctmp.get_camera_center()).two_norm()));
    assert(mw_util::near_zero((ctmp.get_rotation().as_h_matrix_3d().get_upper_3x3().get_matrix()-R_id.get_upper_3x3().get_matrix()).array_two_norm()));
  }

  cam[0]->set_camera_center(vgl_point_3d<double>(0,0,0));
  cam[0]->set_rotation(vgl_rotation_3d<double>(R_id));



  for (unsigned i=1; i < cam.size(); ++i) {

    vpgl_perspective_camera<double> ctmp;

    ctmp =  vpgl_perspective_camera<double>::postmultiply(*(cam[i]),tr_0_inv_hmg);
    delete cam[i];

    cam[i] = new vpgl_perspective_camera<double>(ctmp);

//  static  vpgl_perspective_camera<T>
 //  postmultiply( const vpgl_perspective_camera<T>& in_cam,
  //               const vgl_h_matrix_3d<T>& euclid_trans);
  }

  vgl_homg_point_3d<double> c_0_hmg(c_0_pt);

  vgl_point_3d<double> transl = R_0*c_0_hmg;

  double tx=-transl.x(),ty=-transl.y(),tz=-transl.z();

//  transl.get_nonhomogeneous(x,y,z);
  R_0.set_translation(tx,ty,tz);

  for (unsigned i=0; i < pts.size(); ++i) {
    pts[i] = R_0*vgl_homg_point_3d<double>(pts[i]);
  }
}


//----------------------------------------------------------------------

bool mw_algo_util::
dg_reprojection_error(
    vcl_vector<dbdif_3rd_order_point_2d> &pts, //:< pts[iv] points in view iv
    const vcl_vector<dbdif_camera> &cam,
    unsigned v,
    double &dpos,
    double &dtheta,
    double &dnormal_plus,
    double &dnormal_minus,
    double &dtangential_plus,
    double &dtangential_minus,
    double &dk,
    double &dkdot,
    unsigned &n
    )
{
  bool one_true=false;

  for (unsigned u1=0; u1 < cam.size(); ++u1) {
    if (u1 == v)
      continue;
    for (unsigned u2=0; u2 < cam.size(); ++u2) {
      if (u2 == u1 || u2 == v)
        continue;

//      p1 -> pts[u1]
//      p2 -> pts[u2]
//      Obtain p_v_reproj. Then compare to pts[v]

      dbdif_3rd_order_point_2d p_v_reproj; 
      dbdif_3rd_order_point_3d Prec;

      dbdif_rig rig(cam[u1].Pr_,cam[u2].Pr_);

      bool retval = 
      dbdif_transfer::transfer_by_reconstruct_and_reproject ( 
          pts[u1], pts[u2], p_v_reproj, Prec, cam[v], rig);

      if (retval) {
        one_true = true;
        double angle = mw_util::angle_unit(p_v_reproj.t, pts[v].t);

        double dtheta_inc = vcl_min(angle, vnl_math::pi - angle);
        dtheta += dtheta_inc*dtheta_inc;

        // TODO: make bellow squared too.
        dk     += vcl_fabs(p_v_reproj.k - pts[v].k);
        dkdot  += vcl_fabs(p_v_reproj.kdot - pts[v].kdot);

        double d = (p_v_reproj.gama - pts[v].gama).two_norm();
        dpos   += d*d;
        ++n;

        double d_e, d_n_1, d_t_1;
        dbgl_distance::projected_distance(
            p_v_reproj.gama[0],  p_v_reproj.gama[1],
            pts[v].gama[0],  pts[v].gama[1], vcl_atan2(pts[v].t[1],pts[v].t[0]),
            &d_e, &d_n_1, &d_t_1 
            );

        double d_n_2, d_t_2;
        dbgl_distance::projected_distance(
            pts[v].gama[0],  pts[v].gama[1], 
            p_v_reproj.gama[0],  p_v_reproj.gama[1], vcl_atan2(p_v_reproj.t[1],p_v_reproj.t[0]),
            &d_e, &d_n_2, &d_t_2 
            );

        if (d_n_1 < d_n_2) {
          dnormal_minus +=d_n_1*d_n_1;
          dnormal_plus  +=d_n_2*d_n_2;
        } else {
          dnormal_minus +=d_n_2*d_n_2;
          dnormal_plus  +=d_n_1*d_n_1;
        }

        if (d_t_1 < d_t_2) {
          dtangential_minus += d_t_1*d_t_1;
          dtangential_plus  += d_t_2*d_t_2;
        } else {
          dtangential_minus += d_t_2*d_t_2;
          dtangential_plus  += d_t_1*d_t_1;
        }
      }
    }
  }
  return one_true;
}

bool mw_algo_util::
dg_reprojection_error(
    vcl_vector<dbdif_3rd_order_point_2d> &pts, //:< pts[iv] points in view iv
    const vcl_vector<dbdif_camera> &cam,
    // no parameter v - do it for all v.
    double &dpos,
    double &dtheta,
    double &dnormal_plus,
    double &dnormal_minus,
    double &dtangential_plus,
    double &dtangential_minus,
    double &dk,
    double &dkdot,
    unsigned &n
    )
{
  assert(cam.size() >= 3 && pts.size() == cam.size());
  dpos=dtheta=dk=dkdot=dnormal_plus=dnormal_minus=dtangential_plus=dtangential_minus=0;
  n =0;
  bool one_true=false;

  for (unsigned v=0; v < cam.size(); ++v) {
    one_true = dg_reprojection_error(
        pts,cam,v,dpos,dtheta,dnormal_plus,dnormal_minus,
        dtangential_plus, dtangential_minus, dk,dkdot,n); 
  }

  return one_true;
}


void mw_algo_util::
extract_edgel_chain(const vsol_polyline_2d &pts, dbdet_edgel_chain *ec)
{
  // Move vsol polyline into vgl_vector of points
  vcl_vector<vgl_point_2d<double> > pts_vgl;
  pts_vgl.reserve(pts.size());
  for (unsigned i=0; i < pts.size(); ++i)
    pts_vgl.push_back(pts.vertex(i)->get_p());

  dbgl_csm(pts_vgl, 1, 1);

  dbsol_geno_curve_2d gc;

  vcl_vector<vsol_point_2d_sptr > pts_vsol;
  pts_vsol.reserve(pts.size());
  for (unsigned i=0; i < pts.size(); ++i)
    pts_vsol.push_back(pts.vertex(i));

  // dbgl_geno
  dbsol_geno::interpolate3_approx(&gc, pts_vsol, false);

  ec->edgels.resize(pts.size());
  // get_tangents
  for (unsigned i=0; i < pts.size(); ++i) {
    dbdet_edgel *e = new dbdet_edgel;
    e->pt = gc.point_at_sample(i)->get_p();
    e->tangent = gc.tangent_angle_at(gc.length_at(i));
    ec->edgels[i] = e;
  }
}
