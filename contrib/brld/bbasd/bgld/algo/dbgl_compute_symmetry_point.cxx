// This is file basic/dbgl/algo/dbgl_compute_symmetry_point.cxx

//:
// \file


#include "dbgl_compute_symmetry_point.h"
#include <dbnl/dbnl_solve_trigonometric_equation.h>
#include <vnl/vnl_math.h>



//------------------------------------------------------------------------------
//: Compute symmetry point on a right boundary line, given a left boundary point-tangent
// Tangent orientation follows Giblin and Kimia (PAMI'03) convention, i.e.,
// right tangent points forward
// left tangent points backward
void dbgl_compute_symmetry_point_on_line(const vgl_point_2d<double >& right_line_start,
                                         const vgl_point_2d<double >& right_line_end,
                                         const vgl_point_2d<double >& left_bnd_pt,
                                         const vgl_vector_2d<double >& left_bnd_tangent,
                                         vcl_vector<double >& signed_distance_from_line_start)
{
  // sanitize storage
  signed_distance_from_line_start.clear();


  // Let A and D be the end points of the line segment
  double xA = right_line_start.x();
  double yA = right_line_start.y();
  vgl_vector_2d<double > tA =normalized(right_line_end-right_line_start);
  double sinA = tA.y();
  double cosA = tA.x();

  double xB = left_bnd_pt.x();
  double yB = left_bnd_pt.y();
  vgl_vector_2d<double > tB = normalized(left_bnd_tangent);
  double sinB = tB.y();
  double cosB = tB.x();

  // solve s such that (C-B) . (tC - tB) = 0
  // This equation turns out to be a linear equation for straight line
  // a s + b = 0;
  double b = (xA - xB) * (cosA - cosB) +
    (yA - yB) * (sinA - sinB);
  double a = cosA * (cosA - cosB) + 
    sinA * (sinA - sinB);

  // case 1: no symmetry point
  if (a == 0 && b != 0)
    return;

  // case 2: one unique point
  double s = -b / a;
  signed_distance_from_line_start.push_back(s);
  
  return;
}




//------------------------------------------------------------------------------
//: Compute symmetry point on a right boundary arc given a point-tangent on the left boundary
// Tangent orientation follow Giblin and Kimia (PAMI'03) convention, i.e.
// right tangent points forward
// left tangent points backward
void dbgl_compute_symmetry_point_on_circ_arc(const dbgl_circ_arc& right_bnd_arc,
                       const vgl_point_2d<double >& left_bnd_pt,
                       const vgl_vector_2d<double >& left_bnd_tangent, // pointing backward, following Giblin & Kimia's convention
                       vcl_vector<double >& s_along_right_bnd_arc)
{

  double k = right_bnd_arc.k();

  if (vnl_math::abs(k) < dbgl_circ_arc::epsilon)
  {
    dbgl_compute_symmetry_point_on_line(right_bnd_arc.start(), right_bnd_arc.end(),
      left_bnd_pt, left_bnd_tangent, s_along_right_bnd_arc);
    return;
  }
  else
  {
    // set up:
    // Let A and D be two end points of the right boundary arc
    // Let B be the left boundary point
    // Let C be the point arc AD which is a symmetry point of A

    double xA = right_bnd_arc.start().x();
    double yA = right_bnd_arc.start().y();
    vgl_vector_2d<double > tA = right_bnd_arc.tangent_at_start();
    double sinA = tA.y();
    double cosA = tA.x();

    double xB = left_bnd_pt.x();
    double yB = left_bnd_pt.y();
    vgl_vector_2d<double > tB = normalized(left_bnd_tangent);
    double sinB = tB.y();
    double cosB = tB.x();

    // Let x = ks + psiA
    // Need to solve:  a sin(x) + b cos(x) + c = 0
    // Compute coefficients a and b:
    double a =         k * (yA - yB) +cosA      -cosB;
    double b =         k * (xA - xB) -sinA      +sinB;
    double c = -cosB * k * (xA - xB) +sinA*cosB
               -sinB * k * (yA - yB) -cosA*sinB;

    vcl_vector<double > cos_x;
    vcl_vector<double > sin_x;
    dbnl_solve_1st_order_trig_equation(a, b, c, sin_x, cos_x);

    if (cos_x.size() > 2) // degenerate
    {
      return;
    }
    else if (cos_x.empty()) // no solution
    {
        return;
    }
    else if (cos_x.size() == 1)
    {
      vgl_vector_2d<double > tC (cos_x[0], sin_x[0]);
      double ks = signed_angle(tA, tC);
      s_along_right_bnd_arc.push_back(ks / k);
      return;
    }
    else if (cos_x.size() == 2) // two solutions, take the one closer to the front
    {
      
      double s[2];
      for (unsigned i =0; i < cos_x.size(); ++i)
      {
        vgl_vector_2d<double > tC (cos_x[i], sin_x[i]);
        double ks = signed_angle(tA, tC);
        // if ks is close to zero, force it to be zero
        ks = (vnl_math::abs(ks) < dbgl_circ_arc::epsilon) ? 0  : ks;
        s[i] = ks / k;
      }

      //double final_s = vnl_math::abs(s[0]) < vnl_math::abs(s[1]) ? s[0] : s[1];
      double min_s = vnl_math::min(s[0], s[1]);
      double max_s = vnl_math::max(s[0], s[1]);

      double final_s = (min_s < 0) ? max_s : min_s;
      s_along_right_bnd_arc.push_back(final_s);
    }                
  }

  //// \debug ////////////////////////////////////////////////////////////
  //for (unsigned i =0; i < s_along_right_bnd_arc.size(); ++i)
  //{
  //  double s = s_along_right_bnd_arc[i];
  //  vgl_point_2d<double > right_bnd_pt = right_bnd_arc.point_at_length(s);
  //  vgl_vector_2d<double > right_bnd_tangent = right_bnd_arc.tangent_at_length(s);
  //  double symmetry_measure = dot_product(left_bnd_pt-right_bnd_pt, left_bnd_tangent-right_bnd_tangent);

  //  int a = 0;
  //}
  ////////////////////////////////////////

}


