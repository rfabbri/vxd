// This is vidpro1/process/vidpro1_load_cem_process.h

#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vcl_cassert.h>

#include <vcl_algorithm.h>
#include <vidpro1/process/vidpro1_load_cem_process.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <vnl/vnl_math.h>
#include <vul/vul_file.h>
#include <vul/vul_file_iterator.h>
#include <dbsol/dbsol_file_io.h>

vidpro1_load_cem_process::vidpro1_load_cem_process() : bpro1_process(), num_frames_(0)
{
  if( !parameters()->add( "Input file <filename.edg...> (gzipped .gz supported)" , "-ceminput" , bpro1_filepath("","*.cem*") ))
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Clone the process
bpro1_process*
vidpro1_load_cem_process::clone() const
{
  return new vidpro1_load_cem_process(*this);
}


vcl_vector< vcl_string > vidpro1_load_cem_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  // no input type required
  to_return.clear();
  return to_return;
}

vcl_vector< vcl_string > vidpro1_load_cem_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  // output type
  to_return.push_back( "vsol2D" );
  return to_return;
}

bool vidpro1_load_cem_process::execute()
{
  bpro1_filepath input;
  parameters()->get_value( "-ceminput" , input);
  vcl_string input_file_path = input.path;

  int num_of_files = 0;

  output_data_.clear();

  // make sure that input_file_path is sane
  if (input_file_path == "") { return false; }

  //vcl_cout << vul_file::dirname(input_file_path);

  // test if fname is a directory
  if (vul_file::is_directory(input_file_path))
  {
    vcl_vector<vcl_string> input_files;
    for(vul_file_iterator fn = input_file_path + "/*.cem*"; fn; ++fn)
      input_files.push_back(fn());

    // Sort - because the file iterator uses readdir() it does not
    //        iterate over files in alphanumeric order 
    vcl_sort(input_files.begin(), input_files.end());

    while (!input_files.empty())
    {
      vcl_string input_file = input_files.back();
  
      vcl_vector< vsol_spatial_object_2d_sptr > contours;
      dbsol_load_cem(contours, input_file);

      // create the output storage class
      vidpro1_vsol2D_storage_sptr new_cem = vidpro1_vsol2D_storage_new();
      new_cem->add_objects(contours, input_file);
      
      output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,new_cem));
      num_of_files++;
      input_files.pop_back();
    }

    //this is the number of frames to be outputted
    num_frames_ = num_of_files;
  }
  else {
    vcl_string input_file = input_file_path;

    vcl_vector< vsol_spatial_object_2d_sptr > contours;
    dbsol_load_cem(contours, input_file);

    // create the output storage class
    vidpro1_vsol2D_storage_sptr new_cem = vidpro1_vsol2D_storage_new();
    new_cem->add_objects(contours, input_file);

    output_data_.push_back(vcl_vector< bpro1_storage_sptr > (1,new_cem));
    num_frames_ = 1;
  }

  return true;
}
