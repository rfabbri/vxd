// This is dbgl_arc_algo.h
#ifndef dbgl_arc_algo_h
#define dbgl_arc_algo_h
//:
//\file
//\brief Algorithms that act on dbgl_arcs
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
// Some content is based on C code by Kwun Han (kwh@lems.brown.edu)
//\date 03/08/2005 07:31:18 PM EST
//

#include <dbgl/dbgl_arc.h>
#include <vgl/vgl_point_2d.h>
#include <vcl_cmath.h>
#include <vcl_limits.h>

class dbgl_arc_algo
{

public:
   //: intersection of two dbgl_arc's
   static unsigned compute_intersections(const dbgl_arc &arc1, const dbgl_arc &arc2, 
            vgl_point_2d<double> *intercept1, vgl_point_2d<double> *intercept2);

private:

   //: intersection of two arcs with non-zero curvature
   static unsigned 
   compute_inters_aa_bkk(const dbgl_arc &arc1, const dbgl_arc &arc2, 
            vgl_point_2d<double> *intercept1, vgl_point_2d<double> *intercept2);
   
   //: finds the two points where a line (arc of virtually zero curvature)
   // intersects an arc
   static unsigned
   compute_inters_al(const dbgl_arc &arc, const dbgl_arc &line, 
         vgl_point_2d<double> *intercept1, vgl_point_2d<double> *intercept2);

public:

   //: compute the radius of the circular arc through three points [p1-----p2----p3]
   static double
   compute_arc_radius_from_three_points(vgl_point_2d<double> p1,vgl_point_2d<double> p2, vgl_point_2d<double> p3);
};


#endif // dbgl_arc_algo_h

