// This is basic/bild/vis/bild_image_region_stats_tool.h
#ifndef bild_image_region_stats_tool_h_
#define bild_image_region_stats_tool_h_
//:
// \file
// \brief A tool for looking at image statistics in a region
// \author Based on original code by  Matt Leotta
// \date 06/27/2005
//
// \verbatim
//  Modifications
// \endverbatim

#include "bild_image_tool.h"
#include <vgl/vgl_polygon.h>
#include <vil/vil_image_view.h>

//: A tool for looking at image gradients in a patch
class bild_image_region_stats_tool : public bild_image_tool 
{
 public:

  //: Constructor
  bild_image_region_stats_tool();
  //: Destructor
  virtual ~bild_image_region_stats_tool();
  
  virtual bool set_storage ( const bpro1_storage_sptr& storage);

  //: Handle all events
  bool handle( const vgui_event & e, 
               const bvis1_view_tableau_sptr& view );
       
  //: Calculate basic statistics and display in the status bar        
  void display_stats_bar() const;

  //: The tool name
  vcl_string name() const { return "Region Stats"; }
  
 protected:
  
  vgl_polygon<float> region_;
  vil_image_view<double> dimage_;
  vgl_point_2d<float> last_pt_;
  bool draw_mode_;
  bool left_button_down_;

};

#endif //bild_image_region_stats_tool_h_
