// This is brl/vidpro1/process/vidpro1_VD_edge_process.cxx

//:
// \file

#include <vidpro1/process/vidpro1_VD_edge_process.h>

#include <vul/vul_timer.h>

#include <bpro1/bpro1_parameters.h>

#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vidpro1/storage/vidpro1_vtol_storage.h>
#include <vidpro1/storage/vidpro1_vtol_storage_sptr.h>

#include <vil/vil_convert.h>

//required because sdet still uses vil1
#include <vil1/vil1_vil.h>
#include <vil1/vil1_memory_image_of.h>

#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_curve_2d_sptr.h>

#include <vtol/vtol_edge_2d.h>

#include <vdgl/vdgl_digital_curve.h>
#include <vdgl/vdgl_digital_curve_sptr.h>
#include <vdgl/vdgl_interpolator.h>
#include <vdgl/vdgl_interpolator_sptr.h>
#include <vdgl/vdgl_edgel_chain.h>
#include <vdgl/vdgl_edgel_chain_sptr.h>

#include <sdet/sdet_detector.h>


//: Constructor
vidpro1_VD_edge_process::vidpro1_VD_edge_process()
{
  if( !parameters()->add( "Gaussian sigma" ,      "-ssigma" ,            (float)dp.smooth ) ||
      !parameters()->add( "Noise Threshold" ,     "-snoisethresh" ,      (float)dp.noise_multiplier ) ||
      !parameters()->add( "Automatic Threshold" , "-sauto_thresh" ,      (bool)dp.automatic_threshold ) ||
      !parameters()->add( "Agressive Closure" ,   "-sagressive_closure" ,(int)dp.aggressive_junction_closure ) ||
      !parameters()->add( "Compute Junctions" ,   "-scompute_juncts" ,   (bool)dp.junctionp ) ||
      !parameters()->add( "Include Borders" ,     "-sinclude_borders" ,  (bool)dp.borderp ) ||
      !parameters()->add( "Min Length" ,          "-min_length" ,        (int)dp.minLength )) 
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
vidpro1_VD_edge_process::~vidpro1_VD_edge_process()
{
}


//: Clone the process
bpro1_process* 
vidpro1_VD_edge_process::clone() const
{
  return new vidpro1_VD_edge_process(*this);
}


//: Return the name of this process
vcl_string
vidpro1_VD_edge_process::name()
{
  return "VD EdgeDetector";
}


//: Return the number of input frame for this process
int
vidpro1_VD_edge_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
vidpro1_VD_edge_process::output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > 
vidpro1_VD_edge_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > 
vidpro1_VD_edge_process::get_output_type()
{  
  vcl_vector<vcl_string > to_return;
  to_return.push_back( "vtol" );
  return to_return;
}


//: Execute the process
bool
vidpro1_VD_edge_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cout << "In vidpro1_VD_Edge_process::execute() - not exactly one"
             << " input image" << vcl_endl;
    return false;
  }
  clear_output();

  //start the timer
  vul_timer t;

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
  parameters()->get_value( "-ssigma" ,  dp.smooth);
  parameters()->get_value( "-snoisethresh" , dp.noise_multiplier );
  parameters()->get_value( "-sauto_thresh" , dp.automatic_threshold );
  parameters()->get_value( "-sagressive_closure" , dp.aggressive_junction_closure );
  parameters()->get_value( "-scompute_juncts" , dp.junctionp );
  parameters()->get_value( "-min_length" , dp.minLength );
  parameters()->get_value( "-sinclude_borders" , dp.borderp );

  //initialize the detector
  sdet_detector detector(dp);
  detector.SetImage(img);
  //process edges
  detector.DoContour();
  vcl_vector<vtol_edge_2d_sptr> * edges = detector.GetEdges();

  if (!edges)
    return false;


  vcl_cout << "process " << edges->size() << " edges in " << t.real() << " msecs." << vcl_endl;

  // create the output storage class
  vidpro1_vtol_storage_sptr output_vtol = vidpro1_vtol_storage_new();
  // add the edges as topology objects to to storage class
  for ( vcl_vector<vtol_edge_2d_sptr>::iterator e_itr = edges->begin();
        e_itr != edges->end();  ++e_itr ) {
    vtol_topology_object_sptr edge = e_itr->ptr();    
    output_vtol->add_vtol(edge);
  }
  output_data_[0].push_back(output_vtol);
  
  return true;
}


//: Finish
bool
vidpro1_VD_edge_process::finish()
{
  return true;
}
