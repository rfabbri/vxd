// Mar 03, 2004  dbgl/dbgl_rigid_transform.cxx
// MingChing Chang

#include <vcl_iostream.h>
#include <stdio.h>
#include <vcl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_svd.h>

#include <dbgl/dbgl_rigid_align.h>

//: Compute the best rigid transformation that minimizing Euclidean distance.
//  Rotate and Translate mov_pts to target_pts that best matches them.
//  Assume that number of points in the two clouds are equal.
//
//Input: match points of mov_pt=(mx_i, my_i, mz_i) to target_pt=(tx_i, ty_i, tz_i)
//
//Output: The rotation matrix R
//        The centroid of target_pts: Cf = (tcx, tcy, tcz), and
//        The centroid of moving_pts: Cm = (mcx, mcy, mcz)
//        The shifted version of input cor_fixPS and cor_movPS to their object centers.
//
//Usage: for any point in P2 space,
//       1)subtract Cm
//       2)multiple by the rotation matrix R
//       3)add by Cf, you get the point in P in P1 space
//
void dbgl_get_rigid_alignment (vcl_vector<vgl_point_3d<double> >& cor_fixPS,
                               vcl_vector<vgl_point_3d<double> >& cor_movPS,
                               vnl_matrix_fixed<double,3,3>& R, 
                               vnl_vector_fixed<double,3>& Cf, 
                               vnl_vector_fixed<double,3>& Cm)
{
  assert (cor_fixPS.size() == cor_movPS.size());
  int N = cor_fixPS.size();

  //1)Compute Centroid
  double c1[3] = {0,0,0};
  double c2[3] = {0,0,0};
  for (int i=0; i<N; i++) {
    c1[0] += cor_fixPS[i].x();
    c1[1] += cor_fixPS[i].y();
    c1[2] += cor_fixPS[i].z();

    c2[0] += cor_movPS[i].x();
    c2[1] += cor_movPS[i].y();
    c2[2] += cor_movPS[i].z();
  }
  c1[0] /= N;
  c1[1] /= N;
  c1[2] /= N;

  c2[0] /= N;
  c2[1] /= N;
  c2[2] /= N;

  Cf.set (c1);
  Cm.set (c2);

  //2)Centering the data
  for (int i=0; i<N; i++) {
    cor_fixPS[i].set (cor_fixPS[i].x() - Cf.get(0),
                      cor_fixPS[i].y() - Cf.get(1),
                      cor_fixPS[i].z() - Cf.get(2));

    cor_movPS[i].set (cor_movPS[i].x() - Cm.get(0),
                      cor_movPS[i].y() - Cm.get(1),
                      cor_movPS[i].z() - Cm.get(2));
  } 

  //3)Computing covariance matrix
  double H[9] = {0,0,0, 0,0,0, 0,0,0};
   for (int i=0; i<N; i++) {
    H[0] += cor_movPS[i].x()*cor_fixPS[i].x();
    H[1] += cor_movPS[i].x()*cor_fixPS[i].y();
    H[2] += cor_movPS[i].x()*cor_fixPS[i].z();
    H[3] += cor_movPS[i].y()*cor_fixPS[i].x();
    H[4] += cor_movPS[i].y()*cor_fixPS[i].y();
    H[5] += cor_movPS[i].y()*cor_fixPS[i].z();
    H[6] += cor_movPS[i].z()*cor_fixPS[i].x();
    H[7] += cor_movPS[i].z()*cor_fixPS[i].y();
    H[8] += cor_movPS[i].z()*cor_fixPS[i].z();
  }

  //4)Computing svd
  vnl_matrix<double> M (H, 3, 3);
  vnl_svd<double> svd (M, 1e-10);
  vnl_matrix<double> U;
  vnl_matrix<double> Ut;
  vnl_matrix<double> V;

  U=svd.U();
  V=svd.V();
  Ut=U.transpose();
  R = V*Ut;  //R is the rotation matrix

  #if DBMSH3D_DEBUG>2
  print_R_C1_C2 (R, Cf, Cm);
  #endif
  //The return value are in R, Cf, and Cm
}

void print_R_C1_C2 (const vnl_matrix_fixed<double,3,3>& R, 
                    const vnl_vector_fixed<double,3>& Cf, 
                    const vnl_vector_fixed<double,3>& Cm)
{
  //: print R, Cf and Cm
  char buffer[128];
  vcl_cout<< "R = \n";
  for (int i=0; i<3; i++) {
    sprintf (buffer, "[%+2.5f, %+2.5f, %+2.5f]\n", R.get(i,0), R.get(i,1), R.get(i,2));
    vcl_cout<< buffer;
  }
  sprintf (buffer, "Cf = (%+2.5f, %+2.5f, %+2.5f)\n", Cf.get(0), Cf.get(1), Cf.get(2));
  vcl_cout<< buffer;
  sprintf (buffer, "Cm = (%+2.5f, %+2.5f, %+2.5f)\n", Cm.get(0), Cm.get(1), Cm.get(2));
  vcl_cout<< buffer;
}

//Input: P, R, Cf, Cm
//Output: modified P (call by reference).
void transform_point_3d (const vnl_matrix_fixed<double,3,3>& R, 
                         const vnl_vector_fixed<double,3>& Cf, 
                         const vnl_vector_fixed<double,3>& Cm,
                         double& px, double& py, double& pz)
{
  double p[3];

  p[0] = (double) px - Cm.get(0);
  p[1] = (double) py - Cm.get(1);
  p[2] = (double) pz - Cm.get(2);
  vnl_vector_fixed<double,3> Point (p);
  vnl_vector_fixed<double,3> PointP;
  PointP=R*Point;

  p[0] = PointP.get(0);
  p[1] = PointP.get(1);
  p[2] = PointP.get(2);

  p[0] += Cf.get(0);
  p[1] += Cf.get(1);
  p[2] += Cf.get(2);

  px = (double) p[0];
  py = (double) p[1];
  pz = (double) p[2];
}

//Input: Pt, R, Cf, Cm
//Return transformed Pt
void transform_point_3d (const vnl_matrix_fixed<double,3,3>& R, 
                         const vnl_vector_fixed<double,3>& Cf, 
                         const vnl_vector_fixed<double,3>& Cm,
                         vgl_point_3d<double>& Pt)
{
  double x = Pt.x();
  double y = Pt.y();
  double z = Pt.z();
  transform_point_3d (R, Cf, Cm, x, y, z);
  Pt.set (x, y, z);
}


