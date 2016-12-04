#include <testlib/testlib_test.h>

#include <vcl_iostream.h> 
#include <vcl_cmath.h>
#include <vcl_limits.h>

#include <vgl/vgl_vector_2d.h>
#include <vgl/algo/vgl_h_matrix_3d.h>

#include <vnl/vnl_math.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vnl/vnl_vector_fixed.h>

#include <vsol/vsol_point_2d.h>

#include <dbdif/dbdif_camera.h>




static const double tolerance=vcl_numeric_limits<double>::epsilon()*100;

/*
static bool almost_equal3(const vgl_point_2d<double> &a, const vgl_point_2d<double> &b) {
  if (vcl_fabs(a.x()-b.x()) < tolerance && vcl_fabs(a.y()-b.y()) < tolerance) {
    return true;
  } else {
//    vcl_cerr << "Data about differing points" << vcl_endl;
//    vcl_cerr << "Point 1: "  << a << vcl_endl;
//    vcl_cerr << "Point 2: "  << b << vcl_endl;
    return false;
  }
}
*/

MAIN( test_camera )
{
  START ("Explicit camera");


  // Intrinsics:
  //

  vnl_double_3x3 m;

  double const x_over_y_ratio=0.9830;
  double const camera_pixel_size = 0.01168 ;

  //unused double const object_to_source = 121.00;
  double const camera_to_source = 161.00; 
  double rotation_step = 0.5; // (deg) artifact 2

  rotation_step *= (vnl_math::pi/180.0);

  double const optical_axis_line = 980.0; /*artifact 2*/
  double const nx = 4000; /* number of cols (artifact 2)*/
  //unused double const ny = 2096; /* number of rows (artifact 2)*/

  double ysize = camera_pixel_size/sqrt(x_over_y_ratio*x_over_y_ratio + 1);
  double xsize = x_over_y_ratio*ysize;

  // focal length and pixel unit conversion
  m[0][0]=camera_to_source/xsize;
  m[1][1]=camera_to_source/ysize;

  // principal points:
  m[0][2]=nx/2.0;
  m[1][2]=optical_axis_line;

  m[0][1]=0;   
  m[1][0]=0;   
  m[1][2]=0;
  m[2][0]=0;   
  m[2][1]=0;   
  m[2][2]=1;

  vpgl_calibration_matrix<double> K(m);


  // Extrinsics:

  vnl_matrix_fixed< double, 3, 3 > R;
  double theta=0;
  double r=130;

  R[0][0] = vcl_cos(theta);
  R[0][1] = 0;
  R[0][2] = vcl_sin(theta);

  R[1][0] = 0;
  R[1][1] = 1;
  R[1][2] = 0;

  R[2][0] = -vcl_sin(theta);
  R[2][1] = 0;
  R[2][2] = vcl_cos(theta);
  

  vgl_rotation_3d<double> rot(R);

  vgl_homg_point_3d<double> transl(-r*vcl_sin(theta), 0, -r*vcl_cos(theta));

  vpgl_perspective_camera<double> Pcam (K, transl, rot);

  dbdif_camera exp_cam;

  exp_cam.set_p(Pcam);

  exp_cam.print();
  vcl_cout << vcl_endl;

  SUMMARY();
}
