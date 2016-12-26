// This is brcv/basic/dbsol/vis/dbsol_cut_contour_tool.h
#ifndef dbsol_cut_contour_tool_h_
#define dbsol_cut_contour_tool_h_
//:
// \file
// \brief 
// author Nhon Trinh (ntrinh@lems.brown.edu)
// date Oct 30, 2006
//
// \verbatim
//  Modifications
// \endverbatim


//#include <vgui/vgui_style.h>
#include <bvis1/tool/bvis1_vsol2D_tools.h>


// =============================================================================
// dbsol_cut_contour_tool
// =============================================================================

//: An abstract base class for tools in this file
class dbsol_cut_contour_tool : public bvis1_vsol2D_tool
{
public:

  dbsol_cut_contour_tool();


  virtual vcl_string name() const {return "Cut contour"; }
  
  ////: Allow the tool to add to the popup menu as a tableau would
  //virtual void get_popup(const vgui_popup_params& params, vgui_menu &menu);
  
  //: This is called when the tool is activated
  virtual void activate();

  //: This is called when the tool is deactivated
  virtual void deactivate();


  // DATA ACCESS ---------------------------------------------------------------

  
  // EVENT HANDLERS ------------------------------------------------------------
  

  //: Handle events
  // \note This function is called once for most events \b before the event
  // is handled by the tableau under the mouse.  For draw and overlay events
  // this function is called once for each view in the grid
  virtual bool handle(const vgui_event & e, const bvis1_view_tableau_sptr& view );

  //:
  bool handle_cut_polyline(float ix, float iy);
  bool handle_open_polygon(float ix, float iy);
  bool handle_close_polyline(float ix, float iy);
  bool handle_delete_vsol(float ix, float iy);
  bool handle_undo();

  bool handle_pick_connect_polyline1(float ix, float iy);
  bool handle_pick_connect_polyline2(float ix, float iy);

  //: When cursor is moved, we want to display the start and end points of the
  // highlighted contour
  bool handle_overlay_redraw(float ix, float iy);

  // Draw a circle, estimated as a polygon
  void draw_point(float cx, float cy, float radius);

  
  // MEMBER VARIABLES ----------------------------------------------------------

  enum cut_contour_mode
  {
    NORMAL = 0,
    CONNECT_POLYLINE = 1
  };
protected:

  // current cursor position
  float ix_;
  float iy_;


  vcl_vector<vsol_spatial_object_2d_sptr > last_removed_;
  vcl_vector<vsol_spatial_object_2d_sptr > last_added_;

  cut_contour_mode active_mode_;

  // user gestures
 
  //: cut a polyline
  vgui_event_condition gesture_cut_polyline_;

  //: open a polygon and turn into a polyline
  vgui_event_condition gesture_open_polygon_;

  //: close a polyline and turn into a polygon
  vgui_event_condition gesture_close_polyline_;

  //: connect two polylines a polyline
  vgui_event_condition gesture_switch_connect_polyline_mode_;

  //: Switch back to normal mode
  vgui_event_condition gesture_switch_normal_mode_;

  //: delete highlighted vsol object (line or polyline)
  vgui_event_condition gesture_delete_vsol_;

  //: undo previous action
  vgui_event_condition gesture_undo_;

  // Status variables for connect-two-polyline mode

  //: Pick polyline1 to connect
  vgui_event_condition gesture_pick_connect_polyline1_;

  //: Pick polyline2 to connect
  vgui_event_condition gesture_pick_connect_polyline2_;

  //: Polyline 1 to connect
  vsol_polyline_2d_sptr connect_polyline1_;
  
  //: Where to connect from polyline1
  bool connect_polyline1_at_start_;

  //: Polyline 2 to connect
  vsol_polyline_2d_sptr connect_polyline2_;

  //: Where to connect from polyline2
  bool connect_polyline2_at_start_;

};



#endif // dbsol_cut_contour_tool_h_
