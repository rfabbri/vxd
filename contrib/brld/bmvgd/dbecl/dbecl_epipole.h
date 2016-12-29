// This is brcv/mvg/dbecl/dbecl_epipole.h
#ifndef dbecl_epipole_h_
#define dbecl_epipole_h_
//:
// \file
// \brief A representation of an epipole
// \author Matt Leotta, (mleotta@lems.brown.edu)
// \date 7/13/04
//
// \verbatim
//  Modifications
//  14 July 2004 - Dave Pacheco: added coepillinear, get_depth
// \endverbatim

#include <vcl_cmath.h>
#include <vgl/vgl_point_2d.h>
#include <vbl/vbl_ref_count.h>
#include <vul/vul_timestamp.h>

//: A representation of an epipole
// Contains member function to convert between image
// and epipolar coordinates
class dbecl_epipole : public vul_timestamp, public vbl_ref_count
{
 public:
  //: Constructor
  dbecl_epipole();
  dbecl_epipole(double u, double v);
  dbecl_epipole(const vgl_point_2d<double>& epipole);

  //: Update the epipole location
  void change_location( const vgl_point_2d<double>& pt );

  //: Return the location of the epipole
  const vgl_point_2d<double>& location() const { return location_; } 

  //================== Inline Coordinate Conversion Functions ==================

  //: Compute the distance from the epipole to a point (u,v)
  double distance(double u, double v) const;
  double distance(const vgl_point_2d<double>& u_v) const { return distance(u_v.x(), u_v.y()); }

  //: Compute the angle (in radians) from the epipole to a point (u,v)
  double angle(double u, double v) const;
  double angle(const vgl_point_2d<double>& u_v) const { return angle(u_v.x(), u_v.y()); }

  //: Conversion to epipolar coordinates from image coordinates
  void to_epi_coords(double u, double v, double &s, double &a) const;
  vgl_point_2d<double> to_epi_coords(const vgl_point_2d<double>& u_v) const;
  
  //: Conversion to image coordinates from epipolar coordinates
  void to_img_coords(double s, double a, double &u, double &v) const;
  vgl_point_2d<double> to_img_coords(const vgl_point_2d<double>& s_a) const;
  
  //============================================================================

  //: Get the depth of two points hypothesized to be the same
  //  3D point in two different frames.
  double depth(const vgl_point_2d<double>&, const vgl_point_2d<double>&, int, double) const;

  //: Take a 2D point * itsTime * itsDepth * time * speed 
  //  and get the 2D point in the given frame.
  vgl_point_2d<double> project(vgl_point_2d<double>, int, double, int, double);

  //: Are two points on the same epipolar line?
  bool coepilinear(const vgl_point_2d<double>&, const vgl_point_2d<double>&) const;



 private:
  //:the epipole location in image coordinates
  vgl_point_2d<double> location_;

};


//================== Inline Functions ==================

//: Computed the distance from the epipole to a point (u,v)
inline double
dbecl_epipole::distance(double u, double v) const
{
  double du = u-location_.x();
  double dv = v-location_.y();
  return vcl_sqrt(du*du + dv*dv);
}

//: Compute the angle (in radians) from the epipole to a point (u,v)
inline double
dbecl_epipole::angle(double u, double v) const
{
  double du = u-location_.x();
  double dv = v-location_.y();
  return vcl_atan2(dv,du);
}

//: Conversion to epipolar coordinates from image coordinates
inline void 
dbecl_epipole::to_epi_coords(double u, double v, double &s, double &a) const
{
  double du = u-location_.x();
  double dv = v-location_.y();
  s = vcl_sqrt(du*du + dv*dv);
  a = vcl_atan2(dv,du);
}

//: Conversion to epipolar coordinates from image coordinates
inline vgl_point_2d<double> 
dbecl_epipole::to_epi_coords(const vgl_point_2d<double>& u_v) const
{ 
  double s,a;
  to_epi_coords(u_v.x(),u_v.y(),s,a); 
  return vgl_point_2d<double>(s,a); 
}

//: Conversion to image coordinates from epipolar coordinates
inline void 
dbecl_epipole::to_img_coords(double s, double a, double &u, double &v) const
{
  u = location_.x() + s*vcl_cos(a);
  v = location_.y() + s*vcl_sin(a);
}

//: Conversion to image coordinates from epipolar coordinates
inline vgl_point_2d<double> 
dbecl_epipole::to_img_coords(const vgl_point_2d<double>& s_a) const
{
  double u,v;
  to_img_coords(s_a.x(),s_a.y(),u,v); 
  return vgl_point_2d<double>(u,v);
}


#endif // dbecl_epipole_h_
