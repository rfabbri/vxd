// This is dvidpro1/process/vidpro1_save_cem_process.h

#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include <vidpro1/process/vidpro1_save_cem_process.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vnl/vnl_math.h>
#include <dbsol/dbsol_file_io.h>

vidpro1_save_cem_process::vidpro1_save_cem_process() : bpro1_process()
{
  if( !parameters()->add( "Output file <filename...>" , "-cemoutput" , bpro1_filepath("","*.cem") ))
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
vidpro1_save_cem_process::clone() const
{
  return new vidpro1_save_cem_process(*this);
}


vcl_vector< vcl_string > vidpro1_save_cem_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

vcl_vector< vcl_string > vidpro1_save_cem_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.clear();
  return to_return;
}

bool vidpro1_save_cem_process::execute()
{
  bpro1_filepath output;
  parameters()->get_value( "-cemoutput" , output );
  return saveCEM(output.path);
}

bool vidpro1_save_cem_process::saveCEM (vcl_string filename)
{
  // get input storage class
    vidpro1_vsol2D_storage_sptr input_vsol;
  input_vsol.vertical_cast(input_data_[0][0]);

  vcl_vector< vsol_spatial_object_2d_sptr > vsol_list = input_vsol->all_data();

  return dbsol_save_cem(vsol_list, filename);
}


