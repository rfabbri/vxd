//This is vidpro1/process/vidpro1_load_con_process.cxx

#include <vcl_iostream.h>
#include <vcl_fstream.h>

#include <vcl_algorithm.h>
#include <vidpro1/process/vidpro1_load_con_process.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polygon_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vgl/algo/vgl_h_matrix_2d.h>

#include <vcl_cstring.h>
#include <vcl_string.h>
#include <vcl_fstream.h>
#include <vul/vul_file.h>
#include <vul/vul_file_iterator.h>

#include <dbsol/dbsol_file_io.h>

vidpro1_load_con_process::vidpro1_load_con_process() : bpro1_process(), num_frames_(0)
{
  if (!parameters()->add( "Input file <filename...>" , "-coninput" , bpro1_filepath("Choose a file or a directory","*.con") ) ||
      !parameters()->add( "Add all cons to the same frame" , "-bsame_frame" , (bool)false) ||
      !parameters()->add( "reverse frame sequence " , "-rev" , (bool)false ))
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
vidpro1_load_con_process::clone() const
{
  return new vidpro1_load_con_process(*this);
}

vcl_vector< vcl_string > vidpro1_load_con_process::get_input_type() 
{
  vcl_vector< vcl_string > to_return;
  return to_return;
}

vcl_vector< vcl_string > vidpro1_load_con_process::get_output_type() 
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  return to_return;
}

bool vidpro1_load_con_process::execute()
{
  bpro1_filepath input;
  parameters()->get_value( "-coninput" , input);
  vcl_string input_file_path = input.path;
  
  int num_of_files = 0;
  output_data_.clear();

  // make sure that input_file_path is sane
  if (input_file_path == "") { return false; }

  //create 
  vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();

  // test if fname is a directory
  if (vul_file::is_directory(input_file_path))
  {
    vcl_cout << input_file_path<<vcl_endl;

    bool bsame_frame;
    parameters()->get_value( "-bsame_frame" , bsame_frame);

    vul_file_iterator fn=input_file_path+"/*.con";
    for ( ; fn; ++fn) 
    {
      vcl_string input_file = fn();
  
      //load the con file      
      vcl_vector< vsol_spatial_object_2d_sptr > contours;
      contours.push_back(dbsol_load_con_file(input_file.c_str()));

      if (!bsame_frame){
        // create the output storage class
        vidpro1_vsol2D_storage_sptr new_con = vidpro1_vsol2D_storage_new();
        new_con->add_objects(contours, input_file);
        
        output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,new_con));
        num_of_files++;
      }
      else
        output_vsol->add_objects(contours, input_file);
    }

    if (bsame_frame){
      output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,output_vsol));
      num_frames_ = 1;
    }
    else {
      //this is the number of frames to be outputted
      num_frames_ = num_of_files;
    }
  }
  else {
    vcl_string input_file = input_file_path;

    vcl_cout << input_file <<vcl_endl;

    //load the con file      
    vcl_vector< vsol_spatial_object_2d_sptr > contours;
    contours.push_back(dbsol_load_con_file(input_file.c_str()));

    output_vsol->add_objects(contours, input_file);

    output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,output_vsol));
    num_frames_ = 1;
  }

  bool rev=false;
  parameters()->get_value( "-rev" , rev);

  if (!rev){
    //reverse the order of the objects so that they come out in the right order
    vcl_reverse(output_data_.begin(),output_data_.end());
  }

  return true;
}

