// This is basic/dbnl/algo/pro/dbnl_function_sampling_process.cxx

#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include <dbnl/algo/pro/dbnl_function_sampling_process.h>
#include <dbnl/algo/dbnl_eno.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_region_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vnl/vnl_math.h>
#include <vcl_functional.h>


struct less_x : 
   public vcl_binary_function< vsol_point_2d_sptr, vsol_point_2d_sptr, bool> 
{
   bool operator()(const vsol_point_2d_sptr &p1, const vsol_point_2d_sptr &p2) { return p1->x() < p2->x(); }
};


dbnl_function_sampling_process::
dbnl_function_sampling_process() : bpro1_process()
{
  if( !parameters()->add(  "Number of samples in new curve:" , "-size" , 90 ) ||
      !parameters()->add(  "Order of interpolation to create curve: (1:linear, 2:eno)" , "-type" , 2 ))
  {
    vcl_cerr << 
       "ERROR: Adding parameters in dbnl_function_sampling_process::dbnl_function_sampling_process()\n";
  }
}

//: Clone the process
bpro1_process* dbnl_function_sampling_process::
clone() const
{
  return new dbnl_function_sampling_process(*this);
}

vcl_vector< vcl_string > dbnl_function_sampling_process::
get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

vcl_vector< vcl_string > dbnl_function_sampling_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

bool dbnl_function_sampling_process::execute()
{
  int new_size=0;
  parameters()->get_value( "-size" , new_size);
  int type=0;
  parameters()->get_value( "-type" , type);
  return function_upsample(new_size, type);
}



bool dbnl_function_sampling_process::
function_upsample(int new_size, int type)
{
  // get input storage class
  vidpro1_vsol2D_storage_sptr input_vsol;
  input_vsol.vertical_cast(input_data_[0][0]);
  vcl_vector<vsol_point_2d_sptr> inp;

  //  We only accept a set of points defined by
  //
  
  vcl_vector< vsol_spatial_object_2d_sptr > vsol_list = input_vsol->all_data();

  if (vsol_list.size() == 0) {
     vcl_cerr << "Empty storage\n";
     return false;
  }

   for (unsigned int b = 0 ; b < vsol_list.size(); b++ ) {
      if (vsol_list[b]->cast_to_point()) {
         inp.push_back(vsol_list[b]->cast_to_point());
      } else {
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
              vcl_cerr << "Storage not supported\n";
              return false;
         }
     }
   }

  vcl_cout << "Original curve size: " << inp.size() << vcl_endl;
  
  // SORT
  vcl_sort(inp.begin(),inp.end(),less_x());

  if (type == 2) {
     if (inp.size() <= 3) {
        vcl_cerr << "Need at least 4 pts\n";
        return false;
     }
  } else {
     vcl_cerr << "Type not supported\n";
     return false;
  }


  dbnl_eno_1d e;

  vnl_vector<double> x, f;

  x.set_size(inp.size());
  f.set_size(inp.size());

  unsigned i;
  for (i=0; i<inp.size(); ++i) {
      x[i] = inp[i]->x();
      f[i] = inp[i]->y();
  }

  e.interpolate(&f,&x);

  e.sample(new_size, f,x);

  vcl_vector < vsol_point_2d_sptr> points;
  points.resize(f.size());

  for (i=0; i<f.size(); ++i)
     points[i] = new vsol_point_2d(x[i],f[i]);
  

  /*
  if (type == 1 || (type == 2 && inp.size() < 3)) {
     curve = new dbsol_interp_curve_2d;
     dbsol_curve_algs::interpolate_linear(&(*curve),inp, closed);
  } else {
     dbsol_geno_curve_2d *gcurve = new dbsol_geno_curve_2d;
     dbsol_geno::interpolate(gcurve,inp, closed);
     curve = gcurve;
  }

  vcl_vector < vsol_point_2d_sptr> points;
  dbsol_curve_algs::sample(*curve, new_size, points);
  
  if (!points.size()) {
       vcl_cout << "Problems in upsampling process!\n";
       return false;
  }

  vcl_cout << "New curve size after upsampling: " << points.size() << vcl_endl;
  */
  
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
