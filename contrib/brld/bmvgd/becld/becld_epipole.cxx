// This is bmvgd/becld/becld_epipole.cxx
//:
// \file

#include "becld_epipole.h"
#include <vgl/vgl_line_segment_2d.h>
#include <vgl/vgl_distance.h>


//: Constructor
becld_epipole::becld_epipole()
 : location_(0.0,0.0)
{
}

//: Constructor
becld_epipole::becld_epipole(double u, double v)
 : location_(u,v)
{
}

//: Constructor
becld_epipole::becld_epipole(const vgl_point_2d<double>& epipole)
 : location_(epipole)
{
}


//: Update the epipole location
void
becld_epipole::change_location( const vgl_point_2d<double>& pt )
{
  location_ = pt;
  this->touch();
}


double
becld_epipole::depth(const vgl_point_2d<double>& p1, 
                        const vgl_point_2d<double>& p2, int time, double speed) const
{
  // this isn't really tau, but everything's just scaled anyway.
  const double tau = 1; 
  //assert(coepilinear(p1,p2));
  return (- tau * speed * time ) / 
                  (1 - (vgl_distance(location_, p1) / 
                        vgl_distance(location_, p2)) );
}


//: Take a 2D point * itsTime * itsDepth * time and 
//  get the 2D point in the given frame.
vgl_point_2d<double> becld_epipole::project(vgl_point_2d<double> orig, int t0, 
                             double depth, int t, double speed) {
  vgl_point_2d<double> orig_epi = to_epi_coords(orig);
  const int tau = 1; // See above
  double num = orig_epi.x();
  double gamma = - tau * speed / depth;
  double denom = 1 - gamma*(t-t0);
  return to_img_coords(vgl_point_2d<double>(num/denom, orig_epi.y()));
}


bool
becld_epipole::coepilinear(const vgl_point_2d<double>& p1,
                          const vgl_point_2d<double>& p2) const {
  // FIXME- should we add a tolerance value?
  vgl_line_segment_2d<double> epi1(location_,p1);
  vgl_line_segment_2d<double> epi2(location_,p2);
  return epi1.slope_radians() == epi2.slope_radians();
}
