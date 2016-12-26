// This is brcv/basic/dbsol/vis/dbsol_cut_contour_tool.cxx
//:
// \file

#include "dbsol_cut_contour_tool.h"

#include <vgui/vgui_style.h>
#include <bsol/bsol_algs.h>
#include <dbsol/dbsol_algos.h>
#include <bvis1/bvis1_manager.h>
#include <vnl/vnl_math.h>


dbsol_cut_contour_tool::
dbsol_cut_contour_tool()
{
  this->gesture_cut_polyline_ = 
    vgui_event_condition(vgui_LEFT, vgui_SHIFT, true);
  this->gesture_delete_vsol_ = 
    vgui_event_condition(vgui_key('r'), vgui_MODIFIER_NULL, true);
  this->gesture_open_polygon_ = 
    vgui_event_condition(vgui_RIGHT, vgui_SHIFT, true);
  this->gesture_close_polyline_ = 
    vgui_event_condition(vgui_key('j'), vgui_MODIFIER_NULL, true);
  this->gesture_undo_ = 
    vgui_event_condition(vgui_key('r'), vgui_SHIFT, true);

  //: Switch back to normal mode
  this->gesture_switch_normal_mode_ = 
    vgui_event_condition(vgui_key('q'), vgui_MODIFIER_NULL, true);

  //: connect two polylines a polyline
  this->gesture_switch_connect_polyline_mode_ = 
    vgui_event_condition(vgui_key('g'), vgui_MODIFIER_NULL, true);

  // Pick polyline1 to connect
  this->gesture_pick_connect_polyline1_ = 
    vgui_event_condition(vgui_LEFT, vgui_SHIFT, true);

  // Pick polyline2 to connect
  this->gesture_pick_connect_polyline2_ = 
    vgui_event_condition(vgui_MIDDLE, vgui_SHIFT, true);
}

  
//: This is called when the tool is activated
void dbsol_cut_contour_tool::
activate()
{
  vcl_cout << "=============================================================================\n";
  vcl_cout << "             TOOL INSTRUCTIONS                             \n";
  vcl_cout << " 'q'                   : Swith to DEFAULT mode             \n";
  vcl_cout << " In DEFAULT mode:                                          \n";
  vcl_cout << "   SHIFT + right click : open a polygon                    \n";
  vcl_cout << "   'j'                 : join two ends of a polyline and turn into a polygon  \n";
  vcl_cout << "   SHIFT + left click  : cut a polyline into 2 pieces      \n";
  vcl_cout << "   'r'                 : delete a line or a polyline                          \n";
  vcl_cout << "   'SHIFT + r'         : undo delete a line or a polyline             \n";
  vcl_cout << " 'g'                   : Swith to CONNECT_POLYLINE mode    \n";
  vcl_cout << " In CONNECT_POLYLINE mode:                                 \n";
  vcl_cout << "   SHIFT + left click  : Pick starting point to connect                      \n";
  vcl_cout << "   SHIFT + middle click: Pick ending point to connect                      \n";
  vcl_cout << "===========================================================\n";
}


  
// =======================================================
// DATA ACCESS
// =======================================================

  
//=========================================================
//      EVENT HANDLERS
//=========================================================

