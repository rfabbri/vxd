// This is dvpgl_epipolar_region_tool.h
#ifndef dvpgl_epipolar_region_tool_h
#define dvpgl_epipolar_region_tool_h
//:
//\file
//\brief Tool for displaying simulated epipolar error bands across frames
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 07/19/2005 12:09:14 AM EDT
//

#include <bvis1/bvis1_tool.h>
#include <bgui/bgui_vsol2D_tableau.h>
#include <bgui/bgui_vsol2D_tableau_sptr.h>

#include <vgui/vgui_event_condition.h>
#include <vgui/vgui_style_sptr.h>
#include <bgui/bgui_vsol_soview2D.h>
#include <vgui/vgui_selector_tableau_sptr.h>

#include <vpgl/vpgl_perspective_camera.h>
#include <vpgl/vpgl_fundamental_matrix.h>



//: This tool displays epipolar lines corresponding to user mouse events.
// It uses Monte Carlo simulation to sample epipolar lines corresponding to points in a localization
// error neighborhood around a given point.  
//
// \seealso The basic tool is similar to dvpgl_epipolar_tool.
//
// \todo extend with perturbations in calibration as well.
//
class dvpgl_epipolar_region_tool : public bvis1_tool {
public:
  //: Constructor
  dvpgl_epipolar_region_tool();
 
  //: Destructor
  virtual ~dvpgl_epipolar_region_tool() {}
  
  //: Returns the string name of this tool
  vcl_string name() const;

  //: Handle events.
  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );

  //: Set the tableau to work with
  bool set_tableau( const vgui_tableau_sptr& /*tableau*/ );
  
  void activate ();
  void deactivate ();

  //: Display dialog to get params
  bool get_epipolar_params(int *initial_view, int *final_view, float *radius);

protected:
  void perturb(vsol_point_2d_sptr &pt, double max_radius) const;

  vgui_event_condition gesture0;
  vgui_soview2D_point *pt_;
  vgl_point_2d<double> point_;
  vgui_soview2D_infinite_line *soview_r_;

  bgui_vsol2D_tableau_sptr tab_l_;
  bgui_vsol2D_tableau_sptr tab_r_;

  vcl_vector<vgui_soview2D_point *> pt_pert_soviews_;
  vcl_vector<vgui_soview2D_infinite_line *> ep_pert_soviews_;

  const vpgl_perspective_camera<double> *p1_;
  const vpgl_perspective_camera<double> *p2_;
  vpgl_fundamental_matrix<double> *fm_;

  bool activated_;
  bool first_time_;

  //: true when the current active tableau is the left one
  bool active_left_tableau_;

  float maxradius_perturb_;
};


#endif // dvpgl_epipolar_region_tool_h

