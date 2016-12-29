#include "dvpgl_triangulation.h"

#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>
#include <vnl/vnl_double_2.h>
#include <vpgl/vpgl_fundamental_matrix.h>


#include <dvpgl/algo/triangulation_kanatani/twoview_triangulation.h>


vgl_point_3d<double>
triangulate_3d_point_optimal_kanatani_fast(
    const vgl_point_2d<double> &pt_img0,
    const vgl_point_2d<double> &pt_img1,
    const vpgl_proj_camera<double> &cam0,
    const vpgl_proj_camera<double> &cam1)
{
  // Call kanatani code to correct the 2D points
  Vector x1_in(2), x2_in(2);

  x1_in[0] = pt_img0.x();
  x1_in[1] = pt_img0.y();

  x2_in[0] = pt_img1.x();
  x2_in[1] = pt_img1.y();

  Matrix       F(3,3);

  {
  vpgl_fundamental_matrix<double> fm(cam1, cam0);

  F[0][0] = fm.get_matrix()(0,0);
  F[0][1] = fm.get_matrix()(0,1);
  F[0][2] = fm.get_matrix()(0,2);
  F[1][0] = fm.get_matrix()(1,0);
  F[1][1] = fm.get_matrix()(1,1);
  F[1][2] = fm.get_matrix()(1,2);
  F[2][0] = fm.get_matrix()(2,0);
  F[2][1] = fm.get_matrix()(2,1);
  F[2][2] = fm.get_matrix()(2,2);
  }

  Vector x1_out(2), x2_out(2);
  two_view_triangulation_fast(x1_in,
                              x2_in,
                              F,
                              x1_out, x2_out,
                              600.0,
                              1000,
                              1.0e-10);

  // Linearly triangulate from corrected pts
  return triangulate_3d_point(
      cam0, vgl_point_2d<double>(x1_out[0], x1_out[1]), 
      cam1, vgl_point_2d<double>(x2_out[0], x2_out[1]));
}

vgl_point_3d<double>
triangulate_3d_point_optimal_kanatani(
    const vgl_point_2d<double> &pt_img0,
    const vgl_point_2d<double> &pt_img1,
    const vpgl_proj_camera<double> &cam0,
    const vpgl_proj_camera<double> &cam1)
{
  // Call kanatani code to correct the 2D points
  Vector x1_in(3), x2_in(3);

  x1_in[0] = pt_img0.x();
  x1_in[1] = pt_img0.y();
  x1_in[2] = 1;

  x2_in[0] = pt_img1.x();
  x2_in[1] = pt_img1.y();
  x2_in[2] = 1;

  Matrix       F(3,3);

  {
  vpgl_fundamental_matrix<double> fm(cam0, cam1);

  F[0][0] = fm.get_matrix()(0,0);
  F[0][1] = fm.get_matrix()(0,1);
  F[0][2] = fm.get_matrix()(0,2);
  F[1][0] = fm.get_matrix()(1,0);
  F[1][1] = fm.get_matrix()(1,1);
  F[1][2] = fm.get_matrix()(1,2);
  F[2][0] = fm.get_matrix()(2,0);
  F[2][1] = fm.get_matrix()(2,1);
  F[2][2] = fm.get_matrix()(2,2);
  }

  Vector x1_out(3), x2_out(3);
  two_view_triangulation(x1_in,
                              x2_in,
                              F,
                              x1_out, x2_out,
                              600.0,
                              1000,
                              1.0e-10);

  // Linearly triangulate from corrected pts
  return triangulate_3d_point(
      cam0, vgl_point_2d<double>(x1_out[0]/x1_out[2], x1_out[1]/x1_out[2]), 
      cam1, vgl_point_2d<double>(x2_out[0]/x2_out[2], x2_out[1]/x2_out[2]));
}
