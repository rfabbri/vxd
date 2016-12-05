#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vidpro1/process/vidpro1_smoothcem_process.h>

#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_point_2d.h>

#include <vdgl/vdgl_digital_curve.h>
#include <vdgl/vdgl_edgel_chain.h>
#include <vdgl/vdgl_edgel.h>
#include <vdgl/vdgl_interpolator.h>

#include <bdgl/bdgl_curve_algs.h>

vidpro1_smoothcem_process::vidpro1_smoothcem_process() : bpro1_process()
{
  if(!parameters()->add(  "Sigma" , "-sigma" , (float)1.0 ) ||
     !parameters()->add(  "Number of times" , "-times" , 5 ))
  {
    vcl_cerr << "ERROR: Adding parameters in vidpro1_smoothcem_process::vidpro1_smoothcem_process()" << vcl_endl;
  }
}

//: Clone the process
bpro1_process*
vidpro1_smoothcem_process::clone() const
{
  return new vidpro1_smoothcem_process(*this);
}

vcl_vector< vcl_string > vidpro1_smoothcem_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

vcl_vector< vcl_string > vidpro1_smoothcem_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

bool vidpro1_smoothcem_process::execute()
{
  int times=0;
  float sigma=0;
  parameters()->get_value( "-times" , times );
  parameters()->get_value( "-sigma" , sigma );

  // create the input storage class
  vidpro1_vsol2D_storage_sptr input_vsol;
  input_vsol.vertical_cast(input_data_[0][0]);

  vcl_vector<vgl_point_2d<double> > curve;
  curve.clear();
  
  
  
    vcl_vector< vsol_spatial_object_2d_sptr > vsol_list = input_vsol->all_data();
    for (unsigned int b = 0 ; b < vsol_list.size() ; b++ ) {
        if( vsol_list[b]->cast_to_curve()->cast_to_polyline() ) {
          for (unsigned int i=0; i<vsol_list[b]->cast_to_curve()->cast_to_polyline()->size();i++){
            vsol_point_2d_sptr pt = vsol_list[b]->cast_to_curve()->cast_to_polyline()->vertex(i);
            curve.push_back(vgl_point_2d<double>(pt->x(), pt->y()));
          }
        }
     }

  vcl_cout << "curve size: " << curve.size() << vcl_endl;
  for (int i = 0; i<times; i++) {
    bdgl_curve_algs::smooth_curve(curve, sigma);
    vcl_cout << "curve size after smoothing: " << curve.size() << vcl_endl;

    curve.erase(curve.begin());
    curve.erase(curve.begin());
    curve.erase(curve.begin());

    curve.erase(curve.end()-1);
    curve.erase(curve.end()-1);
    curve.erase(curve.end()-1);

    vcl_cout << "curve size after erasing 6 points: " << curve.size() << vcl_endl;
  }

  // new vector to store the points
  vcl_vector< vsol_spatial_object_2d_sptr > contours;

  vcl_vector< vsol_point_2d_sptr > points; 
  for (unsigned int i = 0; i<curve.size(); i++) {
                                                //curve[i].x(), curve[i].y())
    vsol_point_2d_sptr newPt = new vsol_point_2d (curve[i].x(), curve[i].y());
    points.push_back(newPt);
  }
  vsol_polyline_2d_sptr newContour = new vsol_polyline_2d (points);
  contours.push_back(newContour->cast_to_spatial_object());
  
  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_vsol->add_objects(contours, "smoothed");
  output_data_[0].push_back(output_vsol);

  return true;
}
