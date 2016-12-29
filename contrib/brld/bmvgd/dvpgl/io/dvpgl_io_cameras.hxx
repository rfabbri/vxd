#include "dvpgl_io_cameras.h"
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/algo/vgl_rotation_3d.h>
#include <vnl/vnl_vector_fixed.h>
#include <vnl/io/vnl_io_vector_fixed.h>
#include <vnl/io/vnl_io_matrix_fixed.h>
#include <vgl/io/vgl_io_point_2d.h>
#include <vgl/io/vgl_io_point_3d.h>
#include <vsl/vsl_binary_loader.h>

#include <vpgl/vpgl_proj_camera.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vpgl/vpgl_affine_camera.h>
#include <vpgl/vpgl_rational_camera.h>
#include <vpgl/vpgl_local_rational_camera.h>
#include <vpgl/io/vpgl_io_proj_camera.h>
#include <vpgl/io/vpgl_io_perspective_camera.h>
#include <vpgl/io/vpgl_io_affine_camera.h>
#include <vpgl/io/vpgl_io_rational_camera.h>
#include <vpgl/io/vpgl_io_local_rational_camera.h>



//: Allows derived class to be loaded by base-class pointer.
//  A loader object exists which is invoked by calls
//  of the form "vsl_b_read(os,base_ptr);".  This loads derived class
//  objects from the stream, places them on the heap and
//  returns a base class pointer.
//  In order to work the loader object requires
//  an instance of each derived class that might be
//  found.  This function gives the model class to
//  the appropriate loader.
//template <class T>
//void vsl_add_to_binary_loader(vpgl_proj_camera<T> const& b)
//{
//  vsl_binary_loader<vpgl_proj_camera<T> >::instance().add(b);
//}

// dvpgl_calibration_matrix I/O -----------------------------------------------

