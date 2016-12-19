// This is breye1/bvis1/tool/bvis1_soview2D_tools.cxx
//:
// \file

#include "bvis1_soview2D_tools.h"
#include <vgui/vgui_projection_inspector.h>
#include <vgui/vgui_style.h>
#include <vgui/vgui_dialog.h>
#include <vcl_sstream.h>
#include <vnl/vnl_random.h>
#include <vil/algo/vil_colour_space.h>


static vnl_random g_myrand;

//Constructor
bvis1_translate_tool::bvis1_translate_tool( const vgui_event_condition& lift,
                                          const vgui_event_condition& drop )
 : gesture_lift_(lift), gesture_drop_(drop), active_(false), object_(NULL),
   tableau_(NULL), cached_tableau_(NULL)
{
}


//: Destructor
bvis1_translate_tool::~bvis1_translate_tool()
{
}


//: Return the name of this tool
vcl_string
bvis1_translate_tool::name() const
{
  return "Translate";
}


//: Set the tableau to work with
bool
bvis1_translate_tool::set_tableau( const vgui_tableau_sptr& tableau )
{
  tableau_ = vgui_displaylist2D_tableau_sptr(dynamic_cast<vgui_displaylist2D_tableau*>(tableau.ptr()));
  if( !tableau_  ) 
    return false;
  
  return true;
}

//: Handle events
bool
bvis1_translate_tool::handle( const vgui_event & e, 
                             const bvis1_view_tableau_sptr& view )
{ 
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  // cache the current position as the last known position
  if( e.type == vgui_MOTION ) {
    last_x = ix;
    last_y = iy;
  }
  
  if( active_ ){
    if( gesture_drop_(e) ){
      float x,y;
      object_->get_centroid( &x , &y );
      object_->translate( last_x - x - diff_x , last_y - y - diff_y );
      active_ = false;
      object_ = NULL;
      cached_tableau_->highlight( 0 );
      cached_tableau_->motion( e.wx , e.wy );
      cached_tableau_->post_redraw();
      cached_tableau_ = vgui_displaylist2D_tableau_sptr(NULL);
      return true;
    }
    else if ( e.type == vgui_MOTION ){
      float x,y;
      object_->get_centroid( &x , &y );
      object_->translate( last_x - x - diff_x , last_y - y - diff_y );
      cached_tableau_->post_redraw();
      return true;
    }
  }
  else{
    if( tableau_.ptr() && gesture_lift_(e) ){
      object_ = (vgui_soview2D*)tableau_->get_highlighted_soview();
      if( object_ != NULL ) {
        float x, y;
        active_ = true;
        cached_tableau_ = tableau_;
        object_->get_centroid( &x , &y );
        // the distance to the centroid
        diff_x = last_x - x;
        diff_y = last_y - y;
        return true;
      }
    }
  }
  
  return false;
}

//============================== Sytle Tool ============================

//Constructor
bvis1_style_tool::bvis1_style_tool( const vgui_event_condition& select )
 : gesture_select_(select), object_(NULL), tableau_(NULL)
{
}


//: Destructor
bvis1_style_tool::~bvis1_style_tool()
{
}


//: Return the name of this tool
vcl_string
bvis1_style_tool::name() const
{
  return "Change Style";
}


//: Set the tableau to work with
bool
bvis1_style_tool::set_tableau( const vgui_tableau_sptr& tableau )
{
  tableau_ = vgui_displaylist2D_tableau_sptr(dynamic_cast<vgui_displaylist2D_tableau*>(tableau.ptr()));
  if( tableau.ptr() == NULL )
    return false;

  return true;
}

