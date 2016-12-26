// This is basic/dbspi/pro/dbspi_curvemap_storage.cxx

//:
// \file

#include "dbspi_curvemap_storage.h"
#include <vil/io/vil_io_image_view.h>
#include <vil/vil_image_view.h>
#include <vil/vil_new.h>


//: Constructor
dbspi_curvemap_storage::dbspi_curvemap_storage()
{
}


//: Destructor
dbspi_curvemap_storage::~dbspi_curvemap_storage()
{
}


//: Return IO version number;
short 
dbspi_curvemap_storage::version() const
{
  return 2;
}


//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
dbspi_curvemap_storage::clone() const
{
  return new dbspi_curvemap_storage(*this);
}


//: Binary save self to stream.
void 
dbspi_curvemap_storage::b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  bpro1_storage::b_write(os);
  if(image_){
    vsl_b_write(os, true);
    vil_image_view<vxl_byte> image = this->image_->get_view();
    vsl_b_write(os, image);
  }
  else
    vsl_b_write(os, false);
}


//: Binary load self from stream.
void 
dbspi_curvemap_storage::b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch(ver)
  {
  case 1:
  {
    bpro1_storage::b_read(is);
    vil_image_view<vxl_byte> image;
    vsl_b_read(is, image);
    this->image_ = vil_new_image_resource_of_view(image);
    break;
  }
  case 2:
  {
    bpro1_storage::b_read(is);
    bool is_valid;
    vsl_b_read(is, is_valid);
    if(is_valid){
      vil_image_view<vxl_byte> image;
      vsl_b_read(is, image);
      this->image_ = vil_new_image_resource_of_view(image);
    }
    else
      this->image_ = NULL;
    break;
  }

  default:
    vcl_cerr << "I/O ERROR: vidpro1_image_storage::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}



//: Return a smart pointer to the image resource
vil_image_resource_sptr
dbspi_curvemap_storage::get_image()
{
  return image_;
}
//: Return dbspi_curve_map object
dbspi_curve_map *
dbspi_curvemap_storage::get_curvemap()
{
  return cmap_;
}


//: Store the image in the storage class
void
dbspi_curvemap_storage::set_image( const vil_image_resource_sptr& img )
{
  image_ = img;
}

//: Store the image in the storage class
void
dbspi_curvemap_storage::set_curvemap(  dbspi_curve_map * cmap )
{
  cmap_ = cmap;
}

