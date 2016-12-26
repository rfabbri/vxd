// This is basic/dbsol/pro/dbsol_fit_circ_arc_spline_process.cxx

//:
// \file


#include "dbsol_fit_circ_arc_spline_process.h"

#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polygon_2d_sptr.h>
#include <vsol/vsol_conic_2d.h>
#include <vsol/vsol_conic_2d_sptr.h>

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>

#include <dbgl/algo/dbgl_fit_circ_arc_spline.h>


#include <vgl/algo/vgl_fit_lines_2d.h>
#include <vnl/vnl_math.h>


// START HERE !!!!!!!!!!!!!!!!!!

// TEST this process on polyline and polygon
// TEST this process on 99 shapes database






//: Constructor
dbsol_fit_circ_arc_spline_process::
dbsol_fit_circ_arc_spline_process(void)
{
  if( 
    !parameters()->add("Tolerance (max error allowed)", "-tolerance", 1.0f )
    ) 
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
dbsol_fit_circ_arc_spline_process::
~dbsol_fit_circ_arc_spline_process()
{
}


//: Clone the process
bpro1_process* dbsol_fit_circ_arc_spline_process::
clone() const
{
  return new dbsol_fit_circ_arc_spline_process(*this);
}


//: Return the name of this process
vcl_string dbsol_fit_circ_arc_spline_process::
name()
{
  return "Fit circular arc spline";
}


//: Return the number of input frame for this process
int dbsol_fit_circ_arc_spline_process::
input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int dbsol_fit_circ_arc_spline_process::
output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > dbsol_fit_circ_arc_spline_process::
get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > dbsol_fit_circ_arc_spline_process::
get_output_type()
{
  vcl_vector<vcl_string > to_return;
  to_return.push_back("vsol2D");
  return to_return;
}



//: Execute the process
bool dbsol_fit_circ_arc_spline_process::
execute()
{
  if ( input_data_.size() != 1 )
  {
    vcl_cout << "In dbsol_fit_circ_arc_spline_process::execute() - not exactly 1"
             << " input frames \n";
    return false;
  }
  this->clear_output();

  // parse process parameters
  float tolerance = 1.0f;
  this->parameters()->get_value("-tolerance", tolerance);

  // retrieve input storage : vsol2D
  vidpro1_vsol2D_storage_sptr frame_vsol;
  frame_vsol.vertical_cast(input_data_[0][0]);
  vcl_vector<vsol_spatial_object_2d_sptr > all_vsols = frame_vsol->all_data();

  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_data_[0].push_back(output_vsol);

  // fit (the first found) the polyline or polygon with a circular arc spline
  vcl_vector<dbgl_circ_arc > arc_list;
  for (unsigned int i=0; i<all_vsols.size(); ++i)
  {
    vsol_spatial_object_2d_sptr vsol = all_vsols[i];
    if (vsol->cast_to_curve() && vsol->cast_to_curve()->cast_to_polyline())
    {
      vsol_polyline_2d_sptr polyline = vsol->cast_to_curve()->cast_to_polyline();
      vcl_cout << "Number of line segments in original contour = " << polyline->size()-1 << ".\n";
      
      // collect the points
      vcl_vector<vgl_point_2d<double > > pts(polyline->size());
      for (unsigned i=0; i<polyline->size(); ++i)
      {
        pts[i] = polyline->vertex(i)->get_p();
      }
      

      //float rms = tolerance/3;
      //int min_fit_length = (int) (2 * tolerance);
      //vgl_fit_lines_2d<double> fitter; 
      //fitter.set_min_fit_length(min_fit_length);
      //fitter.set_rms_error_tol(rms);
      //for (unsigned int i = 0; i<pts.size(); ++i) 
      //{
      //  fitter.add_point(pts[i]);
      //}
      //fitter.fit();
      //vcl_vector<vgl_line_segment_2d<double> >& segs = fitter.get_line_segs();
      //vcl_cout << "Number of segments after line fitting = " << segs.size() << ".\n";

      //// collect the points again
      //vcl_vector<vgl_point_2d<double > > pts2;
      //for (unsigned int i =0; i < segs.size(); ++i)
      //{
      //  // add a lot of points to reduce fitting error
      //  
      //  
      //  double len = (segs[i].point2() - segs[i].point1()).length();
      //  vgl_point_2d<double > p1 = segs[i].point1();
      //  vgl_vector_2d<double > t1 = segs[i].direction();

      //  int num_segs = vnl_math_rnd(len / min_fit_length);
      //  num_segs = vnl_math_max(1, num_segs);
      //  double increment = len / num_segs;

      //  for (int k =0; k < num_segs; ++k)
      //  {
      //    pts2.push_back(p1 + (k*increment) * t1);
      //  }




      //  //pts2[i] = segs[i].point1();
      //}
      //pts2[segs.size()] = segs.back().point2();

      //// insert the line segments in for debug
      //for (unsigned int i =0; i < segs.size(); ++i)
      //{
      //  //output_vsol->add_object(new vsol_line_2d(segs[i].point1(), segs[i].point2()));
      //}
      //




      dbgl_fit_circ_arc_spline_to_polyline(arc_list, pts, tolerance);
      break;
    } 
    else if (vsol->cast_to_region() && vsol->cast_to_region()->cast_to_polygon()) 
    {
      vsol_polygon_2d_sptr polygon = vsol->cast_to_region()->cast_to_polygon();
      vcl_cout << "Number of line segments in original contour = " << polygon->size() << ".\n";
      
      // collect the points
      vcl_vector<vgl_point_2d<double > > pts(polygon->size());
      for (unsigned i=0; i<polygon->size(); ++i)
      {
        pts[i] = polygon->vertex(i)->get_p();
      }

      // fit polygon with circular arc spline
      dbgl_fit_circ_arc_spline_to_polygon(arc_list, pts, tolerance);
      break;
    }
  }

  vcl_cout << "Number of arc segments in final contour = " << arc_list.size() << ".\n";

  // putting each circular arc as a vsol2D object (since there is not poly-arc in vsol)
  for (unsigned i =0; i < arc_list.size(); ++i)
  {
    dbgl_circ_arc arc = arc_list[i];

    ////
    //vcl_cout << "arc [ " << i << " ] = ";
    //arc.print(vcl_cout);
    //vcl_cout << "\n.";

    //// output a polyline for each arc with 5 pts
    //vcl_vector<vsol_point_2d_sptr > arc_pts;
    //for (double t = 0; t <= 1; t = t + 0.2)
    //{
    //  arc_pts.push_back(new vsol_point_2d(arc.point_at(t)));
    //}
    //output_vsol->add_object(new vsol_polyline_2d(arc_pts));

    if (vcl_abs(arc.k()) < 1e-4)
    {
      vsol_line_2d_sptr line = new vsol_line_2d(arc.start(), arc.end());
      
      // put the new polyline in the output storage
      output_vsol->add_object(line->cast_to_spatial_object());
    }
    else
    {
      // convert to vsol_conic
      vgl_point_2d<double > c = arc.center();
      double r = 1 / vcl_abs(arc.k());

      // parameter of conic (circle)
      double conic_a = 1;
      double conic_b = 0;
      double conic_c = 1;
      double conic_d = -2*c.x();
      double conic_e = -2*c.y();
      double conic_f = c.x()*c.x()+c.y()*c.y()-r*r;

      vgl_point_2d<double > s, e; // start and end
      if (arc.k() > 0)
      {
        s = arc.start();
        e = arc.end();
      }
      else
      {
        s = arc.end();
        e = arc.start();
      }

      vsol_conic_2d_sptr new_arc = 
          new vsol_conic_2d(conic_a, conic_b, conic_c, conic_d, conic_e, conic_f);
      new_arc->set_p0(new vsol_point_2d(s));
      new_arc->set_p1(new vsol_point_2d(e));

      // put the new polyline in the output storage
      output_vsol->add_object(new_arc->cast_to_spatial_object());
    }
  }

  
  
  return true;
}



bool
dbsol_fit_circ_arc_spline_process::finish()
{
  return true;
}

