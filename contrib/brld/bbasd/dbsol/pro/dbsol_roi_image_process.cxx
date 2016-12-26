// This is basic/dbsol/pro/dbsol_roi_image_process.cxx

//:
// \file


#include "dbsol_roi_image_process.h"
#include <vil/vil_image_resource.h>
#include <vil/vil_image_view.h>
#include <vil/vil_new.h>

#include <vsol/vsol_region_2d_sptr.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_point_2d.h>

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <bil/algo/bil_roi_mask.h>



//#include <vgl/vgl_vector_2d.h>

#include <dbsol/algo/dbsol_linear_transform_2d.h>

//: Constructor
dbsol_roi_image_process::dbsol_roi_image_process()
{
  if( !parameters()->add("Override default size ", "-override", false ) ||
    !parameters()->add("Image width " , "-width" , (unsigned)100 ) ||
    !parameters()->add( "Image height " , "-height" , (unsigned)100 )
      ) 
  {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
}


//: Destructor
dbsol_roi_image_process::~dbsol_roi_image_process()
{
}


//: Clone the process
bpro1_process* dbsol_roi_image_process::
clone() const
{
  return new dbsol_roi_image_process(*this);
}


//: Return the name of this process
vcl_string dbsol_roi_image_process::
name()
{
  return "ROI Image";
}


//: Return the number of input frame for this process
int dbsol_roi_image_process::
input_frames()
{
  return 1;
}


//: Return the number of output frames for this process
int dbsol_roi_image_process::
output_frames()
{
  return 1;
}


//: Provide a vector of required input types
vcl_vector< vcl_string > dbsol_roi_image_process::
get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "vsol2D" );
  to_return.push_back( "image" );
  return to_return;
}


//: Provide a vector of output types
vcl_vector< vcl_string > dbsol_roi_image_process::
get_output_type()
{
  vcl_vector<vcl_string > to_return;
  to_return.push_back("image");
  return to_return;
}


//: Execute the process
bool dbsol_roi_image_process::
execute()
{
  if ( input_data_.size() != 1 )
  {
    vcl_cout << "In dbsol_roi_image_process::execute() - not exactly 1"
             << " input frames \n";
    return false;
  }
  clear_output();

  // extract proces parameters
  bool override = false;
  unsigned width = 0,
    height = 0;

  this->parameters()->get_value("-override", override);
  this->parameters()->get_value("-width", width );
  this->parameters()->get_value("-height", height);

  // get vsol2D object from the storage class
  vidpro1_vsol2D_storage_sptr frame_vsol;
  frame_vsol.vertical_cast(input_data_[0][0]);
  vcl_vector<vsol_spatial_object_2d_sptr > all_vsols = frame_vsol->all_data();

  // construct binary image to return;
  vil_image_view<bool > mask;
  if (override)
  {
    mask.set_size(width, height);
  }
  else
  {
    // if available, get image size from input image
    if (this->input_data_[0].size() > 1)
    {
      vidpro1_image_storage_sptr frame_image;
      frame_image.vertical_cast(input_data_[0][1]);
      vil_image_resource_sptr im_sptr = frame_image->get_image();
      mask.set_size(im_sptr->ni(), im_sptr->nj());
    }
    
    // otherwise, compute bounding box of all vsols and use it as size
    // of new image
    // TODO - implement this
    else
    {
      return false;
    }
  }

  mask.fill(false);
  
  // search for regions (polygons) in the vsol list 
  // and apply them to the mask
  for (unsigned int i=0; i<all_vsols.size(); ++i)
  {
    vsol_spatial_object_2d_sptr p = all_vsols[i];
    if (p->cast_to_region())
    {
      vsol_region_2d_sptr region = p->cast_to_region();
      if (region->cast_to_polygon())
      {
        vsol_polygon_2d_sptr poly = region->cast_to_polygon();

        // turn into a vgl_polygon
        vcl_vector<vgl_point_2d<double > > pts;
        for (unsigned m=0; m<poly->size(); ++m)
        {
          pts.push_back(poly->vertex(m)->get_p());
        }

        // scan polygon
        vgl_polygon<double > polygon(pts);
        //dbil_roi_mask_add_polygon(mask, polygon);

        // if we have two polygons, one is completely inside the other
        // this will generate a polygon with hole
        bil_roi_mask_toggle_polygon(mask, polygon);
        //
      }
    }
  }

  

  //vsol_image_storage
  // create the output storage class
  vidpro1_image_storage_sptr output_image = vidpro1_image_storage_new();
  output_image->set_image(vil_new_image_resource_of_view(mask));
  output_data_[0].push_back(output_image);

  return true;
}



bool
dbsol_roi_image_process::finish()
{
  return true;
}

