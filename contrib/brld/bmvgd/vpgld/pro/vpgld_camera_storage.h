#ifndef vpgld_camera_storage_h_
#define vpgld_camera_storage_h_

//:
// \file
// \brief The vidpro1 storage class for projective cameras.
// \author Based on original code by  Thomas Pollard
// \date 7/14/05
//
// \verbatim
//  Modifications
//  7/20/2005  Ricardo Fabbri  Improved binary I/O 
//  7/20/2005  Ricardo Fabbri  It now works as a pointer to accomodate for all
//  camera classes
// \endverbatim
//
// \verbatim
//
// \endverbatim


#include <vcl_string.h>
#include <bpro1/bpro1_storage.h>
#include "vpgld_camera_storage_sptr.h"
#include <vpgl/vpgl_proj_camera.h>
// #include <vpgld/io/vpgld_io_proj_camera.h>
#include <vpgld/io/vpgld_proj_camera_vsl.h>

class vpgld_camera_storage : public bpro1_storage {

public:

  vpgld_camera_storage();
  virtual ~vpgld_camera_storage();
  virtual vcl_string type() const { return "vpgl camera"; }

  //: Return IO version number;
  short version() const;
  
  //: Binary save self to stream. Uses old vpgld instead of vpgl
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream. Uses old vpgld instead of vpgl
  void b_read(vsl_b_istream &is);

  //: Register camera types for I/O
  void register_binary_io() const;
  
  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "vpgld_camera_storage"; }

  //: Sets the camera pointer. Storage must be allocated by the user and will be
  // deleted by _this_ class. In the future, this might be replaced by a smart pointer.
  void set_camera( vpgl_proj_camera<double>* camera ){ camera_->set(camera); }

  const vpgl_proj_camera<double> *get_camera(){ return camera_->get(); }


protected:
  // storage should be able to hold any camera derived from proj_camera
  vpgld_proj_camera_vsl<double>  *camera_;
private:
  //: Is the binary I/O registered
  static bool registered_;
};


//: Create a smart-pointer to a vpgld_camera_storage.
struct vpgld_camera_storage_new : public vpgld_camera_storage_sptr
{
  typedef vpgld_camera_storage_sptr base;

  vpgld_camera_storage_new() : base( new vpgld_camera_storage() ) { }
};

#endif //vpgld_camera_storage_h_
