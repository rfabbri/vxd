//:
// \file

#include <vcl_cmath.h>

#include "dbsol_curve_3d_algs.h"
#include <dbsol/dbsol_interp_curve_3d.h>
#include <dbgl/dbgl_eno_curve_3d.h>
#include <dbgl/dbgl_linear_curve_3d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vcl_iostream.h>
#include <dbnl/algo/dbnl_eno_third_order.h>
#include <vnl/vnl_vector.h>


//: Destructor
dbsol_curve_3d_algs::~dbsol_curve_3d_algs()
{
}

//-------------------------------------------------------------
//: sample the input curve at the rate length()/size
bool dbsol_curve_3d_algs::
sample(dbsol_interp_curve_3d const &c, int size,
       vcl_vector<vsol_point_3d_sptr>& pts)
{
  double L = c.length();
  double ds = L/size;
#ifndef NDEBUG
  vcl_cout << "Length of curve: " << L << " rate of sampling is: " << ds << vcl_endl;
  vcl_cout << "Output size wanted is: " << size << vcl_endl;
#endif

  pts.reserve(size);

  double sum = 0;
  while (sum <= L)
  {
    pts.push_back(c.point_at(sum));
    sum += ds;
  }

  if (*(c.point_at(0)) != *(c.point_at(L)))  // if not closed curve
    pts.push_back(c.point_at(L));  

#ifndef NDEBUG
  vcl_cout << "after sampling pts size: " << pts.size() << " (should be " << size << ")\n";
#endif
  return true;

}

bool dbsol_curve_3d_algs::interpolate_linear_3d(dbsol_interp_curve_3d *c, vcl_vector<vsol_point_3d_sptr> const &pts)
{
  int num_points = pts.size();
  vcl_vector<dbgl_param_curve_3d *> ints(num_points-1);
  for(int i=0; i < num_points-1; i++)
  {
    vgl_vector_3d<double> start(pts[i]->x(), pts[i]->y(), pts[i]->z());
    vgl_vector_3d<double> end(pts[i+1]->x(), pts[i+1]->y(), pts[i+1]->z());
    ints[i] = new dbgl_linear_curve_3d(start, end);
  }

  c->make(ints);

  return true;
}

bool dbsol_curve_3d_algs::interpolate_eno_3d(dbsol_interp_curve_3d *c, 
                                             vcl_vector<vsol_point_3d_sptr> const &pts,
                                             vcl_vector<double> &sample_pts)
{
  int num_points = pts.size();
  
  vcl_vector<double> data_x;
  vcl_vector<double> data_y;
  vcl_vector<double> data_z;

  // get the data
  for(int i=0; i < num_points; i++)
  {
    data_x.push_back(pts[i]->get_p().x());
    data_y.push_back(pts[i]->get_p().y());
    data_z.push_back(pts[i]->get_p().z());
  }

  // compute sampling point indices using the simple arclength between each datapoint
  // if the smple_pts array is empty
  if(sample_pts.size() == 0)
  {
    double cumulative_length = 0.0;
    sample_pts.push_back(cumulative_length);
    for(int i=1; i < num_points; i++)
    {
      double diffx2 = vcl_pow(data_x[i] - data_x[i-1], 2.0);
      double diffy2 = vcl_pow(data_y[i] - data_y[i-1], 2.0);
      double diffz2 = vcl_pow(data_z[i] - data_z[i-1], 2.0);
      double length = vcl_sqrt(diffx2 + diffy2 + diffz2);
      cumulative_length += length;
      sample_pts.push_back(cumulative_length);
    }
  }

  dbnl_eno_third_order eno_x;
  dbnl_eno_third_order eno_y;
  dbnl_eno_third_order eno_z;

  eno_x.interpolate(sample_pts, data_x);
  eno_y.interpolate(sample_pts, data_y);
  eno_z.interpolate(sample_pts, data_z);

  vcl_vector<dbgl_param_curve_3d *> ints(num_points-1);

  vcl_vector<double> coefs_x;
  vcl_vector<double> coefs_y;
  vcl_vector<double> coefs_z;

  for(int i=0; i < num_points-1; i++)
  {
    for(int j=0; j<4; j++)
    {
      coefs_x.push_back(eno_x.coefficient(i, j));
      coefs_y.push_back(eno_y.coefficient(i, j));
      coefs_z.push_back(eno_z.coefficient(i, j));
    }
//    ints[i] = new dbgl_eno_curve_3d(coefs_x, coefs_y, coefs_z, i, i+1);
    ints[i] = new dbgl_eno_curve_3d(coefs_x, coefs_y, coefs_z, sample_pts[i], sample_pts[i+1]);
    coefs_x.clear();
    coefs_y.clear();
    coefs_z.clear();
  }

  c->make(ints);

  return true;
}
