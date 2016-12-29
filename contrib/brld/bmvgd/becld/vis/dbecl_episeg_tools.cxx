// This is brcv/mvg/dbecl/vis/dbecl_episeg_tools.cxx 
//:
// \file

#include "dbecl_episeg_tools.h"
#include "dbecl_episeg_soview2D.h"
#include <bvis1/bvis1_manager.h>
#include <bvis1/bvis1_view_tableau.h>
#include <vgui/vgui.h> 
#include <vgui/vgui_style.h>
#include <vgui/vgui_projection_inspector.h>
#include <vgui/vgui_displaylist2D_tableau.h>
#include <vgui/internals/vgui_draw_line.h>
#include <dbecl/pro/dbecl_episeg_storage.h> 
#include <dbecl/dbecl_episeg.h>
#include <vgl/vgl_line_2d.h>

//------------------------------------


//: Constructor - protected
dbecl_episeg_tool::dbecl_episeg_tool()
 :  tableau_(NULL), storage_(NULL)
{
}


//: Set the tableau to work with
bool
dbecl_episeg_tool::set_tableau ( const vgui_tableau_sptr& tableau )
{
  if( !this->set_storage(bvis1_manager::instance()->storage_from_tableau(tableau)) )
    return false;

  tableau_ = vgui_displaylist2D_tableau_sptr(dynamic_cast<vgui_displaylist2D_tableau*>(tableau.ptr()));
  if( tableau_.ptr() == NULL ) 
    return false;
  
  return true;
}


//: Set the storage class for the active tableau
bool
dbecl_episeg_tool::set_storage ( const bpro1_storage_sptr& storage )
{
  if (!storage.ptr())
    return false;
  //make sure its an episeg storage class
  if (storage->type() == "episeg"){
    storage_.vertical_cast(storage);
    return true;
  }
  return false;
}

//----------------------dbecl_episeg_inspector_tool---------------------------


//: Constructor
dbecl_episeg_inspector_tool::dbecl_episeg_inspector_tool()
 : draw_bounds_(true), draw_neighbors_(true), draw_epipolar_line_(true), 
   print_stats_(true), epipole_(NULL),
   object_(NULL), last_x_(0.0), last_y_(0.0)
{
}


//: Destructor
dbecl_episeg_inspector_tool::~dbecl_episeg_inspector_tool()
{
}


//: Return the name of this tool
vcl_string
dbecl_episeg_inspector_tool::name() const
{
  return "Epi-Segment Inspector"; 
}


//: Print stats about the node 
void 
dbecl_episeg_inspector_tool::print_stats(const dbecl_episeg_sptr seg) const
{
  vcl_cout << "Min Angle = " << seg->min_angle() << vcl_endl;
  vcl_cout << "Max Angle = " << seg->max_angle() << vcl_endl;
}


static void draw_bounds(const dbecl_episeg_sptr& episeg)
{
  double min_a = episeg->min_angle();
  double max_a = episeg->max_angle();
  double min_s = episeg->min_dist();
  double max_s = episeg->max_dist();
  dbecl_epipole_sptr ep = episeg->epipole();

  double x,y;

  vgui_style::new_style(1.0, 0.0, 0.0, 1.0, 1.0)->apply_all();
  glBegin(GL_LINE_STRIP);
  
  //Draw the lower angle line
  ep->to_img_coords(min_s,min_a,x,y);
  glVertex2f(x,y);  
  ep->to_img_coords(max_s,min_a,x,y);
  glVertex2f(x,y); 

  // Draw the max distance arc
  double da = 1.0/max_s;
  for(double a=min_a; a<max_a; a+=da){
    ep->to_img_coords(max_s,a,x,y);
    glVertex2f(x,y); 
  }

  //Draw the upper angle line
  ep->to_img_coords(max_s,max_a,x,y);
  glVertex2f(x,y); 
  ep->to_img_coords(min_s,max_a,x,y);
  glVertex2f(x,y); 

  //Draw the min distance arc
  da = 1.0/min_s;
  for(double a=max_a; a>min_a; a-=da){
    ep->to_img_coords(min_s,a,x,y);
    glVertex2f(x,y); 
  }

  ep->to_img_coords(min_s,min_a,x,y);
  glVertex2f(x,y);

  glEnd();
}