//: Handle events coming to the tool
bool dbsol_cut_contour_tool::
handle( const vgui_event & e, const bvis1_view_tableau_sptr& view )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);


  // switch to NORMAL mode
  if (this->gesture_switch_normal_mode_(e))
  {
    this->active_mode_ = dbsol_cut_contour_tool::NORMAL;
    vcl_cout << "\nSwitched to mode NORMAL." << vcl_endl;
    return true;
  }

  // swith to "CONNECT_POLYLINE" mode
  if (this->gesture_switch_connect_polyline_mode_(e))
  {
    this->active_mode_ = dbsol_cut_contour_tool::CONNECT_POLYLINE;
    vcl_cout << "\nSwitched to mode CONNECT_POLYLINE." << vcl_endl;
    this->connect_polyline1_ = 0;
    this->connect_polyline2_ = 0;
    return true;
  }

  // Operating depending on the mode
  if (this->active_mode_ == dbsol_cut_contour_tool::NORMAL)
  {

    if (this->gesture_cut_polyline_(e))
    {
      return this->handle_cut_polyline(ix, iy);
    }

    if (this->gesture_delete_vsol_(e))
    {
      return this->handle_delete_vsol(ix, iy);
    }

    if (this->gesture_open_polygon_(e))
    {
      return this->handle_open_polygon(ix, iy);
    }

    if (this->gesture_close_polyline_(e))
    {
      return this->handle_close_polyline(ix, iy);
    }
  }
  else if (this->active_mode_ == dbsol_cut_contour_tool::CONNECT_POLYLINE)
  {
    if (this->gesture_pick_connect_polyline1_(e))
    {
      return this->handle_pick_connect_polyline1(ix, iy);
    }

    if (this->gesture_pick_connect_polyline2_(e))
    {
      return this->handle_pick_connect_polyline2(ix, iy);
    }
  }


  if (this->gesture_undo_(e))
  {
    return this->handle_undo();
  }


  if( e.type == vgui_MOTION )
  {
    this->ix_ = ix;
    this->iy_ = iy;
    this->tableau()->post_overlay_redraw();
  }

  if( e.type == vgui_OVERLAY_DRAW ) 
  {
    return this->handle_overlay_redraw(this->ix_, this->iy_);
  }

    
  return false;
}



// ----------------------------------------------------------------------------
bool dbsol_cut_contour_tool::
handle_cut_polyline(float ix, float iy)
{
  vcl_cout << "Action: Cut polyline\n";
  vgui_soview* obj = this->tableau()->get_highlighted_soview();

  if (!obj)
    return false;


  if (obj->type_name() == "bgui_vsol_soview2D_polyline")
  {
    bgui_vsol_soview2D_polyline* polyline_soview = 
      static_cast<bgui_vsol_soview2D_polyline* >(obj);
    vsol_polyline_2d_sptr polyline = polyline_soview->sptr();
    
    // find the closest point
    vcl_vector<vsol_point_2d_sptr > pt_set;
    pt_set.reserve(polyline->size());
    for (unsigned int i=0; i<polyline->size(); ++i)
    {
      pt_set.push_back(polyline->vertex(i));
    }
    double min_dist = 1e100;
    vsol_point_2d_sptr closest_pt = 
      bsol_algs::closest_point(new vsol_point_2d(ix, iy), pt_set, min_dist);

    vcl_vector<vsol_polyline_2d_sptr > new_polys = 
      dbsol_algos::cut_polyline(polyline, closest_pt);

    // convert to list of vsol_spatial_objects to add to the storage
    vcl_vector<vsol_spatial_object_2d_sptr > vsol_list;
    for (unsigned int i=0; i<new_polys.size(); ++i)
    {
      vsol_list.push_back(new_polys[i]->cast_to_spatial_object());
    }


    this->storage()->remove_object(polyline->cast_to_spatial_object());
    this->storage()->add_objects(vsol_list);

    // update backup data
    this->last_removed_.clear();
    this->last_removed_.push_back(polyline->cast_to_spatial_object());

    this->last_added_.clear();
    this->last_added_ = vsol_list;

    bvis1_manager::instance()->display_current_frame();
    
  }
  else
  {
    vcl_cout << "ERROR: highlighted object is not a polyline.\n";
  }
  return true;
}



