// This is dvidpro/process/vidpro1_save_vsol_process.h

#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include "vidpro1_save_vsol_process.h"
#include <vnl/vnl_math.h>

#include <vsl/vsl_binary_io.h>


vidpro1_save_vsol_process::vidpro1_save_vsol_process() : bpro1_process()
{
  if( !parameters()->add( "Output file <filename...>" , "-fname" , bpro1_filepath("","*.cem") ))
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
vidpro1_save_vsol_process::clone() const
{
  return new vidpro1_save_vsol_process(*this);
}


vcl_vector< vcl_string > vidpro1_save_vsol_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

vcl_vector< vcl_string > vidpro1_save_vsol_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.clear();
  return to_return;
}

bool vidpro1_save_vsol_process::execute()
{
  bpro1_filepath output;
  parameters()->get_value( "-fname" , output );


  vidpro1_vsol2D_storage_sptr input_vsol;
  input_vsol.vertical_cast(input_data_[0][0]);

  vsl_b_ofstream bp_out(output.path);

  if (!bp_out){
    vcl_cout << " Error opening file  " << output.path << vcl_endl;
    return false;
  }

  vcl_cout << "Created " << output.path << " for writing vsol\n";

  input_vsol->b_write(bp_out);

  bp_out.close();

  return true;
}
