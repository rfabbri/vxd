/*************************************************************************
 *    NAME: Mark Johnson
 *    USER: mrj
 *    FILE: bvis1_vsol2D_tools.cxx
 *    DATE: Thu Aug 14 15:33:32 2003
 *************************************************************************/
#include <bvis1/tool/bvis1_vsol2D_tools.h>
#include <bvis1/bvis1_manager.h>
#include <vidpro1/vidpro1_repository.h>
#include <vgui/vgui_style.h>
#include <vgui/vgui_command.h>
#include <vgui/vgui_dialog.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_box_2d.h>
#include <vcl_cstdlib.h>

/*************************************************************************
 * Function Name: bvis1_vsol2D_tool::bvis1_vsol2D_tool
 * Parameters: 
 * Effects: 
 *************************************************************************/

bvis1_vsol2D_tool::bvis1_vsol2D_tool()
{
}

/*************************************************************************
 * Function Name: bvis1_vsol2D_tool::~bvis1_vsol2D_tool
 * Parameters: 
 * Effects: 
 *************************************************************************/

bvis1_vsol2D_tool::~bvis1_vsol2D_tool()
{
}

/*************************************************************************
 * Function Name: bvis1_vsol2D_tool::set_tableau
 * Parameters:  vgui_tableau_sptr tableau 
 * Returns: void
 * Effects: 
 *************************************************************************/
bool
bvis1_vsol2D_tool::set_tableau( const vgui_tableau_sptr& tableau )
{
  bgui_vsol2D_tableau_sptr temp_tab;
  temp_tab.vertical_cast(tableau);
  if (!temp_tab.ptr())
    return false;
    
  if (this->set_storage(bvis1_manager::instance()->storage_from_tableau(tableau))){
    tableau_ = temp_tab;
    return true;
  }
  return false;
}

/*************************************************************************
 * Function Name: bvis1_vsol2D_tool::set_storage
 * Parameters:  bpro1_storage_sptr storage 
 * Returns: void
 * Effects: 
 *************************************************************************/
bool
bvis1_vsol2D_tool::set_storage ( const bpro1_storage_sptr& storage)
{
  if (!storage.ptr())
    return false;
  //make sure its a vsol storage class
  if (storage->type() == "vsol2D"){
    storage_ = storage;
    return true;
  }
  return false;
}

/*************************************************************************
 * Function Name: bvis1_vsol2D_tool::tableau
 * Parameters:
 * Returns: bui_vsol2D_tableau_sptr
 * Effects:
 *************************************************************************/
bgui_vsol2D_tableau_sptr
bvis1_vsol2D_tool::tableau()
{
  return tableau_;
}

/*************************************************************************
 * Function Name: bvis1_vsol2D_tool::storage
 * Parameters:
 * Returns: vidpro1_vsol2D_storage_sptr
 * Effects:
 *************************************************************************/
vidpro1_vsol2D_storage_sptr
bvis1_vsol2D_tool::storage()
{
  vidpro1_vsol2D_storage_sptr vsol_storage;
  vsol_storage.vertical_cast(storage_);
  return vsol_storage;
}


//----------------------------------------------------------------------------
//: A vgui command to set the active group
class bvis1_vsol_group_command : public vgui_command
{
  public:
  bvis1_vsol_group_command(bvis1_vsol2D_tool* tool, 
                          const vcl_string& name) 
   : tool_(tool), name_(name) {}
  void execute()
  {
    if(name_ == ""){
      vgui_dialog name_dlg("Set active group name");
      name_ = tool_->active_group();
      name_dlg.field("Group Name", name_);
      if(!name_dlg.ask())
        return;
    }
    tool_->set_active_group(name_);
  }

  bvis1_vsol2D_tool* tool_;
  vcl_string name_;
};

//: Allow the tool to add to the popup menu as a tableau would
void 
bvis1_vsol2D_tool::get_popup(const vgui_popup_params& /*params*/, vgui_menu &menu)
{
  vidpro1_vsol2D_storage_sptr stg = this->storage();
  if(stg){
    vgui_menu grp_menu;
    vcl_vector<vcl_string> names = stg->groups();
    grp_menu.add("New Group", new bvis1_vsol_group_command(this,""));
    for(unsigned int i=0; i<names.size(); ++i){
      vcl_string label = "[";
      label += (names[i] == active_group_)?"+":" ";
      label += vcl_string("] ") + names[i];
      grp_menu.add(label, new bvis1_vsol_group_command(this,names[i]));
    }
    menu.add("Active Group", grp_menu); 
  }
}

//==========================================================================


/*************************************************************************
 * Function Name: bvis1_vsol2D_point_tool::bvis1_vsol2D_point_tool
 * Parameters:
 * Effects:
 *************************************************************************/

