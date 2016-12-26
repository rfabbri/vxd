// This is basic/dbsol/pro/dbsol_merge_storage_process.cxx

//:
// \file


#include "dbsol_merge_storage_process.h"
////#include <vil/vil_image_resource.h>
////#include <vil/vil_image_view.h>
////#include <vil/vil_new.h>
////
////#include <vsol/vsol_region_2d_sptr.h>
//#include <vsol/vsol_polyline_2d.h>
//#include <vsol/vsol_polyline_2d_sptr.h>
//
//#include <vsol/vsol_point_2d.h>
////
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
//#include <bdgl/bdgl_curve_algs.h>




//#include <vgl/vgl_vector_2d.h>

//#include <dbsol/algo/dbsol_linear_transform_2d.h>
//
//: Constructor
dbsol_merge_storage_process::
dbsol_merge_storage_process()
{
  if( false 
    //!parameters()->add("Number of times " , "-num_times" , (unsigned)1 )
    ) 
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
dbsol_merge_storage_process::
~dbsol_merge_storage_process()
{
}


//: Clone the process
bpro1_process* dbsol_merge_storage_process::
clone() const
{
  return new dbsol_merge_storage_process(*this);
}


//: Return the name of this process
vcl_string dbsol_merge_storage_process::
name()
{
  return "Merge vsol2D storages";
}


//: Return the number of input frame for this process
int dbsol_merge_storage_process::
input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int dbsol_merge_storage_process::
output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > dbsol_merge_storage_process::
get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > dbsol_merge_storage_process::
get_output_type()
{
  vcl_vector<vcl_string > to_return;
  return to_return;
}



//: Execute the process
bool dbsol_merge_storage_process::
execute()
{
  if ( input_data_.size() != 1 )
  {
    vcl_cout << "In dbsol_merge_storage_process::execute() - not exactly 1"
             << " input frames \n";
    return false;
  }
  this->clear_output();

  // get vsol2D object from the storage class

  // the combined storage will be saved in the first vsol storage
  vidpro1_vsol2D_storage_sptr vsol_storage_0;
  vidpro1_vsol2D_storage_sptr vsol_storage_1;
  vsol_storage_0.vertical_cast(input_data_[0][0]);
  vsol_storage_1.vertical_cast(input_data_[0][1]);

  vsol_storage_0->add_objects(vsol_storage_1->all_data(), "appended");

  this->output_data_[0].push_back(vsol_storage_0);
  return true;
}



bool
dbsol_merge_storage_process::finish()
{
  return true;
}

