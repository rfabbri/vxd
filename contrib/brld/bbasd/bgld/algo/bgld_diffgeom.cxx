#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vcl_cassert.h>
#include "bgld_diffgeom.h"
#define DIFFGEOM_EPS 2.2204e-16

/**
 * Compute the inverse matrix of | 1.0, -Ta, Ta^2 |
 *                               | 1.0, 0.0, 0.0  |
 *                               | 1.0, -Tb, Tb^2 |
 */
void inverse3(double Ta, double Tb, double (&inv)[3][3])
{
  //Compiler should remove operations with const
  const double m0 = 1.0, m3 = 1.0, m6 = 1.0;
  const double m4 = 0.0, m5 = 0.0;

  double m1 = -Ta;
  double m2 = Ta * Ta;
  double m7 = -Tb;
  double m8 = Tb * Tb;

  inv[0][0] = m4 * m8 - m7 * m5;
  inv[0][1] = -(m3 * m8 - m6 * m5);
  inv[0][2] = m3 * m7 - m6 * m4;
  inv[1][0] = -(m1 * m8 - m7 * m2);
  inv[1][1] = m0 * m8 - m6 * m2;
  inv[1][2] = -(m0 * m7 - m6 * m1);
  inv[2][0] = m1 * m5 - m4 * m2;
  inv[2][1] = -(m0 * m5 - m3 * m2);
  inv[2][2] = m0 * m4 - m3 * m1;

  double invDet = m0 * m4 * m8 - m0 * m7 * m5 - m3 * m1 * m8 + m3 * m7 * m2 + m6 * m1 * m5 - m6 * m4 * m2;
  invDet = 1.0 / invDet;
 
  inv[0][0] *= invDet;
  inv[0][1] *= invDet;
  inv[0][2] *= invDet;
  inv[1][0] *= invDet;
  inv[1][1] *= invDet;
  inv[1][2] *= invDet;
  inv[2][0] *= invDet;
  inv[2][1] *= invDet;
  inv[2][2] *= invDet;
}

double compute_k(double xa, double x, double xb,
                 double ya, double y, double yb,
                 double inv[3][3])
{
  double a1 = inv[0][0] * xa + inv[1][0] * x + inv[2][0] * xb;
  double a2 = inv[0][1] * xa + inv[1][1] * x + inv[2][1] * xb;
  double a3 = inv[0][2] * xa + inv[1][2] * x + inv[2][2] * xb;
  double b1 = inv[0][0] * ya + inv[1][0] * y + inv[2][0] * yb;
  double b2 = inv[0][1] * ya + inv[1][1] * y + inv[2][1] * yb;
  double b3 = inv[0][2] * ya + inv[1][2] * y + inv[2][2] * yb;

  return 2.0 * (a2 * b3 - a3 * b2 ) / vcl_pow((a2 * a2 + b2 * b2), 3.0/2.0);
}

double compute_dist(double x1, double x2, double y1, double y2)
{
  return vcl_sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void 
bgld_compute_curvature(
    const vcl_vector< vgl_point_2d<double> > &vertices, 
    vnl_vector<double> *k
    )
{
  int size = vertices.size();
  assert(size >= 2);

  k->set_size(size);

  double inv[3][3];
  double xa, x, xb, ya, y, yb, ta, tb;

  xb = vertices[0].x(); x = vertices[1].x(); xa = vertices[2].x();
  yb = vertices[0].y(); y = vertices[1].y(); ya = vertices[2].y();
  
  ta = compute_dist(xb, x, yb, y);
  tb = compute_dist(xa, xb, ya, yb);

  inverse3(-ta, -tb, inv);
  (*k)[0] = compute_k(vertices[1].x(), vertices[0].x(), vertices[2].x(),
                      vertices[1].y(), vertices[0].y(), vertices[2].y(),
                      inv);

  for (int i = 1; i < size - 1; ++i)
  {
      xa = vertices[i+1].x();
      ya = vertices[i+1].y();
      
      tb = ta;
      ta = compute_dist(xa, x, ya, y);

      inverse3(-ta, tb, inv);
      (*k)[i] = compute_k(vertices[i+1].x(), vertices[i].x(), vertices[i-1].x(),
                          vertices[i+1].y(), vertices[i].y(), vertices[i-1].y(),
                          inv);
      x = xa; y = ya;
  }
  
  tb = ta;
  ta = compute_dist(x, vertices[size-3].x(), y, vertices[size-3].y());

  inverse3(ta, tb, inv);
  (*k)[size-1] = compute_k(vertices[size-3].x(), vertices[size-1].x(), vertices[size-2].x(),
                      vertices[size-3].y(), vertices[size-1].y(), vertices[size-2].y(),
                      inv);
}

void bgld_compute_normals(
    const vcl_vector< vgl_point_2d<double> > &vertices, 
    vcl_vector< vnl_vector_fixed<double, 2> > *n
    )
{
  unsigned size = vertices.size();
  assert(size);
  n->resize(size);

  double lastx = 0.0;
  double lasty = 0.0;
  double vx = vertices[0].x();
  double vy = vertices[0].y();

  for (unsigned i = 0; i + 1 < size; ++i) {
    double vxp1 = vertices[i+1].x();
    double vyp1 = vertices[i+1].y();

    double x = vx - vxp1;
    double y = vy - vyp1;

    vx = vxp1;
    vy = vyp1;

    double norm2 = (x * x + y * y);
    norm2 = (norm2 > DIFFGEOM_EPS) ? norm2 : DIFFGEOM_EPS;
    
    double xx = lastx;
    double yy = lasty;

    lastx = x / norm2;
    lasty = y / norm2;

    xx += lastx;
    yy += lasty;

    double norm = vcl_sqrt(xx * xx + yy * yy);
    (*n)[i][0] = -yy / norm;
    (*n)[i][1] = xx / norm;
  }

  double norm = vcl_sqrt(lastx * lastx + lasty * lasty);
  (*n)[size - 1][0] = -lasty / norm;
  (*n)[size - 1][1] = lastx / norm;
}