bvis1_vsol2D_point_tool::bvis1_vsol2D_point_tool()
{
  gesture0 = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
}




/*************************************************************************
 * Function Name: bvis1_vsol2D_point_tool::~bvis1_vsol2D_point_tool
 * Parameters:
 * Effects:
 *************************************************************************/

bvis1_vsol2D_point_tool::~bvis1_vsol2D_point_tool()
{
}


/*************************************************************************
 * Function Name: bvis1_vsol2D_point_tool::handle
 * Parameters:  const vgui_event & e
 * Returns: bool
 * Effects:
 *************************************************************************/
bool
bvis1_vsol2D_point_tool::handle( const vgui_event & e, 
                                const bvis1_view_tableau_sptr& /*view*/ )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  if( gesture0(e) ) {

    vsol_point_2d_sptr newpoint =  new vsol_point_2d( ix , iy );

   //add to storage
    storage()->add_object( newpoint->cast_to_spatial_object() , active_group_ );

   //add to the tableau
    tableau()->set_current_grouping( active_group_ );
    tableau()->add_vsol_point_2d( newpoint );
    tableau()->set_current_grouping( "default" );

    tableau()->post_redraw();
    return true;
  }

  return false;

}


/*************************************************************************
 * Function Name: bvis1_vsol2D_point_tool::name
 * Parameters:
 * Returns: vcl_string
 * Effects:
 *************************************************************************/
vcl_string
bvis1_vsol2D_point_tool::name() const
{
  return "Point";
}



/*************************************************************************
 * ***********************************************************************
 * ***********************************************************************
 * ***********************************************************************
 * ***********************************************************************/


/*************************************************************************
 * Function Name: bvis1_vsol2D_line_tool::bvis1_vsol2D_line_tool
 * Parameters:
 * Effects:
 *************************************************************************/

bvis1_vsol2D_line_tool::bvis1_vsol2D_line_tool()
{
  gesture0 = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
  gesture1 = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
  active = false;
}


/*************************************************************************
 * Function Name: bvis1_vsol2D_line_tool::~bvis1_vsol2D_line_tool
 * Parameters:
 * Effects:
 *************************************************************************/

bvis1_vsol2D_line_tool::~bvis1_vsol2D_line_tool()
{
}

/*************************************************************************
 * Function Name: bvis1_vsol2D_line_tool::handle
 * Parameters:  const vgui_event & e
 * Returns: bool
 * Effects:
 *************************************************************************/
bool
bvis1_vsol2D_line_tool::handle( const vgui_event & e, 
                               const bvis1_view_tableau_sptr& /*view*/ )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  if( e.type == vgui_MOTION ) {
    x1 = ix;
    y1 = iy;
    if( active ) {
        tableau()->post_overlay_redraw();
    }
  }

  if( !active && gesture0(e) ) {
    active = true;
    x0 = x1 = ix;
    y0 = y1 = iy;
    active_tableau_ = tableau();
    active_storage_ = storage();
    return true;
  } 
  else if( active ) {

    if( e.type == vgui_OVERLAY_DRAW ) {

      glLineWidth(1);
      glColor3f(1,1,1);

      glBegin(GL_LINES);
      glVertex2f(x0, y0);
      glVertex2f(x1, y1);
      glEnd();
      return true;
    }

    if( gesture1(e) ) {

      vsol_line_2d_sptr newline =  new vsol_line_2d( vsol_point_2d_sptr(
                                               new vsol_point_2d( x0 , y0 ) ),
                                            vsol_point_2d_sptr(
                                               new vsol_point_2d( x1 , y1 ) ) );

      //add to the storage class
      active_storage_->add_object( newline->cast_to_spatial_object() , active_group_ );

      //add it to the tableau as an soview
      active_tableau_->set_current_grouping( active_group_ );
      active_tableau_->add_vsol_line_2d( newline );
      active_tableau_->set_current_grouping( "default" );

      active_tableau_->post_redraw();


      active_tableau_ = NULL;
      active_storage_ = NULL;
      active = false;
      return true;
    }

  }

  return false;

}


/*************************************************************************
 * Function Name: bvis1_vsol2D_line_tool::name
 * Parameters:
 * Returns: vcl_string
 * Effects:
 *************************************************************************/
vcl_string
bvis1_vsol2D_line_tool::name() const
{
  return "Line";
}

/*************************************************************************
 * Function Name: bvis1_vsol2D_line_tool::reset
 * Parameters:
 * Returns: void
 * Effects:
 *************************************************************************/
void
bvis1_vsol2D_line_tool::deactivate()
{
  active = false;
}


/*************************************************************************
 * ***********************************************************************
 * ***********************************************************************
 * ***********************************************************************
 * ***********************************************************************/


/*************************************************************************
 * Function Name: bvis1_vsol2D_polyline_tool::bvis1_vsol2D_polyline_tool
 * Parameters:
 * Effects:
 *************************************************************************/

