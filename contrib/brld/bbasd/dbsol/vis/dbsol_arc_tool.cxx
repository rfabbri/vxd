// This is basic/dbsol/vis/dbsol_arc_tool.cxx
//:
// \file
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date 4/9/2005

#include "dbsol_arc_tool.h"
#include <vnl/vnl_math.h>
#include <vgui/vgui.h>
#include <vcl_ostream.h>


//: constructor
dbsol_arc_tool::dbsol_arc_tool()
{
  this->choose_center_ = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
  this->choose_circle_point_ = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
  this->active_ = false;
}


//: destructor
dbsol_arc_tool::~dbsol_arc_tool()
{
}


bool
dbsol_arc_tool::handle( const vgui_event & e, 
                               const bvis1_view_tableau_sptr& /*view*/ )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);
  

  if( e.type == vgui_MOTION ) 
  {
    this->x1_ = ix;
    this->y1_ = iy;
    if( this->active_ ) 
    {
      this->tableau()->post_overlay_redraw();
    }
    else{
      vgui::out<< "( " << ix << " , " << iy << ")" << vcl_endl; 
    }
    return true;
  }

  if( ! this->active_ && this->choose_center_(e) ) 
  {
    this->active_ = true;
    this->x0_ = this->x1_ = ix;
    this->y0_ = this->y1_ = iy;
    this->active_tableau_ = this->tableau();
    this->active_storage_ = this->storage();
    return true;
  } 
  else if( this->active_ ) 
  {
    if( e.type == vgui_OVERLAY_DRAW ) 
    {
      double r = vcl_sqrt((this->x1_-this->x0_)*(this->x1_-this->x0_) + 
        (this->y1_-this->y0_)*(this->y1_-this->y0_));
      glLineWidth(1);
      glPointSize(3);
      glColor3f(1,1,1);

      glBegin(GL_POINTS);
      glVertex2f(this->x0_, this->y0_);
      glEnd();

      glBegin(GL_LINE_LOOP);
      for (unsigned int i=0;i<100;i++)
      {
        double angle = i*(2*vnl_math::pi/100);
        glVertex2d(this->x0_+r*vcl_cos(angle), this->y0_+r*vcl_sin(angle));
      }
      glEnd();

      vgui::out << "center (" << this->x0_ << " , " << this->y0_ << ")  r= " << r << vcl_endl;                                                                                    
      return true;
    }
  

    if( this->choose_circle_point_(e) ) 
    {

      // radius
      double r = vcl_sqrt((this->x1_-this->x0_)*(this->x1_-this->x0_) + 
        (this->y1_-this->y0_)*(this->y1_-this->y0_));
      // represent the circle as a polygon
      vcl_vector<vsol_point_2d_sptr > point_list;
      for (unsigned int i=0;i<50;i++)
      {
        double angle = i*(2*vnl_math::pi/50);
        vsol_point_2d_sptr point_sptr = new vsol_point_2d(this->x0_+r*vcl_cos(angle), 
          this->y0_+r*vcl_sin(angle));
        point_list.push_back(point_sptr);
      }
      //// this is a hack for now because "Save .bnd" does not save polygon
      //// in the future replace when polyline is replace by a polygon then no need to add
      //// the front point to the end
      //point_list.push_back(point_list.front());
      

      ////////////////////////////////
      vcl_cout << vcl_endl << "----------------------------------------------------------------" << vcl_endl;
      vcl_cout << "Circle center = ( " << this->x0_ << " , " << this->y0_ << " )" << vcl_endl;
      vcl_cout << "Radius r = " << r << vcl_endl; 
      vcl_cout << "----------------------------------------------------------------" << vcl_endl;

      vsol_polygon_2d_sptr newpolygon = new vsol_polygon_2d(point_list);
      
      //add to the storage class
      this->active_storage_->add_object( vsol_spatial_object_2d_sptr(newpolygon->cast_to_spatial_object()), 
        "Drawing" );


      //add it to the tableau as an soview
      this->active_tableau_->set_current_grouping( "Drawing" );


      this->active_tableau_->add_vsol_polygon_2d( newpolygon );
      this->active_tableau_->set_current_grouping( "default" );

      this->active_tableau_->post_redraw();


      this->active_tableau_ = 0;
      this->active_storage_ = 0;
      this->active_ = false;
      return true;
    }

  }
 
  return false;
}


//: name of the tool
vcl_string
dbsol_arc_tool::name() const
{
  return "Circle";
}


void
dbsol_arc_tool::deactivate()
{
  this->active_ = false;
}




//vsol_polyline_2d_sptr newpolyline = new vsol_polyline_2d(point_list);
      //
      ////add to the storage class
      //this->active_storage_->add_object( newpolyline->cast_to_spatial_object() , "Drawing" );


