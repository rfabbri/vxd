//This is bbasd/bsold/vis/bsold_soview2D_circ_arc.cxx
#include "bsold_soview2D_circ_arc.h"

#include <vcl_iostream.h>
#include <vcl_vector.h>
#include <vnl/vnl_math.h>

#include <vgl/vgl_distance.h>
#include <vgl/algo/vgl_h_matrix_2d.h>
#include <vgl/algo/vgl_homg_operators_2d.h>

#include <vgui/vgui_gl.h>


//--------------------------------------------------------------------------
//: vsol_circ_arc_2d view
//--------------------------------------------------------------------------

bsold_soview2D_circ_arc::bsold_soview2D_circ_arc( bsold_circ_arc_2d_sptr const & circ_arc)
: bgui_vsol_soview2D(circ_arc.ptr())
{
  double one_degree = vnl_math::pi/180;

  //compute center and arc radius
  c= *circ_arc->arc_center();
  r=length<double>(vgl_vector_2d<double>(c.get_p()-circ_arc->point1()));

  //compute the arc lenght
  arclen= circ_arc->length();

  //compute sample points at arc lenght
  for (double temp_arclen=0; temp_arclen<=arclen; temp_arclen+=one_degree*r){
    arc_points.push_back( vsol_point_2d_sptr( new vsol_point_2d(circ_arc->point_at_length(temp_arclen))));
  }
  arc_points.push_back( vsol_point_2d_sptr( new vsol_point_2d(circ_arc->point_at_length(arclen))));
}

bsold_circ_arc_2d_sptr bsold_soview2D_circ_arc::sptr() const
{
  return (sptr_->cast_to_curve()->is_a() == "bsold_circ_arc_2d") ? 
    static_cast<bsold_circ_arc_2d * > (sptr_->cast_to_curve()) : 0;
}


void bsold_soview2D_circ_arc::draw() const
{
  double px, py;
  glBegin(GL_LINE_STRIP);
  
  // Display the list
    for( int i = 0; i<arc_points.size(); i++ ) {
    px=(arc_points[i])->x();
    py=(arc_points[i])->y();
      glVertex2f(px,py);
  }
  glEnd();  
}

float bsold_soview2D_circ_arc::distance_squared(float x, float y) const
{
  return (sptr()->bsold_circ_arc_2d::distance(new vsol_point_2d(x,y))) *
      (sptr()->bsold_circ_arc_2d::distance(new vsol_point_2d(x,y)));
    
}

void bsold_soview2D_circ_arc::get_centroid(float* x, float* y) const
{
  *x = c.x();
  *y = c.y();
}

void bsold_soview2D_circ_arc::translate(float tx, float ty)
{
   //Translating the endpoints is sufficient
  sptr()->p0()->set_x( sptr()->p0()->x() + tx );
  sptr()->p0()->set_y( sptr()->p0()->y() + ty );
  sptr()->p1()->set_x( sptr()->p1()->x() + tx );
  sptr()->p1()->set_y( sptr()->p1()->y() + ty );
  
}