bvis1_vsol2D_polyline_tool::bvis1_vsol2D_polyline_tool()
{
  active = false;
  gesture0 = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
  gesture1 = vgui_event_condition(vgui_MIDDLE, vgui_MODIFIER_NULL, true);
  gesture2 = vgui_event_condition(vgui_END, vgui_MODIFIER_NULL, true);
  point_list.clear();
}

/*************************************************************************
 * Function Name: bvis1_vsol2D_polyline_tool::~bvis1_vsol2D_polyline_tool
 * Parameters:
 * Effects:
 *************************************************************************/

bvis1_vsol2D_polyline_tool::~bvis1_vsol2D_polyline_tool()
{
  point_list.clear();
}


/*************************************************************************
 * Function Name: bvis1_vsol2D_polyline_tool::handle
 * Parameters:  const vgui_event & e
 * Returns: bool
 * Effects:
 *************************************************************************/
bool
bvis1_vsol2D_polyline_tool::handle( const vgui_event & e, 
                                   const bvis1_view_tableau_sptr& /*view*/ )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  if( e.type == vgui_MOTION ) {
    last_x = ix;
    last_y = iy;
    if( active ) {
        tableau()->post_overlay_redraw();
    }
  }

  if( !active && gesture0(e) ) {
    active = true;
    active_tableau_ = tableau();
    active_storage_ = storage();
    point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( ix , iy ) ) );
    return true;
  } else if( active ) {

    if( gesture0(e) ) {
      point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( ix , iy ) ) );
      return true;
    }

    if( e.type == vgui_OVERLAY_DRAW ) {

      glLineWidth(1);
      glColor3f(1,1,1);

      glBegin(GL_LINE_STRIP);
      for (unsigned i=0; i<point_list.size(); ++i)
        glVertex2f(point_list[i]->x(), point_list[i]->y() );
      glVertex2f(last_x,last_y);
      glEnd();
      return true;
    }

    if( gesture1(e)||gesture2(e) ) {
      if(gesture1(e)) 
        point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( ix , iy ) ) );

      vsol_polyline_2d_sptr newpline =  new vsol_polyline_2d( point_list );

      //add to storage
      active_storage_->add_object( newpline->cast_to_spatial_object() , active_group_ );

      //add to the tableau
      active_tableau_->set_current_grouping( active_group_ );
      active_tableau_->add_vsol_polyline_2d( newpline );
      active_tableau_->set_current_grouping( "default" );

      active_tableau_->post_redraw();
      active = false;

      active_tableau_ = NULL;
      active_storage_ = NULL;
      point_list.clear();
      return true;
    }

  }

  return false;

}


/*************************************************************************
 * Function Name: bvis1_vsol2D_polyline_tool::name
 * Parameters:
 * Returns: vcl_string
 * Effects:
 *************************************************************************/
vcl_string
bvis1_vsol2D_polyline_tool::name() const
{
  return "Polyline";
}


/*************************************************************************
 * Function Name: bvis1_vsol2D_polyline_tool::reset
 * Parameters:
 * Returns: void
 * Effects:
 *************************************************************************/
void
bvis1_vsol2D_polyline_tool::deactivate()
{
  active = false;
  point_list.clear();
}



/*************************************************************************
 * ***********************************************************************
 * ***********************************************************************
 * ***********************************************************************
 * ***********************************************************************/


/*************************************************************************
 * Function Name: bvis1_vsol2D_polygon_tool::bvis1_vsol2D_polygon_tool
 * Parameters:
 * Effects:
 *************************************************************************/

bvis1_vsol2D_polygon_tool::bvis1_vsol2D_polygon_tool()
{
  active = false;
  gesture0 = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
  gesture1 = vgui_event_condition(vgui_MIDDLE, vgui_MODIFIER_NULL, true);
  gesture2 = vgui_event_condition(vgui_END, vgui_MODIFIER_NULL, true);
  point_list.clear();
}


/*************************************************************************
 * Function Name: bvis1_vsol2D_polygon_tool::~bvis1_vsol2D_polygon_tool
 * Parameters:
 * Effects:
 *************************************************************************/

bvis1_vsol2D_polygon_tool::~bvis1_vsol2D_polygon_tool()
{
  point_list.clear();
}


/*************************************************************************
 * Function Name: bvis1_vsol2D_polygon_tool::handle
 * Parameters:  const vgui_event & e
 * Returns: bool
 * Effects:
 *************************************************************************/
