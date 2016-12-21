// This is basic/dbgl/dbgl_dist.cxx
//:
//\file
// Mar 07, 2007  Basic distance computation.

#include <dbgl/dbgl_dist.h>

vgl_box_3d<double> dbgl_reduce_box (const vgl_box_3d<double>& box, const double red)
{
  vgl_box_3d<double> red_box;

  red_box.set_min_x (box.min_x() + red);
  red_box.set_min_y (box.min_y() + red);
  red_box.set_min_z (box.min_z() + red);
  
  red_box.set_max_x (box.max_x() - red);
  red_box.set_max_y (box.max_y() - red);
  red_box.set_max_z (box.max_z() - red);

  assert (box.max_x() > box.min_x());
  assert (box.max_y() > box.min_y());
  assert (box.max_z() > box.min_z());

  return red_box;
}

vgl_box_3d<double> dbgl_extend_box (const vgl_box_3d<double>& box, const double ext)
{
  vgl_box_3d<double> ext_box;

  ext_box.set_min_x (box.min_x() - ext);
  ext_box.set_min_y (box.min_y() - ext);
  ext_box.set_min_z (box.min_z() - ext);
  
  ext_box.set_max_x (box.max_x() + ext);
  ext_box.set_max_y (box.max_y() + ext);
  ext_box.set_max_z (box.max_z() + ext);

  return ext_box;
}

vgl_box_3d<double> dbgl_extend_box_by_factor (const vgl_box_3d<double>& box, const float factor)
{
  vgl_box_3d<double> ext_box;

  float extX = (float) box.width() * factor;
  float extY = (float) box.height() * factor;
  float extZ = (float) box.depth() * factor;

  ext_box.set_min_x (box.min_x() - extX);
  ext_box.set_min_y (box.min_y() - extY);
  ext_box.set_min_z (box.min_z() - extZ);
  
  ext_box.set_max_x (box.max_x() + extX);
  ext_box.set_max_y (box.max_y() + extY);
  ext_box.set_max_z (box.max_z() + extZ);

  return ext_box;
}


