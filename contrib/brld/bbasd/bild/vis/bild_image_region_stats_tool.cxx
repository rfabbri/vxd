// This is basic/dbil/vis/dbil_image_region_stats_tool.cxx
//:
// \file

#include "dbil_image_region_stats_tool.h"
#include <bvis1/bvis1_manager.h>
#include <vgui/vgui_style.h>
#include <vgui/vgui.h>
#include <vgl/vgl_polygon_scan_iterator.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_convert.h>

//: Constructor
dbil_image_region_stats_tool::dbil_image_region_stats_tool()
 : region_(),
   draw_mode_(false),
   left_button_down_(false)
{
  
}


//: Destructor
dbil_image_region_stats_tool::~dbil_image_region_stats_tool()
{
}

bool 
dbil_image_region_stats_tool::set_storage ( const bpro1_storage_sptr& storage)
{
  if(!dbil_image_tool::set_storage(storage))
    return false;
  
  double dummy=0.0;
  dimage_ = vil_convert_cast(dummy, storage_->get_image()->get_view()); 
  display_stats_bar();
  bvis1_manager::instance()->post_overlay_redraw();
  return true;
}


//: Handle all events
bool 
dbil_image_region_stats_tool::handle( const vgui_event & e, 
                                      const bvis1_view_tableau_sptr& view )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  if( e.type == vgui_BUTTON_DOWN && e.button == vgui_LEFT ){
    left_button_down_ = true;
    if(!draw_mode_){
      region_.new_sheet();
      draw_mode_ = true;
    }
    region_.push_back(ix,iy);
    display_stats_bar();
    bvis1_manager::instance()->post_overlay_redraw();
    return true;
  }
  if( e.type == vgui_BUTTON_DOWN && e.button == vgui_MIDDLE ){
    draw_mode_ = false;
    bvis1_manager::instance()->post_overlay_redraw();
    return true;
  }
  if( e.type == vgui_BUTTON_UP && e.button == vgui_LEFT ){
    left_button_down_ = false;
    return true;
  }
  
  if( e.type == vgui_MOTION ) 
  {
    last_pt_ = vgl_point_2d<float>(ix,iy);
    // continuously add point if holding down the left mouse button
    if( draw_mode_ ){
      if(left_button_down_){
        region_.push_back(ix,iy);
        display_stats_bar();
      }
      bvis1_manager::instance()->post_overlay_redraw();
      return true;
    } 
  }
  
  if( e.type == vgui_KEY_PRESS && e.key == vgui_DELETE ){
    // remove the last sheet
    vcl_vector<vgl_polygon<float>::sheet_t> old_sheets;
    for (unsigned int s = 0; s < region_.num_sheets()-1; ++s)
      old_sheets.push_back(region_[s]);
    region_.clear();
    region_ = vgl_polygon<float>(old_sheets);
    draw_mode_ = false;
    display_stats_bar();
    bvis1_manager::instance()->post_overlay_redraw();
    return true;
  }

  // Draw the region boundaries
  if( e.type == vgui_OVERLAY_DRAW ) 
  {
    glLineWidth(1);
    glColor3f(1,1,1);
    unsigned int num_sheets = region_.num_sheets();
    if(draw_mode_) --num_sheets;
    for (unsigned int s = 0; s < num_sheets; ++s){
      glBegin( GL_LINE_LOOP );
      for (unsigned int p = 0; p < region_[s].size(); ++p){ 
        glVertex2f(region_[s][p].x(), region_[s][p].y());
      }
      glEnd();
    }
    if(draw_mode_){
      glColor3f(1,0,0);
      glBegin( GL_LINE_LOOP );
      for (unsigned int p = 0; p < region_[num_sheets].size(); ++p){ 
        glVertex2f(region_[num_sheets][p].x(), region_[num_sheets][p].y());
      }
      if(!left_button_down_)
        glVertex2f(last_pt_.x(), last_pt_.y());
      glEnd();
    }
    
    return true;
  }

  return false;
}


//: Calculate basic statistics and display in the status bar        
void 
dbil_image_region_stats_tool::display_stats_bar() const
{

  vgl_polygon_scan_iterator<float> psi(region_);
  double mean = 0.0;
  double var = 0.0;
  unsigned int count = 0;
  for (psi.reset(); psi.next(); ) {
    int y = psi.scany();
    for (int x = psi.startx(); x <= psi.endx(); ++x) {
      if(x >=0 && x < (int)dimage_.ni() && y >= 0 && y < (int)dimage_.nj()){
        double value = 0.0;
        if(dimage_.nplanes() == 1)
          value = dimage_(x,y,0);
        else if(dimage_.nplanes() == 3)
          value = 0.2125*dimage_(x,y,0) + 0.7154*dimage_(x,y,1) + 0.0721*dimage_(x,y,2);
 
        mean += value;
        var += value*value;
        ++count;
      }
    }
  }
  
  mean /= (count>0)?count:1;
  var = var - (mean*mean)*count;
  var /= (count>1)?count-1:1;
  vgui::out << "intensity: mean="<<mean<<" stdev="<<vcl_sqrt(var)<<'\n';
}

