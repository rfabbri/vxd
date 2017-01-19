
#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include "vpgld_load_camera_process.h"

#include <vcl_cstring.h>
#include <vcl_string.h>
#include <vcl_fstream.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vpgld/pro/vpgld_camera_storage.h>

#include <bmcsd/bmcsd_util.h>
#include <vpgld/io/vpgld_io_cameras.h>
#include <bdifd/algo/bdifd_data.h>

// camfiletype parameter:
#define INTRINSIC_EXTRINSIC 1 
#define PROJCAMERA  2 

static int file_num = -1;

vpgld_load_camera_process::vpgld_load_camera_process() : bpro1_process()
{
  if(  
      !parameters()->add( "Input file name" , "-nameprefix" , bpro1_filepath("","*.*")) ||
      !parameters()->add( "File type ASCII Camera matrix?" , "-camera_matrix_ftype" , false)  ||
      !parameters()->add( "File type ASCII Intrinsic/Extrinsic?" , "-intrinsic_extrinsic_ftype" , false)  ||
      !parameters()->add( "File type binary VSL (old vpgld)?" , "-vsl_ftype" , false)  ||
      !parameters()->add( "Multiple Intrinsic/Extrinsic Sequence?" , "-multi_intrextr" , true)  ||
      !parameters()->add( "     # of first file (integer from 0 to 499)" , "-multi_intrextr_first_file"     , 0 ) ||
      !parameters()->add( "     Reset numbering?" , "-multi_intrextr_reset" , false)  ||
      !parameters()->add( "     Prefix", "-multi_intrextr_prefix"     , vcl_string("frame_00") ) ||
      !parameters()->add( "Turntable ct?"   , "-turn_ct" , false ) ||
      !parameters()->add( "     # angle steps (integer multiple of 1 deg)" , "-angle"     , 0 ) ||
      !parameters()->add( "Turntable Taubin?"   , "-turn_olympus" , false ) ||
      !parameters()->add( "     Angle (degrees)" , "-angle_olympus"     , 0.0 )
   )
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
vpgld_load_camera_process::clone() const
{
  return new vpgld_load_camera_process(*this);
}

vcl_vector< vcl_string > vpgld_load_camera_process::get_input_type() 
{
  vcl_vector< vcl_string > to_return;
  return to_return;
}

vcl_vector< vcl_string > vpgld_load_camera_process::get_output_type() 
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vpgl camera" );
  return to_return;
}

