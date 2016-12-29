#ifndef dvpgl_proj_camera_vsl_h_
#define dvpgl_proj_camera_vsl_h_

#include <vpgl/vpgl_proj_camera.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vpgl/io/vpgl_io_proj_camera.h>
#include <vsl/vsl_binary_io.h>


//: Wrapper class on the new vpgl_perspective_camera to allow for old binary
// I/O
template <class T>
class dvpgl_proj_camera_vsl
{
 public:
  // ----------------- Constructors:----------------------

  dvpgl_proj_camera_vsl() : cam_(0) { }

  //: Copy constructor.
  dvpgl_proj_camera_vsl( const dvpgl_proj_camera_vsl& c ) :
      cam_(c.cam_)
  {}
  

  //: Binary save self to stream.
  virtual void b_write(vsl_b_ostream &os) const { 
    assert (cam_ != 0);
    vsl_b_write(os, *cam_);
  }

  //: Binary load self from stream.
  virtual void b_read(vsl_b_istream &is) {
    assert (cam_ != 0);
    vsl_b_read(is, *cam_);
  }

  //: IO version number
  short version() const {return 1;}

  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const { os << *this->get(); }

  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return vcl_string("vpgl_proj_camera"); }

  //: Return true if the argument matches the string identifying the class or any parent class
  virtual bool is_class(vcl_string const& cls) const
  { return cls==is_a() || cls==vcl_string("vpgl_camera"); }

  //: Return `this' if `this' is a vpgl_proj_camera, 0 otherwise
  // This is used by e.g. polymorphic binary i/o
  virtual vpgl_proj_camera<T> *cast_to_proj_camera() {return cam_;}
  virtual const vpgl_proj_camera<T> *cast_to_proj_camera() const {return cam_;}

  //: Return `this' if `this' is a vpgl_perspective_camera, 0 otherwise
  // This is used by e.g. the storage class
  // \todo code for affine camera and other children
  virtual vpgl_perspective_camera<T> *cast_to_perspective_camera() {return 0;}
  virtual const vpgl_perspective_camera<T> *cast_to_perspective_camera() const {return 0;}

  virtual ~dvpgl_proj_camera_vsl() { delete cam_; }

  vpgl_proj_camera<T> * get() { return cam_; }
  const vpgl_proj_camera<T> * get() const { return cam_; }
  void set(vpgl_proj_camera<T> *c) { assert (c); cam_ = c; }

  //: Clone `this': creation of a new object and initialization
  //  See Prototype pattern
  virtual dvpgl_proj_camera_vsl<T>* clone(void) const { return new dvpgl_proj_camera_vsl<T>(*this); }


 protected:
  vpgl_proj_camera<T> *cam_;
};

//: Allows derived class to be loaded by base-class pointer
//  A loader object exists which is invoked by calls
//  of the form "vsl_b_read(os,base_ptr)".  This loads derived class
//  objects from the disk, places them on the heap and
//  returns a base class pointer.
//  In order to work the loader object requires
//  an instance of each derived class that might be
//  found.  This function gives the model class to
//  the appropriate loader.
template <class T>
void vsl_add_to_binary_loader(dvpgl_proj_camera_vsl<T> const& b);


#endif // dvpgl_proj_camera_vsl_h_
