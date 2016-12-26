// This is basic/dbil/pro/dbil_distance_transform_process.cxx

//:
// \file

#include "dbil_distance_transform_process.h"

#include <vcl_cstdlib.h>
#include <bpro1/bpro1_parameters.h>
#include <bpro1/bpro1_storage.h>
#include <vidpro1/storage/vidpro1_image_storage.h>

#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>
#include <vil/vil_math.h>
#include <dbil/algo/dbil_exact_distance_transform.h>
#include <bil/algo/bil_cedt.h>
#include <brip/brip_vil_float_ops.h>


//: Constructor
dbil_distance_transform_process::dbil_distance_transform_process()
{
  vcl_vector<vcl_string> algorithm_choices;
  algorithm_choices.push_back("Fast Exact (squared)"); // 0
  algorithm_choices.push_back("CEDT");                 // 1
  algorithm_choices.push_back("Brute Force (squared)");// 2

  if( 
    !parameters()->add( "Algorithm"   , "-dt_algo" , algorithm_choices, 0) ||
    !parameters()->add( "Pixels of interest are black" , "-is_black" , (bool)false ) ||
    !parameters()->add( "Output labels of nearest pixels?" , "-do_label" , (bool)true )
      ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
dbil_distance_transform_process::~dbil_distance_transform_process()
{
}


//: Clone the process
bpro1_process*
dbil_distance_transform_process::clone() const
{
  return new dbil_distance_transform_process(*this);
}


//: Returns a vector of strings describing the input types to this process
vcl_vector< vcl_string > dbil_distance_transform_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}


//: Returns a vector of strings describing the output types of this process
vcl_vector< vcl_string > dbil_distance_transform_process::get_output_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );

  bool do_label = false;
  parameters()->get_value( "-do_label" , do_label );

  if (do_label)
    to_return.push_back( "image" );

  return to_return;
}

//: Return the number of input frame for this process
int
dbil_distance_transform_process::input_frames()
{
  return 1;
}

//: Return the number of output frames for this process
int
dbil_distance_transform_process::output_frames()
{
  return 1;
}

//: Run the process on the current frame
bool 
dbil_distance_transform_process::execute()
{
  bool is_black;
  parameters()->get_value( "-is_black" , is_black );

  clear_output();

  // get image from the storage class
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);
  vil_image_resource_sptr in_img = frame_image->get_image();
  vil_image_view<vxl_byte> image_view = in_img->get_view();

  unsigned dt_algo;

  parameters()->get_value("-dt_algo", dt_algo);

  bool do_label;
  parameters()->get_value("-do_label", do_label);



  if (dt_algo == 1) {
    vil_image_view<unsigned char> bool_image_view(in_img->ni(),in_img->nj(),1);

    if(is_black)
        {
          for(unsigned i=0;i<bool_image_view.ni();i++)
              for(unsigned j=0;j<bool_image_view.nj();j++)
                  if(image_view(i,j)>127)
                      bool_image_view(i,j)=255;
                  else
                      bool_image_view(i,j)=0;

        }
    else
        {
           for(unsigned i=0;i<bool_image_view.ni();i++)
              for(unsigned j=0;j<bool_image_view.nj();j++)
                  if(image_view(i,j)>127)
                      bool_image_view(i,j)=0;
                  else
                      bool_image_view(i,j)=255;
        }

    vil_image_view<vxl_uint_32> edt_img(bool_image_view.ni(), bool_image_view.nj(), 1);
    
    bil_cedt cedt(bool_image_view);
    cedt.compute_cedt();

    for (unsigned i=0; i < edt_img.ni(); ++i)
      for (unsigned j=0; j < edt_img.nj(); ++j) {
        float d = cedt.cedtimg()(i,j);
        edt_img(i,j) = static_cast<vxl_uint_32>(d*d);
      }
    
    vidpro1_image_storage_sptr output_img_storage = vidpro1_image_storage_new();
    output_img_storage->set_image( vil_new_image_resource_of_view(edt_img) );
    output_data_[0].push_back(output_img_storage);
    output_img_storage->set_name("EDT Image");

    if (do_label) {
      vcl_cerr << "Not yet supported with CEDT\n" ;
      return true;
      /*
      vil_image_view<unsigned> label_img(edt_img.ni(), edt_img.nj(), 1);
      for (unsigned i=0; i < edt_img.ni(); ++i)
        for (unsigned j=0; j < edt_img.nj(); ++j) {
          double x = cedt.closest_point_x(i,j);
          double y = cedt.closest_point_y(i,j);
          assert ((unsigned) x < edt_img.nj());
          assert ((unsigned) y < edt_img.ni());
          label_img(i,j) = static_cast<unsigned>(y + x*edt_img.ni());
        }
        */
    }

  } else { // dt_algo == 0 || 2
    vil_image_view<vxl_uint_32> bool_image_view(in_img->ni(),in_img->nj(),1);
    if(is_black)
        {
          for(unsigned i=0; i<bool_image_view.ni(); i++)
              for(unsigned j=0; j<bool_image_view.nj(); j++)
                  if(image_view(i,j)>127)
                      bool_image_view(i,j)=1;
                  else
                      bool_image_view(i,j)=0;

        }
    else
        {
           for(unsigned i=0;i<bool_image_view.ni();i++)
              for(unsigned j=0;j<bool_image_view.nj();j++)
                  if(image_view(i,j)>127)
                      bool_image_view(i,j)=0;
                  else
                      bool_image_view(i,j)=1;
        }

    if (!do_label) {
      if (dt_algo == 0) {
        if (!dbil_exact_distance_transform_maurer(bool_image_view))
          vcl_abort();
      } else { // brute 
        if (!dbil_exact_distance_transform_brute_force_with_list(bool_image_view))
          vcl_abort();
      }

      vidpro1_image_storage_sptr output_img_storage = vidpro1_image_storage_new();
      output_img_storage->set_image( vil_new_image_resource_of_view( bool_image_view ) );
      output_data_[0].push_back(output_img_storage);
      output_img_storage->set_name("EDT Image");
    } else {
      vil_image_view<unsigned> imlabel(bool_image_view.ni(), bool_image_view.nj(), 1);
      bool retval=false;
     
      if (dt_algo == 0)
        retval = dbil_exact_distance_transform_maurer_label( bool_image_view, imlabel);
      else
        retval = dbil_exact_distance_transform_brute_force_with_list_label( bool_image_view, imlabel);

      if (!retval)
        vcl_abort();

      vidpro1_image_storage_sptr output_img_storage = vidpro1_image_storage_new();
      output_img_storage->set_image( vil_new_image_resource_of_view( bool_image_view ) );
      output_data_[0].push_back(output_img_storage);
      output_img_storage->set_name("EDT Image");

      output_img_storage =  vidpro1_image_storage_new();
      output_img_storage->set_image( vil_new_image_resource_of_view( imlabel ) );
      output_data_[0].push_back(output_img_storage);
      output_img_storage->set_name("Label Image");
    }
  }
 
  return true;
}

bool
dbil_distance_transform_process::finish()
{
  return true;
}

