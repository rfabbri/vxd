//:
// \file

#include "bmcsd_project_curve_sketch_process.h"

#include <vcl_vector.h>
#include <vcl_string.h>
#include <bdifd/algo/bdifd_data.h>
#include <vpgl/vpgl_perspective_camera.h>


#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <bvis1/bvis1_manager.h>

#include <vpgld/pro/vpgld_camera_storage.h>

#include <vsol/vsol_point_3d.h>
#include <vsol/vsol_point_3d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_polyline_2d.h>
#include <bmcsd/bmcsd_util.h> 
#include <bmcsd/bmcsd_curve_3d_sketch.h>
#include <vpgld/io/vpgld_io_cameras.h>



//: Constructor
bmcsd_project_curve_sketch_process::bmcsd_project_curve_sketch_process()
{
  if( 
      !parameters()->add("Path", "-curvesketch", bpro1_filepath("./","*.*")) ||
      !parameters()->add("Total support threshold", "-total_support", 100.0) ||
      !parameters()->add("Only project inliers", "-inliers", false) ||
      !parameters()->add("Project certain curve", "-onecurve", -1)
    ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
bmcsd_project_curve_sketch_process::~bmcsd_project_curve_sketch_process()
{
}


//: Clone the process
bpro1_process*
bmcsd_project_curve_sketch_process::clone() const
{
  return new bmcsd_project_curve_sketch_process(*this);
}


//: Return the name of this process
vcl_string
bmcsd_project_curve_sketch_process::name()
{
  return "Project Curve Sketch";
}


//: Return the number of input frame for this process
int
bmcsd_project_curve_sketch_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
bmcsd_project_curve_sketch_process::output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > bmcsd_project_curve_sketch_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vpgl camera" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > bmcsd_project_curve_sketch_process::get_output_type()
{
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

#define MANAGER bvis1_manager::instance()

//: Execute the process
bool
bmcsd_project_curve_sketch_process::execute()
{
  clear_output();

  // get camera matrix

  vpgld_camera_storage_sptr cam_storage;

  cam_storage.vertical_cast(input_data_[0][0]);

  const vpgl_perspective_camera<double> *pcam = 
    vpgld_cast_to_perspective_camera(cam_storage->get_camera());
  if(!pcam) {
    vcl_cerr << "Error: process requires a perspective camera" << vcl_endl;
    return false;
  }

  vcl_cout << "NAME: " << cam_storage->name() << vcl_endl;
  vcl_cout << "Camera: \n" << pcam->get_matrix();

  const vpgl_perspective_camera<double> &cam = *pcam;


  bmcsd_curve_3d_sketch *csk = new bmcsd_curve_3d_sketch;

  bpro1_filepath input;
  parameters()->get_value("-curvesketch", input);
  vcl_string csk_fname = input.path;

  vcl_cout << "Reading curve sketch at " << csk_fname << vcl_endl;

  bool retval  = csk->read_dir_format(csk_fname);
  if (!retval) {
    vcl_cerr << "Error reading 3D curve sketch in current directory.";
  }

  // prune
  double tau_total_support;
  parameters()->get_value("-total_support", tau_total_support);

  vcl_cout << "Pruning " << csk->num_curves() << 
    " curves with cost < " << tau_total_support << vcl_endl;
  csk->prune_by_total_support(tau_total_support);
  vcl_cout << "Pruned down to " << csk->num_curves() << " curves" << vcl_endl;

  bool inliers;
  parameters()->get_value("-inliers", inliers);

  int onecurve;
  parameters()->get_value("-onecurve", onecurve);

  // project curves

  vcl_vector<vsol_spatial_object_2d_sptr> poly;

  for (unsigned i=0; i < csk->num_curves(); ++i) {
    if (onecurve != -1 && i != onecurve)
      continue;

    if (inliers) {
      unsigned f = MANAGER->current_frame();

//      if (f != csk->attributes()[i].v_->stereo0() &&
//          f != csk->attributes()[i].v_->stereo1()) {

        vcl_vector<unsigned>::const_iterator it = vcl_find(
            csk->attributes()[i].inlier_views_.begin(), 
            csk->attributes()[i].inlier_views_.end(),
            f);

        if (it == csk->attributes()[i].inlier_views_.end())
          continue;
//      }
    }
    vcl_cout << "Including curve id: " << i <<  " with length " << csk->curves_3d()[i].size() << vcl_endl;

    vcl_vector<vsol_point_2d_sptr> pts(csk->curves_3d()[i].size());

    for (unsigned k=0; k < pts.size(); ++k) {
      vgl_point_3d<double> p(
          csk->curves_3d()[i][k].Gama[0],
          csk->curves_3d()[i][k].Gama[1], 
          csk->curves_3d()[i][k].Gama[2]);
      pts[k] = new vsol_point_2d(vgl_point_2d<double>( cam.project(p) ));
    }

    poly.push_back(new vsol_polyline_2d (pts));
  }


  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_vsol->add_objects(poly, "curve_sketch");
  output_data_[0].push_back(output_vsol);

  return true;
}

bool
bmcsd_project_curve_sketch_process::finish()
{
  return true;
}

