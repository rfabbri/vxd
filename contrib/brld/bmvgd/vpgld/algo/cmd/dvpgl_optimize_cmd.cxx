//:
// \file


#include <vcl_vector.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>
#include <vnl/vnl_double_3x4.h>
#include <vnl/vnl_double_3x3.h>
#include <vnl/vnl_inverse.h>
#include <brct/brct_algos.h>
#include <vpgl/algo/vpgl_optimize_camera.h>
#include <vcl_iostream.h>
#include <vcl_fstream.h>



int main(int argc, char** argv)
{
  if(argc != 3){
    vcl_cout << "Usage: "<< argv[0] << " init_cam_file correspondence_file"<< vcl_endl;
    return -1;
  }

  //=========================== load the files ==========================
  vcl_ifstream istr(argv[1]);   
  vnl_double_3x4 cam_matrix;
  istr >> cam_matrix;
  if(istr.fail()){
    vcl_cerr << "Failed to read camera" << vcl_endl;
    return -1;
  }
  istr.close();

  istr.open(argv[2]);   
  vcl_vector<bool> valid;
  vcl_vector<vgl_point_2d<double> > i_pts;
  vcl_vector<vgl_point_3d<double> > w_pts;
  if(!brct_algos::read_target_corrs(istr, valid, i_pts, w_pts) )
  {
    vcl_cout << "Failed to read correspondences" << vcl_endl;
    return -1;
  }
  istr.close();


  vcl_vector<vgl_point_2d<double> > image_pts;
  vcl_vector<vgl_homg_point_3d<double> > world_pts;
  for( unsigned int i=0; i<valid.size(); ++i ){
    if(valid[i]){
      world_pts.push_back(vgl_homg_point_3d<double>(w_pts[i]));
      image_pts.push_back(i_pts[i]);
    }
  }

  //===========================  ==========================

  vcl_cout << "======== Initial Camera Matrix ========="<<vcl_endl;
  vcl_cout << cam_matrix  << vcl_endl;
  
  vpgl_perspective_camera<double> cam;
  if(!vpgl_perspective_decomposition(cam_matrix,cam)){
    vcl_cerr << "Could not decompose matrix" << vcl_endl;
  }

  // This is the known internal calibration calibration
  vpgl_calibration_matrix<double> K(2000.0,vgl_homg_point_2d<double>(512,384));
  cam.set_calibration(K);

  vpgl_perspective_camera<double> opt_cam = vpgl_optimize_camera::opt_orient_pos(cam,world_pts,image_pts);

  vcl_cout << "======== Original Camera Parameters ========="<<vcl_endl;
  vcl_cout << cam << vcl_endl;
  vcl_cout << "======== Optimized Camera Parameters ========="<<vcl_endl;
  vcl_cout << opt_cam << vcl_endl;

  vcl_cout << "======== Optimized Camera Matrix ========="<<vcl_endl;
  vcl_cout << opt_cam.get_matrix() << vcl_endl;
  return 0;
}

