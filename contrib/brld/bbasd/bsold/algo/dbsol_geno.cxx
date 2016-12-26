//:
// \file
// \author Ricardo Fabbri

#include "dbsol_geno.h"
#include <dbgl/algo/dbgl_eulerspiral.h>
#include <vnl/vnl_math.h>
#include <vcl_cmath.h>

//: 2nd order GENO (Geometric Non-Oscillatory) Interpolation
void dbsol_geno::
interpolate(dbsol_geno_curve_2d *c, vcl_vector<vsol_point_2d_sptr> const &pts, bool closed) 
{
   if (pts.size() <= 2){
      vcl_cerr << "geno interpolation currently needs at least 2 points";
      return;
   }

   if (!closed) {
      vcl_vector<dbgl_param_curve *> ints(pts.size()-1);
      unsigned i;
      bool fwd;

      c->stencil_.resize(ints.size());

      // do first interval separately
      ints[0] = new dbgl_arc(pts[0]->get_p(),pts[1]->get_p(), pts[2]->get_p());
      c->set_upper(0,true);

      // do middle intervals
      for (i=1; i<ints.size()-1; ++i) {
         ints[i] = interpolate_interval ( pts[i-1]->get_p(), pts[i]->get_p(), 
               pts[i+1]->get_p(), pts[i+2]->get_p(), &fwd);
         c->set_upper(i,fwd);
      }
      // do last interval separately
      ints[i] = new dbgl_arc(pts[i]->get_p(), pts[i+1]->get_p(), pts[i-1]->get_p());
      c->set_upper(i,false);

      c->make(ints,closed);
   } else {

      vcl_vector<dbgl_param_curve *> ints(pts.size());
      unsigned i;
      bool fwd;

      c->stencil_.resize(ints.size());

      // do first interval separately
      ints[0] = interpolate_interval ( pts.back()->get_p(), pts[0]->get_p(), 
            pts[1]->get_p(), pts[2]->get_p(), &fwd);
      c->set_upper(0,fwd);

      // do middle intervals
      for (i=1; i<ints.size()-2; ++i) {
         ints[i] = interpolate_interval ( pts[i-1]->get_p(), pts[i]->get_p(), 
               pts[i+1]->get_p(), pts[i+2]->get_p(), &fwd);
         c->set_upper(i,fwd);
      }

      // do last interval
      ints[i] = interpolate_interval ( pts[i-1]->get_p(), pts[i]->get_p(), 
      pts[i+1]->get_p(), pts[0]->get_p(), &fwd);
      c->set_upper(i,fwd);

      // close cycle
      ++i;
      ints[i] = interpolate_interval ( pts[i-1]->get_p(), pts[i]->get_p(), 
      pts[0]->get_p(), pts[1]->get_p(), &fwd);
      c->set_upper(i,fwd);
 
      c->make(ints,closed);
   }
}