bool
bvis1_vsol2D_polygon_tool::handle( const vgui_event & e, 
                                  const bvis1_view_tableau_sptr& /*view*/ )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  if( e.type == vgui_MOTION ) {
    last_x = ix;
    last_y = iy;
    if( active ) {
        tableau()->post_overlay_redraw();
    }
  }

  if( !active && gesture0(e) ) {
    active = true;
    point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( ix , iy ) ) );
    return true;
  } 
  else if( active ) {

    if( gesture0(e) ) {
      point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( ix , iy ) ) );
      return true;
    }

    if( e.type == vgui_OVERLAY_DRAW ) {

      glLineWidth(1);
      glColor3f(1,1,1);

      glBegin(GL_LINE_LOOP);
      for (unsigned i=0; i<point_list.size(); ++i)
        glVertex2f(point_list[i]->x(), point_list[i]->y() );
      glVertex2f(last_x,last_y);
      glEnd();
      return true;
    }

    if( gesture1(e)||gesture2(e) ) {
      if(gesture1(e))
         point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( ix , iy ) ) );

      if (point_list.size() < 3)
         return true;

      vsol_polygon_2d_sptr newpgon =  new vsol_polygon_2d( point_list );

      storage()->add_object( newpgon->cast_to_spatial_object() , active_group_ );

      tableau()->set_current_grouping( active_group_ );
      tableau()->add_vsol_polygon_2d( newpgon );
      tableau()->set_current_grouping( "default" );

      tableau()->post_redraw();
      active = false;
      point_list.clear();
      return true;
    }

  }
    
  return false;

}


/*************************************************************************
 * Function Name: bvis1_vsol2D_polygon_tool::name
 * Parameters: 
 * Returns: vcl_string
 * Effects: 
 *************************************************************************/
vcl_string
bvis1_vsol2D_polygon_tool::name() const
{
  return "Polygon";
}


/*************************************************************************
 * Function Name: bvis1_vsol2D_polygon_tool::reset
 * Parameters: 
 * Returns: void
 * Effects: 
 *************************************************************************/
void
bvis1_vsol2D_polygon_tool::deactivate()
{
  active = false;
  point_list.clear();
}


//=========================================================================
// Draw Digital Curve Tool
//=========================================================================


//: Constructor
bvis1_vsol2D_digital_curve_tool::bvis1_vsol2D_digital_curve_tool()
{
  active = false;
  gesture0 = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
  gesture1 = vgui_event_condition(vgui_MIDDLE, vgui_MODIFIER_NULL, true);
  gesture2 = vgui_event_condition(vgui_END, vgui_MODIFIER_NULL, true);
  point_list.clear();
}


//: Destructor
bvis1_vsol2D_digital_curve_tool::~bvis1_vsol2D_digital_curve_tool()
{
  point_list.clear();
}


//: Handle mouse clicks
bool
bvis1_vsol2D_digital_curve_tool::handle( const vgui_event & e, 
                                        const bvis1_view_tableau_sptr& /*view*/ )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  if( e.type == vgui_MOTION ) {
    last_x = ix;
    last_y = iy;
    if( active ) {
      if( e.button == vgui_LEFT )
        point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( ix , iy ) ) );  
      tableau()->post_overlay_redraw();
    }
  }

  if( !active && gesture0(e) ) {
    active = true;
    active_tableau_ = tableau();
    active_storage_ = storage();
    point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( ix , iy ) ) );
    return true;
  } 
  else if( active ) {

    if( gesture0(e) ) {
      point_list.push_back( vsol_point_2d_sptr( new vsol_point_2d( ix , iy ) ) );
      return true;
    }

    if( e.type == vgui_OVERLAY_DRAW ) {

      glLineWidth(1);
      glColor3f(1,1,1);

      glBegin(GL_LINE_STRIP);
      for (unsigned i=0; i<point_list.size(); ++i)
        glVertex2f(point_list[i]->x(), point_list[i]->y() );
      glVertex2f(last_x,last_y);
      glEnd();
      return true;
    }

    if( gesture1(e)||gesture2(e)) {

      vsol_digital_curve_2d_sptr newcurve =  new vsol_digital_curve_2d( point_list );

      //add to storage
      active_storage_->add_object( newcurve.ptr() , active_group_ );

      //add to the tableau
      active_tableau_->set_current_grouping( active_group_ );
      active_tableau_->add_digital_curve( newcurve );
      active_tableau_->set_current_grouping( "default" );

      active_tableau_->post_redraw();
      active = false;

      active_tableau_ = NULL;
      active_storage_ = NULL;
      point_list.clear();
      return true;
    }

  }

  return false;

}


//: Return the name
vcl_string
bvis1_vsol2D_digital_curve_tool::name() const
{
  return "Digital Curve";
}


//: Deactivate the tool
void
bvis1_vsol2D_digital_curve_tool::deactivate()
{
  active = false;
  point_list.clear();
}



//=========================================================================
// Digital Curve Splitting Tool
//=========================================================================


