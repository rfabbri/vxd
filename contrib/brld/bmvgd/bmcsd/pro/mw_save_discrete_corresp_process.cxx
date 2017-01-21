#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include "mw_save_discrete_corresp_process.h"

#include <vcl_cstring.h>
#include <vcl_string.h>
#include <vcl_fstream.h>

#include <mw/pro/mw_discrete_corresp_storage.h>
#include <mw/pro/mw_discrete_corresp_storage_3.h>


mw_save_discrete_corresp_process::mw_save_discrete_corresp_process() : bpro1_process()
{
  if( 
      !parameters()->add( "Output filename" , "-nameprefix" , bpro1_filepath("","*.*") ) ||
      !parameters()->add( "Binocular correspondence?" , "-binocular_ftype" , false)  ||
      !parameters()->add( "Trinocular correspondence?" , "-trinocular_ftype" , true)
      )
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
mw_save_discrete_corresp_process::clone() const
{
  return new mw_save_discrete_corresp_process(*this);
}

vcl_vector< vcl_string > mw_save_discrete_corresp_process::get_input_type() 
{
  vcl_vector< vcl_string > to_return;

  bool binocular_ftype=false;
  parameters()->get_value( "-binocular_ftype", binocular_ftype);
  if (binocular_ftype)
    to_return.push_back( "mw pt corresp" );
  else
    to_return.push_back( "mw_3_pt_corresp" );

  return to_return;
}

vcl_vector< vcl_string > mw_save_discrete_corresp_process::get_output_type() 
{
  vcl_vector< vcl_string > to_return;
  return to_return;
}

//: Loads a _full_ perspective camera (not a pure vpgl_proj_camera)
bool mw_save_discrete_corresp_process::execute()
{
  bpro1_filepath input;
  parameters()->get_value( "-nameprefix" , input);
  vcl_string fname = input.path;

  vsl_b_ofstream bp_out(fname.c_str());
  if (!bp_out) {
    vcl_cerr << "ERROR: while opening " << fname << " for reading\n";
    return false;
  }
  vcl_cout << "Created " << fname << " for writing point correspondence\n";

  bool binocular_ftype=false;
  parameters()->get_value( "-binocular_ftype", binocular_ftype);
  if (binocular_ftype) {
    mw_discrete_corresp_storage_sptr input_corr;
    input_corr.vertical_cast(input_data_[0][0]);
    input_corr->corresp()->b_write(bp_out);
  } else {
    mw_discrete_corresp_storage_3_sptr input_corr;
    input_corr.vertical_cast(input_data_[0][0]);
    input_corr->corresp()->b_write(bp_out);
  }

  bp_out.close();

  return true;
}
