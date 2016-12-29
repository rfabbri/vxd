#ifndef dvpgl_io_proj_camera_h_
#define dvpgl_io_proj_camera_h_
//:
// \file
// \brief the dvpgl development utilities for binary I/O. Contains legacy and
// experimental I/O
// \author Ricardo Fabbri
// \date  April 2 2015
//
// vpgl_io_proj_camera is identical to the old one, so we don't have a _dvpgl
// vesion
//
#include <vsl/vsl_binary_io.h>
#include <vsl/vsl_binary_loader.h>
#include <vpgl/vpgl_calibration_matrix.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vpgl/io/vpgl_io_camera.h>
#include <vpgl/io/vpgl_io_proj_camera.h>
#include <vpgl/io/vpgl_io_perspective_camera.h>

// dvpgl_calibration_matrix I/O -----------------------------------------------

//: Binary load self from stream.
template <class T> void 
b_read_dvpgl(vsl_b_istream &is, vpgl_calibration_matrix<T>* tis);

//: Binary save self to stream.
template <class T> void 
b_write_dvpgl(vsl_b_ostream &os, const vpgl_calibration_matrix<T>* tis);

//: Binary save
template <class T> void 
vsl_b_write_dvpgl(vsl_b_ostream &os, const vpgl_calibration_matrix<T>* p);

//: Binary read
template <class T> void 
vsl_b_read_dvpgl(vsl_b_istream &is, vpgl_calibration_matrix<T>* &p);


// dvpgl_perspective_camera I/O -----------------------------------------------

//: Binary load self from stream.
template <class T> void 
b_read_dvpgl(vsl_b_istream &is, vpgl_perspective_camera<T>* tis);

//: Binary save self to stream.
template <class T> void 
b_write_dvpgl(vsl_b_ostream &os, const vpgl_perspective_camera<T>* tis);

//: Binary save
template <class T> void 
vsl_b_write_dvpgl(vsl_b_ostream &os, const vpgl_perspective_camera<T>* p);

//: Binary read
template <class T> void 
vsl_b_read_dvpgl(vsl_b_istream &is, vpgl_perspective_camera<T>* &p);

// dvpgl_camera basepointer I/O -----------------------------------------------

//: Binary save camera to stream
template<class T>
void vsl_b_write_dvpgl(vsl_b_ostream & os, vpgl_camera<T>* const camera);

//: Binary load camera from stream.
template<class T>
void vsl_b_read_dvpgl(vsl_b_istream & is, vpgl_camera<T>* &camera);


//: Allows derived class to be loaded by base-class pointer
//  A loader object exists which is invoked by calls
//  of the form "vsl_b_read(os,base_ptr)".  This loads derived class
//  objects from the disk, places them on the heap and
//  returns a base class pointer.
//  In order to work the loader object requires
//  an instance of each derived class that might be
//  found.  This function gives the model class to
//  the appropriate loader.
//template <class T>
//void vsl_add_to_binary_loader(vpgl_proj_camera<T> const& b);

// some missing functions from the new vpgl that were used in old I/O ----------

inline const vpgl_perspective_camera<double> *
dvpgl_cast_to_perspective_camera(const vpgl_proj_camera<double> *cam)
{
  if (cam->type_name() != "vpgl_perspective_camera")
    return 0;
  return static_cast<const vpgl_perspective_camera<double> *> (cam);
}

#endif
