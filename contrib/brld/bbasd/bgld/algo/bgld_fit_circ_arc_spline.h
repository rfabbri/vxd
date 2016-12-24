// This is bgld/algo/bgld_fit_circ_arc_spline.h
#ifndef bgld_fit_circ_arc_spline_h_
#define bgld_fit_circ_arc_spline_h_

//:
// \file
// \brief Functions to fit a circular arc spline to a curve
//        
// \author Based on original code by  Nhon Trinh ( @lems.brown.edu)
// \date April 11, 2008
//
// \verbatim
//  Modifications
// \endverbatim


#include <bgld/algo/bgld_circ_arc.h>
#include <vcl_vector.h>


//: Interpolate a set of points with a circular arc spline
bool bgld_fit_circ_arc_spline_to_polyline(vcl_vector<bgld_circ_arc >& arc_list,
                                 const vcl_vector<vgl_point_2d<double > >& pts,
                                 double tolerance = 1.0);

//: Interpolate a polygon with circular-arc polygon, i.e., a closed smooth contour
// consisting circular arc segments
bool bgld_fit_circ_arc_spline_to_polygon(vcl_vector<bgld_circ_arc >& arc_list,
                                 const vcl_vector<vgl_point_2d<double > >& pts,
                                 double tolerance = 1.0);




#endif // bgld/algo/bgld_fit_circ_arc_spline.h


