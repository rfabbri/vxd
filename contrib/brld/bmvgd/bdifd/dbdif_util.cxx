#include "dbdif_util.h"

#include <vsol/vsol_point_2d.h>
#include <vcl_cstring.h>
#include <vnl/vnl_vector_fixed.h>
#include <vgl/algo/vgl_homg_operators_2d.h>
#include <vcl_sstream.h>
#include <vcl_fstream.h>

//: transfers a point to a 3rd image given p1,p1,F13 and F23. It will intersect
// the epipolar lines l13 and l23 on the 3rd image, returning p3.
// \todo move to vpgl/algo
vgl_homg_point_2d<double>
dbdif_epipolar_point_transfer( 
      const vgl_homg_point_2d<double> &p1, 
      const vgl_homg_point_2d<double> &p2, 
      const vpgl_fundamental_matrix<double> &f13,
      const vpgl_fundamental_matrix<double> &f23)
{
   vgl_homg_point_2d<double> p3; 
   vgl_homg_line_2d<double> l13,l23;

   l13 = f13.l_epipolar_line(p1);
   l23 = f23.l_epipolar_line(p2);

   p3 = vgl_homg_operators_2d<double>::intersection(l13,l23);

   return p3;
}
/*
bool
reconstruct_pt_tangents(
      // Input:
      dbsol_interp_curve_2d *curve1, 
      dbsol_interp_curve_2d *curve2,
      const vnl_matrix <double> &s,  // nx2 w/ pairs of arc lengths of pts to reconstruct
      const vpgl_perspective_camera <double> &Pr1,
      const vpgl_perspective_camera <double> &Pr2,
      // Output: 
      vcl_vector<vsol_point_2d_sptr> *Gamas,
      vcl_vector<double> *error, // distance btw two rays in 3D
      vcl_vector<vgl_vector_3d<double>> *Ts
      )
      */

