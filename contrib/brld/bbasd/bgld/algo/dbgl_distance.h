// This is basic/dbgl/algo/dbgl_distance.h

#ifndef dbgl_distance_h_
#define dbgl_distance_h_

//:
// \file
// \brief Algorithms to compute shortest distance between geometry objects
// vgl already has functions to compute distance from a point to other objects such as line, polygon, etc
// This class will add to the function list, i.e. compute distance between line segments, line-arc, arc-arc, etc.
//
// \author Nhon Trinh ( ntrinh@lems.brown.edu)
// \date 07/13/05
//
// \verbatim  
//  Modifications:
//    Nhon Trinh       7/13/2005       Initial version
//    Ming-Ching Chang Mar 07, 2007    Add point-to-triangle distance computation.
//  
// \endverbatim

#include <vnl/vnl_math.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_distance.h>

#include <dbgl/algo/dbgl_intersect.h>


//#######################################################################
// Point to Triangle Distance

//: Compute the foot point F of a point P on triangle ABC.
//  Also return the barycentric coord. of F.
//  t: dist(PF), (u, v, 1-u-v): barycentric coord of F on triangle ABC.
bool dbgl_pt_fpt_tri (const vgl_point_3d<double>& P, const vgl_point_3d<double>& A,
                      const vgl_point_3d<double>& B, const vgl_point_3d<double>& C,
                      vgl_point_3d<double>& F, double& t, double& u, double& v);

//: Compute the closest distance d of P to ABC:
//  If the foot point F is inside ABC, d = PF, F = G.
//  Otherwise d = PG, where G is on the boundary of ABC.
double dbgl_pt_tri_dist_3d (const vgl_point_3d<double>& P, const vgl_point_3d<double>& A,
                            const vgl_point_3d<double>& B, const vgl_point_3d<double>& C,
                            vgl_point_3d<double>& G);


//#######################################################################

//: A set of static functions to compute distance between geometric objects
class dbgl_distance
{
public:
  ~dbgl_distance(){};
  
  ////------------------------------------------------------------------------
  ////: Return distance between two points
  //static double point_point(const vgl_point_2d<double >& p1,
  //  vgl_point_2d<double >& p2);

  
  //--------------------------------------------------------------------------
  //: Return shortest distance between two line segments.
  static double lineseg_lineseg(const vgl_point_2d<double >& line1_p1,
    const vgl_point_2d<double >& line1_p2,
    const vgl_point_2d<double >& line2_p1,
    const vgl_point_2d<double >& line2_p2);


  ////: Return shortest distance between a line segment and a circular arc
  //// `line_p1' and 'line_p2' are two end points of line segments
  //// `arc_p1' and `arc_p2' are two end points of circular arc
  //// `curvature' is signed curvature of the circular arc, going in the direction
  //// from `arc_p1' to `arc_p2'
  //// If given, ratio1 and ratio2 will indicate the positions of the 
  //// two points(line_p and arc_p) on line segment and arc that correspond to 
  //// the shortest distance.
  //// ratio_line is the ratio between (signed) distance from `line_p1' to
  //// `line_p' and the length of line segment. 
  //// ratio_arc is the ratio between the (signed) arclength from `arc_p1' to 
  //// `arc_p' and the arclength of the circular arc
  //static double lineseg_arc(const vgl_point_2d<double >& line_p1,
  //  const vgl_point_2d<double >& line_p2,
  //  const vgl_point_2d<double >& arc_p1,
  //  const vgl_point_2d<double >& arc_p2,
  //  double curvature,
  //  double *ratio_line = 0,
  //  double *ratio_arc = 0);

  //--------------------------------------------------------------------------
  
  //:  Computes orthogonal distances between a point (x1,y1) and a point-tangent given by
  //(x2,y2,theta2), where theta2 is the tangential orientation with respect to x-axis. In computer
  //vision terms, this function computes distances d(E1,E2) between two edges E1 (\p x1, \p x2, \p
  //theta1) and E2 (\p x1, \p x2, \p theta 2).  The distances are asymmetric. d_e is the Euclidean
  //distance between E1 and E2, d_n is the normal distance (distance measured orthogonally to E2's
  //direction), and d_t is the tangential distance (distance measured along E2's direction). These
  //are related by: d_e = d_t^2 + d_n^2.
  //
  // \param[in] theta2 : angle of edge 2 from positive x-axis towards positive y-axis.
  //
  // \authors Vishal Jain and Ricardo Fabbri
  //
  static void projected_distance (double x1,double y1,double x2,double y2,double theta2, double
      *d_e_ptr, double *d_p_ptr, double *d_t_ptr);

  //: Same as projected_distance, but returns a weighted distance d_w.
  // The returned d_w  is to be fed into exp() function to obtain a Gaussian-weighted distance
  static double 
  weighted_projected_distance (double x1,double y1,double theta1, double x2,double y2,double theta2, double *d_e_ptr, double *d_p_ptr, double *d_t_ptr);


private:
  dbgl_distance(){};
};

//--------------------------------------------------------------------------

//:  Computes orthogonal distances between a point (x1,y1) and a point-tangent given by
//(x2,y2,theta2), where theta2 is the tangential orientation with respect to x-axis. In computer
//vision terms, this function computes distances d(E1,E2) between two edges E1 (\p x1, \p x2, \p
//theta1) and E2 (\p x1, \p x2, \p theta 2).  The distances are asymmetric. d_e is the Euclidean
//distance between E1 and E2, d_n is the normal distance (distance measured orthogonally to E2's
//direction), and d_t is the tangential distance (distance measured along E2's direction). These
//are related by: d_e = d_t^2 + d_n^2.
//
// \param[in] theta2 : angle of edge 2 from x-axis
//
// \authors Vishal Jain and Ricardo Fabbri
//
inline void dbgl_distance:: 
projected_distance (
    double x1,double y1,
    double x2,double y2,double theta2, 
    double *d_e_ptr, double *d_n_ptr, double *d_t_ptr)
{

  double d_e=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);

  double startx2=x2-vcl_sin(theta2);
  double starty2=y2+vcl_cos(theta2);
  double endx2  =x2+vcl_sin(theta2);
  double endy2  =y2-vcl_cos(theta2);

  double d_p= vcl_fabs((y1-starty2)*(endx2-startx2) - (x1-startx2)*(endy2-starty2))/2.0;

  *d_e_ptr = vcl_sqrt(d_e);
  *d_t_ptr = d_p;
  double d_n = d_e - d_p*d_p;
  if (d_n <= 0)
    *d_n_ptr = 0;
  else
    *d_n_ptr = vcl_sqrt(d_n);
}

//: Same as projected_distance, but returns a weighted distance d_w.
// The returned d_w  is to be fed into exp() function to obtain a Gaussian-weighted distance
inline double dbgl_distance::
weighted_projected_distance(
    double x1,double y1,double theta1,
    double x2,double y2,double theta2, 
    double *d_e_ptr, double *d_n_ptr, double *d_t_ptr)
{
  double sigma_e=5.0;
  double sigma_p=4.0;
  double sigma_t=vnl_math::pi/6;

  double d_theta=(theta1-theta2)*(theta1-theta2);

  projected_distance( x1, y1, x2, y2, theta2, d_e_ptr,  d_n_ptr,  d_t_ptr);
  const double &d_e = *d_e_ptr;
  const double &d_p = *d_t_ptr;

  return (d_e/(2*sigma_e*sigma_e)+d_p*d_p/(2*sigma_p*sigma_p)+d_theta/(2.0*sigma_t*sigma_t));
}


#endif // dbgl_distance_h_
