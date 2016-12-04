// This is brcv/seg/dbdet/pro/dbdif_edgel_data_process.cxx

//:
// \file

#include "dbdif_edgel_data_process.h"
#include <dbdif/dbdif_analytic.h>
#include <dbdif/algo/dbdif_data.h>

#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>

#include <dbdif/pro/dbdif_3rd_order_geometry_storage.h>

#include <vcl_vector.h>
#include <vcl_string.h>

#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_line_2d_sptr.h>

//: Constructor
dbdif_edgel_data_process::dbdif_edgel_data_process()
{
  if( 
      !parameters()->add( "Circle?"   , "-circle" , true ) ||
      !parameters()->add( "     radius" , "-radius"     , 20.0 ) ||
      !parameters()->add( "Ellipse?"   , "-ellipse" , false ) ||
      !parameters()->add( "     radius A" , "-a"     , 20.0 ) ||
      !parameters()->add( "     radius B" , "-b"     , 60.0 ) ||
      !parameters()->add( "Perturb?" , "-perturb"     , false) ||
      !parameters()->add( "   perturb position" , "-dpos"     , 0.2) ||
      !parameters()->add( "   perturb tangents (deg)" , "-dtan"     , 10.0)
      ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
dbdif_edgel_data_process::~dbdif_edgel_data_process()
{
}


//: Clone the process
bpro1_process*
dbdif_edgel_data_process::clone() const
{
  return new dbdif_edgel_data_process(*this);
}


//: Return the name of this process
vcl_string
dbdif_edgel_data_process::name()
{
  return "Edgel Test Data";
}


//: Return the number of input frame for this process
int
dbdif_edgel_data_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
dbdif_edgel_data_process::output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > dbdif_edgel_data_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > dbdif_edgel_data_process::get_output_type()
{
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  to_return.push_back( "3rdOrderGeometry" );
  return to_return;
}


//: Execute the process
bool
dbdif_edgel_data_process::execute()
{
  clear_output();

  vcl_vector<dbdif_3rd_order_point_2d> C_subpixel;
  vcl_vector<vsol_line_2d_sptr> lines;

  //get the parameters
  bool do_circle=false;
  parameters()->get_value( "-circle", do_circle);
  bool do_ellipse=false;
  parameters()->get_value( "-ellipse", do_ellipse);

  bool do_perturb=false;
  parameters()->get_value( "-perturb", do_perturb);
  double dpos, dtan;
  if (do_perturb) {
    parameters()->get_value( "-dpos", dpos);
    parameters()->get_value( "-dtan", dtan);
  }

  if (do_circle) {
    double radius;
    parameters()->get_value( "-radius", radius);

    dbdif_data::get_circle_edgels(radius, lines, C_subpixel, do_perturb, dpos, dtan);

  } else if (do_ellipse) {
    double ra, rb;
    parameters()->get_value( "-a", ra);
    parameters()->get_value( "-b", rb);
    
    dbdif_data::get_ellipse_edgels(ra, rb, lines, C_subpixel, do_perturb, dpos, dtan);
  }

  
    vcl_vector< vsol_spatial_object_2d_sptr > edgels;

    const double scale=0.5;
    for (unsigned i=0; i<lines.size(); i++) {
      lines[i]->set_length(lines[i]->length()*scale);
      edgels.push_back(lines[i]->cast_to_spatial_object());
    }

    // create the output storage class
    vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
    output_vsol->add_objects(edgels, "edgels");
    output_data_[0].push_back(output_vsol);


    // create the output storage class
    dbdif_3rd_order_geometry_storage_sptr output_gt = dbdif_3rd_order_geometry_storage_new();
    output_gt->set_c(C_subpixel);
    output_data_[0].push_back(output_gt);
  
  return true;
}

bool
dbdif_edgel_data_process::finish()
{
  return true;
}