//: Loads a _full_ perspective camera (not a pure vpgl_proj_camera)
bool vpgld_load_camera_process::execute()
{
  bool multi_intrextr=false;
  parameters()->get_value( "-multi_intrextr", multi_intrextr);

  bool turn_ct=false;
  parameters()->get_value( "-turn_ct", turn_ct);

  bool turn_olympus=false;
  parameters()->get_value( "-turn_olympus", turn_olympus);

  vpgl_perspective_camera<double> *P;

  if (turn_ct) {
    int angle=0;
    parameters()->get_value( "-angle" , angle);

    //: Viewport size; TODO get these from parameters
//    int nrows_ = 314;
    int ncols_ = 600;

    vnl_double_3x3 Kmatrix;
    bdifd_turntable::internal_calib_ctspheres(Kmatrix, ncols_);
    vpgl_calibration_matrix<double> K(Kmatrix);
    P = bdifd_turntable::camera_ctspheres(2*angle, K);
  } else if (turn_olympus) {
    double angle=0;
    parameters()->get_value( "-angle_olympus" , angle);

    vcl_cout <<"Angle: " << angle << vcl_endl; //: Viewport size; TODO get these from parameters 
    unsigned  crop_origin_x_ = 450; unsigned  crop_origin_y_ = 1750; vnl_double_3x3 Kmatrix;
    bdifd_turntable::internal_calib_olympus(Kmatrix, 500, crop_origin_x_, crop_origin_y_);
//    bdifd_turntable::internal_calib_olympus(Kmatrix);
    vpgl_calibration_matrix<double> K(Kmatrix);
    P = bdifd_turntable::camera_olympus(angle, K);
  } else if (multi_intrextr) {


    
    char file_num_s[4];

    bool multi_intrextr_reset;
    parameters()->get_value( "-multi_intrextr_reset", multi_intrextr_reset);
      
    if (file_num == -1 || multi_intrextr_reset) {
      int ini_file_num = 0;
      parameters()->get_value( "-multi_intrextr_first_file", ini_file_num);
      if (ini_file_num > 499) {
        vcl_cout << "initial file number outside bounds.\n";
        return false;
      }
      file_num = ini_file_num;
    } else 
      ++file_num;

    char num2char[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    int i0 = file_num/100;
    int i1 = ((file_num-file_num%10)/10)%10;
    int i2 = file_num%10;

    vcl_cout << "File number " << i0 << i1 << i2 << vcl_endl;

    file_num_s[0] = num2char[i0];
    file_num_s[1] = num2char[i1];
    file_num_s[2] = num2char[i2];
    file_num_s[3] = '\0';
           
    bpro1_filepath input;
    parameters()->get_value( "-nameprefix" , input);
    vcl_string input_file = input.path;
    vcl_string mypath(input_file + vcl_string("/"));


    if (input_file.empty())
      mypath = "/home/rfabbri/work/may9/multi_intrextr/right-dome/all-frames/calib-opt-500/";

    vcl_cout << "Reading multi_intrextr cameras from " << mypath << vcl_endl;

    vcl_string prefix;
    parameters()->get_value("-multi_intrextr_prefix", prefix);
    vcl_string suffix(".anything");

    vcl_cout << "File number (vcl_string)" << file_num_s << vcl_endl;

    vpgl_perspective_camera<double> cam;
    if (! read_cam(mypath + prefix + vcl_string(file_num_s) + suffix, &cam) ) 
       return false;
    P = new vpgl_perspective_camera<double>(cam);

  } else {
    bpro1_filepath input;
    parameters()->get_value( "-nameprefix" , input);
    vcl_string input_file = input.path;


    //unused int cam_type;
    bool vsl_ftype=false, intrinsic_extrinsic_ftype=false, camera_matrix_ftype=false;
    parameters()->get_value( "-vsl_ftype" , vsl_ftype);
    parameters()->get_value( "-intrinsic_extrinsic_ftype" , intrinsic_extrinsic_ftype);
    parameters()->get_value( "-camera_matrix_ftype" , camera_matrix_ftype);


    vpgl_perspective_camera<double> cam;


    if (intrinsic_extrinsic_ftype) {

      vcl_cout << "Selected type: " << "ASCII intrinsic/extrinsic" << vcl_endl;
      if (! read_cam(input.path,&cam) )
         return false;
    } else if (camera_matrix_ftype) {
      vcl_cout << "Selected type: " << "ASCII camera matrix" << vcl_endl;
      if (! read_3x4_matrix_into_cam(input.path,&cam) )
         return false;
    } else if (vsl_ftype){
      vcl_cout << "Selected type: " << "VSL binary OLD format" << vcl_endl;
      vsl_b_ifstream bp_in(input.path);
      if (!bp_in) {
        vcl_cerr << "ERROR: couldn't open file: " << input.path << vcl_endl;
        return false;
      }
      b_read_vpgld(bp_in, &cam);
      bp_in.close();
    } else {
      vcl_cerr << "Error: no camera type selected!\n";
      return false;
    }
    
    P = new vpgl_perspective_camera<double>(cam);
  }

  clear_output();

  vpgld_camera_storage_sptr 
     cam_storage = vpgld_camera_storage_new();
  
  cam_storage->set_camera(P);

  output_data_[0].push_back(cam_storage);
  
  if (cam_storage->get_camera()->type_name() != "vpgl_perspective_camera")
    vcl_cerr << "Error: load camera requires perspective camera\n";

  const vpgl_perspective_camera<double> *psp_cam = 
    static_cast<const vpgl_perspective_camera<double>*>(cam_storage->get_camera());

  vcl_cout << "Camera (process): \n" << *psp_cam;

  return true;
}
