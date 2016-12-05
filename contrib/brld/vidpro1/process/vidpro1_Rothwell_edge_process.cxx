// This is brl/vidpro1/process/vidpro1_Rothwell_edge_process.cxx

//:
// \file

#include <vidpro1/process/vidpro1_Rothwell_edge_process.h>
#include <vcl_list.h>
#include <vnl/vnl_math.h>

#include <bpro1/bpro1_parameters.h>

#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>

#include <vil/vil_convert.h>

//required because sdet still uses vil1
#include <vil1/vil1_vil.h>
#include <vil1/vil1_memory_image_of.h>

#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_line_2d_sptr.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_curve_2d_sptr.h>

#include <vgl/vgl_vector_2d.h>
#include <vgl/vgl_point_2d.h>

#include <osl/osl_topology.h>
#include <osl/osl_canny_rothwell.h>

//: Constructor
vidpro1_Rothwell_edge_process::vidpro1_Rothwell_edge_process()
{
  if( !parameters()->add( "Gaussian sigma" ,  "-sigma" ,       (float)dp.sigma ) ||
      !parameters()->add( "Low Threshold" ,   "-low_thresh" ,  (float)dp.low ) ||
      !parameters()->add( "High Threshold" ,  "-high_thresh" , (float)dp.high ) ||
      !parameters()->add( "Range" ,           "-range" ,       (float)dp.range ) ||
      !parameters()->add( "Verbose" ,         "-verbose" ,     (bool)dp.verbose ) ||
      !parameters()->add( "Show edgels?"   ,  "-show_lines",   (bool)true )) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
vidpro1_Rothwell_edge_process::~vidpro1_Rothwell_edge_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_Rothwell_edge_process::clone() const
{
  return new vidpro1_Rothwell_edge_process(*this);
}


//: Return the name of this process
vcl_string
vidpro1_Rothwell_edge_process::name()
{
  return "Rothwell EdgeDetector";
}


//: Return the number of input frame for this process
int
vidpro1_Rothwell_edge_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
vidpro1_Rothwell_edge_process::output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > 
vidpro1_Rothwell_edge_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > 
vidpro1_Rothwell_edge_process::get_output_type()
{  
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Execute the process
bool
vidpro1_Rothwell_edge_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cout << "In vidpro1_Rothwell_Edge_process::execute() - not exactly one"
             << " input image" << vcl_endl;
    return false;
  }
  clear_output();

  // get image from the storage class
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image_sptr = frame_image->get_image();
  vil_image_view<vxl_byte> grey_view = vil_convert_stretch_range(vxl_byte(),
    vil_convert_to_grey_using_rgb_weighting(image_sptr->get_view()) );
      
  if(grey_view.nplanes() != 1) {
    vcl_cerr << "Returning false. nplanes(): " << grey_view.nplanes() << vcl_endl;
    return false;
  }

  vil1_memory_image_of< unsigned char > img = vil1_from_vil_image_view( grey_view );

  //get parameters
  bool show_lines;
  parameters()->get_value( "-sigma" ,       dp.sigma );
  parameters()->get_value( "-low_thresh" ,  dp.low );
  parameters()->get_value( "-high_thresh" , dp.high ); 
  parameters()->get_value( "-range" ,       dp.range );
  parameters()->get_value( "-verbose" ,     dp.verbose );
  parameters()->get_value( "-show_lines" ,  show_lines );

  //Rothwell detector
  vcl_list<osl_edge*> edges_osl;

  //initialize the detector
  osl_canny_rothwell detector(dp);
  detector.detect_edges(img, &edges_osl);

  //process edges (osl_edgel_chain->vsol)
  vcl_vector< vsol_spatial_object_2d_sptr > edgels;
  
  //get the edgels
  if (show_lines){
    vcl_list<osl_edge*>::iterator e_it = edges_osl.begin();
    for (; e_it != edges_osl.end(); e_it++){
      osl_edge* edgel = (*e_it);
      for (unsigned i=0; i<edgel->size(); i++)
      {
        vgl_point_2d<double> middle(edgel->GetY(i), edgel->GetX(i));
        double theta = edgel->GetTheta(i)*vnl_math::pi/180.0;
        vgl_vector_2d<double> dir(vcl_cos(theta), vcl_sin(theta));
        vsol_line_2d_sptr edge_line = new vsol_line_2d(dir, middle); 
        edgels.push_back(edge_line->cast_to_spatial_object());
      }
    }
  }
  else {
    vcl_list<osl_edge*>::iterator e_it = edges_osl.begin();
    for (; e_it != edges_osl.end(); e_it++){
      osl_edge* edgel = (*e_it);
      vsol_polyline_2d_sptr poly_line = new vsol_polyline_2d();
      for (unsigned i=0; i<edgel->size(); i++)
        poly_line->add_vertex(new vsol_point_2d(edgel->GetY(i), edgel->GetX(i)));
      edgels.push_back(poly_line->cast_to_spatial_object());
    }
  }

  //housecleaning
  vcl_list<osl_edge*>::iterator e_it = edges_osl.begin();
  for (; e_it != edges_osl.end(); e_it++)
    delete (*e_it);
  edges_osl.clear();

  // create the output storage class
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_vsol->add_objects(edgels, "edgels");
  output_data_[0].push_back(output_vsol);

  return true;
}


//: Finish
bool
vidpro1_Rothwell_edge_process::finish()
{
  return true;
}
