// This is breye1/bvis1/bvis1_tool.h
#ifndef bvis1_tool_h_
#define bvis1_tool_h_
//:
// \file
// \brief To debug the scurve computations from shock edges 
// \author Mark Johnson (mrj)
// \date Thu Aug 14 13:39:20 EDT 2003
//
// \verbatim
//  Modifications
//
//    Amir Tamrakar 05/15/06       Added more functions for tool parameter changes
//
// \endverbatim

#include <vcl_string.h>
#include <vbl/vbl_ref_count.h>
#include <vgui/vgui_event.h>
#include <vgui/vgui_popup_params.h>
#include <vgui/vgui_menu.h>
#include <bvis1/bvis1_view_tableau_sptr.h>

class bvis1_tool : public vbl_ref_count 
{
public:
  bvis1_tool();
  virtual ~bvis1_tool();

  //: Return the name of the tool
  virtual vcl_string name() const = 0;

  //: Handle events
  // \note This function is called once for most events \b before the event
  // is handled by the tableau under the mouse.  For draw and overlay events
  // this function is called once for each view in the grid
  virtual bool handle( const vgui_event& e, 
                       const bvis1_view_tableau_sptr& view ) = 0;

  //: Allow the tool to add to the popup menu as a tableau would
  virtual void get_popup(const vgui_popup_params& params, vgui_menu &menu);

  //: Set the tableau associated with the current view
  virtual bool set_tableau ( const vgui_tableau_sptr& tableau ) = 0;

  //: This is called when the tool is activated
  virtual void activate();

  //: This is called when the tool is deactivated
  virtual void deactivate();

};


//: Use in menus to toggle a parameter
void bvis1_tool_toggle(const void* boolref);

//: Use in menus to toggle a parameter
void bvis1_tool_inc(const void* intref);

//: Use in menus to toggle a parameter
void bvis1_tool_dec(const void* intref);

//: Use in menus to set the value of a double parameter
void bvis1_tool_set_param(const void* doubleref);

//: Use in menus to set the value of a string parameter
void bvis1_tool_set_string(const void* vcl_stringref);

#endif //bvis1_tool_h_
