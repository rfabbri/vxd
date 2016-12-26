// This is basic/dbsol/pro/dbsol_smooth_contours_process.cxx

//:
// \file


#include "dbsol_smooth_contours_process.h"
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polygon_2d_sptr.h>
//
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <bdgl/bdgl_curve_algs.h>
#include <dbgl/algo/dbgl_curve_smoothing.h>



//: Constructor
dbsol_smooth_contours_process::
dbsol_smooth_contours_process(void)
{
  if( 
    !parameters()->add("Gaussian smoothing?", "-gaussian", (bool) true ) ||
    !parameters()->add("    Gaussian sigma ", "-sigma", 1.0f ) ||
    !parameters()->add("Discrete Curvature Smoothing?", "-discrete_csm", false) ||
    !parameters()->add("    Stepsize (psi) ", "-psi", 1.0f ) ||
    !parameters()->add("Number of times " , "-num_times" , (unsigned)1 )||
    !parameters()->add("In-place smoothing (no new storage will be created)" , "-in_place_smoothing" , false)
    ) 
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
dbsol_smooth_contours_process::
~dbsol_smooth_contours_process()
{
}


//: Clone the process
bpro1_process* dbsol_smooth_contours_process::
clone() const
{
  return new dbsol_smooth_contours_process(*this);
}


//: Return the name of this process
vcl_string dbsol_smooth_contours_process::
name()
{
  return "Smooth Contours";
}


//: Return the number of input frame for this process
int dbsol_smooth_contours_process::
input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int dbsol_smooth_contours_process::
output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > dbsol_smooth_contours_process::
get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > dbsol_smooth_contours_process::
get_output_type()
{
  vcl_vector<vcl_string > to_return;
   bool in_place_smoothing = false;
  this->parameters()->get_value("-in_place_smoothing", in_place_smoothing);
  if (!in_place_smoothing)
    to_return.push_back("vsol2D");
  return to_return;
}



//: Execute the process
bool dbsol_smooth_contours_process::
execute()
{
  if ( input_data_.size() != 1 )
  {
    vcl_cout << "In dbsol_smooth_contours_process::execute() - not exactly 1"
             << " input frames \n";
    return false;
  }
  this->clear_output();


  // extract proces parameters
  
  bool gaussian = false;
  this->parameters()->get_value("-gaussian", gaussian);

  float sigma = 0;
  this->parameters()->get_value("-sigma", sigma);
  
  bool discrete_csm;
  this->parameters()->get_value("-discrete_csm", discrete_csm);

  float psi;
  this->parameters()->get_value("-psi", psi);

  unsigned num_times = 0;
  this->parameters()->get_value("-num_times", num_times);

  bool in_place_smoothing = false;
  this->parameters()->get_value("-in_place_smoothing", in_place_smoothing);

  // get vsol2D object from the storage class
  vidpro1_vsol2D_storage_sptr frame_vsol;
  frame_vsol.vertical_cast(input_data_[0][0]);
  vcl_vector<vsol_spatial_object_2d_sptr > all_vsols = frame_vsol->all_data();

  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  

  // smooth and save the contours (polylines) to output storage
  
  // search for regions (polygons) in the vsol list 
  // and apply them to the mask
  for (unsigned int i=0; i<all_vsols.size(); ++i)
  {
    vsol_spatial_object_2d_sptr vsol = all_vsols[i];
    if (vsol->cast_to_curve() && vsol->cast_to_curve()->cast_to_polyline())
    {
      vsol_polyline_2d_sptr polyline = vsol->cast_to_curve()->cast_to_polyline();
      
      // collect the points
      vcl_vector<vgl_point_2d<double > > pts;
      pts.reserve(polyline->size());
      for (unsigned i=0; i<polyline->size(); ++i)
      {
        pts.push_back(polyline->vertex(i)->get_p());
      }

      if (gaussian) {
        // smooth the contours
        for (unsigned i=0; i<num_times; ++i)
        {
          bdgl_curve_algs::smooth_curve(pts, sigma);
          pts.erase(pts.begin());
          pts.erase(pts.begin());
          pts.erase(pts.begin());
          pts.pop_back();
          pts.pop_back();
          pts.pop_back();
        }
      }

      if (discrete_csm) { 
        dbgl_csm(pts,psi,num_times);
      }

      assert(pts.size() == polyline->size());

      // construct the new polyline
      vcl_vector<vsol_point_2d_sptr > vsol_pts;
      vsol_pts.reserve(pts.size());
      for (unsigned j=0; j<pts.size(); ++j)
      {
        vsol_pts.push_back(new vsol_point_2d(pts[j]));
      }

      // if in-place smoothing then just delete the one vsoline and save a new one in
      if (in_place_smoothing)
      {
        frame_vsol->remove_object(vsol);
        frame_vsol->add_object(new vsol_polyline_2d(vsol_pts));
      }
      else
      {
        // put the new polyline in the output storage
        output_vsol->add_object(new vsol_polyline_2d(vsol_pts));
      }
    } else if (vsol->cast_to_region() && vsol->cast_to_region()->cast_to_polygon()) {
      vsol_polygon_2d_sptr poly = vsol->cast_to_region()->cast_to_polygon();
      
      // collect the points
      vcl_vector<vgl_point_2d<double > > pts;
      pts.reserve(poly->size());
      for (unsigned i=0; i<poly->size(); ++i)
      {
        pts.push_back(poly->vertex(i)->get_p());
      }

      if (gaussian) {
        // smooth the contours
        for (unsigned i=0; i<num_times; ++i)
        {
          bdgl_curve_algs::smooth_curve(pts, sigma);
          pts.erase(pts.begin());
          pts.erase(pts.begin());
          pts.erase(pts.begin());
          pts.pop_back();
          pts.pop_back();
          pts.pop_back();
        }
      }

      if (discrete_csm) { 
        dbgl_csm(pts,psi,num_times);
      }

      assert(pts.size() == poly->size());

      // construct the new polyline
      vcl_vector<vsol_point_2d_sptr > vsol_pts;
      vsol_pts.reserve(pts.size());
      for (unsigned j=0; j<pts.size(); ++j)
      {
        vsol_pts.push_back(new vsol_point_2d(pts[j]));
      }

      // if in-place smoothing then just delete the one vsoline and save a new one in
      if (in_place_smoothing)
      {
        frame_vsol->remove_object(vsol);
        frame_vsol->add_object(new vsol_polygon_2d(vsol_pts));
      }
      else
      {
        // put the new polyline in the output storage
        output_vsol->add_object(new vsol_polygon_2d(vsol_pts));
      }
    }
  }

  // if a new storage is requested, give it
  if (!in_place_smoothing)
  {
    output_data_[0].push_back(output_vsol);
  }
  // otherwise, just save the old storage to the repository so that its tableau is updated
  else
  {
    output_data_[0].push_back(frame_vsol);
  }

  return true;
}



bool
dbsol_smooth_contours_process::finish()
{
  return true;
}