template <class T> void 
b_read_dvpgl(vsl_b_istream &is, vpgl_calibration_matrix<T>* self)
{
  if (!is) return;
  assert(self);
  vcl_cerr << "[dvpgl_io] Warning: using legacy I/O format\n";

  vgl_point_2d<T> pp;

  short ver;
  vsl_b_read(is, ver);
  switch (ver)
  {
   case 1:
     T tmp;
     vsl_b_read(is, tmp);
     self->set_focal_length(tmp);

     vsl_b_read(is, pp);
     self->set_principal_point(pp);

     vsl_b_read(is, tmp);
     self->set_x_scale(tmp);
     vsl_b_read(is, tmp);
     self->set_y_scale(tmp);
     vsl_b_read(is, tmp);
     self->set_skew(tmp);
    break;
   default:
    vcl_cerr << "I/O ERROR: vpgl_calibration_matrix::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}

//: Binary save self to stream.
template <class T> void 
b_write_dvpgl(vsl_b_ostream &os, const vpgl_calibration_matrix<T>* self)
{
  vcl_cerr << "[dvpgl_io] Warning: using legacy I/O format\n";
  vsl_b_write(os, 1);  // matches latest in b_read_dvpgl
  vsl_b_write(os, self->focal_length());
  vsl_b_write(os, self->principal_point());
  vsl_b_write(os, self->x_scale());
  vsl_b_write(os, self->y_scale());
  vsl_b_write(os, self->skew());
}

//: Binary save
template <class T> void
vsl_b_write_dvpgl(vsl_b_ostream &os, const vpgl_calibration_matrix<T> * p)
{
  if (p==0) {
    vsl_b_write(os, false); // Indicate null pointer stored
  }
  else {
    vsl_b_write(os,true); // Indicate non-null pointer stored
    b_write_dvpgl(os, p);
  }
}


//: Binary load
template <class T> void
vsl_b_read_dvpgl(vsl_b_istream &is, vpgl_calibration_matrix<T>* &p)
{
  delete p;
  bool not_null_ptr;
  vsl_b_read(is, not_null_ptr);
  if (not_null_ptr) {
    p = new vpgl_calibration_matrix<T>();
    b_read_dvpgl(is, p);
  }
  else
    p = 0;
}

// dvpgl_perspective_camera I/O -----------------------------------------------

template <class T> void 
b_read_dvpgl(vsl_b_istream &is, vpgl_perspective_camera<T>* self)
{
  if (!is) return;
  vcl_cerr << "[dvpgl_io] Warning: using legacy I/O format\n";

  vnl_matrix_fixed<T,4,4> Rot;
  vgl_rotation_3d<T> vglRot;
  vnl_vector_fixed<T,4> q;
  vpgl_calibration_matrix<T> K;
  vgl_point_3d<T> camera_center;

  short ver;
  vsl_b_read(is, ver);
  switch (ver)
  {
   case 1:
     vcl_cerr << "[dvpgl_io] warning: camera i/o version 1 might also currently be the new vpgl I/O\n"; 
     // vpgl_proj_camera<T>::b_read(is);
     vsl_b_read(is, * (static_cast<vpgl_proj_camera<T> *>(self)));
     b_read_dvpgl(is, &K); // K.b_read(is);
     self->set_calibration(K);
     vsl_b_read(is, camera_center);
     self->set_camera_center(camera_center);
     vsl_b_read(is, Rot);
     self->set_rotation(vgl_rotation_3d<T>(vgl_h_matrix_3d<T>(Rot)));
     // TODO: if error, then try to load new core/vpgl/io style
    break;
   case 2:
     //vpgl_proj_camera<T>::b_read(is);
     vsl_b_read(is, *static_cast<vpgl_proj_camera<T> *>(self));
     b_read_dvpgl(is, &K); // K.b_read(is);
     self->set_calibration(K);
     vsl_b_read(is, camera_center);
     self->set_camera_center(camera_center);
     vsl_b_read(is, q);
     self->set_rotation(vgl_rotation_3d<T>(vnl_quaternion<T>(q)));
    break;
   default:
    vcl_cerr << "I/O ERROR: vpgl_persperctive_camera::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}

//: Binary save self to stream.
template <class T> void
b_write_dvpgl(vsl_b_ostream &os, const vpgl_perspective_camera<T>* self)
{
  vcl_cerr << "[dvpgl_io] Warning: using legacy I/O format\n";
  vsl_b_write(os, 2); // matches version 2 in b_read_dvpgl
  vsl_b_write(os, *static_cast<const vpgl_proj_camera<T> *>(self));
  b_write_dvpgl(os, &(self->get_calibration())); // K.b_read(is);
  vsl_b_write(os, self->get_camera_center());
  vsl_b_write(os, static_cast<vnl_vector_fixed<T,4> >(self->get_rotation().as_quaternion()));
}

//: Binary save
template <class T> void
vsl_b_write_dvpgl(vsl_b_ostream &os, const vpgl_perspective_camera<T> * p)
{
  if (p==0) {
    vsl_b_write(os, false); // Indicate null pointer stored
  }
  else{
    vsl_b_write(os,true); // Indicate non-null pointer stored
    b_write_dvpgl(os, p);
  }
}


//: Binary load
template <class T> void
vsl_b_read_dvpgl(vsl_b_istream &is, vpgl_perspective_camera<T>* &p)
{
  delete p;
  bool not_null_ptr;
  vsl_b_read(is, not_null_ptr);
  if (not_null_ptr) {
    p = new vpgl_perspective_camera<T>();
    b_read_dvpgl(is, p);
  }
  else
    p = 0;
}

// dvpgl_camera basepointer I/O -----------------------------------------------

//: Binary save camera to stream
template <class T>
void vsl_b_write_dvpgl(vsl_b_ostream & os, vpgl_camera<T>* const camera)
{
  vcl_cerr << "[dvpgl_io] Warning: using legacy I/O format\n";
  if ( camera->type_name() == "vpgl_proj_camera" ){
    // projective camera
    vpgl_proj_camera<T>* procam = static_cast<vpgl_proj_camera<T>*>(camera);
    vsl_b_write(os,procam->type_name());
    vsl_b_write(os,*procam); // uses new I/O as it is identical to the old one
  }
  else if ( camera->type_name() == "vpgl_perspective_camera" ) {
    // perspective camera
    vpgl_perspective_camera<T>* percam =
      static_cast<vpgl_perspective_camera<T>*>(camera);
    vsl_b_write(os,percam->type_name());
    vsl_b_write_dvpgl(os,percam); // use the old I/O
  }
  else if ( camera->type_name() == "vpgl_affine_camera" ) {
    // affine camera
    vpgl_affine_camera<T>* affcam =
      static_cast<vpgl_affine_camera<T>*>(camera);
    vsl_b_write(os,affcam->type_name());
    vsl_b_write(os,*affcam);
  }
  else if ( camera->type_name() == "vpgl_rational_camera" ) {
    // rational camera
    vpgl_rational_camera<T>* ratcam =
      static_cast<vpgl_rational_camera<T>*>(camera);
    vsl_b_write(os,ratcam->type_name());
    vsl_b_write(os,*ratcam);
  }
  else if ( camera->type_name() == "vpgl_local_rational_camera" ) {
    // local rational camera
    vpgl_local_rational_camera<T>* lratcam =
      static_cast<vpgl_local_rational_camera<T>*>(camera);
    vsl_b_write(os,lratcam->type_name());
    vsl_b_write(os,*lratcam);
  }
  else {
    vcl_cerr << "tried to write unknown camera type!\n";
    vcl_string cam_type("unknown");
    vsl_b_write(os,cam_type);
  }
  return;
}


//: Binary load camera from stream.
template <class T>
void vsl_b_read_dvpgl(vsl_b_istream & is, vpgl_camera<T>* &camera)
{
  vcl_string cam_type;
  vsl_b_read(is,cam_type);
  vcl_cerr << "[dvpgl_io] Warning: using legacy I/O format\n";

  if (cam_type == "vpgl_proj_camera") {
    // projective camera
    vpgl_proj_camera<T>* procam = new vpgl_proj_camera<T>();
    vsl_b_read(is,*procam);  // uses new I/O as it is identical to the old one
    camera = procam;
  }
  else if (cam_type == "vpgl_perspective_camera") {
    // perspective camera
    vpgl_perspective_camera<T>* percam = new vpgl_perspective_camera<T>();
    vsl_b_read_dvpgl(is,percam);  // uses old I/O
    camera = percam;
  }
  else if (cam_type == "vpgl_affine_camera") {
    // rational camera
    vpgl_affine_camera<T>* affcam = new vpgl_affine_camera<T>();
    vsl_b_read(is,*affcam);
    camera = affcam;
  }
  else if (cam_type == "vpgl_rational_camera") {
    // rational camera
    vpgl_rational_camera<T>* ratcam = new vpgl_rational_camera<T>();
    vsl_b_read(is,*ratcam);
    camera = ratcam;
  }
  else if (cam_type == "vpgl_local_rational_camera") {
    // rational camera
    vpgl_local_rational_camera<T>* lratcam=new vpgl_local_rational_camera<T>();
    vsl_b_read(is,*lratcam);
    camera = lratcam;
  }
  else if (cam_type == "unknown") {
    vcl_cerr << "cannot read camera of unknown type!\n";
  }
  else {
    vcl_cerr << "error reading vpgl_camera!\n";
  }
  return;
}



#undef DVPGL_IO_CAMERAS_INSTANTIATE
#define DVPGL_IO_CAMERAS_INSTANTIATE(T) \
template void vsl_b_read_dvpgl(vsl_b_istream &, vpgl_calibration_matrix<T >* &); \
template void vsl_b_write_dvpgl(vsl_b_ostream &, const vpgl_calibration_matrix<T > *); \
template void b_read_dvpgl(vsl_b_istream &, vpgl_calibration_matrix<T>* ); \
template void b_write_dvpgl(vsl_b_ostream &, const vpgl_calibration_matrix<T>* ); \
template void vsl_b_read_dvpgl(vsl_b_istream &, vpgl_perspective_camera<T >* &); \
template void vsl_b_write_dvpgl(vsl_b_ostream &, const vpgl_perspective_camera<T > *); \
template void b_read_dvpgl(vsl_b_istream &, vpgl_perspective_camera<T>* ); \
template void b_write_dvpgl(vsl_b_ostream &, const vpgl_perspective_camera<T>* );\
template void vsl_b_read_dvpgl(vsl_b_istream &, vpgl_camera<T >* &); \
template void vsl_b_write_dvpgl(vsl_b_ostream &, vpgl_camera<T >* const)

  
//template void vsl_add_to_binary_loader(vpgl_proj_camera<T > const& b);
