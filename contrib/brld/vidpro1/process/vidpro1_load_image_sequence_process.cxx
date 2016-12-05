// This is brl/vidpro1/process/vidpro1_load_image_sequence_process.cxx

#include <vidpro1/process/vidpro1_load_image_sequence_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>
#include <vul/vul_file.h>
#include <vil/vil_load.h>
#include <vcl_iostream.h>


//: Constructor
vidpro1_load_image_sequence_process::
vidpro1_load_image_sequence_process() : bpro1_process(), num_frames_(0)
{
  if( !parameters()->add( "Image folder:" , "-image_folder", bpro1_filepath("","*") ) ||
      !parameters()->add( "Image list file:" , "-image_list", bpro1_filepath("","*") )
        )
    {
        vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
    }
}


//: Destructor
vidpro1_load_image_sequence_process::
~vidpro1_load_image_sequence_process()
{
}


//: Clone the process
bpro1_process*
vidpro1_load_image_sequence_process::clone() const
{
    return new vidpro1_load_image_sequence_process(*this);
}


//: Return the name of the process
vcl_string vidpro1_load_image_sequence_process::
name()
{
    return "Load Image Sequence";
}


//: Call the parent function and reset num_frames_
void vidpro1_load_image_sequence_process::
clear_output(int resize)
{
  num_frames_ = 0;
  bpro1_process::clear_output(resize);  
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > vidpro1_load_image_sequence_process::
get_input_type()
{
    vcl_vector< vcl_string > to_return;
    // no input type required
    to_return.clear();
    return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string >vidpro1_load_image_sequence_process::
get_output_type()
{
    vcl_vector< vcl_string > to_return;
    to_return.push_back( "image" );
    return to_return;
}


//: Returns the number of input frames to this process
int vidpro1_load_image_sequence_process::
input_frames()
{
    return 0;
}


//: Returns the number of output frames from this process
int vidpro1_load_image_sequence_process::
output_frames()
{
    return num_frames_;
}


//: Run the process on the current frame
bool vidpro1_load_image_sequence_process::
execute()
{
  // 1. parse input parameters

  // image folder
  bpro1_filepath image_folder_path;
  this->parameters()->get_value("-image_folder" , image_folder_path);
  vcl_string image_folder = image_folder_path.path;

  // be smart: in case user enters a filename instead of a folder, extract the
  // folder containing the file
  if (!vul_file::is_directory(image_folder))
  {
    image_folder = vul_file::dirname(image_folder);
  }

  // image name list
  bpro1_filepath image_list_path;
  this->parameters()->get_value("-image_list" , image_list_path);
  //vcl_string image_list = image_list_path.path;

  // parse the image_list file to extract the list of file names
  vcl_vector<vcl_string > image_names;
  
  // 2. parse the image_list file to extract the names
  vcl_ifstream fp(image_list_path.path.c_str());
  if (!fp) 
  {
    vcl_cout<<"ERROR: Can't open " << image_list_path.path << vcl_endl;
    return false;
  }

  while (!fp.eof()) 
  {
    vcl_string name;
    fp >> name;
    if (!name.empty())
    {
      image_names.push_back(name);
    }
  }
  fp.close();


  // 3. load images in the files and push to the repository
  for (unsigned i = image_names.size(); i > 0; --i)
  {
    // load the image
    vcl_string image_file = image_folder + "/" + image_names[i-1];

    vil_image_resource_sptr image_resource = vil_load_image_resource(image_file.c_str());
    if (!image_resource)
    {
      vcl_cerr << "ERROR: can't load image " << image_file << ". Process canceled.\n";
      this->output_data_.clear();
      this->num_frames_ = 0;
      return false;
    }

    // place holder for the image
    vidpro1_image_storage_sptr image_storage = vidpro1_image_storage_new();
    image_storage->set_image( image_resource );
    output_data_.push_back(vcl_vector< bpro1_storage_sptr >(1,image_storage));
    this->num_frames_++;
  }
  return true;   
}


//: Finish
bool vidpro1_load_image_sequence_process::
finish() 
{
  return true;
}



