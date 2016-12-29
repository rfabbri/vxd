// This is brcv/mvg/dbecl/vis/bgui_dbecl_soview2D.cxx
//:
// \file
#include "dbecl_episeg_soview2D.h"
#include <dbecl/dbecl_episeg.h>
#include <vsol/vsol_digital_curve_2d.h>
#include <vsol/vsol_point_2d.h>

#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vgl/vgl_distance.h>
#include <vgui/vgui_gl.h>


// Constructor
dbecl_episeg_soview2D::dbecl_episeg_soview2D( dbecl_episeg_sptr const & seg)
 : episeg_(seg)
{
  if(episeg_)
    update_sample_points();
}


//: Destructor
dbecl_episeg_soview2D::~dbecl_episeg_soview2D() 
{
}


//: Render the dbecl_episeg on the display.
void
dbecl_episeg_soview2D::draw() const
{
  glBegin(GL_LINE_STRIP);
  for( vcl_vector<vgl_point_2d<double> >::const_iterator p_itr = samples_.begin(); 
       p_itr != samples_.end(); ++p_itr)
    glVertex2f((*p_itr).x(), (*p_itr).y());
  glEnd();
}


//: Print details about this dbecl_episeg to the given stream.
vcl_ostream&
dbecl_episeg_soview2D::print(vcl_ostream&s) const
{
  return s << "dbecl_episeg[angle("<<episeg_->min_angle()<<","<<episeg_->max_angle()<<") "
           << "dist("<<episeg_->min_dist()<<","<<episeg_->max_dist()<<")]";
}


//: Returns the distance squared from this dbecl_episeg to the given position.
float
dbecl_episeg_soview2D::distance_squared(float x, float y) const
{
  if(samples_.size() == 1){
    float dx = x - samples_[0].x();
    float dy = y - samples_[0].y();
    return dx*dx + dy*dy;
  }
  
  float dd = -1.0f;
  for( vcl_vector<vgl_point_2d<double> >::const_iterator p_itr = samples_.begin(); 
       p_itr != samples_.end()-1; ++p_itr){
    float nd = vgl_distance2_to_linesegment(float((*p_itr).x()),     float((*p_itr).y()),
                                            float((*(p_itr+1)).x()), float((*(p_itr+1)).y()),
                                            x, y);
    if (dd<0 || nd<dd)
      dd = nd;
  }
  return dd;
}


//: Returns the centroid of this dbecl_episeg.
void
dbecl_episeg_soview2D::get_centroid(float* x, float* y) const
{
  *x = 0;
  *y = 0;
  for( vcl_vector<vgl_point_2d<double> >::const_iterator p_itr = samples_.begin(); 
       p_itr != samples_.end(); ++p_itr){
    *x += (*p_itr).x();
    *y += (*p_itr).y();
  }
  float s = 1.0f / float(samples_.size());
  *x *= s;
  *y *= s;
}


//: Translate this soview2D by the given x and y distances.
void
dbecl_episeg_soview2D::translate(float x, float y)
{
  // WARNING - This updates x,y position of each point but DOES NOT
  //           adjust any other dependent variables such as s and alpha
  int min_index = (int) vcl_ceil(episeg_->min_index());
  int max_index = (int) vcl_floor(episeg_->max_index());
  vsol_digital_curve_2d_sptr dc = episeg_->curve();

  for(int i=min_index; i<=max_index; ++i){
    vsol_point_2d_sptr pt = dc->point(i);
    pt->set_x(pt->x()+x);
    pt->set_y(pt->y()+y);
  }
  update_sample_points();
}


//: Update the sample points from the episeg
void 
dbecl_episeg_soview2D::update_sample_points()
{
  samples_.clear();

  double min_i = episeg_->min_index();
  int min_index = (int) vcl_ceil(min_i);
  double max_i = episeg_->max_index();
  int max_index = (int) vcl_floor(max_i);
  vsol_digital_curve_2d_sptr dc = episeg_->curve();

  if(min_i == max_i){
    samples_.push_back(dc->interp(min_i));
    return;
  }

  if(min_i != double(min_index))
    samples_.push_back(dc->interp(min_i));
  for(int i=min_index; i<=max_index; ++i)
    samples_.push_back(dc->point(i)->get_p());
  if(max_i != double(max_index))
    samples_.push_back(dc->interp(max_i));
}