//: Constructor
bvis1_vsol2D_split_curve_tool::bvis1_vsol2D_split_curve_tool()
: object_(NULL), index_(0.0)
{
  gesture_click = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
}


//: Destructor
bvis1_vsol2D_split_curve_tool::~bvis1_vsol2D_split_curve_tool()
{
}


//: Handle mouse clicks
bool
bvis1_vsol2D_split_curve_tool::handle( const vgui_event & e, 
                                      const bvis1_view_tableau_sptr& /*view*/ )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  if( e.type == vgui_MOTION){
    tableau()->motion(e.wx, e.wy);
    
    vgui_soview2D* curr_obj =  (vgui_soview2D*)tableau()->get_highlighted_soview();
    if( curr_obj != object_ )
      object_ = dynamic_cast<bgui_vsol_soview2D_digital_curve*>(curr_obj);
    if(object_ != NULL){
      index_ = closest_index(vgl_point_2d<double>(ix,iy), object_->sptr());
      tableau()->post_overlay_redraw();
    }
    return true;
  }


  if( object_ && gesture_click(e) ) {
    vsol_digital_curve_2d_sptr out1, out2;
    if(split(object_->sptr(),index_,out1, out2)){
      vcl_string group = storage()->remove_object(object_->sptr().ptr());
      tableau()->remove(object_);

      storage()->add_object(out1.ptr(), group);
      storage()->add_object(out2.ptr(), group);
      tableau()->add_digital_curve(out1);
      tableau()->add_digital_curve(out2);
    }
    return true;
  }

  if( object_ && e.type == vgui_OVERLAY_DRAW ) {
    vgui_style::new_style(0.5f, 0.5f ,1.0f, 1.0f, 3.0f)->apply_all();
    object_->draw();
    vgl_point_2d<double> split_pt = object_->sptr()->interp(index_);
    vgui_style::new_style(1.0f, 0.0f ,0.0f, 5.0f, 1.0f)->apply_all();
    vgui_soview2D_point(split_pt.x(), split_pt.y()).draw();
    return true;
  }

  return false;

}


//: Return the name
vcl_string
bvis1_vsol2D_split_curve_tool::name() const
{
  return "Split Curves";
}


//: Deactivate the tool
void
bvis1_vsol2D_split_curve_tool::deactivate()
{
}


///////Inspection tool to display info about vsol2d objects

//Constructor
bvis1_vsol2D_inspect_tool::bvis1_vsol2D_inspect_tool( const vgui_event_condition& lift)
: gesture_lift_(lift),  active_(false), object_(NULL),
tableau_(NULL)
{
}


//: Destructor
bvis1_vsol2D_inspect_tool::~bvis1_vsol2D_inspect_tool()
{
}


//: Return the name of this tool
vcl_string
bvis1_vsol2D_inspect_tool::name() const
{
    return "Inspect";
}


//: Set the tableau to work with
bool
bvis1_vsol2D_inspect_tool::set_tableau( const vgui_tableau_sptr& tableau )
{ 
    if( tableau.ptr() != NULL && tableau->type_name() == "bgui_vsol2D_tableau" ){
        tableau_.vertical_cast(tableau);
        return true;
    }

    tableau_ = vgui_easy2D_tableau_sptr(NULL);
    return false;
}


//: Handle events
bool
bvis1_vsol2D_inspect_tool::handle( const vgui_event & e, const bvis1_view_tableau_sptr& /*selector*/ )
{ 



    if( tableau_.ptr() && gesture_lift_(e) ){
        //  vcl_cout<<"Click object";
        object_ = (vgui_soview2D*)tableau_->get_highlighted_soview();
        if( object_ != NULL ) {
            active_ = true;
            bgui_vsol_soview2D* object_ptr=(bgui_vsol_soview2D *)object_;
            if ((object_ptr->type_name()=="bgui_vsol_soview2D_point")                   
                ||
                (object_ptr->type_name()=="bgui_vsol_soview2D_line_seg")
                ||
                (object_ptr->type_name()=="bgui_vsol_soview2D_polyline")
                ||
                (object_ptr->type_name()=="bgui_vsol_soview2D_polygon")
                )

                object_ptr->print(vcl_cout);



            //vcl_cout<<object_ptr->type_name()<<"\n";

            return true;
        }
    }


    return false;
}

void
bvis1_vsol2D_inspect_tool::deactivate()
{
}

///////selection tool to create a new vsol2D storage and tableau

//Constructor
bvis1_vsol2D_selection_tool::bvis1_vsol2D_selection_tool( const vgui_event_condition& add, const vgui_event_condition& set)
: gesture_add_(add), gesture_set_(set), active_(false), object_(NULL),
tableau_(NULL)
{
}


//: Destructor
bvis1_vsol2D_selection_tool::~bvis1_vsol2D_selection_tool()
{
}


