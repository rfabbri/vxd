// This is brl/vidpro1/storage/vidpro1_image_storage.cxx

//:
// \file

#include "vidpro1_image_storage.h"
#include <vil/io/vil_io_image_view.h>
#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_new.h>


//: Constructor
vidpro1_image_storage::vidpro1_image_storage()
{
}


//: Destructor
vidpro1_image_storage::~vidpro1_image_storage()
{
}


//: Return IO version number;
short 
vidpro1_image_storage::version() const
{
  return 3;
}


//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
vidpro1_image_storage::clone() const
{
  return new vidpro1_image_storage(*this);
}


//: Binary save self to stream.
void 
vidpro1_image_storage::b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  bpro1_storage::b_write(os);
  if(image_){
    vsl_b_write(os, true);
    vsl_b_write(os, int(image_->pixel_format()));
    switch(image_->pixel_format())
    {
    case VIL_PIXEL_FORMAT_BYTE:
      {  
        vil_image_view<vxl_byte> image = this->image_->get_view();
        vsl_b_write(os, image);
        break;
      }
    case VIL_PIXEL_FORMAT_FLOAT:
      {  
        vil_image_view<float> image = this->image_->get_view();
        vsl_b_write(os, image);
        break;
      }
    case VIL_PIXEL_FORMAT_DOUBLE:
      {  
        vil_image_view<double> image = this->image_->get_view();
        vsl_b_write(os, image);
        break;
      }    
    case VIL_PIXEL_FORMAT_UINT_32:
      {
        vil_image_view<vxl_uint_32> image = this->image_->get_view();
        vsl_b_write(os, image);
        break;
      }
    default:
      vcl_cerr << "I/O ERROR: This pixel format is not supported: " 
               << image_->pixel_format() << vcl_endl;
      return; 
    }
  }
  else
    vsl_b_write(os, false);
}


//: Binary load self from stream.
void 
vidpro1_image_storage::b_read(vsl_b_istream &is)
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
  case 3:
  {
    bpro1_storage::b_read(is);
    bool is_valid;
    vsl_b_read(is, is_valid);
    if(is_valid) {
      int format;
      vil_pixel_format pixel_format;
      vsl_b_read(is, format); pixel_format=vil_pixel_format(format);
      switch(pixel_format)
      {
      case VIL_PIXEL_FORMAT_BYTE:
        {
          vil_image_view<vxl_byte> image;
          vsl_b_read(is, image);
          this->image_ = vil_new_image_resource_of_view(image);
          break;
        }
      case VIL_PIXEL_FORMAT_FLOAT:
        {
          vil_image_view<float> image;
          vsl_b_read(is, image);
          this->image_ = vil_new_image_resource_of_view(image);
          break;
        }
      case VIL_PIXEL_FORMAT_DOUBLE:
        {
          vil_image_view<double> image;
          vsl_b_read(is, image);
          this->image_ = vil_new_image_resource_of_view(image);
          break;
        }
      case VIL_PIXEL_FORMAT_UINT_32:
        {
          vil_image_view<vxl_uint_32> image;
          vsl_b_read(is, image);
          this->image_ = vil_new_image_resource_of_view(image);
          break;
        }
      default:
        vcl_cerr << "I/O ERROR: This pixel format is not supported: " 
                 << pixel_format << vcl_endl;
        is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
        return; 
      }
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
vidpro1_image_storage::get_image()
{
  return image_;
}


//: Store the image in the storage class
void
vidpro1_image_storage::set_image( const vil_image_resource_sptr& img )
{
  image_ = img;
}

