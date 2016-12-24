// This is bgld_arc_algo.h
#ifndef bgld_arc_algo_h
#define bgld_arc_algo_h
//:
//\file
//\brief Algorithms that act on bgld_arcs
//\author Based on original code by  Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
// Some content is based on C code by Kwun Han (@lems.brown.edu)
//\date 03/08/2005 07:31:18 PM EST
//

#include <bgld/bgld_arc.h>
#include <vgl/vgl_point_2d.h>
#include <vcl_cmath.h>
#include <vcl_limits.h>

class bgld_arc_algo
{

public:
   //: intersection of two bgld_arc's
   static unsigned compute_intersections(const bgld_arc &arc1, const bgld_arc &arc2, 
            vgl_point_2d<double> *intercept1, vgl_point_2d<double> *intercept2);

private:

   //: intersection of two arcs with non-zero curvature
   static unsigned 
   compute_inters_aa_bkk(const bgld_arc &arc1, const bgld_arc &arc2, 
            vgl_point_2d<double> *intercept1, vgl_point_2d<double> *intercept2);
   
   //: finds the two points where a line (arc of virtually zero curvature)
   // intersects an arc
   static unsigned
   compute_inters_al(const bgld_arc &arc, const bgld_arc &line, 
         vgl_point_2d<double> *intercept1, vgl_point_2d<double> *intercept2);

public:

   //: compute the radius of the circular arc through three points [p1-----p2----p3]
   static double
   compute_arc_radius_from_three_points(vgl_point_2d<double> p1,vgl_point_2d<double> p2, vgl_point_2d<double> p3);
};


#endif // bgld_arc_algo_h

