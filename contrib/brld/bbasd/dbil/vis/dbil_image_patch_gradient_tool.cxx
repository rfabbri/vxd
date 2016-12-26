// This is basic/dbil/vis/dbil_image_patch_gradient_tool.cxx
//:
// \file

#include "dbil_image_patch_gradient_tool.h"
#include <bvis1/bvis1_manager.h>
#include <vgui/vgui_style.h>

dbil_image_patch_gradient_tool::dbil_image_patch_gradient_tool()
{
  left_click = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
}

dbil_image_patch_gradient_tool::~dbil_image_patch_gradient_tool()
{
}

bool dbil_image_patch_gradient_tool::handle( const vgui_event & e, 
                               const bvis1_view_tableau_sptr& view )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  if( e.type == vgui_MOTION ) 
  {
     bvis1_manager::instance()->post_overlay_redraw();
  }

  if( e.type == vgui_OVERLAY_DRAW ) 
  {
    glLineWidth(1);
    glColor3f(1,1,1);

    //glBegin( GL_LINE_LOOP );
    glBegin( GL_LINE_STRIP );
    glVertex2f(ix-5, iy-5);
    glVertex2f(ix-5, iy+5);
    //glVertex2f(ix+5, iy+5);
    //glVertex2f(ix+5, iy-5);
    glEnd();

    return true;
  }

  return false;
}

vcl_string dbil_image_patch_gradient_tool::name() const
{
  return "Patch Gradient";
}
