// This is basic/dbsol/pro/dbsol_translate_2d_process.cxx

//:
// \file

#include "dbsol_translate_2d_process.h"
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vsol/vsol_spatial_object_2d.h>
#include <vgl/vgl_vector_2d.h>

#include <dbsol/algo/dbsol_linear_transform_2d.h>

//: Constructor
dbsol_translate_2d_process::dbsol_translate_2d_process()
{
  if( !parameters()->add( "Translate dx = " , "-dx" , (int)0 ) ||
      !parameters()->add( "Translate dy = " , "-dy" , (int)0 )) 
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
dbsol_translate_2d_process::~dbsol_translate_2d_process()
{
}


//: Clone the process
bpro1_process*
dbsol_translate_2d_process::clone() const
{
  return new dbsol_translate_2d_process(*this);
}


//: Return the name of this process
vcl_string
dbsol_translate_2d_process::name()
{
  return "Translate vsol2D objects";
}


//: Return the number of input frame for this process
int
dbsol_translate_2d_process::input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int
dbsol_translate_2d_process::output_frames()
{
  return 0;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > dbsol_translate_2d_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > dbsol_translate_2d_process::get_output_type()
{
  vcl_vector<vcl_string > to_return;
  return to_return;
}


//: Execute the process
bool
dbsol_translate_2d_process::execute()
{
  if ( input_data_.size() != 1 )
  {
    vcl_cout << "In dbsol_translate_2d_process::execute() - not exactly 1"
             << " input frames \n";
    return false;
  }
  clear_output();

  // get vsol2D object from the storage class
  // vidpro1_image_storage_sptr frame_image;
  // frame_image.vertical_cast(input_data_[0][0]);
  
  // translation vector
  int dx = 0;
  int dy = 0;
  this->parameters()->get_value("-dx", dx);
  this->parameters()->get_value("-dy", dy);
  vgl_vector_2d< double > translate_v(dx, dy);

  // translate all vsol2D objects one by one
  vidpro1_vsol2D_storage_sptr vsol_storage;
  vsol_storage.vertical_cast(this->input_data_[0][0]);
  vcl_vector< vsol_spatial_object_2d_sptr > vsol_data = vsol_storage->all_data();
  bool success = true;
  unsigned int count = 0;
  
  for ( count = 0; count < vsol_data.size() && success; count ++ )
  {
    success = success && dbsol_linear_transform_2d::translate(vsol_data.at(count), translate_v);
  }
  // check if all objects were translated succesfully. 
  // If not, put all input objects to their original status.
  if (success)
    return true;
  
  vcl_cout << "Translation could not be done on object " << vcl_endl;
  vsol_data.at(count - 1)->print();
  
  translate_v = -translate_v;
  for (unsigned int i = 0; i < count; i ++)
  {
    dbsol_linear_transform_2d::translate(vsol_data.at(i), translate_v);
  }
  vcl_cout << "Translation failed. All objects remain at the original positions." << vcl_endl;
  return true;
}



bool
dbsol_translate_2d_process::finish()
{
  return true;
}

