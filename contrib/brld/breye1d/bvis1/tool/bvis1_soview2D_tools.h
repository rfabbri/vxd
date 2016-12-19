// This is breye1/bvis1/tool/bvis1_soview2D_tools.h
#ifndef bvis1_soview2D_tools_h_
#define bvis1_soview2D_tools_h_
//:
// \file
// \brief Tools that work on any soview object
// \author Matt Leotta, (mleotta@lems.brown.edu)
// \date 2/6/04
//
// This file contains bvis1_translate_tool
//
// \verbatim
//  Modifications
// \endverbatim

#include <bvis1/bvis1_tool.h>
#include <bpro1/bpro1_storage_sptr.h>
#include <vgui/vgui_event_condition.h>
#include <vgui/vgui_soview2D.h>
#include <vgui/vgui_displaylist2D_tableau.h>


//: A tool for translating soview objects by calling their translate method
class bvis1_translate_tool : public bvis1_tool {
public:
  //: Constructor
  bvis1_translate_tool( const vgui_event_condition& lift = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true),
                       const vgui_event_condition& drop = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true) );
  //: Destructor
  virtual ~bvis1_translate_tool();

  //: Returns the string name of this tool
  virtual vcl_string name() const;

  //: Set the tableau to work with
  virtual bool set_tableau( const vgui_tableau_sptr& tableau );

  //: handle events
  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

protected:
  vgui_event_condition gesture_lift_;
  vgui_event_condition gesture_drop_;
  float diff_x, diff_y;
  float last_x, last_y;

private:
  bool active_;
  vgui_soview2D * object_;
  vgui_displaylist2D_tableau_sptr tableau_;
  vgui_displaylist2D_tableau_sptr cached_tableau_;
};


//: A tool for changing the style of soview objects
class bvis1_style_tool : public bvis1_tool {
public:
  //: Constructor
  bvis1_style_tool( const vgui_event_condition& select = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true) );
  //: Destructor
  virtual ~bvis1_style_tool();

  //: Returns the string name of this tool
  virtual vcl_string name() const;

  //: Set the tableau to work with
  virtual bool set_tableau( const vgui_tableau_sptr& tableau );

  //: handle events
  bool handle( const vgui_event & e,
               const bvis1_view_tableau_sptr& view );

protected:
  vgui_event_condition gesture_select_;

private:
  vgui_soview2D * object_;
  vgui_displaylist2D_tableau_sptr tableau_;
};

//: A tool for creating keyholes to look at local image patches
class bvis1_keyhole_tool : public bvis1_tool {
public:
  //: Constructor
  bvis1_keyhole_tool();
  //: Destructor
  virtual ~bvis1_keyhole_tool();

  //: Returns the string name of this tool
  virtual vcl_string name() const;

  //: Set the tableau to work with
  virtual bool set_tableau( const vgui_tableau_sptr& tableau );

  //: handle events
  bool handle( const vgui_event & e,
               const bvis1_view_tableau_sptr& view );

  virtual bpro1_storage_sptr active_storage() {return 0;} //FIX LATER JLM
  virtual vcl_vector< vcl_string > get_input_type()//FIX LATER JLM
    {return vcl_vector< vcl_string >();}
  
  virtual vcl_vector< vcl_string > get_output_type()//FIX LATER JLM
    {return vcl_vector< vcl_string >();}

protected:
  vgui_tableau_sptr tableau_;
  float x0 , y0;
  float r;

};

#endif // bvis1_soview2D_tools_h_
