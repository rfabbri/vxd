// This is basic/dbgl/dbgl_area.hxx

#ifndef dbgl_area_txx_
#define dbgl_area_txx_

#include "dbgl_area.h"
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_vector_3d.h>



//: Computes the area of a triangle in 3D
template <class T> 
T dbgl_area_triangle(vgl_point_3d<T> const& p1,
                     vgl_point_3d<T> const& p2,
                     vgl_point_3d<T> const& p3)
{
  // The norm of the cross product vector is the area of the parallelogram they form.  
  // The triangle they form has half that area.
  vgl_vector_3d<T> v = cross_product(p2-p1, p3-p1);
  return T(v.length()/2);
}


#undef BGLD_AREA_INSTANTIATE
#define BGLD_AREA_INSTANTIATE(T) \
template T dbgl_area_triangle(vgl_point_3d<T> const&,vgl_point_3d<T> const&,vgl_point_3d<T> const&)

#endif // dbgl_area_txx_
