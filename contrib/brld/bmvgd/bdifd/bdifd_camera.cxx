#include "dbdif_camera.h"

void dbdif_camera::
compute_explicit_params()
{
  // Cache calib matrix as vnl_matrix
  K_ = Pr_.get_calibration().get_matrix();

  k_a_ = K_(0,0);
  k_b_ = K_(1,1);

  assert (k_a_ && k_b_);

  k_inv_00_ = 1./k_a_;
  k_inv_11_ = 1./k_b_;
  k_inv_01_ = -K_(0,1)/(k_a_*k_b_);

  // Cache Rotations
  vnl_matrix_fixed<double,4,4> Raux;

  Pr_.get_rotation().as_h_matrix_3d().get(&Raux);

  for (unsigned i=0; i<3; ++i) for (unsigned j=0; j<3; ++j) {
     Rot(i,j)   = Raux(i,j);
     Rot_t(i,j) = Raux(j,i);
  }
  
  // Cache camera center as vnl vector
  vgl_point_3d<double> aux_pt;
 
  aux_pt = Pr_.get_camera_center();
  c[0] = aux_pt.x(); c[1] = aux_pt.y(); c[2] = aux_pt.z(); 

  b1_base_e =     Rot_t * unit_x; // 1st column of Rot_t
  b2_base_e =     Rot_t * unit_y; // 2nd column of Rot_t
  b3_base_e = F = Rot_t * unit_z; // 3rd column of Rot_t

//  b1_base_e.normalize();
//  b2_base_e.normalize();
//  b3_base_e.normalize();
//  F = b3_base_e;
}


dbdif_vector_2d  dbdif_camera::
project_to_image(const dbdif_vector_3d &Gamma) const
{ 

  vgl_homg_point_3d<double> point3d(Gamma(0),Gamma(1), Gamma(2));

  vgl_homg_point_2d<double> point2d = Pr_.project(point3d);

  if (point2d.ideal(1e-10)) {
    vcl_cout << "Warning: dbdif_camera::project_to_image : infinite projection\n";
    vcl_cout << "         point3d: " << point3d << "; point2d: " << point2d << vcl_endl;
  }

  dbdif_vector_2d p2dv;

  p2dv[0] = point2d.x()/point2d.w();
  p2dv[1] = point2d.y()/point2d.w();

  return p2dv;
}

bool dbdif_camera::
project_1st_order_to_image(
    const dbdif_1st_order_point_3d &p3d, 
    dbdif_1st_order_point_2d *pimg_ptr) const
{
  dbdif_1st_order_point_2d &pimg = *pimg_ptr;

  // FIXME, hack, just running 3rd order code and using only 1st order results
  dbdif_3rd_order_point_3d Prec;

  Prec.Gama = p3d.Gama;
  Prec.T = p3d.T;
  Prec.valid = p3d.valid;

  // theta1 = theta(view3) 
  bool valid;
  pimg = project_to_image(Prec, &valid);
  return valid && Prec.valid;
}

//: returns projected 3rd order gemetry in *image* coordinates
//\param[out] stat : false if some geometrical degeneracy occurred. In this
//case, differential-geometric results are undefined.
//
dbdif_3rd_order_point_2d dbdif_camera::
project_to_image(const dbdif_3rd_order_point_3d &p3d, bool *stat) const
{
  *stat=true;

  dbdif_3rd_order_point_2d pimg;

  //: Project point
  dbdif_vector_2d uv = project_to_image(p3d.Gama);

  pimg.gama[0] = uv[0];
  pimg.gama[1] = uv[1];
  pimg.gama[2] = 0;

  //: Project tangent + normal
  dbdif_3rd_order_point_2d pw; //:< normalized image plane geometry in world coordinates

  *stat = project_3rd(p3d, &pw);

  pimg.valid = pw.valid;

  dbdif_vector_3d t_pcam;
  world_to_cam_vector(pw.t, &t_pcam);

  dbdif_vector_3d n_pcam_3dconv;
  world_to_cam_vector(pw.n, &n_pcam_3dconv);

  // sign: we now have 3rd component 0, so we transition to 2D convention of n being
  // anti-cw rotation:
  dbdif_vector_3d n_pcam;
  n_pcam[0] = -t_pcam[1];
  n_pcam[1] =  t_pcam[0];
  n_pcam[2] =  0;

  double k_pcam = pw.k;
  double kdot_pcam = pw.kdot;

  if (n_pcam_3dconv[0]*n_pcam[0] < 0) {
    k_pcam = -k_pcam;
    kdot_pcam = -kdot_pcam;
  }

#ifndef NDEBUG
  if (!dbdif_util::near_zero(dot_product(n_pcam, t_pcam),1e-6))
    if (dbdif_util::near_zero(dot_product(pw.n, pw.t),1e-6))
      vcl_cout << ">>> Pau em WORLD TO CAM vector! " << dot_product(pw.n, pw.t) <<"\n";
#endif

  dbdif_vector_2d t_pcam_2; 
  t_pcam_2[0] = t_pcam[0];
  t_pcam_2[1] = t_pcam[1];

  dbdif_vector_2d n_pcam_2; 
  n_pcam_2[0] = n_pcam[0];
  n_pcam_2[1] = n_pcam[1];


  vnl_matrix_fixed<double,2,2> L;

  L(0,0) = K_(0,0);
  L(0,1) = K_(0,1);
  L(1,1) = K_(1,1);
  L(1,0) = 0;

  dbdif_vector_2d timg_2, nimg_2;

  bool ret = dbdif_frenet::linear_transform(n_pcam_2, t_pcam_2, k_pcam, kdot_pcam, &(pimg.k), &(pimg.kdot),
      &timg_2, &nimg_2, L);

//  if (!dbdif_util::near_zero((Rot*timg_2 - pimg.t).two_norm(), 1e-8))
//    vcl_cout << "Linear transform invalid in project to image\n";

  *stat = *stat && ret;

  pimg.valid = pimg.valid && ret;

  pimg.t[0] = timg_2[0];
  pimg.t[1] = timg_2[1];
  pimg.t[2] = 0;

  pimg.n[0] = nimg_2[0];
  pimg.n[1] = nimg_2[1];
  pimg.n[2] = 0;
  return pimg;
}

