// This is breye1/bvis1/bvis1_tool.cxx
//:
// \file

#include <vgui/vgui_dialog.h>

#include "bvis1_tool.h"

//: Constructor
bvis1_tool::bvis1_tool()
{
}


//: Destructor
bvis1_tool::~bvis1_tool()
{
}


//: Allow the tool to add to the popup menu as a tableau would
void
bvis1_tool::get_popup(const vgui_popup_params& /*params*/, vgui_menu &/*menu*/)
{
}


//: This is called when the tool is activated
void 
bvis1_tool::activate()
{
}


//: This is called when the tool is activated
void 
bvis1_tool::deactivate()
{
}

//: Use in menus to toggle a parameter
void bvis1_tool_toggle(const void* boolref)
{
  bool* bref = (bool*) boolref;
  *bref = !(*bref);
}

//: Use in menus to toggle a parameter
void bvis1_tool_inc(const void* intref)
{
  int* iref = (int*) intref;
  *iref = (*iref)++;
}

//: Use in menus to toggle a parameter
void bvis1_tool_dec(const void* intref)
{
  int* iref = (int*) intref;
  if ((*iref)>1)
    *iref = (*iref)--;
}

//: Use in menus to set the value of a double parameter
void bvis1_tool_set_param(const void* doubleref)
{
  double* dref = (double*)doubleref;
  double param_val = *dref;
  vgui_dialog param_dlg("Set Tool Param");
  param_dlg.field("Parameter Value", param_val);
  if(!param_dlg.ask())
    return;

  *dref = param_val;
}

//: Use in menus to set the value of a string paramter
void bvis1_tool_set_string(const void* vcl_stringref)
{
  vcl_string* dref = (vcl_string*)vcl_stringref;
  vcl_string param_val = *dref;
  vgui_dialog param_dlg("Set Tool Param");
  param_dlg.field("Parameter Value", param_val);
  if(!param_dlg.ask())
    return;

  *dref = param_val;
}
