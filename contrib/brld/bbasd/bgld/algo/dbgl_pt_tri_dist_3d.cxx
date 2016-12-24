// This is basic/dbgl/algo/dbgl_pt_tri_dist_3d.cxx
//:
// \file
// \brief  Point to Triangle Distance in 3D.
// \author Ming-Ching Chang (mcchang@lems.brown.edu)
// \date   May/07/2007
//
// \verbatim
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vcl_cfloat.h>
#include <dbgl/dbgl_fuzzy_boolean.h>
#include <dbgl/algo/dbgl_distance.h>


//: Compute the foot point F of a point P on triangle ABC.
//  If P is inside ABC, also return t = dist(PF) and
//  (1-u-v, u, v) the barycentric coord of F on triangle ABC.
//
bool dbgl_pt_fpt_tri (const vgl_point_3d<double>& P, const vgl_point_3d<double>& A,
                      const vgl_point_3d<double>& B, const vgl_point_3d<double>& C,
                      vgl_point_3d<double>& F, double& t, double& u, double& v)
{
  //Determine the unit normal N of ABC in the direction of PF.
  vgl_vector_3d<double> N = cross_product (B-A, C-A);

  N /= N.length();

  //Check validality of N
  if (dbgl_eq_m (N.sqr_length(), 1) == false)
    return false;

  //Make sure that N is the same direction of PF.
  if (dot_product (P-A, N) > 0)
    N = -N;

  bool F_inside_ABC = dbgl_intersect_ray_tri (P, N, A, B, C, t, u, v);

  //The foot point F on ABC
  F = P + N * t;

  return F_inside_ABC;
}

//: Compute the closest distance d of P to ABC:
//  If the foot point F is inside ABC, d = PF, F = G.
//  Otherwise d = PG, where G is on the boundary of ABC.
//  Method:
//    1. Compute the normal of triangle ABC: vector N = cross (AB, AC).
//    2. Determine the intersection F of the ray PN on ABC. Also check -PN.
//    3. If F is inside ABC, return F as G.
//       Otherwise, determine G using the barycentric coordinate of F.
//    
double dbgl_pt_tri_dist_3d (const vgl_point_3d<double>& P, const vgl_point_3d<double>& A,
                            const vgl_point_3d<double>& B, const vgl_point_3d<double>& C,
                            vgl_point_3d<double>& G)
{
  vgl_point_3d<double> F; 
  double t, a, b, c, l, edge_len; 
  bool F_inside_ABC = dbgl_pt_fpt_tri (P, A, B, C, F, t, b, c);

  if (F_inside_ABC) {
    G = F;    
    assert (t >= 0);
    return t;
  }
  else { 
    //Determine the barycentric region of F on the plane ABC
    //Cooresponding barycentric value: A: a, B: b, C: c, a+b+c=1.
    a = 1-b-c;

    //For barycentric region of vertices, the closest is the vertex.
    if (a>0 && b<=0 && c<=0) { //a=0 does not belong here.
      G = A;
      return vgl_distance (P, G);
    }
    else if (b>0 && a<=0 && c<=0) { //b=0 does not belong here.
      G = B;
      return vgl_distance (P, G);
    }
    else if (c>0 && a<=0 && b<=0) { //c=0 does not belong here.
      G = C;
      return vgl_distance (P, G);
    }
    else if (a>0 && b>0 && c<=0) { //c=0 belongs here.
      //Compute the cloest to A, B, and line_AB.
      //dot(AP, AB) to determine if closest point is on line_AB.
      edge_len = vgl_distance (A, B);
      l = dot_product (P-A, B-A) / edge_len;      
      if (l <= 0) //closest is A
        G = A;
      else if (edge_len <= l) //closest is B
        G = B;
      else //cloeset is on line_AB
        G = A + (B-A) * l/edge_len; //foot point on AB
      return vgl_distance (P, G);
    }
    else if (b>0 && c>0 && a<=0) { //a=0 belongs here.
      //Compute the cloest to B, C, and line_BC.
      //dot(BP, BC) to determine if closest point is on line_BC.
      edge_len = vgl_distance (B, C);
      l = dot_product (P-B, C-B) / edge_len;
      if (l <= 0) //closest is B
        G = B;
      else if (edge_len <= l) //closest is C
        G = C;
      else //cloeset is on line_BC
        G = B + (C-B) * l/edge_len; //foot point on BC
      return vgl_distance (P, G);
    }
    else if (c>0 && a>0 && b<=0) { //b=0 belongs here.
      //Compute the cloest to A, C, and line_AC.
      //dot(AP, AC) to determine if closest point is on line_AC.
      edge_len = vgl_distance (A, C);
      l = dot_product (P-A, C-A) / edge_len;
      if (l <= 0) //closest is A
        G = A;
      else if (edge_len <= l) //closest is C
        G = C;
      else //cloeset is on line_AC
        G = A + (C-A) * l/edge_len; //foot point on AC
      return vgl_distance (P, G);
    }
    else {
      assert (0);
      return DBL_MAX;
    }
  }
}

