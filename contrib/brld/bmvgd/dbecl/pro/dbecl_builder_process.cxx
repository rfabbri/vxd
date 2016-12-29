// This is brcv/mvg/dbecl/pro/dbecl_builder_process.cxx

//:
// \file

#include "dbecl_builder_process.h"
#include "dbecl_episeg_storage.h"
#include <bpro1/bpro1_parameters.h>

#include <vidpro1/storage/vidpro1_vtol_storage.h>

#include <vtol/vtol_edge.h>
#include <vtol/vtol_edge_2d.h>
#include <vdgl/vdgl_digital_curve.h>
#include <vdgl/vdgl_interpolator.h>
#include <vdgl/vdgl_edgel_chain.h>
#include <vsol/vsol_point_2d.h>

#include <dbecl/dbecl_episeg_from_curve_converter.h>
#include <vsol/vsol_digital_curve_2d.h>

//: Constructor
dbecl_builder_process::dbecl_builder_process()
 : bpro1_process()
{
  if( !parameters()->add( "Epipole Position X", "-epix", -400.0f) ||
      !parameters()->add( "Epipole Position Y", "-epiy",  250.0f) ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  } 
}


//: Copy Constructor
dbecl_builder_process::
dbecl_builder_process(const dbecl_builder_process& other)
: bpro1_process(other)
{
}


//: Destructor
dbecl_builder_process::~dbecl_builder_process()
{
}


//: Clone the process
bpro1_process* 
dbecl_builder_process::clone() const
{
  return new dbecl_builder_process(*this);
}


//: Return the name of the process
vcl_string
dbecl_builder_process::name()
{
  return "Build Episegments";
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > dbecl_builder_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vtol" );
  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string > dbecl_builder_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "episeg" );
  return to_return;
}


//: Returns the number of input frames to this process
int
dbecl_builder_process::input_frames()
{
  return 1;
}


//: Returns the number of output frames from this process
int
dbecl_builder_process::output_frames()
{
  return 1;
}


//: Run the process on the current frame
bool
dbecl_builder_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cerr << __FILE__ << " - not exactly one input frame" << vcl_endl;
    return false;
  }

  float epi_x=false, epi_y=false;
  parameters()->get_value( "-epix", epi_x );
  parameters()->get_value( "-epiy", epi_y );
  dbecl_epipole_sptr epipole = new dbecl_epipole((double)epi_x, (double)epi_y);

  dbecl_episeg_from_curve_converter factory(epipole);
  vcl_vector<dbecl_episeg_sptr> episegs;

  vidpro1_vtol_storage_sptr frame_vtol;
  frame_vtol.vertical_cast(input_data_[0][0]);

  for ( vcl_set<vtol_topology_object_sptr>::const_iterator itr = frame_vtol->begin();
        itr != frame_vtol->end();  ++itr ) {
    // Cast the vtol edge into a vdgl_digital_curve
    vtol_edge *edge = (*itr)->cast_to_edge();
    if (edge){
      vtol_edge_2d *edge_2d = edge->cast_to_edge_2d();
      if (edge_2d){
        vsol_curve_2d_sptr c = edge_2d->curve();
        vdgl_digital_curve_sptr vdc = c->cast_to_vdgl_digital_curve();
        if(vdc){
          // Extract the points along the edgel chain and make a vsol_digital_curve_2d
          vdgl_edgel_chain_sptr ec = vdc->get_interpolator()->get_edgel_chain();
          vsol_digital_curve_2d_sptr dc = new vsol_digital_curve_2d;
          for(unsigned int i=0; i<ec->size(); ++i){
            // For some reason vtol edges often contain duplicate points
            if ( dc->size() > 0 && dc->p1()->get_p() == ec->edgel(i).get_pt() )
              continue;  
            dc->add_vertex(new vsol_point_2d(ec->edgel(i).get_pt()));
          }
          // Cover the digital curve with episegs
          vcl_vector<dbecl_episeg_sptr> eps = factory.convert_curve(dc);
          for(vcl_vector<dbecl_episeg_sptr>::iterator itr = eps.begin();
              itr != eps.end();  ++itr)
            episegs.push_back(*itr);
        }
      }
    }
  }


  // create the output storage class
  dbecl_episeg_storage_sptr output_episeg = dbecl_episeg_storage_new();
  output_episeg->set_episegs(episegs);
  output_data_[0].push_back(output_episeg);

  return true;
}


//: Finish
bool
dbecl_builder_process::finish()
{
  return true;
}

