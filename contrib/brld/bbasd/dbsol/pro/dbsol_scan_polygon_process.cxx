// This is basic/dbsol/pro/dbsol_scan_polygon_process.cxx

//:
// \file

#include "dbsol_scan_polygon_process.h"
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vsol/vsol_polygon_2d.h>
#include <vgl/vgl_box_2d.h>
#include <vgl/vgl_polygon_scan_iterator.h>
#include <bsol/bsol_algs.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_image_view.h>
#include <vil/vil_new.h>



//: Constructor
dbsol_scan_polygon_process::dbsol_scan_polygon_process()
{

  if( !parameters()->add( "Label Step" ,   "-label_step" ,  (unsigned int)1 ) ||
      !parameters()->add( "Image Width" ,  "-width" ,       (unsigned int)340 ) ||
      !parameters()->add( "Image Height" , "-height" ,      (unsigned int)240 )||
          !parameters()->add( "Do you want to create global image" , "-isglobal" ,      (bool)true )){
    vcl_cerr << "ERROR: Adding parameters in " __FILE__<< vcl_endl;
  }
}


//: Destructor
dbsol_scan_polygon_process::~dbsol_scan_polygon_process()
{
}


//: Clone the process
bpro1_process* 
dbsol_scan_polygon_process::clone() const
{
  return new dbsol_scan_polygon_process(*this);
}


//: Return the name of this process
vcl_string
dbsol_scan_polygon_process::name()
{
  return "Scan Convert Polygons";
}


//: Return the number of input frame for this process
int
dbsol_scan_polygon_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
dbsol_scan_polygon_process::output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > dbsol_scan_polygon_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > dbsol_scan_polygon_process::get_output_type()
{  
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Execute the process
bool
dbsol_scan_polygon_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cout << "In dbsol_scan_polygon_process::execute() - "
             << "not exactly one input image \n";
    return false;
  }
  clear_output();

  // get the process parameters 
  bool isglobal=false;
  int label_step=0,w=0,h=0;
  parameters()->get_value( "-label_step" ,  label_step );
  parameters()->get_value( "-width" ,  w );
  parameters()->get_value( "-height" ,  h );
  parameters()->get_value( "-isglobal" ,  isglobal );

  // get contours from the storage class
  vidpro1_vsol2D_storage_sptr frame_vsol;
  frame_vsol.vertical_cast(input_data_[0][0]);
  vcl_vector<vcl_string> groups = frame_vsol->groups();
  vil_image_view<vxl_byte> label_image(w,h);
  label_image.fill(0);
  unsigned int curr_idx = label_step;
  if(label_step==0)
                curr_idx=255;
  for( vcl_vector<vcl_string>::const_iterator gitr = groups.begin();
       gitr != groups.end();  ++gitr, curr_idx+=label_step )
  {
    vcl_vector<vsol_spatial_object_2d_sptr> contours = frame_vsol->data_named(*gitr);

    vgl_polygon<double> poly_region;
    for( vcl_vector<vsol_spatial_object_2d_sptr>::const_iterator vitr = contours.begin();
         vitr != contours.end();  ++vitr )
    {
      if(vsol_region_2d* r = (*vitr)->cast_to_region())
        if(vsol_polygon_2d* p = r->cast_to_polygon())
        {
          vgl_polygon<double> poly = bsol_algs::vgl_from_poly(p);
          for(unsigned int i=0; i<poly.num_sheets(); ++i)
            poly_region.push_back(poly[i]);
        }
    }

    if(poly_region.num_sheets() > 0){
      vgl_box_2d<double> box(0.0,w, 0.0,h);
      vgl_polygon_scan_iterator<double> psi(poly_region,false,box);
      for (psi.reset(); psi.next(); ) 
      {
        int y = psi.scany();
        for (int x = psi.startx(); x <= psi.endx(); ++x)
          label_image(x,y) = curr_idx;
      }
    }
  }

  vidpro1_image_storage_sptr image_stg = new vidpro1_image_storage();
  image_stg->set_image(vil_new_image_resource_of_view(label_image));
  output_data_[0].push_back(image_stg);
  if(isglobal)
        image_stg->mark_global();
  return true;
}


bool
dbsol_scan_polygon_process::finish()
{
  return true;
}