//: project 3D 2nd order intrinsics into image1 
// 2d frame1 is still in world coodinates
bool dbdif_camera::
project_k(const dbdif_2nd_order_point_3d &Frame, dbdif_2nd_order_point_2d *frame1) const
{
  double lambda_dummy;
  return project_k(Frame, frame1, &lambda_dummy);
}

bool dbdif_camera::
project_k(const dbdif_2nd_order_point_3d &Frame, dbdif_2nd_order_point_2d *frame1, double *plambda) const
{
  bool retval=true;
  // Compute gama

  dbdif_vector_3d Gama_i = Frame.Gama - c;

  double lambda = *plambda = dot_product(Gama_i,F);
  if (dbdif_util::near_zero(lambda,1e-8)) {
    frame1->valid = false;
    retval = false;
  }
  frame1->gama = Gama_i/lambda;

  double g = speed(Frame,lambda);
  if (dbdif_util::near_zero(g,1e-8)) {
    frame1->valid = false;
    retval=false;
  }

  project_t(Frame.T, frame1->gama, &(frame1->t));
  frame1->t.normalize();

  frame1->n = vnl_cross_3d(frame1->t,F);

#ifndef NDEBUG
  if (!dbdif_util::near_zero(dot_product(frame1->n,frame1->t))) {
    vcl_cerr << "Cross product imprecision\n";
  }
#endif

  if (dbdif_util::near_zero(g*g*lambda,1e-8)) {
    frame1->valid = false;
    retval=false;
  }

  frame1->k = dot_product (frame1->n, (Frame.N - dot_product(Frame.N,F)*frame1->gama)/(lambda*g*g) );
  frame1->k *= Frame.K;

  // sign correction: we are still in 3D world (although vectors are in a
  // plane), so the convention is k > 0. What matters is that n*k  remains
  // unchanged.

  if (frame1->k < 0) {
    frame1->k = -frame1->k;
    frame1->n = -frame1->n;
  }

  return retval;
}

//: Derivative of speed of image curve wrt. arclenght of space curve
double dbdif_camera::
tangential_accel(const dbdif_2nd_order_point_3d *Frm, double lambda, double lambda_dot, double g, const dbdif_vector_3d &t) const
{
  double dg;

  // Formula (3.42) in subm. paper
  dg = dot_product ((Frm->N - dot_product(Frm->N,F)*project(Frm->Gama - c))  ,  t);

  dg = (dg*Frm->K - 2*g*lambda_dot)/lambda;

  return dg;
}

//: project 3rd order measures. Result is still in world coordinates & units
bool dbdif_camera::
project_3rd(const dbdif_3rd_order_point_3d &Fr, dbdif_3rd_order_point_2d *frame1) const
{
  bool retval=true;
  double lambda;
  // get 2nd order
  if (!project_k(Fr,frame1, &lambda))
    retval=false;

  // now 3rd

  if (frame1->k == 0) {
    frame1->kdot = 0;
    return retval;
  }

  double g = speed(Fr,lambda);
  double lambda_dot = dot_product(Fr.T,F);
  double gprime = tangential_accel(&Fr,lambda, lambda_dot,g,frame1->t);

  dbdif_vector_3d U = vnl_cross_3d(frame1->gama,frame1->t);
  dbdif_vector_3d V = Fr.Kdot * Fr.N  +  Fr.K * Fr.Tau * Fr.B;
  frame1->kdot   = dot_product(V,U);
  frame1->kdot  /= lambda*g*g*g*dot_product(frame1->n,U);
  frame1->kdot  -= 3*frame1->k*( lambda_dot/(lambda*g)  + gprime/(g*g));
  return retval;
}

void dbdif_camera::
print(vcl_ostream &os) const
{
//  os << Pr;

  os << "Rot:\n";
  Rot.print(os);

  os << "Rot_t:\n";
  Rot_t.print(os);

  os << "c:\n";
  c.print(os);

  os << "b1_base_e:\n";
  b1_base_e.print(os);
  os << "b2_base_e:\n";
  b2_base_e.print(os);
  os << "b3_base_e:\n";
  b3_base_e.print(os);

  os << "F:\n";
  F.print(os);
}