//: Handle events
bool
dbecl_episeg_inspector_tool::handle( const vgui_event & e, 
                                  const bvis1_view_tableau_sptr& view )
{
  if( !tableau_.ptr() )
    return false;

  bool is_mouse_over = ( bvis1_manager::instance()->active_tableau()
                         == view->selector()->active_tableau() );

  if ( e.type == vgui_DRAW )
    return false;

  // Draw neighbors as overlays
  if( e.type == vgui_DRAW_OVERLAY){
    if(object_){
      dbecl_episeg_sptr episeg = object_->episeg();
      if(draw_bounds_)
        draw_bounds(episeg);
      if(draw_neighbors_){
        vgui_style::new_style(1.0f, 0.5f, 1.0f, 1.0f, 2.0f)->apply_all();
        if(episeg->next())
          dbecl_episeg_soview2D(episeg->next()).draw();
        if(episeg->prev())
          dbecl_episeg_soview2D(episeg->prev()).draw();
      }
    }
     
    // Draw the epipolar line through the mouse position
    if(draw_epipolar_line_ && epipole_){
      vgui_style::new_style(1.0f, 1.0f, 1.0f, 5.0f, 1.0f)->apply_all();
      vgl_point_2d<double> ep = epipole_->location();
      vgl_line_2d<double> epl(ep, vgl_point_2d<double>(last_x_,last_y_));
      vgui_soview2D_point(ep.x(), ep.y()).draw();
      vgui_draw_line(epl.a(), epl.b(), epl.c());

      if(object_){
        double angle = epipole_->angle(last_x_, last_y_);
        if(angle < object_->episeg()->max_angle() &&
           angle > object_->episeg()->min_angle() ){
          vgl_point_2d<double> pt = object_->episeg()->point(angle);
          vgui_soview2D_point(pt.x(), pt.y()).draw();
        }
      }
    }
    return !is_mouse_over;
  }

  if( !is_mouse_over )
    return true;
    
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);
  last_x_ = ix;
  last_y_ = iy;
  

  if( e.type == vgui_MOUSE_DOWN && e.button == vgui_LEFT ){
    if( object_ ){
      
      if(print_stats_)
        print_stats(object_->episeg());
    }
  }

  if( e.type == vgui_MOTION){
    tableau_->motion(e.wx, e.wy);
    
    vgui_soview2D* curr_obj =  (vgui_soview2D*)tableau_->get_highlighted_soview();
    if( curr_obj != object_ ){ 
      object_ = (dbecl_episeg_soview2D*)curr_obj;
      if(object_)
        epipole_ = object_->episeg()->epipole();
      tableau_->post_overlay_redraw();
    }
    if(draw_epipolar_line_)
      tableau_->post_overlay_redraw();

    if(epipole_){
      double s, a;
      epipole_->to_epi_coords(double(ix),double(iy),s,a);
      vgui::out.width(6);
      vgui::out.fill(' ');
      vgui::out.precision(6);
      vgui::out << "image coords ("<<ix<<", "<<iy<<")  epipolar coords ("<<s<<", "<<a<<")\n";
    }

    return true;
  }
  return false;
}


//: Add popup menu items
void 
dbecl_episeg_inspector_tool::get_popup( const vgui_popup_params& /*params*/, 
                                     vgui_menu &menu )
{
  vcl_string on = "[x] ", off = "[ ] ";
  menu.add( ((draw_bounds_)?on:off)+"Draw Bounds", 
            bvis1_tool_toggle, (void*)(&draw_bounds_) );
  menu.add( ((draw_neighbors_)?on:off)+"Draw Neighbors", 
            bvis1_tool_toggle, (void*)(&draw_neighbors_) );
  menu.add( ((draw_epipolar_line_)?on:off)+"Draw Epipolar Line", 
            bvis1_tool_toggle, (void*)(&draw_epipolar_line_) );
  menu.add( ((print_stats_)?on:off)+"Print Stats", 
            bvis1_tool_toggle, (void*)(&print_stats_) );
}