// -----------------------------------------------------------------------------
//:
bool dbsol_cut_contour_tool::
handle_open_polygon(float ix, float iy)
{
  vcl_cout << "Action: Open a polygon\n";
  vgui_soview* obj = this->tableau()->get_highlighted_soview();
  if (!obj) return true;
  if (obj->type_name() == "bgui_vsol_soview2D_polygon")
  {
    bgui_vsol_soview2D_polygon* polygon_soview = 
      static_cast<bgui_vsol_soview2D_polygon* >(obj);
    vsol_polygon_2d_sptr polygon = polygon_soview->sptr();
    
    // find the closest point
    vcl_vector<vsol_point_2d_sptr > pt_set;
    pt_set.reserve(polygon->size());
    for (unsigned int i=0; i<polygon->size(); ++i)
    {
      pt_set.push_back(polygon->vertex(i));
    }
    double min_dist = 1e100;
    vsol_point_2d_sptr closest_pt = 
      bsol_algs::closest_point(new vsol_point_2d(ix, iy), pt_set, min_dist);

    vsol_polyline_2d_sptr new_poly = 
      dbsol_algos::polyline_from_polygon(polygon, closest_pt);

  
    // add to the storage
    
    this->storage()->remove_object(polygon->cast_to_spatial_object());
    this->storage()->add_object(new_poly->cast_to_spatial_object());


    // update backup data
    this->last_removed_.clear();
    this->last_removed_.push_back(polygon->cast_to_spatial_object());

    this->last_added_.clear();
    this->last_added_.push_back(new_poly->cast_to_spatial_object());

    bvis1_manager::instance()->display_current_frame();
  }
  else
  {
    vcl_cout << "ERROR: highlighted object is not a polygon.\n";
  }
  return true;
}




// -----------------------------------------------------------------------------
//: Handle close a polyline and turn into a polygon
bool dbsol_cut_contour_tool::
handle_close_polyline(float ix, float iy)
{
  vcl_cout << "Action: Close a polyline\n";
  vgui_soview* obj = this->tableau()->get_highlighted_soview();
  if (!obj) return true;
  if (obj->type_name() == "bgui_vsol_soview2D_polyline")
  {
    bgui_vsol_soview2D_polyline* polyline_soview = 
      static_cast<bgui_vsol_soview2D_polyline* >(obj);
    vsol_polyline_2d_sptr polyline = polyline_soview->sptr();
    
  
    vsol_polygon_2d_sptr new_poly = 
      dbsol_algos::polygon_from_polyline(polyline);

  
    // add to the storage
    
    this->storage()->remove_object(polyline->cast_to_spatial_object());
    this->storage()->add_object(new_poly->cast_to_spatial_object());


    // update backup data
    this->last_removed_.clear();
    this->last_removed_.push_back(polyline->cast_to_spatial_object());

    this->last_added_.clear();
    this->last_added_.push_back(new_poly->cast_to_spatial_object());

    bvis1_manager::instance()->display_current_frame();
  }
  else
  {
    vcl_cout << "ERROR: highlighted object is not a polyline.\n";
  }
  return false;



}





// ----------------------------------------------------------------------------
bool dbsol_cut_contour_tool::
handle_delete_vsol(float ix, float iy)
{
  vcl_cout << "Action: Delete a polyline or a line segment\n";
  vgui_soview* obj = this->tableau()->get_highlighted_soview();
  if (!obj) return false;

  
  bgui_vsol_soview2D* vsol_soview = static_cast<bgui_vsol_soview2D* >(obj);
  if (vsol_soview)
  {
    // update backup data
    this->last_added_.clear();

    this->last_removed_.clear();
    this->last_removed_.push_back(vsol_soview->base_sptr());
    
    // remove from storage
    this->storage()->remove_object(vsol_soview->base_sptr());

    bvis1_manager::instance()->display_current_frame();
    
  
  }
  else
  {
    vcl_cout << "ERROR: hightlighted object is not a vsol_soview2D object.\n";
  }
    
  return true;
}








// ----------------------------------------------------------------------------
bool dbsol_cut_contour_tool::
handle_undo()
{
  vcl_cout << "Action: Undo vsol deletion\n";
  for (unsigned int i=0; i<this->last_added_.size(); ++i)
  {
    this->storage()->remove_object(this->last_added_[i]);
  }

  for (unsigned int i=0; i<this->last_removed_.size(); ++i)
  {
    this->storage()->add_object(this->last_removed_[i]);
  }

  this->last_added_.clear();
  this->last_removed_.clear();

  vcl_cout << "Undo done.\n";

  bvis1_manager::instance()->display_current_frame();


  return false;
}




