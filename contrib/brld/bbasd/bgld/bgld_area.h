// This is basic/bgld/bgld_area.h

#ifndef bgld_area_h_
#define bgld_area_h_

//:
// \file
// \brief Functions to compute area of geometric objects
// 
// \author Based on original code by  Nhon Trinh (@lems.brown.edu)
// \date 06/15/2007
//
// \verbatim
//  Modifications:
// \endverbatim


#include <vgl/vgl_fwd.h>

//: Computes the area of a triangle in 3D
// \sa bgld_area
template <class T> T bgld_area_triangle(vgl_point_3d<T> const& p1,
                                        vgl_point_3d<T> const& p2,
                                        vgl_point_3d<T> const& p3);


#define DBGL_AREA_INSTANTIATE(T) extern "please include vgl/vgl_area.hxx instead"

#endif // vgl_area_h_