//: Return the name of this tool
vcl_string
bvis1_vsol2D_selection_tool::name() const
{
    return "Select Subset";
}


//: Set the tableau to work with
bool
bvis1_vsol2D_selection_tool::set_tableau( const vgui_tableau_sptr& tableau )
{ 
    if( tableau.ptr() != NULL && tableau->type_name() == "bgui_vsol2D_tableau" ){
        tableau_.vertical_cast(tableau);
        return true;
    }

    tableau_ = vgui_easy2D_tableau_sptr(NULL);
    return false;
}


//: Handle events
bool
bvis1_vsol2D_selection_tool::handle( const vgui_event & e, const bvis1_view_tableau_sptr& /*selector*/ )
{ 
    if( tableau_.ptr() && gesture_add_(e) ) {
        //  vcl_cout<<"Click object";
        object_ = (vgui_soview2D*)tableau_->get_highlighted_soview();
        if( object_ != NULL ) {
            active_ = true;
            bgui_vsol_soview2D* object_ptr=(bgui_vsol_soview2D *)object_;
            objs_.insert(object_ptr->base_sptr());
            vcl_cout<< "added a " << object_ptr->type_name() << "\n";
            return true;
        }
    }

    if (tableau_.ptr() && gesture_set_(e) ) {
      vidpro1_repository_sptr res = bvis1_manager::instance()->repository();
      if(!res) {
        vcl_cout << "Could not access repository!\n";
        return false;
      }

      vidpro1_vsol2D_storage_sptr output_vsol = vidpro1_vsol2D_storage_new();
      vcl_set<vsol_spatial_object_2d_sptr>::iterator it;
      for ( it = objs_.begin(); it != objs_.end(); it++)
        output_vsol->add_object(*it);
      vcl_set<bpro1_storage_sptr> st_set = res->get_all_storage_classes(res->current_frame());
      vcl_string name_initial = "subvsol2D";
      int len = name_initial.length();
      int max = 0;
      for (vcl_set<bpro1_storage_sptr>::iterator iter = st_set.begin();
        iter != st_set.end(); iter++) {
          if ((*iter)->type() == output_vsol->type() && 
              (*iter)->name().find(name_initial) != vcl_string::npos) {
            vcl_string name = (*iter)->name();
            vcl_string numbr = name.substr(len, 3);
            int n = vcl_atoi(numbr.c_str());
            if (n > max)
              max = n;
          }
      }
      char buffer[1000];
      sprintf(buffer, "%s%03d", name_initial.c_str(), max+1);
      name_initial = buffer;
      output_vsol->set_name(name_initial);
      res->store_data(output_vsol);
      bvis1_manager::instance()->add_to_display(output_vsol);
      bvis1_manager::instance()->display_current_frame();
    }

    return false;
}

void
bvis1_vsol2D_selection_tool::deactivate()
{
}


//=========================================================================
// VSOL 2D vertex adjustment tool
//=========================================================================

//Constructor
bvis1_vsol2D_adjust_tool::bvis1_vsol2D_adjust_tool( const vgui_event_condition& lift,
                                                  const vgui_event_condition& drop)
: gesture_lift_(lift), gesture_drop_(drop), 
  active_(false), cycle_(false), object_(NULL)
{
}


//: Destructor
bvis1_vsol2D_adjust_tool::~bvis1_vsol2D_adjust_tool()
{
}


//: Return the name of this tool
vcl_string
bvis1_vsol2D_adjust_tool::name() const
{
    return "Adjust";
}


//: Return the name of this tool
void
bvis1_vsol2D_adjust_tool::set_points(const vsol_spatial_object_2d_sptr& obj)
{
  points_.clear();
  cycle_ = false;
  if(vsol_point_2d_sptr pt = obj->cast_to_point())
  {
    points_.push_back(pt);
  }
  else if(vsol_curve_2d* curve = obj->cast_to_curve())
  {
    if(vsol_line_2d_sptr line = curve->cast_to_line())
    {
      points_.push_back(line->p0());
      points_.push_back(line->p1());
    }
    else if(vsol_polyline_2d_sptr pline = curve->cast_to_polyline())
    {
      for(unsigned int i=0; i<pline->size(); ++i)
        points_.push_back(pline->vertex(i));
    }
    else if(vsol_digital_curve_2d_sptr dc = curve->cast_to_digital_curve())
    {
      for(unsigned int i=0; i<dc->size(); ++i)
        points_.push_back(dc->point(i));
    }
  }
  else if(vsol_region_2d* region = obj->cast_to_region())
  {
    cycle_ = true;
    if(vsol_polygon_2d_sptr pg = region->cast_to_polygon())
    {
      for(unsigned int i=0; i<pg->size(); ++i)
        points_.push_back(pg->vertex(i));
    }
  }
}


