#ifndef dvpgl_perspective_camera_vsl_h_
#define dvpgl_perspective_camera_vsl_h_

#include <vpgl/vpgl_perspective_camera.h>
#include <dvpgl/io/dvpgl_proj_camera_vsl.h>
#include <dvpgl/io/dvpgl_io_cameras.h>


//: Wrapper class on the new vpgl_perspective_camera to allow for old binary
// I/O
template <class T>
class dvpgl_perspective_camera_vsl : public dvpgl_proj_camera_vsl<T>
{
 public:
  //: Default constructor
  dvpgl_perspective_camera_vsl() {}

  //: Destructor
  virtual ~dvpgl_perspective_camera_vsl() {}

  //: Return `this' if `this' is a vpgl_perspective_camera, 0 otherwise
  // This is used by e.g. the storage class
  // \todo code for affine camera and other children
  virtual vpgl_perspective_camera<T> *cast_to_perspective_camera() {return static_cast<vpgl_perspective_camera<T> *> (this->get());}
  virtual const vpgl_perspective_camera<T> *cast_to_perspective_camera() const {return static_cast<const vpgl_perspective_camera<T> *> (this->get());}

  //: Binary save self to stream.
  virtual void b_write(vsl_b_ostream &os) const {
    assert(this->get() != 0);
    b_write_dvpgl(os, cast_to_perspective_camera());
  }

  //: Binary load self from stream.
  virtual void b_read(vsl_b_istream &is)  {
    assert(this->get() != 0);
    b_read_dvpgl(is, cast_to_perspective_camera());
  }

  //: IO version number
  short version() const {return 2;}

  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const { os << *this->get(); }

  //: Return a platform independent string identifying the class.
  // This is used by e.g. polymorphic binary i/o
  virtual vcl_string is_a() const { return vcl_string("vpgl_perspective_camera"); }

  //: Return true if the argument matches the string identifying the class or any parent class
  virtual bool is_class(vcl_string const& cls) const
  { return cls==is_a() || dvpgl_proj_camera_vsl<T>::is_class(cls); }

};

//: Binary save
template <class T>
void vsl_b_write(vsl_b_ostream &os, const dvpgl_perspective_camera_vsl<T>* p);


//: Binary read
template <class T>
void vsl_b_read(vsl_b_istream &is, dvpgl_perspective_camera_vsl<T>* &p);


#endif // dvpgl_perspective_camera_vsl_h_