// ----------------------------------------------------------------------------
bool dbsol_cut_contour_tool::
handle_pick_connect_polyline1(float ix, float iy)
{
  vcl_cout << "Action: Pick polyline1 to connect\n";
  vgui_soview* obj = this->tableau()->get_highlighted_soview();

  if (!obj)
  {
    vcl_cout << "\n ERROR: no vsol object highlighted." << vcl_endl;
    return false;
  }

  if (obj->type_name() == "bgui_vsol_soview2D_polyline")
  {
    // retrieve the polyline from highlighted object
    bgui_vsol_soview2D_polyline* polyline_soview = 
      static_cast<bgui_vsol_soview2D_polyline* >(obj);
    vsol_polyline_2d_sptr poly = polyline_soview->sptr();
    

    // determine whether to pick the start or end point based on distance
    double d0 = vnl_math::hypot(ix-poly->p0()->x(), iy - poly->p0()->y());
    double d1 = vnl_math::hypot(ix-poly->p1()->x(), iy - poly->p1()->y());

    this->connect_polyline1_ = poly;
    this->connect_polyline1_at_start_ = d0 < d1;
    return true;
  }
  else
  {
    vcl_cout << "ERROR: highlighted object is not a polyline.\n";
  }
  return true;
}




// ----------------------------------------------------------------------------
//:
bool dbsol_cut_contour_tool::
handle_pick_connect_polyline2(float ix, float iy)
{
  vcl_cout << "Action: Pick polyline2 to connect\n";

  if (!this->connect_polyline1_)
  {
    vcl_cout << "\nERROR: need to pick polyline1 to connect first!" << vcl_endl;
    return true;
  }

  vgui_soview* obj = this->tableau()->get_highlighted_soview();

  if (!obj)
  {
    vcl_cout << "\n ERROR: no vsol object highlighted." << vcl_endl;
    return false;
  }

  if (obj->type_name() == "bgui_vsol_soview2D_polyline")
  {
    // retrieve the polyline from highlighted object
    bgui_vsol_soview2D_polyline* polyline_soview = 
      static_cast<bgui_vsol_soview2D_polyline* >(obj);
    vsol_polyline_2d_sptr poly = polyline_soview->sptr();

    if (poly == this->connect_polyline1_)
    {
      vcl_cout << "\nERROR: selected object for polyline2 is the same as polyline1. Choose a different polyline.\n";
      return true;
    }
    

    // determine whether to pick the start or end point based on distance
    double d0 = vnl_math::hypot(ix-poly->p0()->x(), iy - poly->p0()->y());
    double d1 = vnl_math::hypot(ix-poly->p1()->x(), iy - poly->p1()->y());

    this->connect_polyline2_ = poly;
    this->connect_polyline2_at_start_ = d0 < d1;
  }
  else
  {
    vcl_cout << "ERROR: highlighted object is not a polyline.\n";
  }


  // Now connect the two polyline
  // We will create a new polyline combining polyline1 and polyline2
  // Then delete both polyline1 and polyline2

  // collect points from polyline1, change their order if necessary
  vcl_vector<vsol_point_2d_sptr > pt_set1;
  if (this->connect_polyline1_at_start_)
  {
    for (unsigned i = this->connect_polyline1_->size(); i > 0; --i)
    {
      pt_set1.push_back(new vsol_point_2d(*this->connect_polyline1_->vertex(i-1)));
    }
  }
  else
  {
    for (unsigned i =0; i < this->connect_polyline1_->size(); ++i)
    {
      pt_set1.push_back(new vsol_point_2d(*this->connect_polyline1_->vertex(i)));
    }
  }


  // collect points from polyline2, change their order if necessary
  vcl_vector<vsol_point_2d_sptr > pt_set2;
  if (this->connect_polyline2_at_start_)
  {
    for (unsigned i =0; i < this->connect_polyline2_->size(); ++i)
    {
      pt_set2.push_back(new vsol_point_2d(*this->connect_polyline2_->vertex(i)));
    }
  }
  else
  {
    for (unsigned i =this->connect_polyline2_->size(); i > 0; --i)
    {
      pt_set2.push_back(new vsol_point_2d(*this->connect_polyline2_->vertex(i-1)));
    }
  }

  // group the two point sets into one, merge end points if necessary
  vcl_vector<vsol_point_2d_sptr > new_pts;
  new_pts.insert(new_pts.end(), pt_set1.begin(), pt_set1.end());

  vsol_point_2d_sptr poly1_end = pt_set1.back();
  vsol_point_2d_sptr poly2_start = pt_set2.front();

  // if the two end points are far apart, just add the points
  if (poly1_end->distance(*poly2_start) > 1e-4)
  {
    new_pts.insert(new_pts.end(), pt_set2.begin(), pt_set2.end());
  }
  // if they're too close, skip the first point in polyline2
  else
  {
    new_pts.insert(new_pts.end(), ++pt_set2.begin(), pt_set2.end());
  }
  vsol_polyline_2d_sptr new_polyline = new vsol_polyline_2d(new_pts);

  // Insert the newly create polyline to storage
  this->storage()->remove_object(this->connect_polyline1_->cast_to_spatial_object());
  this->storage()->remove_object(this->connect_polyline2_->cast_to_spatial_object());
  this->storage()->add_object(new_polyline->cast_to_spatial_object());
  

  // update backup data
  this->last_removed_.clear();
  this->last_removed_.push_back(this->connect_polyline1_->cast_to_spatial_object());
  this->last_removed_.push_back(this->connect_polyline2_->cast_to_spatial_object());

  this->last_added_.clear();
  this->last_added_.push_back(new_polyline->cast_to_spatial_object());

  // Clean up
  this->connect_polyline1_ = 0;
  this->connect_polyline2_ = 0;


  bvis1_manager::instance()->display_current_frame();

  return true;;
}


