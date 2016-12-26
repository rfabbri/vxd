// This is bbasd/bsold/vis/bsold_arc_tool.h
#ifndef bsold_arc_tool_h
#define bsold_arc_tool_h
//:
// \file
// \brief A tool for designing a circular arc - full circle for now
// \author Based on original code by  Nhon Trinh (@lems.brown.edu)
// \date 4/9/2005
//
// \verbatim
//  Modifications
//    2005/04/09 Nhon Trinh   Creation
// \endverbatim

#include <bvis1/bvis1_tool.h>
#include <bvis1/tool/bvis1_vsol2D_tools.h>

//: A tool for drawing lines
class bsold_arc_tool : public bvis1_vsol2D_tool {

public:
  //: constructor
  bsold_arc_tool();
  //: destructor
  virtual ~bsold_arc_tool();
  //: handle
  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );
  //: name of the tool
  vcl_string name() const;
  //: deactive the tool - stop tracking mouse position
  void deactivate();
  
protected:
  //: choose center of the circle
  vgui_event_condition choose_center_;
  //: choose a point on the circle
  vgui_event_condition choose_circle_point_;
  
  bgui_vsol2D_tableau_sptr active_tableau_;
  vidpro1_vsol2D_storage_sptr active_storage_;
  
  bool active_;
  float x0_ , y0_;
  float x1_ , y1_;
  
private:

};



#endif // bsold_arc_tool_h

