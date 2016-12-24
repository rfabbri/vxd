// This is dbgl/algo/dbgl_fit_circ_arc_spline.h
#ifndef dbgl_fit_circ_arc_spline_h_
#define dbgl_fit_circ_arc_spline_h_

//:
// \file
// \brief Functions to fit a circular arc spline to a curve
//        
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date April 11, 2008
//
// \verbatim
//  Modifications
// \endverbatim


#include <dbgl/algo/dbgl_circ_arc.h>
#include <vcl_vector.h>


//: Interpolate a set of points with a circular arc spline
bool dbgl_fit_circ_arc_spline_to_polyline(vcl_vector<dbgl_circ_arc >& arc_list,
                                 const vcl_vector<vgl_point_2d<double > >& pts,
                                 double tolerance = 1.0);

//: Interpolate a polygon with circular-arc polygon, i.e., a closed smooth contour
// consisting circular arc segments
bool dbgl_fit_circ_arc_spline_to_polygon(vcl_vector<dbgl_circ_arc >& arc_list,
                                 const vcl_vector<vgl_point_2d<double > >& pts,
                                 double tolerance = 1.0);




#endif // dbgl/algo/dbgl_fit_circ_arc_spline.h


