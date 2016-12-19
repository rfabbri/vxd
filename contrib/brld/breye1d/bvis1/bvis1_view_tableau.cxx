// This is bvis1/bvis1_view_tableau.cxx
#include "bvis1_view_tableau.h"
//:
// \file
// \author Matt Leotta
// \brief  See bvis1_view_tableau.h for a description of this file.

#include <bvis1/bvis1_manager.h>
#include <vgui/vgui.h>
#include <vgui/vgui_menu.h>


//: Constructor 
bvis1_view_tableau::bvis1_view_tableau()
{
}


//: Constructor 
bvis1_view_tableau::bvis1_view_tableau(const vgui_selector_tableau_sptr selector,
                                     int offset, bool absolute)
 : child_(this,selector), offset_(offset), absolute_(absolute)
{
}


bvis1_view_tableau::~bvis1_view_tableau()
{
}


vcl_string bvis1_view_tableau::type_name() const
{
  return "bvis1_view_tableau";
}


//: Access the selector tableau
vgui_selector_tableau_sptr 
bvis1_view_tableau::selector() const
{ 
  vgui_selector_tableau_sptr selector;
  selector.vertical_cast(child_.child());
  return selector; 
}


//: Access the current frame to draw
int 
bvis1_view_tableau::frame() const
{
  if(absolute_)
    return offset_;
  else
    return bvis1_manager::instance()->current_frame() + offset_;
}


bool bvis1_view_tableau::handle(vgui_event const& e)
{
  bool ret = false;
  
  //intercept event and send it to the current active tool
  bvis1_tool_sptr active_tool = bvis1_manager::instance()->active_tool();
  
  if( active_tool != NULL ) {
    //get the active tableau as reported by the grid tableau
    vgui_tableau_sptr active_tab = bvis1_manager::instance()->active_tableau();
    
    //need to check if everything went smoothly here
    bool tool_ok = active_tool->set_tableau( active_tab );
    
    //only send the event to the tool if the tool has all the necessary
    //information
    if (tool_ok)
      ret = active_tool->handle( e, this );
    else {
      // send complaints to the status bar
      vgui::out << "Tool cannot use active tableau!\n";
    }
  }
  
  if (ret) return true; //event has been handled by the active tool
  if (selector()->handle(e)) return true;

  // Handle the ESC key on the way back up the traversal only
  // if it is not handled by anyone else
  if (e.type == vgui_KEY_PRESS && e.key == vgui_ESC){
    // disable the current tool
    bvis1_manager::instance()->set_active_tool(NULL);
    vgui::out << "Tool Deactivated\n";
    return true;
  }
  return false;
}


void 
bvis1_view_tableau::get_popup(const vgui_popup_params& params, vgui_menu &menu)
{
  selector()->get_popup(params, menu);
  bvis1_tool_sptr active_tool = bvis1_manager::instance()->active_tool();
  if( active_tool.ptr() != NULL ) {
    vgui_menu submenu;
    active_tool->get_popup(params, submenu);
    menu.add("Tool Options", submenu);
  }
}
