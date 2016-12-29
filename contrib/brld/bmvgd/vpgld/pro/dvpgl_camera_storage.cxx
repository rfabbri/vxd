//:
// \file

#include "dvpgl_camera_storage.h"
#include <vpgl/vpgl_perspective_camera.h>
#include <vpgl/vpgl_camera.h>
#include <vpgl/io/vpgl_io_proj_camera.h>
#include <vpgl/io/vpgl_io_camera.h>
#include <dvpgl/io/dvpgl_proj_camera_vsl.h>
#include <dvpgl/io/dvpgl_perspective_camera_vsl.h>

bool dvpgl_camera_storage::registered_ = false;

//: Constructor
dvpgl_camera_storage::dvpgl_camera_storage()
  : camera_(0)
{
}

//: Destructor
dvpgl_camera_storage::~dvpgl_camera_storage()
{
  if (camera_) delete camera_;
}


//: Create a copy of the object on the heap.
// The caller is responsible for deletion
bpro1_storage* 
dvpgl_camera_storage::clone() const
{
  return new dvpgl_camera_storage(*this);
}


//: Return IO version number;
short 
dvpgl_camera_storage::version() const
{
  return 1;
}


//: Binary save self to stream.
void 
dvpgl_camera_storage::b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  bpro1_storage::b_write(os);
  vsl_b_write(os, camera_); 
  // to use the new one, uncomment:
  // vsl_b_write(os, static_cast<vpgl_camera<double> *> (camera_) ); 
  // vsl_b_write_dvpgl(os, camera_);  // stick to old I/O
}


//: Binary load self from stream.
void 
dvpgl_camera_storage::b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch(ver)
  {
  case 1:
  {
    bpro1_storage::b_read(is);
    vsl_b_read(is, camera_);  
      //    to use the new one, can try this, but assumes type vcl_string in
      //    file, which is not the case for the legacy I/O
//    vpgl_camera<double> *basecam;
//    vsl_b_read(is, basecam); // or vsl_b_read_dvpgl
//    camera_ = static_cast<vpgl_proj_camera<double> *> (basecam);
    break;
  }

  default:
    vcl_cerr << "I/O ERROR: dvpgl_camera_storage::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}


//: Register camera types for I/O
void 
dvpgl_camera_storage::register_binary_io() const
{
  if(!registered_){
    vsl_add_to_binary_loader(dvpgl_proj_camera_vsl<double>());
    vsl_add_to_binary_loader(dvpgl_perspective_camera_vsl<double>());
    // TODO add affine and other specializations here to support their IO when available
    registered_ = true;
  }
}

