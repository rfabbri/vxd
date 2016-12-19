//This is vidpro1/process/vidpro1_load_vsol_process.cxx

#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include "vidpro1_load_vsol_process.h"

#include <vcl_cstring.h>
#include <vcl_string.h>
#include <vcl_fstream.h>
#include <vsl/vsl_binary_io.h>
#include <vsl/vsl_binary_loader.h>

#include <vidpro1/storage/vidpro1_vsol2D_storage.h>

vidpro1_load_vsol_process::vidpro1_load_vsol_process() : bpro1_process()
{
  if( !parameters()->add( "Input file <filename...>" , "-fname" , bpro1_filepath("","*") )) 
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
vidpro1_load_vsol_process::clone() const
{
  return new vidpro1_load_vsol_process(*this);
}

vcl_vector< vcl_string > vidpro1_load_vsol_process::get_input_type() 
{
  vcl_vector< vcl_string > to_return;
  return to_return;
}

vcl_vector< vcl_string > vidpro1_load_vsol_process::get_output_type() 
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

bool vidpro1_load_vsol_process::execute()
{
  bpro1_filepath input;
  parameters()->get_value( "-fname" , input);
  vcl_string input_file = input.path;
  

  vsl_b_ifstream bp_in(input_file.c_str());
  if (!bp_in) {
    vcl_cout << " Error opening file  " << input_file << vcl_endl;
    return false;
  }

  vcl_cout << "Opened " << input_file <<  " for reading" << vcl_endl;

  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
  output_vsol->b_read(bp_in);
  bp_in.close();

//  output_vsol->print_summary(vcl_cout);


  //: clone

  vidpro1_vsol2D_storage_sptr output_vsol_2;
  output_vsol_2.vertical_cast(output_vsol->clone());

  output_vsol_2->set_frame(-10); //:< means its not in rep
  // try to copy by hand if doesnt work

  output_data_[0].push_back(output_vsol_2);

  return true;
}
