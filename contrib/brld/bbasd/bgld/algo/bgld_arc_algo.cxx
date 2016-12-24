#include "bgld_arc_algo.h"
#include <vcl_cstdio.h>

// copied this definition from visualizer's base_defines.h
#ifdef HUGE
#undef HUGE
#endif
#ifndef HUGE
#define HUGE    1E28
#endif
///////////////////////////////////


//: intersection of two circular arcs
// Don't expect sensible results if you want to find intercections of 
// _enpoints_ of the arcs with something. This is not currently supported, but
// may work.
unsigned bgld_arc_algo::
compute_intersections(const bgld_arc &arc1, const bgld_arc &arc2, 
         vgl_point_2d<double> *intercept1, vgl_point_2d<double> *intercept2)
{

   
  bool a1_curv_is_zero,a2_curv_is_zero;

  a1_curv_is_zero = bgld_arc::is_almost_zero(arc1.curvature());
  a2_curv_is_zero = bgld_arc::is_almost_zero(arc2.curvature());

  if (a1_curv_is_zero && a2_curv_is_zero) {
#ifndef NDEBUG
    vcl_printf("line-line \n");
#endif
    return bgld_arc::compute_inters_ll(arc1.point_at(0), arc1.point_at(1), 
          arc2.point_at(0), arc2.point_at(1), intercept1);
  } else if (!a1_curv_is_zero && !a2_curv_is_zero) {
#ifndef NDEBUG
    vcl_printf("arc-arc \n");
#endif
    return compute_inters_aa_bkk(arc1, arc2, intercept1, intercept2);
  } else {
#ifndef NDEBUG
    vcl_printf("arc-line \n");
#endif
    if(a2_curv_is_zero) {
      return compute_inters_al(arc1, arc2, intercept1, intercept2);
    } else {
      return compute_inters_al(arc2, arc1, intercept1, intercept2);
    }       
  }
}


//: intersection of two arcs having non-zero curvature
// This variant is originally by "bkk". Other experimental variants are
// available in the old GENO interpolation code from LEMS, file geno-geometric.c
//
// \return number of intersections, or 3 if arcs come from same circle.
unsigned bgld_arc_algo::
compute_inters_aa_bkk(const bgld_arc &arc1, const bgld_arc &arc2, 
         vgl_point_2d<double> *intercept1, vgl_point_2d<double> *intercept2)
{
    double R1, R2;
    double P2;
    double M, N;
    double x1, x2, y1, y2;
    double x_int1, x_int2, y_int1, y_int2;

    double a, b, c, d;

    double p2my1;

    R1 = 1 / arc1.curvature();
    R2 = 1 / arc2.curvature();

    x1 = arc1.center().x();
    x2 = arc2.center().x(); 
    y1 = arc1.center().y();
    y2 = arc2.center().y(); 

    N = x2 - x1;
    M = y2 - y1;

    
    //: write the implicit equation for both circles, expand squares, then
    // subtract one from the other, getting rid of the quadratic terms in
    // the unknowns x and y
    P2 = (R1*R1 - R2*R2) - (x1*x1 - x2*x2) - (y1*y1 - y2*y2);
    P2 /= 2.0;

    if(bgld_arc::is_almost_zero(M)){
       if (bgld_arc::is_almost_zero(N))
          return 3; // arcs from coincident circles

        x_int1 = P2 / N;    //:< bug fix wrt. original
        x_int2 = x_int1;

        a = 1.0;
        b = 2.0 * y1;
        c = y1*y1 + x_int1*x_int1 - 2*x1*x_int1 + x1*x1 - R1*R1;
        d = (b*b - 4.0*a*c);

        if (d < 0.0 && !bgld_arc::is_almost_zero(d)) { //:< bug fix wrt. original
            return 0;
        } else if (bgld_arc::is_almost_zero(d)){
            y_int1 = -b / (2.0 * a);
            intercept1->set(x_int1,y_int1);
            return 1;
        }
        
        d = vcl_sqrt(d);

        y_int1 = (-b + d) / (2.0 * a);
        y_int2 = (-b - d) / (2.0 * a);

        intercept1->set(x_int1,y_int1);
        intercept2->set(x_int2,y_int2);

        return 2;
    }


    p2my1 = (P2 / M) - y1;

    a = 1.0 + ((N*N) / (M*M));
    b = - (2.0 * (x1 + ((N * p2my1)/M)));
    c = (x1 * x1) + (p2my1 * p2my1) - (R1 * R1);

    d = (b*b - 4.0 * a * c);

    if (d < 0.0 && !bgld_arc::is_almost_zero(d)) { //:< bug fix wrt. original
        return 0;
    } else if (bgld_arc::is_almost_zero(d)) {
        x_int1 = -b / (2.0 * a);
        y_int1 = (P2 / M)  - ((N * x_int1) / M);

        intercept1->set(x_int1,y_int1);
        return 1;
    }

    d = vcl_sqrt(d);

    x_int1 = (-b + d) / (2.0 * a);
    x_int2 = (-b - d) / (2.0 * a);

    y_int1 = (P2 / M)  - ((N * x_int1) / M);
    y_int2 = (P2 / M)  - ((N * x_int2) / M);

    intercept1->set(x_int1,y_int1);
    intercept2->set(x_int2,y_int2);

    return 2;
}

