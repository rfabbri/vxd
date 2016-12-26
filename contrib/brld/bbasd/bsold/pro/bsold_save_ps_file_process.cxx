// This is basic/dbsol/pro/dbsol_save_ps_file_process.cxx

//:
// \file

#include "dbsol_save_ps_file_process.h"

//#include <vil/vil_image_resource.h>
//#include <vil/vil_image_resource_sptr.h>
//#include <vil/vil_convert.h>
//#include <vil/vil_new.h>
//
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>
#include <dbsol/dbsol_file_io.h>


//
//#include <bil/algo/bil_harr_wavelet_transform.h>

//: Constructor
dbsol_save_ps_file_process::
dbsol_save_ps_file_process()
{
  if( !parameters()->add( "Output PS file:", "-ps_file", bpro1_filepath("","*.ps")) 
    ) 
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}

//: Destructor
dbsol_save_ps_file_process::
~dbsol_save_ps_file_process()
{
}

bpro1_process*
dbsol_save_ps_file_process::clone() const
{
  return new dbsol_save_ps_file_process(*this);
}

//: Return the name of this process
vcl_string 
dbsol_save_ps_file_process::name()
{
  return "Save vsol2D to Postscript File";
}

int dbsol_save_ps_file_process::
input_frames()
{
  return 1;
}

int dbsol_save_ps_file_process::
output_frames()
{
  return 1;
}

vcl_vector< vcl_string > dbsol_save_ps_file_process::
get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  to_return.push_back( "vsol2D" );
  return to_return;
}


vcl_vector< vcl_string >
dbsol_save_ps_file_process::get_output_type()
{
  vcl_vector<vcl_string > to_return;
  to_return.clear();
  return to_return;
}


bool dbsol_save_ps_file_process::
execute()
{
  if ( input_data_.size() != 1 )
  {
    vcl_cout << "In dbsol_save_ps_file_process::execute() - not exactly one" 
      << " input image" << vcl_endl;
    return false;
  }
  this->clear_output();


  // Process parameters ////////////////////////////
  bpro1_filepath ps_file;
  this->parameters()->get_value("-ps_file", ps_file);


  // Input data from storage /////////////////////////

  // get image from the storage class
  vidpro1_image_storage_sptr image_storage;
  image_storage.vertical_cast(input_data_[0][0]);
  vil_image_resource_sptr image_resource = image_storage->get_image();

  // get vsol from the storage
  vidpro1_vsol2D_storage_sptr vsol_storage;
  vsol_storage.vertical_cast(input_data_[0][1]);

  // Process data /////////////////////////



  vcl_string filename = ps_file.path;

  vcl_vector<vsol_spatial_object_2d_sptr > vsol_data = vsol_storage->all_data();
  vcl_vector<vil_rgb<float > > colors;
  // everything blue
  for (unsigned i =0; i < vsol_data.size(); ++i)
  {
    colors.push_back(vil_rgb<float >(0, 0, 1));
  }


  // MAIN function ///////////////////////////////////////////////////////
  if (dbsol_save_ps_file(ps_file.path, image_resource, vsol_data, colors))
  {
    vcl_cout << "Writing postscript file succeeded.\n";
  }
  else
  {
    vcl_cout << "ERROR: Writing postscript file failed.\n";
  }
  ////////////////////////////////////////////////////////////////////////

  return false;
}


// -----------------------------------------------------------------------------
bool dbsol_save_ps_file_process::
finish()
{
  return true;
}