//: 3nd order GENO, approximate algorithm:
// 1) Compute tangents at each sample point as mean of left and right intervals,
// using circular GENO; 
//  - if left and right tangents are too different, the estimate is not reliable; 
//  so we output a vector of tangent differences so you can choose whether to
//  discard that tangent or not; or whether to place shocks
//
// 2) For each tangent pair, get the Euler spiral
//
// TODO: should call interpolate3_from_tangents, to avoid repetition of code
void dbsol_geno::
interpolate3_approx(dbsol_geno_curve_2d *c, vcl_vector<vsol_point_2d_sptr> const &pts, bool closed) 
{
  if (pts.size() <= 2){
     vcl_cerr << "interpolation needs least 2 points\n";
     return;
  } if (closed) {
     vcl_cerr << "closed curves not yet supported\n";
     return;
  }

  unsigned i;

  double delta_angle_threshold=vnl_math::pi/2;

  dbsol_geno_curve_2d arc_geno;
  interpolate(&arc_geno,pts,closed);

  vcl_vector<double> tangents(pts.size());
  vcl_vector<bool> tangent_reliable(pts.size(),true);

  // --- TANGENTS ---
  tangents[0] = arc_geno[0].tangent_angle_at(0);
  for (i=1; i<pts.size()-1; ++i) {
    double angle0, angle1, mid; float delta;

    angle0 = arc_geno[i-1].tangent_angle_at(1);
    angle1 = arc_geno[i].tangent_angle_at(0);

    mid = (angle0 + angle1)/2;
    
    if (vcl_fabs(angle1 - angle0) > vnl_math::pi) {
      mid = mid + vnl_math::pi;
      delta = 2*vnl_math::pi - vcl_fabs(angle1 - angle0);
    } else
      delta = vcl_fabs(angle1 - angle0);

    if (delta > delta_angle_threshold)
      tangent_reliable[i] = false;
      
    tangents[i] = vcl_fmod(mid,2*vnl_math::pi);
  }

  tangents[i] = arc_geno[i-1].tangent_angle_at(1);

  /* 
  vcl_ofstream f_points, f_tangents;

  f_points.open("/tmp/points.dat");
  f_tangents.open("/tmp/tangents.dat");
  for (i=0; i<pts.size(); ++i) {
     f_points << pts[i]->x() << " " << pts[i]->y() << vcl_endl;
     f_tangents << tangents[i] << vcl_endl;
  }
  f_points.close();
  f_tangents.close();
  */

  // --- INTERPOLATION ---

  vcl_vector<dbgl_param_curve *> ints(pts.size()-1);

  /*
  for (i=0; i<ints.size(); ++i) {
    double angle0, angle1;
    if (tangent_reliable[i])
      angle0 = tangents[i];
    else
      angle0 = arc_geno[i].tangent_angle_at(0);

    if (tangent_reliable[i+1])
      angle1 = tangents[i+1];
    else 
      angle1 = arc_geno[i].tangent_angle_at(1);

    ints[i] = (dbgl_param_curve *)(new dbgl_eulerspiral(pts[i]->get_p(),angle0,pts[i+1]->get_p(),angle1));
  }
  */

  for (i=0; i<ints.size(); ++i) {
    double angle0, angle1;
    angle0 = tangents[i];
    angle1 = tangents[i+1];

    ints[i] = new dbgl_eulerspiral(pts[i]->get_p(),angle0,pts[i+1]->get_p(),angle1);
    /* Debugging code:
    if (i ==743 || i == 742 || i == 744) {
      vcl_cout << "I: " << i << vcl_endl;
      vcl_cout.precision(25);
      vcl_cout << "angle0: " << angle0 << "   angle1: " << angle1 << vcl_endl;
      vcl_cout << "ang_ints0: " << ints[i]->tangent_angle_at(0) << 
               "    ang_ints1: " << ints[i]->tangent_angle_at(1) << vcl_endl;

      vcl_cout << "points:" << *(pts[i]) << " ; " << *(pts[i+1]) << vcl_endl;
      vcl_cout << "Ints size: " << ints.size() << vcl_endl;
      vcl_cout << "Kdot: " << (reinterpret_cast<dbgl_eulerspiral*> (ints[i]))->gamma() << vcl_endl;
      vcl_cout << "K_start: " << ints[i]->curvature_at(0) << "   K_end:" << ints[i]->curvature_at(1);
      vcl_cout << "\n==========================\n\n\n" << vcl_endl;
    }
    */
  }

  c->make(ints,closed);

  c->order_ = 3;
  
  // dont care about stencil position in this implementation
  c->stencil_.resize(ints.size(),0);
}

//: 3nd order GENO, approximate algorithm, variant code:
//
// 1) Get a vector with tangent field 
//
// 2) For each tangent pair, get the Euler spiral
//
void dbsol_geno::
interpolate3_from_tangents(
    dbsol_geno_curve_2d *c, 
    vcl_vector<vsol_point_2d_sptr> const &pts, 
    vcl_vector<double> const &tangent_angles,
    bool closed) 
{
  if (pts.size() <= 2){
     vcl_cerr << "interpolation needs least 2 points\n";
     return;
  } if (closed) {
     vcl_cerr << "closed curves not yet supported\n";
     return;
  }

  unsigned i;

  // --- INTERPOLATION ---

  vcl_vector<dbgl_param_curve *> ints(pts.size()-1);

  for (i=0; i<ints.size(); ++i) {
    double angle0, angle1;
    angle0 = tangent_angles[i];
    angle1 = tangent_angles[i+1];

    ints[i] = new dbgl_eulerspiral(pts[i]->get_p(),angle0,pts[i+1]->get_p(),angle1);
  }

  c->make(ints,closed);

  c->order_ = 3;
  
  // dont care about stencil position in this implementation
  c->stencil_.resize(ints.size(),0);
}

