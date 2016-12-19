/**************************************************************/
/*  Name: Mark Johnson (mrj)
//  File: bvis1_vsol2D_tools.h
//  Asgn: bvis
//  Date: Thu Aug 14 15:24:53 EDT 2003
***************************************************************/

#ifndef bvis1_vsol2D_tools_header
#define bvis1_vsol2D_tools_header

#include <bvis1/bvis1_tool.h>
#include <vgui/vgui_event_condition.h>
#include <vgui/vgui_projection_inspector.h>
#include <vgui/vgui_soview2D.h>
#include <bgui/bgui_vsol_soview2D.h>

#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_digital_curve_2d.h>

#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>

#include <bgui/bgui_vsol2D_tableau_sptr.h>
#include <bgui/bgui_vsol2D_tableau.h>

#include <vcl_set.h>

class bvis1_vsol2D_tool : public bvis1_tool
{
public:
  bvis1_vsol2D_tool();
  virtual ~bvis1_vsol2D_tool();

  virtual bool set_tableau ( const vgui_tableau_sptr& tableau );
  virtual bool set_storage ( const bpro1_storage_sptr& storage);

  virtual vcl_string name() const = 0;
  
  //: Allow the tool to add to the popup menu as a tableau would
  virtual void get_popup(const vgui_popup_params& params, vgui_menu &menu);
  
  //: Return the active group name
  vcl_string active_group() const { return active_group_; }
  
  //: Set the active group name
  void set_active_group(const vcl_string& name) { active_group_ = name; }

protected:

  vgui_soview2D* object_;
  bgui_vsol2D_tableau_sptr tableau();
  vidpro1_vsol2D_storage_sptr storage();

  bgui_vsol2D_tableau_sptr tableau_;
  bpro1_storage_sptr storage_;
  
  vcl_string active_group_;

};

// ***************************************************
// Various Geometry tools
// ***************************************************

//: A tool for drawing points
class bvis1_vsol2D_point_tool : public bvis1_vsol2D_tool {

public:

  bvis1_vsol2D_point_tool();
  virtual ~bvis1_vsol2D_point_tool();

  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  vcl_string name() const;

protected:

  vgui_event_condition gesture0;
  //: point deletion gesture
  vgui_event_condition gesture1;

private:

};

//: A tool for drawing lines
class bvis1_vsol2D_line_tool : public bvis1_vsol2D_tool {

public:

  bvis1_vsol2D_line_tool();
  virtual ~bvis1_vsol2D_line_tool();

  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  vcl_string name() const;
  void deactivate();
  
protected:

  vgui_event_condition gesture0;
  vgui_event_condition gesture1;

  bgui_vsol2D_tableau_sptr active_tableau_;
  vidpro1_vsol2D_storage_sptr active_storage_;
  
  bool active;
  float x0 , y0;
  float x1 , y1;
  
private:

};


//: A tool for drawing polylines
class bvis1_vsol2D_polyline_tool : public bvis1_vsol2D_tool {

public:

  bvis1_vsol2D_polyline_tool();
  virtual ~bvis1_vsol2D_polyline_tool();

  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  vcl_string name() const;
  void deactivate();

protected:

  vgui_event_condition gesture0;
  vgui_event_condition gesture1;
  vgui_event_condition gesture2;

  bgui_vsol2D_tableau_sptr active_tableau_;
  vidpro1_vsol2D_storage_sptr active_storage_;
  
private:

  bool active;
  vcl_vector< vsol_point_2d_sptr > point_list;
  float last_x;
  float last_y;
};


//: A tool for drawing polygons
class bvis1_vsol2D_polygon_tool : public bvis1_vsol2D_tool {

public:

  bvis1_vsol2D_polygon_tool();
  virtual ~bvis1_vsol2D_polygon_tool();

  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  vcl_string name() const;
  void deactivate();

protected:

  vgui_event_condition gesture0;
  vgui_event_condition gesture1;
  vgui_event_condition gesture2;  

  bgui_vsol2D_tableau_sptr active_tableau_;
  vidpro1_vsol2D_storage_sptr active_storage_;
  
private:

  bool active;
  vcl_vector< vsol_point_2d_sptr > point_list;
  float last_x;
  float last_y;
};


//: A tool for drawing digital curves
class bvis1_vsol2D_digital_curve_tool : public bvis1_vsol2D_tool {

public:

  bvis1_vsol2D_digital_curve_tool();
  virtual ~bvis1_vsol2D_digital_curve_tool();

  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  vcl_string name() const;
  void deactivate();

protected:

  vgui_event_condition gesture0;
  vgui_event_condition gesture1;
  vgui_event_condition gesture2;

  bgui_vsol2D_tableau_sptr active_tableau_;
  vidpro1_vsol2D_storage_sptr active_storage_;
  
private:

  bool active;
  vcl_vector< vsol_point_2d_sptr > point_list;
  float last_x;
  float last_y;
};


