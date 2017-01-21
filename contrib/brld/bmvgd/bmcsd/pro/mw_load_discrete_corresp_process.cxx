#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include "mw_load_discrete_corresp_process.h"

#include <vcl_cstring.h>
#include <vcl_string.h>
#include <vcl_fstream.h>
#include <mw/pro/bmcsd_discrete_corresp_storage.h>
#include <mw/pro/bmcsd_discrete_corresp_storage_3.h>


mw_load_discrete_corresp_process::mw_load_discrete_corresp_process() : bpro1_process()
{
  if( 
      !parameters()->add( "Input filename" , "-nameprefix" , bpro1_filepath("","*.*") ) ||
      !parameters()->add( "Binocular correspondence?" , "-binocular_ftype" , false)  ||
      !parameters()->add( "Trinocular correspondence?" , "-trinocular_ftype" , true)
      )
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
mw_load_discrete_corresp_process::clone() const
{
  return new mw_load_discrete_corresp_process(*this);
}

vcl_vector< vcl_string > mw_load_discrete_corresp_process::get_input_type() 
{
  vcl_vector< vcl_string > to_return;
  return to_return;
}

vcl_vector< vcl_string > mw_load_discrete_corresp_process::get_output_type() 
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

//: Loads a _full_ perspective camera (not a pure vpgl_proj_camera)
bool mw_load_discrete_corresp_process::execute()
{
  bpro1_filepath input;
  parameters()->get_value( "-nameprefix" , input);
  vcl_string input_file = input.path;

  vsl_b_ifstream bfs_in(input_file.c_str());
  if (!bfs_in) {
    vcl_cerr << "ERROR: while opening " << input_file << " for reading\n";
    return false;
  }


  bool binocular_ftype=false;
  parameters()->get_value( "-binocular_ftype", binocular_ftype);

  if (binocular_ftype) {
    bmcsd_discrete_corresp *cp_in = new bmcsd_discrete_corresp();
    vsl_b_read(bfs_in, *cp_in);
    bfs_in.close();
    clear_output();

    bmcsd_discrete_corresp_storage_sptr 
       c_storage = bmcsd_discrete_corresp_storage_new();
    
    c_storage->set_corresp(cp_in);

    output_data_[0].push_back(c_storage);
    
    vcl_cout << "Corresp (process): \n" << *(c_storage->corresp());
  } else {
    bmcsd_discrete_corresp_3 *cp_in_3 = new bmcsd_discrete_corresp_3();
    vsl_b_read(bfs_in, *cp_in_3);
    bfs_in.close();
    clear_output();

    bmcsd_discrete_corresp_storage_3_sptr 
       c_storage = bmcsd_discrete_corresp_storage_3_new();
    
    c_storage->set_corresp(cp_in_3);

    output_data_[0].push_back(c_storage);
    
    vcl_cout << "Corresp (process): \n" << *(c_storage->corresp());
  }


  return true;
}
