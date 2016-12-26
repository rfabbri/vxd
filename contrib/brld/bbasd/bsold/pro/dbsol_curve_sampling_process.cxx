// This is basic/dbsol/pro/dbsol_curve_sampling_process.cxx

#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include <dbsol/pro/dbsol_curve_sampling_process.h>
#include <dbsol/algo/dbsol_curve_algs.h>
#include <dbsol/algo/dbsol_geno.h>
#include <dbsol/dbsol_interp_curve_2d_sptr.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_region_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vnl/vnl_math.h>

dbsol_curve_sampling_process::dbsol_curve_sampling_process() : bpro1_process()
{
  if( !parameters()->add(  "Number of samples in new curve (negative for resampling factor):" , "-size" , 150 ) ||
      !parameters()->add(  "Type of interpolation to create curve: (1:linear, 2:geno, 3:geno-eulerspiral)" , "-type" , 2 ))
  {
    vcl_cerr << 
       "ERROR: Adding parameters in dbsol_curve_sampling_process::dbsol_curve_sampling_process()\n";
  }
}

//: Clone the process
bpro1_process*
dbsol_curve_sampling_process::clone() const
{
  return new dbsol_curve_sampling_process(*this);
}

vcl_vector< vcl_string > dbsol_curve_sampling_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

vcl_vector< vcl_string > dbsol_curve_sampling_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

bool dbsol_curve_sampling_process::execute()
{
  int new_size=0;
  parameters()->get_value( "-size" , new_size);
  int type=0;
  parameters()->get_value( "-type" , type);
  vcl_cout << "TYPE READ: " << type << vcl_endl;
  return curveUpsample(new_size, type);
}


bool dbsol_curve_sampling_process::curveUpsample (int new_size, int type)
{
  // get input storage class
  vidpro1_vsol2D_storage_sptr input_vsol;
  input_vsol.vertical_cast(input_data_[0][0]);
  vcl_vector<vsol_point_2d_sptr> inp;
  // The contour can either be a polyline producing an open contour 
  // or a polygon producing a close contour
  vcl_vector< vsol_spatial_object_2d_sptr > vsol_list = input_vsol->all_data();

  bool closed = false;

  if (vsol_list.size() == 0) {
     vcl_cerr << "Empty storage\n";
     return false;
  }

  for (unsigned int b = 0 ; b < vsol_list.size(); b++ ) {
    if( vsol_list[b]->cast_to_curve()) {
      if( vsol_list[b]->cast_to_curve()->cast_to_polyline() ) {
        for (unsigned int i=0; i<vsol_list[b]->cast_to_curve()->cast_to_polyline()->size();i++) {
          vsol_point_2d_sptr pt = vsol_list[b]->cast_to_curve()->cast_to_polyline()->vertex(i);
          inp.push_back(pt);
         } 
      } else {
        vcl_cerr << "Storage not supported\n";
        return false;
      }
    } else {
       if( vsol_list[b]->cast_to_region()) {
         if( vsol_list[b]->cast_to_region()->cast_to_polygon() ) {
           closed = true;
           for (unsigned int i=0; i<vsol_list[b]->cast_to_region()->cast_to_polygon()->size();i++) {
             vsol_point_2d_sptr pt = vsol_list[b]->cast_to_region()->cast_to_polygon()->vertex(i);
             //curve1->add_vertex(pt->x(), pt->y());
             inp.push_back(pt);
           } 
         } else {
           vcl_cerr << "Storage not supported\n";
           return false;
         }
       } else {
         vcl_cerr << "Storage not supported\n";
         return false;
       }
    }
  }

  vcl_cout << "Original curve size: " << inp.size() << vcl_endl;

  if (new_size < 0)
    new_size = inp.size()*(-new_size);


  dbsol_interp_curve_2d_sptr curve;
  if (type == 1 || (type == 2 && inp.size() < 3)) {
     curve = new dbsol_interp_curve_2d;
     dbsol_curve_algs::interpolate_linear(&(*curve),inp, closed);
  } else if (type == 2) {
     dbsol_geno_curve_2d *gcurve = new dbsol_geno_curve_2d;
     dbsol_geno::interpolate(gcurve,inp, closed);
     curve = gcurve;
  } else {
     dbsol_geno_curve_2d *gcurve = new dbsol_geno_curve_2d;
     dbsol_geno::interpolate3_approx(gcurve,inp, false);
     curve = gcurve;
  }

  vcl_vector < vsol_point_2d_sptr> points;
  dbsol_curve_algs::sample(*curve, new_size, points);
  
  if (!points.size()) {
       vcl_cout << "Problems in upsampling process!\n";
       return false;
  }

  vcl_cout << "New curve size after upsampling: " << points.size() << vcl_endl;
  
  vcl_vector< vsol_spatial_object_2d_sptr > contours;
  vsol_polyline_2d_sptr newContour = new vsol_polyline_2d (points);
  contours.push_back(newContour->cast_to_spatial_object());

  for (unsigned i = 0; i<points.size(); i++)
    contours.push_back(points[i]->cast_to_spatial_object());

  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_vsol->add_objects(contours, "upsampled_contour");
  output_data_[0].push_back(output_vsol);

  return true;
}