/* Doing dbdif_rig ...
bool
reconstruct_pt_tangents(
      // Input:
      dbsol_interp_curve_2d *curve1, 
      dbsol_interp_curve_2d *curve2,
      double s1p, double s2p, double s1, double s2,
      const vpgl_perspective_camera <double> &Pr1,
      const vpgl_perspective_camera <double> &Pr2,
      // Output (caller's storage; we do not alloc em here): 
      double vnl_vector_fixed<double,3> *Gama_s,
      double vnl_vector_fixed<double,3> *Gama_sp,
      vcl_vector<vgl_vector_3d<double>> *T_s,
      vcl_vector<vgl_vector_3d<double>> *T_sp,
      double *error_s1, // distance btw two rays in 3D
      double *error_s2, // distance btw two rays in 3D
      )
{
   vnl_matrix_fixed<double,4,4> Rc1;
   vnl_matrix_fixed<double,3,3> Rct1, Rct2;

   Pr1.get_rotation_matrix().get(&Rc1);
   Pr2.get_rotation_matrix().get(&Rc2);

   // transpose and reduce to 3x3:
   for (unsigned ii=0; ii<3; ++ii) for (unsigned j=0; j<3; ++j) {
      Rct1(ii,j) = Rc1(j,ii);
      Rct2(ii,j) = Rc2(j,ii);
   }
   
   // Write things in 3D world coordinate system
   vgl_point_3d<double> c1_pt = Pr1.get_camera_center();
   
   vnl_vector_fixed<double,3> 
      c1(c1_pt.x(),c1_pt.y(),c1_pt.z()),
      e11, e12, e13, gama1, F1,   //:< world coordinates
      e11_cam(1,0,0), e12_cam(0,1,0), e13_cam(0,0,1); //:< camera coordinates

   e11 = Rct*e11_cam;
   e12 = Rct*e12_cam;
   e13 = F1 = Rct*e13_cam;
   
   double x_scale, y_scale, u, v;
   
   x_scale = Pr1.get_calibration().x_scale();
   y_scale = Pr1.get_calibration().y_scale();



   unsigned i;
   for (i=0; i < s.rows(); ++i) {

      
      vsol_point_2d p1 = curve1.point_at(s(i,1));
      vsol_point_2d p2 = curve1.point_at(s(i,2));

      u = p1.x();
      v = p1.y();
      
      vgl_point_2d<double> uv0 = Pr1.get_calibration().principal_point();
      
      gama1 = ((u - uv0.x())/x_scale)*e11 + ((v - uv0.y())/y_scale)*e12  + e13;

      // Camera 2
      vgl_point_3d<double> c2_pt = Pr2.get_camera_center();
      
      vnl_vector_fixed<double,3> 
         c2(c2_pt.x(),c2_pt.y(),c2_pt.z()), //:< world coordinates
         e21, e22, e23, gama2, F2, //:< camera coordinates
         e21_cam(1,0,0), e22_cam(0,1,0), e23_cam(0,0,1);
      
      e21 = Rct2*e21_cam;
      e22 = Rct2*e22_cam;
      e23 = F2 = Rct2*e23_cam;
      
      x_scale = Pr2.get_calibration().x_scale();
      y_scale = Pr2.get_calibration().y_scale();
      u = p2.x();
      v = p2.y();
      
      uv0 = Pr2.get_calibration().principal_point();
      
      gama2 = ((u - uv0.x())/x_scale)*e21 + ((v - uv0.y())/y_scale)*e22  + e23;

      // At this pt we have, for each camera:  gama, F, bases of camera system
      // written in world coordinates, and 

      // Least squares pt reconstr.
      vnl_matrix_fixed<double,3,2> A;
    
      for (unsigned i=0; i<3; ++i)
         A(i,0) = gama1(i);
    
      for (unsigned i=0; i<3; ++i)
         A(i,1) = -gama2(i);
    
      vnl_svd<double> svd(A);
      vnl_vector<double> lambda = svd.solve(c2-c1);
    
      vcl_cout << "Lambda:\n" << lambda << vcl_endl;
      // the error is:   (A*lambda +c1 - c2).two_norm()
    
      vnl_vector_fixed<double,3> Cpt_v = c1 + lambda(0)*gama1;
      vgl_homg_point_3d<double> Cpt(Cpt_v(0), Cpt_v(1), Cpt_v(2));
      vcl_cout << "Reconstructed point: " << Cpt << vcl_endl;

     //=========== Tangents
      vcl_cout << "\n\n\n";
      vcl_cout << "================= Tangent reconstruction: =======================" << vcl_endl;
    
      // Camera 1:
      vnl_vector_fixed<double,3> t1_cam_bkwd;
      vnl_vector_fixed<double,3> t1_world_bkwd;
    
      t1_cam_bkwd[0] = t1_img[0]/x_scale;   //:< take off streching;
      t1_cam_bkwd[1] = t1_img[1]/y_scale;
      t1_cam_bkwd[2] = 0;
      t1_cam_bkwd.normalize();
    
      t1_world_bkwd = Rct*t1_cam_bkwd;
    
      // Camera 2:
      vnl_vector_fixed<double,3> t2_cam_bkwd;
      vnl_vector_fixed<double,3> t2_world_bkwd;
      vnl_vector_fixed<double,3> T_rec;
    
      t1_cam_bkwd[0] = t2_img[0]/x_scale;   //:< take off streching;
      t1_cam_bkwd[1] = t2_img[1]/y_scale;
      t2_cam_bkwd[2] = 0;
      t2_cam_bkwd.normalize();
    
      t2_world_bkwd = Rct2*t2_cam_bkwd;
    
      vcl_cout << "Test t1 dot F1 zero: " << dot_product(t1_world_bkwd,F1) << vcl_endl << vcl_endl;
      vcl_cout << "Test t2 dot F2 zero: " << dot_product(t2_world_bkwd,F2) << vcl_endl << vcl_endl;
    
      T_rec = vnl_cross_3d( vnl_cross_3d(t1_world_bkwd,gama1), vnl_cross_3d(t2_world_bkwd,gama2) );

   }

   return true;
}
*/