//: A tool for splitting digital curves
class bvis1_vsol2D_split_curve_tool : public bvis1_vsol2D_tool {

public:

  bvis1_vsol2D_split_curve_tool();
  virtual ~bvis1_vsol2D_split_curve_tool();

  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  vcl_string name() const;
  void deactivate();

protected:

  vgui_event_condition gesture_click;
  
private:
  bgui_vsol_soview2D_digital_curve* object_;
  double index_;
};


//: Inspect tool to use describe function of vsol_point_2d, vsol_line_2d, vsol_polyline_2d
// and vsol_polygon_2d and print out info about selected soview object

class  bvis1_vsol2D_inspect_tool: public bvis1_vsol2D_tool {
public:
    //: Constructor
    bvis1_vsol2D_inspect_tool( const vgui_event_condition& lift = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true));
    //: Destructor
    virtual ~bvis1_vsol2D_inspect_tool();

    //: Returns the string name of this tool
    vcl_string name() const;

    void deactivate();

    //: Set the tableau to work with
    virtual bool set_tableau( const vgui_tableau_sptr& tableau );

    //: handle events
    bool handle( const vgui_event & e, const bvis1_view_tableau_sptr& selector );

protected:
    vgui_event_condition gesture_lift_;


private:
    bool active_;
    vgui_soview2D * object_;
    vgui_easy2D_tableau_sptr tableau_;

};

//: Selection tool to create a new vsol2D storage class and tableau from the selected vsols
class  bvis1_vsol2D_selection_tool: public bvis1_vsol2D_tool {
public:
    //: Constructor
    bvis1_vsol2D_selection_tool( const vgui_event_condition& add = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true),
                                const vgui_event_condition& set = vgui_event_condition(vgui_LEFT, vgui_SHIFT, true));
    //: Destructor
    virtual ~bvis1_vsol2D_selection_tool();

    //: Returns the string name of this tool
    vcl_string name() const;

    void deactivate();

    //: Set the tableau to work with
    virtual bool set_tableau( const vgui_tableau_sptr& tableau );

    //: handle events
    bool handle( const vgui_event & e, const bvis1_view_tableau_sptr& selector );

protected:
    vgui_event_condition gesture_add_;
    vgui_event_condition gesture_set_;

private:
    bool active_;
    vgui_soview2D * object_;
    vgui_easy2D_tableau_sptr tableau_;
    vcl_set<vsol_spatial_object_2d_sptr> objs_;

};


//: translate the vertices of any object built from vsol_point_2d objects 
class  bvis1_vsol2D_adjust_tool: public bvis1_vsol2D_tool {
public:
  //: Constructor
  bvis1_vsol2D_adjust_tool( const vgui_event_condition& lift = vgui_event_condition(vgui_LEFT, 
                                                                                    vgui_MODIFIER_NULL, 
                                                                                    true), 
                            const vgui_event_condition& drop = vgui_event_condition(vgui_LEFT, 
                                                                                    vgui_MODIFIER_NULL, 
                                                                                    false) );
  //: Destructor
  virtual ~bvis1_vsol2D_adjust_tool();

  //: Returns the string name of this tool
  vcl_string name() const;

  void deactivate();
  
  //: extract the point set for manipulation from this object
  void set_points(const vsol_spatial_object_2d_sptr& obj);

  //: handle events
  bool handle( const vgui_event & e, const bvis1_view_tableau_sptr& selector );

protected:
  vgui_event_condition gesture_lift_;
  vgui_event_condition gesture_drop_;


private:
  float last_x_;
  float last_y_;
  bool active_;
  bool cycle_;
  bgui_vsol_soview2D * object_;
  vcl_vector<vsol_point_2d_sptr> points_;
  vsol_point_2d_sptr active_pt_;

};
//: Crop the vsol objects in a region specified by user
class  bvis1_vsol2D_crop_tool: public bvis1_vsol2D_tool {
public:
  //: Constructor
  bvis1_vsol2D_crop_tool();
  //: Destructor
  virtual ~bvis1_vsol2D_crop_tool();

  //: Returns the string name of this tool
  vcl_string name() const;

  void deactivate();
  

  void set_points(const vsol_spatial_object_2d_sptr& obj);
  //: handle events
  bool handle( const vgui_event & e, const bvis1_view_tableau_sptr& selector );

  void get_popup( const vgui_popup_params& params, 
                                            vgui_menu &menu );
protected:


private:
    float last_x;
    float last_y;

    bool active_;

    vcl_string active_group_;

    vgui_event_condition gesture0;
    vgui_event_condition gesture1;
    vgui_event_condition gesture2;

    vcl_vector<vsol_point_2d_sptr> point_list;

    bool draw_polygon_;


    bgui_vsol_soview2D * object_;
    vcl_vector<vsol_point_2d_sptr> points_;
    vsol_point_2d_sptr active_pt_;

};

#endif