//: Handle events
bool
bvis1_vsol2D_adjust_tool::handle( const vgui_event & e, const bvis1_view_tableau_sptr& /*selector*/ )
{ 
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);
  
  
  if(active_){
    if( gesture_drop_(e) ){
      active_ = false;
      tableau()->post_redraw();
    }
    if( e.type == vgui_MOTION){ 
      vgl_vector_2d<double> motion(ix-last_x_, iy-last_y_);
      last_x_ = ix;
      last_y_ = iy;
      active_pt_->add_vector(motion);
      tableau()->post_overlay_redraw();
    }
  }
  else{
    if( active_pt_ && gesture_lift_(e) ) {
      active_ = true;
      last_x_ = ix;
      last_y_ = iy;
      return true;
    }
    if( e.type == vgui_MOTION){
      tableau()->motion(e.wx, e.wy);
      
      vgui_soview2D* curr_obj =  (vgui_soview2D*)tableau()->get_highlighted_soview();
      if( curr_obj != object_ ){
        object_ = dynamic_cast<bgui_vsol_soview2D*>(curr_obj);
        if(object_ != NULL){
          set_points(object_->base_sptr());
        }
      }
    
      // activate the closest point to the mouse pointer
      // if there is a point closer than 10 pixels
      active_pt_ = NULL;
      double min_dist2 = 100.0;
      for(unsigned int i=0; i<points_.size(); ++i){
        double dist2 = sqr_length(points_[i]->get_p() - vgl_point_2d<double>(ix,iy));
        if(dist2 < min_dist2){
          active_pt_ = points_[i];
          min_dist2 = dist2;
        }
      }
      tableau()->post_overlay_redraw();
      return true;
    }
  }

  if( object_ && e.type == vgui_OVERLAY_DRAW ) {
    vgui_style_sptr pt_style = vgui_style::new_style(1.0f, 0.0f ,0.0f, 4.0f, 3.0f);
    pt_style->apply_all();
    for(unsigned int i=0; i<points_.size(); ++i){
      bgui_vsol_soview2D_point view(points_[i]);
      if(points_[i] == active_pt_){
        vgui_style::new_style(0.0f, 1.0f ,0.0f, 6.0f, 2.0f)->apply_all();
        glBegin(GL_LINE_STRIP);
        if(i>0)
          glVertex2f(points_[i-1]->x(), points_[i-1]->y());
        else if(cycle_)
          glVertex2f(points_.back()->x(), points_.back()->y());
        glVertex2f(points_[i]->x(), points_[i]->y());
        if(i+1<points_.size())
          glVertex2f(points_[i+1]->x(), points_[i+1]->y());
        else if(cycle_)
          glVertex2f(points_.front()->x(), points_.front()->y());
        glEnd();  
        view.draw();
        pt_style->apply_all();
      }
      else
        view.draw();
    }
    return true;
  }

  return false;

}

void
bvis1_vsol2D_adjust_tool::deactivate()
{
}



bvis1_vsol2D_crop_tool::bvis1_vsol2D_crop_tool()
{
draw_polygon_=false;
  gesture0 = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
  gesture1 = vgui_event_condition(vgui_MIDDLE, vgui_MODIFIER_NULL, true);
  gesture2 = vgui_event_condition(vgui_END, vgui_MODIFIER_NULL, true);
  point_list.clear();
}


bvis1_vsol2D_crop_tool::~bvis1_vsol2D_crop_tool()
{
point_list.clear();
}

void
bvis1_vsol2D_crop_tool::deactivate()
{
}