//: This is based on Kwun Han's arc-line intersection function - it
// finds the two points where the line (arc of virtually zero curvature) intersects the
// given arc; Note that it doesn't check the line _segment_!
unsigned bgld_arc_algo::
compute_inters_al(const bgld_arc &arc, const bgld_arc &line, 
         vgl_point_2d<double> *intercept1, vgl_point_2d<double> *intercept2)

{
    double x1, y1, x2, y2, a, b, c;
    double const1, const2, const3, arg, m, d;
    double x_int1, x_int2;
    
    // (x-a)^2 + (y-b)^2 = c^2

    x1 = line.point_at(0).x();
    y1 = line.point_at(0).y();
    x2 = line.point_at(1).x();
    y2 = line.point_at(1).y();
    a = arc.center().x();
    b = arc.center().y();
    c = 1 / arc.curvature();

    if (x1 == x2) {
         if (arc.is_almost_equal(x1,a-c,arc.near_zero_value) || 
             arc.is_almost_equal(x1,a+c,arc.near_zero_value)) {
            intercept1->set(x1,b);
            return 1;
         } else if ( a-c <= x1 && x1 <= a+c ) {
            double rcost = x1-a;
            double rsint = vcl_sqrt(c*c - rcost*rcost);
            intercept1->set(x1, b + rsint);
            intercept2->set(x1, b - rsint);
            return 2;
         } else
            return 0;
    } else {
        // y = mx + d
        
        m = (y2 - y1) / (x2 - x1);
        d = y1 - m*x1;
        const1 = m*m + 1.0;

        const2 = -2.0 * (a + m*(b-d)); // small sine bug was in kwh original

        const3 = (a+c)*(a-c) + (d+b)*(d+b);

        arg = (const2*const2) - 4*const1*const3;

        if (arc.is_almost_zero(arg)) {
           x_int1 = -const2/(2.0*const1);
           intercept1->set(x_int1, m * x_int1 + d);
           return 1;
        } else if(arg < 0) { // no intercept
            return 0;
        } else {
           double two_c1 = 2.0*const1;
            x_int1 = (vcl_sqrt(arg) - const2) / two_c1;
            x_int2 = (- vcl_sqrt(arg) - const2) / two_c1;

            intercept1->set(x_int1, m * x_int1 + d);
            intercept2->set(x_int2, m * x_int2 + d);
            return 2;
        }
    }
}

//: compute the radius of the circular arc through three points [p1-----p2----p3]
double bgld_arc_algo::
compute_arc_radius_from_three_points(vgl_point_2d<double> p1,vgl_point_2d<double> p2, vgl_point_2d<double> p3)
{
  double x1 = p1.x();  double y1 = p1.y();
  double x2 = p2.x();  double y2 = p2.y();
  double x3 = p3.x();  double y3 = p3.y();

  double s1x = (x1+x3)/2;  double s1y = (y1+y3)/2;
  double s2x = (x2+x3)/2;  double s2y = (y2+y3)/2;

  double psi1 = vcl_atan2(y3-y1, x3-x1) + vnl_math::pi_over_2;
  double psi2 = vcl_atan2(y3-y2, x3-x2) + vnl_math::pi_over_2;

  double psihat = vcl_atan2(s2y - s1y, s2x - s1x);

  if (vcl_fabs(vcl_sin(psi2 - psi1)) < 1e-7){// collinear points
    return HUGE;
  }
  else {
    double s = vcl_sin(psi2 - psihat)/vcl_sin(psi2 - psi1);
    double H = vcl_sqrt( (s1y-s2y)*(s1y-s2y) + (s1x-s2x)*(s1x-s2x) );
    
    double cx = s1x + H*s*vcl_cos(psi1);
    double cy = s1y + H*s*vcl_sin(psi1);

    return vcl_sqrt((cx-x1)*(cx-x1) + (cy-y1)*(cy-y1));
  }
}