//: Handle events
bool
bvis1_style_tool::handle( const vgui_event & e,
                         const bvis1_view_tableau_sptr& view )
{
  if( tableau_ && gesture_select_(e) ){
    object_ = (vgui_soview2D*)tableau_->get_highlighted_soview();
    if( object_ != NULL ) {
      vgui_style_sptr style = object_->get_style();
      if(!style)
        return false;

      vgui_dialog style_dlg("Change Style");
      static vcl_string color = "";
      float point_size = style->point_size;
      float line_width = style->line_width;
      bool random_color=false;

      style_dlg.inline_color("Color",color);
      style_dlg.field("Point Size",point_size);
      style_dlg.field("Line Width",line_width);
      style_dlg.checkbox("Randomize Color",random_color);

      if(!style_dlg.ask())
        return false;

      if (!random_color) {
        style->point_size = point_size;
        style->line_width = line_width;
        vcl_istringstream color_strm(color);
        color_strm >> style->rgba[0] >> style->rgba[1] >> style->rgba[2];
      } else {
        // Loop through all objects and display in random colors

        vcl_vector< vgui_soview * > objs = tableau_->get_all();

        for (unsigned i=0; i < objs.size(); ++i) {
          double r;
          double g;
          double b;

          // randomize hue
          double h = g_myrand.drand64(0,360);

          vil_colour_space_HSV_to_RGB<double>(h,1,255,&r, &g,&b);
          r/=255.0;
          g/=255.0;
          b/=255.0;

          vgui_style_sptr oldstyle = objs[i]->get_style();
          vgui_style_sptr newstyle = vgui_style::new_style(r,g,b,oldstyle->point_size,oldstyle->line_width);
          // After this, all objects are going to have independent color forever, differently from
          // what is implicitly assumed in vsols, where the displayer inserts the styles as smart
          // pointers to the same style.
          objs[i]->set_style(newstyle);
        }
      }


      return true;
    }
  }

  return false;
}

//============================== Keyhole Tool ============================

//Constructor
bvis1_keyhole_tool::bvis1_keyhole_tool()
 : tableau_(NULL), x0(0), y0(0), r(10)
{
}


//: Destructor
bvis1_keyhole_tool::~bvis1_keyhole_tool()
{
}


//: Return the name of this tool
vcl_string
bvis1_keyhole_tool::name() const
{
  return "KeyHole Tool";
}


//: Set the tableau to work with
bool
bvis1_keyhole_tool::set_tableau( const vgui_tableau_sptr& tableau )
{
  tableau_ = tableau;
  if( tableau.ptr() == NULL )
    return false;

  return true;
}

//: Handle events
//: Handle events
bool
bvis1_keyhole_tool::handle( const vgui_event & e,
                         const bvis1_view_tableau_sptr& view )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  if( e.type == vgui_MOTION ) {
    x0 = ix;
    y0 = iy;
    if (tableau_)
      tableau_->post_overlay_redraw();
  }

  if( e.type == vgui_OVERLAY_DRAW ) 
  {
    //form a circular hole around the mouse
    glLineWidth(1);
    glColor3f(0,0,0);
    
    //glBegin(GL_POLYGON);
    //for( int i=0; i<=45; i++){
    //  glVertex2f(x0-r + r*vcl_cos(2*3.14159*i/180.0), 
    //             y0-r + r*vcl_sin(2*3.14159*i/180.0));
    //}
    //glVertex2f(x0, y0);
    //glVertex2f(x0-r, 1000);
    //glVertex2f(1000, 1000);
    //glVertex2f(1000, y0-r);
    //glVertex2f(x0, y0-r);
    //glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x0, 0);
    glVertex2f(1000, 0);
    glVertex2f(1000, 1000);
    glVertex2f(x0, 1000);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x0-2*r, 0);
    glVertex2f(0, 0);
    glVertex2f(0, 1000);
    glVertex2f(x0-2*r, 1000);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x0-2*r, 0);
    glVertex2f(x0, 0);
    glVertex2f(x0, y0);
    glVertex2f(x0-2*r, y0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x0-2*r, y0+2*r);
    glVertex2f(x0, y0+2*r);
    glVertex2f(x0, 1000);
    glVertex2f(x0-2*r, 1000);
    glEnd();


    return true;
  }

  return false;
}

