// This is brl/vidpro1/process/vidpro1_load_image_process.cxx

//:
// \file

#include <vidpro1/process/vidpro1_load_image_process.h>
#include <vcl_iostream.h>

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <vil/vil_load.h>
#include <vil/vil_image_resource_sptr.h>

#include <vidpro1/vidpro1_repository.h>
#include <vil/vil_image_view.h>
#include <vil/vil_new.h>

//: Constructor
vidpro1_load_image_process::vidpro1_load_image_process() : bpro1_process()
{
  if( !parameters()->add( "Image file <filename...>" , "-image_filename" , bpro1_filepath("","*") ) 
      )
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
vidpro1_load_image_process::~vidpro1_load_image_process()
{
}


//: Clone the process
bpro1_process*
vidpro1_load_image_process::clone() const
{
  return new vidpro1_load_image_process(*this);
}


//: Return the name of the process
vcl_string vidpro1_load_image_process::name()
{
  return "Load Image";
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > vidpro1_load_image_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;

  // no input type required
  to_return.clear();

  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string > vidpro1_load_image_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;

  // output type
  to_return.push_back( "image" );

  return to_return;
}


//: Run the process on the current frame
bool
vidpro1_load_image_process::execute()
{
  bpro1_filepath image_path;
  parameters()->get_value( "-image_filename" , image_path );
  vcl_string image_filename = image_path.path;

  //no input storage class for this process
  clear_output();

  vil_image_resource_sptr loaded_image = vil_load_image_resource( image_filename.c_str() );
  if( !loaded_image ) {
    vcl_cerr << "Failed to load image file" << image_filename << vcl_endl;
    return false;
  }

  vidpro1_image_storage_sptr image_storage = vidpro1_image_storage_new();

  //if (hacked_loading) {
  //  //: ozge's hack to load the second image shifted, TODO: REMOVE this
  //  vidpro1_repository_sptr res = bvis1_manager::instance()->repository();
  //  vcl_set<bpro1_storage_sptr> st_set = res->get_all_storage_classes(res->current_frame());
  //  vcl_string name_initial = "image";
  //  int len = name_initial.length();
  //  int max = 0;
  //  int offset_x = 0;
  //  for (vcl_set<bpro1_storage_sptr>::iterator iter = st_set.begin();
  //    iter != st_set.end(); iter++) {
  //      if ((*iter)->type() == image_storage->type()) {
  //        vidpro1_image_storage_sptr image_stg;
  //        image_stg.vertical_cast(*iter);
  //        offset_x = image_stg->get_image()->ni();
  //        break;
  //      }
  //  }
  //  if (offset_x != 0) {
  //    vil_image_view<vxl_byte> new_img(loaded_image->ni()+offset_x, loaded_image->nj(), loaded_image->nplanes());
  //    vil_image_view<vxl_byte> loaded_view = loaded_image->get_view();
  //    for (unsigned p=0;p<loaded_image->nplanes();++p)
  //      for (unsigned j=0;j<loaded_image->nj();++j)
  //        for (unsigned i=0;i<loaded_image->ni()+offset_x;++i)
  //          new_img(i,j,p) = vxl_byte(255);
  //    for (unsigned p=0;p<loaded_image->nplanes();++p)
  //      for (unsigned j=0;j<loaded_image->nj();++j)
  //        for (unsigned i=offset_x;i<loaded_image->ni()+offset_x;++i)
  //          new_img(i,j,p) = loaded_view(i-offset_x, j, p);

  //    loaded_image = vil_new_image_resource_of_view(new_img);

  //  }
  //} 

  // create the storage data structure
  
  image_storage->set_image( loaded_image );

  output_data_[0].push_back(image_storage);

  return true;
}


//: Finish
bool vidpro1_load_image_process::finish() 
{
  return true;
}


//: Returns the number of input frames to this process
int
vidpro1_load_image_process::input_frames()
{
  return 1;
}


//: Returns the number of output frames from this process
int
vidpro1_load_image_process::output_frames()
{
  return 1;
}