// ----------------------------------------------------------------------------
// When cursor is moved, we want to display the start and end points of the
// highlighted contour
bool dbsol_cut_contour_tool::
handle_overlay_redraw(float ix, float iy)
{
  vgui_soview* obj = this->tableau()->get_highlighted_soview();

  if (obj && obj->type_name() == bgui_vsol_soview2D_polyline::type_name_())
  {
    // draw the two end points of the polyline
    bgui_vsol_soview2D_polyline* polyline_soview = 
      static_cast<bgui_vsol_soview2D_polyline* >(obj);
    vsol_polyline_2d_sptr polyline = polyline_soview->sptr();
    vsol_point_2d_sptr start = polyline->vertex(0);
    vsol_point_2d_sptr end = polyline->vertex(polyline->size()-1);

    this->draw_point(float(start->x()), float(start->y()), 8.0f);
    this->draw_point(float(end->x()), float(end->y()), 8.0f);
  }


  // connect-polyline-mode: draw a line from the start point of polyline 1
  if (this->active_mode_ == dbsol_cut_contour_tool::CONNECT_POLYLINE && this->connect_polyline1_)
  {
    vsol_point_2d_sptr start = (this->connect_polyline1_at_start_) ? 
      this->connect_polyline1_->p0() : this->connect_polyline1_->p1();
    glLineWidth(1);
    glColor3f(1,1,1);

    glBegin(GL_LINES);
    glVertex2f(start->x(), start->y());
    glVertex2f(ix, iy);
    glEnd();
    return false;  
  }


  return false;
}


// Draw a circle, estimated as a polygon
void dbsol_cut_contour_tool::
draw_point(float cx, float cy, float radius)
{
  //float const rad = 0.0001f;
  float const rad = radius;

  vgui_style_sptr pt_style = 
    vgui_style::new_style(0.1f, 1.0f, 1.0f, radius, 1.0f);
  pt_style->apply_all();
  glBegin(GL_POINTS);
  glVertex2f(cx, cy);
  glEnd();



  return;
}



//: This is called when the tool is deactivated
void dbsol_cut_contour_tool::
deactivate()
{
}