bool
bvis1_vsol2D_crop_tool::handle( const vgui_event & e, 
                               const bvis1_view_tableau_sptr& /*view*/ )
{
  float ix, iy;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);


  vcl_vector<vsol_spatial_object_2d_sptr> croppedobjs;
  if( e.type == vgui_KEY_PRESS && e.key == 'p' && vgui_SHIFT  ) {
        
      vgui_soview2D *object_type = (vgui_soview2D*)tableau_->get_highlighted_soview();
      if( object_ != NULL ) {
          active_ = true;
          object_=(bgui_vsol_soview2D*)object_type;
          if (object_->type_name()=="bgui_vsol_soview2D_polygon")
              {
              vsol_polygon_2d_sptr poly=
                  object_->base_sptr()->cast_to_region()->cast_to_polygon();

              vsol_box_2d_sptr box=poly->get_bounding_box();
              vcl_cout<<"\n box dimensions are "<<box->get_min_x()<<","<<box->get_min_y()<<"---->"<<box->get_max_x()<<","<<box->get_max_y()<<"\n";
              double minx=box->get_min_x();
              double miny=box->get_min_y();
              double maxx=box->get_max_x();
              double maxy=box->get_max_y();
              vcl_vector<vgui_soview*> all=tableau_->get_all();
              
              for(unsigned i=0;i<all.size();i++)
                  {
                    vgui_soview2D *temp = (vgui_soview2D*)all[i];
                    if(temp!=NULL)
                        {
                           bgui_vsol_soview2D * temp_object=(bgui_vsol_soview2D*)temp; 
                           if(temp_object->type_name()=="bgui_vsol_soview2D_point")
                               {
                                    vsol_point_2d_sptr point=temp_object->base_sptr()->cast_to_point();
                                    if(point->x()<maxx && point->x()>minx && point->y()<maxy && point->y()>miny )
                                        {
                                            croppedobjs.push_back(temp_object->base_sptr());
                                        }
                               }
                           else if(temp_object->type_name()=="bgui_vsol_soview2D_polyline")
                               {
                                    vsol_polyline_2d_sptr polyline=temp_object->base_sptr()->cast_to_curve()->cast_to_polyline();
                                    vsol_box_2d_sptr tempbox=polyline->get_bounding_box();
                                    if(tempbox->get_min_x()<=maxx &&tempbox->get_max_x()>=minx &&tempbox->get_min_y()<=maxy &&tempbox->get_max_y()>=miny )
                                        {
                                            croppedobjs.push_back(temp_object->base_sptr());
                                        }
                               }
                            else if(temp_object->type_name()=="bgui_vsol_soview2D_polygon")
                               {
                               
                                vsol_polygon_2d_sptr polygon=object_->base_sptr()->cast_to_region()->cast_to_polygon();
                                if(poly.ptr()!=polygon.ptr())                               
                                    {
                                vsol_box_2d_sptr tempbox=polygon->get_bounding_box();
                                    if(tempbox->get_min_x()<=maxx &&tempbox->get_max_x()>=minx &&tempbox->get_min_y()<=maxy &&tempbox->get_max_y()>=miny )
                                        {
                                            croppedobjs.push_back(temp_object->base_sptr());
                                        }
                                    }
                               }
                        }
                    
                  }
              }
          tableau_->deselect_all();
          tableau_->clear();
          tableau_->add_spatial_objects(croppedobjs);
          return true;
        }
    
      }
  
  if( e.type == vgui_MOTION ) {
      tableau()->motion(e.wx, e.wy);
      
      vgui_soview2D* curr_obj =  (vgui_soview2D*)tableau()->get_highlighted_soview();
      if( curr_obj != object_ ){
        object_ = dynamic_cast<bgui_vsol_soview2D*>(curr_obj);
        if(object_ != NULL){
          set_points(object_->base_sptr());
        }
      }
    
      // activate the closest point to the mouse pointer
      // if there is a point closer than 10 pixels
      active_pt_ = NULL;
      double min_dist2 = 100.0;
      for(unsigned int i=0; i<points_.size(); ++i){
        double dist2 = sqr_length(points_[i]->get_p() - vgl_point_2d<double>(ix,iy));
        if(dist2 < min_dist2){
          active_pt_ = points_[i];
          min_dist2 = dist2;
        }
      }
  }
  
  
    if( e.type == vgui_OVERLAY_DRAW ) {
        
      return true;
            
    }
    
  return false;
}
void
bvis1_vsol2D_crop_tool::set_points(const vsol_spatial_object_2d_sptr& obj)
{
  points_.clear();
  if(vsol_point_2d_sptr pt = obj->cast_to_point())
  {
    points_.push_back(pt);
  }
  else if(vsol_curve_2d* curve = obj->cast_to_curve())
  {
    if(vsol_line_2d_sptr line = curve->cast_to_line())
    {
      points_.push_back(line->p0());
      points_.push_back(line->p1());
    }
    else if(vsol_polyline_2d_sptr pline = curve->cast_to_polyline())
    {
      for(unsigned int i=0; i<pline->size(); ++i)
        points_.push_back(pline->vertex(i));
    }
    else if(vsol_digital_curve_2d_sptr dc = curve->cast_to_digital_curve())
    {
      for(unsigned int i=0; i<dc->size(); ++i)
        points_.push_back(dc->point(i));
    }
  }
  else if(vsol_region_2d* region = obj->cast_to_region())
  {
    if(vsol_polygon_2d_sptr pg = region->cast_to_polygon())
    {
      for(unsigned int i=0; i<pg->size(); ++i)
        points_.push_back(pg->vertex(i));
    }
  }
}
void 
bvis1_vsol2D_crop_tool::get_popup( const vgui_popup_params& /*params*/, 
                                            vgui_menu &menu )
{
  vcl_string on = "[x] ", off = "[ ] ";
  menu.add( ((draw_polygon_)?on:off)+"Draw Polygon ", 
            bvis1_tool_toggle, (void*)(&draw_polygon_) );
  
}

vcl_string
bvis1_vsol2D_crop_tool::name() const
{
  return "Crop Vsol2D";
}
