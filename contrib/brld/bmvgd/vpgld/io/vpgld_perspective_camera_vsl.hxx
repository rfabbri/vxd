// This is core/vpgl/vpgl_perspective_camera.hxx
#ifndef vpgld_perspective_camera_vsl_hxx_
#define vpgld_perspective_camera_vsl_hxx_
//:
// \file

#include "vpgld_perspective_camera_vsl.h"


//: Binary save
template <class T> void
vsl_b_write(vsl_b_ostream &os, const vpgld_perspective_camera_vsl<T> * p)
{
  if (p==0) {
    vsl_b_write(os, false); // Indicate null pointer stored
  }
  else{
    vsl_b_write(os,true); // Indicate non-null pointer stored
    p->b_write(os);
  }
}


//: Binary load
template <class T> void
vsl_b_read(vsl_b_istream &is, vpgld_perspective_camera_vsl<T>* &p)
{
  delete p;
  bool not_null_ptr;
  vsl_b_read(is, not_null_ptr);
  if (not_null_ptr) {
    p = new vpgld_perspective_camera_vsl<T>();
    p->b_read(is);
  }
  else
    p = 0;
}


// Code for easy instantiation.
#undef VPGLD_PERSPECTIVE_CAMERA_VSL_INSTANTIATE
#define VPGLD_PERSPECTIVE_CAMERA_VSL_INSTANTIATE(T) \
template class vpgld_perspective_camera_vsl<T >; \
template void vsl_b_read(vsl_b_istream &is, vpgld_perspective_camera_vsl<T >* &p); \
template void vsl_b_write(vsl_b_ostream &os, const vpgld_perspective_camera_vsl<T > * p);


#endif // vpgld_perspective_camera_vsl_hxx_
