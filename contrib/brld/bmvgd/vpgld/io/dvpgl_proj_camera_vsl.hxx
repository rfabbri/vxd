#ifndef dvpgl_proj_camera_vsl_hxx_
#define dvpgl_proj_camera_vsl_hxx_

#include "dvpgl_proj_camera_vsl.h"
#include <vsl/vsl_binary_loader.h>


//: Allows derived class to be loaded by base-class pointer.
//  A loader object exists which is invoked by calls
//  of the form "vsl_b_read(os,base_ptr);".  This loads derived class
//  objects from the stream, places them on the heap and
//  returns a base class pointer.
//  In order to work the loader object requires
//  an instance of each derived class that might be
//  found.  This function gives the model class to
//  the appropriate loader.
template <class T>
void vsl_add_to_binary_loader(dvpgl_proj_camera_vsl<T> const& b)
{
  vsl_binary_loader<dvpgl_proj_camera_vsl<T> >::instance().add(b);
}

#undef DVPGL_PROJ_CAMERA_VSL_INSTANTIATE
#define DVPGL_PROJ_CAMERA_VSL_INSTANTIATE(T) \
template class dvpgl_proj_camera_vsl<T >; \
template void vsl_add_to_binary_loader(dvpgl_proj_camera_vsl<T > const& b);



#endif // dvpgl_proj_camera_vsl_hxx_
