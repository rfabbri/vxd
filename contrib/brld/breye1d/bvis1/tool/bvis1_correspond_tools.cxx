// This is breye1/bvis1/tool/bvis1_correspond_tools.cxx
//:
// \file

#include "bvis1_correspond_tools.h"
#include <bvis1/bvis1_manager.h>
#include <bvis1/bvis1_view_tableau.h>
#include <vgui/vgui_style.h>
#include <vgui/vgui_dialog.h>
#include <vgui/vgui_menu.h>
#include <vgui/vgui_command.h>
#include <vgui/vgui_projection_inspector.h>
#include <vsol/vsol_point_2d.h>

#include <vgl/vgl_line_2d.h>
#include <mvl/FMatrixComputeRANSAC.h> 
#include <vnl/algo/vnl_svd.h>

//: The bitmap for a 16x16 crosshair 
static const GLubyte cross_bmp[] = 
{ 0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,    
  0x02,0x40,0x00,0x00,0xf8,0x1f,0x00,0x00,  
  0x00,0x00,0xf8,0x1f,0x00,0x00,0x02,0x40, 
  0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40 };

//: Render the crosshair on the image at (x,y)
static void render_cross(float x, float y)
{
  GLint unpack;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  glRasterPos2d(x, y);
  glBitmap(16, 16, 8, 8, 0, 0, cross_bmp);

  glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
}



//Constructor
bvis1_correspond_point_tool::bvis1_correspond_point_tool( const vgui_event_condition& select )
 : gesture_select_(select), point_style_(vgui_style::new_style(1.0f, 0.0f, 0.0f, 2.0f, 1.0f)),
   modify_(false), object_(NULL), tableau_(NULL), correspondences_(),
   curr_corr_(correspondences_.end())
{
}


//: Destructor
bvis1_correspond_point_tool::~bvis1_correspond_point_tool()
{
}


//: Return the name of this tool
vcl_string
bvis1_correspond_point_tool::name() const
{
  return "Correspond Points";
}


//: Set the tableau to work with
bool
bvis1_correspond_point_tool::set_tableau( const vgui_tableau_sptr& tableau )
{
  tableau_ = vgui_displaylist2D_tableau_sptr(dynamic_cast<vgui_displaylist2D_tableau*>(tableau.ptr()));
  if( tableau.ptr() == NULL ) 
    return false;
  
  return true;
}

//: Handle events
bool
bvis1_correspond_point_tool::handle( const vgui_event & e, 
                                    const bvis1_view_tableau_sptr& view )
{ 
  float ix, iy;
  static float last_x=0.0f, last_y = 0.0f;
  vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

  bool is_mouse_over = ( bvis1_manager::instance()->active_tableau()
                         == view->selector()->active_tableau() );

  if( e.type == vgui_DRAW_OVERLAY ) {
    float x=last_x, y=last_y;
    if(object_ != NULL){
      object_->get_centroid(&x,&y);
    }
    if(is_mouse_over){
      glColor3f(1.0, 1.0, 1.0);
      render_cross(x,y);
    }
    if( curr_corr_ != correspondences_.end() ){
      if(modify_)
        glColor3f(1.0, 0.0, 0.0);
      else
        glColor3f(0.0, 1.0, 0.0);
      for ( corr_map::const_iterator itr = curr_corr_->begin();
            itr != curr_corr_->end();  ++itr )
      {
        if( this->is_visible_child(view->selector(),itr->first) ){
          itr->second->get_centroid(&x,&y);
          render_cross(x,y);
        }
      }
    }

    return true;
  }

  // cache the current position as the last known position
  if( e.type == vgui_MOTION ) {
    tableau_->motion(e.wx, e.wy);
    last_x = ix;
    last_y = iy;
    
    vgui_soview2D* curr_obj =  (vgui_soview2D*)tableau_->get_highlighted_soview();
    if( curr_obj != object_ ){
      object_ = dynamic_cast<bgui_vsol_soview2D_point*>(curr_obj); 
      if ( !modify_ ){
        if ( !object_ || !this->set_curr_corr(object_) )
          curr_corr_ = correspondences_.end();
      }
    }
    tableau_->post_overlay_redraw();
    return true;
  }

  if( gesture_select_(e) ) {
    if( modify_ ){
      if( object_ == NULL ){
        vsol_point_2d_sptr newpoint =  new vsol_point_2d( ix , iy );
        object_ = new bgui_vsol_soview2D_point(newpoint);
        object_->set_style(point_style_);

        //add to the tableau
        tableau_->set_current_grouping( "correspondence" );
        tableau_->add( object_ );
        tableau_->set_current_grouping( "default" );

        tableau_->post_redraw();
      }
    
      if( !this->set_curr_corr(object_) ){
        if ( curr_corr_ == correspondences_.end() )
          curr_corr_ = correspondences_.insert(corr_map()).first;    
        const_cast<corr_map&>(*curr_corr_)[tableau_] = object_;
        tab_set_.insert(tableau_);
      }
      tableau_->post_overlay_redraw();
    }
    else{
      if( object_ != NULL )
        if( this->set_curr_corr(object_) )
          tableau_->post_overlay_redraw();
    }
    return true;
  }
  
  // toggle on/off modify mode
  if( e.type == vgui_KEY_PRESS && e.key == 'm' ) {
    modify_ = !modify_;
    tableau_->post_overlay_redraw();
    curr_corr_ = correspondences_.end();
    return true;
  }

  return false;
}


//: Return the corresponding points as a vector of vectors
//  - corresponding points are in the same order in each vector
//  - one vector is returned for each tableau in \p tabs in the same order
//  - NULL points are returned for missing correspondences
vcl_vector<vcl_vector<vsol_point_2d_sptr> >
bvis1_correspond_point_tool::correspondences(
                          const vcl_vector<vgui_displaylist2D_tableau_sptr>& tabs) const
{
  vcl_vector<vcl_vector<vsol_point_2d_sptr> > result(tabs.size());

  for ( vcl_set<corr_map>::const_iterator citr = correspondences_.begin();
        citr != correspondences_.end();  ++citr )
  {
    for ( unsigned int i=0; i<tabs.size(); ++i )
    {
      corr_map::const_iterator fitr = citr->find(tabs[i]);
      if ( fitr == citr->end() )
        result[i].push_back(NULL);
      else
        result[i].push_back(fitr->second->sptr());
    }
  }
  return result;
}


//: Search the correspondences for this point and make the set active 
bool
bvis1_correspond_point_tool::set_curr_corr(const bgui_vsol_soview2D_point* pt)
{
  for ( vcl_set<corr_map>::iterator itr = correspondences_.begin();
        itr != correspondences_.end();  ++itr )
  {
    corr_map::const_iterator itr2 = itr->find(this->tableau_);
    if ( itr2 != itr->end() ){
      if( itr2->second == pt ){
        curr_corr_ = itr;
        return true;
      }
    }
  }
  return false;
}


//: Returns true if \p tableau is a visible child of \p selector
bool 
bvis1_correspond_point_tool::is_visible_child( const vgui_selector_tableau_sptr& selector,
                                              const vgui_tableau_sptr& tableau ) const
{
  vcl_vector<vcl_string> names(selector->child_names());
  for ( vcl_vector<vcl_string>::const_iterator itr = names.begin();
        itr != names.end();  ++itr )
  {
    if ( tableau == selector->get_tableau(*itr) ){
      if( selector->is_visible(*itr) )
        return true;
      return false;
    }
  }
  return false;
}


//----------------------------------------------------------------------------
//: A vgui command used to compute the fundamental matrix
class bvis1_fmatrix_command : public vgui_command
{
  public:
  bvis1_fmatrix_command(bvis1_correspond_point_tool* tool) : tool_(tool) {}
  void execute()
  {
    vcl_set<vgui_displaylist2D_tableau_sptr> tab_set(tool_->tableaux());
    vcl_vector<vgui_displaylist2D_tableau_sptr> tabs;
    vcl_vector<vcl_string> choices;
    for ( vcl_set<vgui_displaylist2D_tableau_sptr>::iterator itr = tab_set.begin();
          itr != tab_set.end();  ++itr )
    {
      tabs.push_back(*itr);
      bpro1_storage_sptr storage = bvis1_manager::instance()->storage_from_tableau(*itr);
      if( storage != NULL )
        choices.push_back(storage->name());
      else
        choices.push_back("");
    }
    unsigned int val1=0, val2=0;
    if(choices.size() > 1)
      val2 = 1;
    vgui_dialog tab_dlg("Select Tableau");
    tab_dlg.choice("View 1", choices, val1);
    tab_dlg.choice("View 2", choices, val2);

    static bool is_skew = false;
    tab_dlg.checkbox("Translation only", is_skew);
    if (!tab_dlg.ask())
      return; // cancelled

    vcl_vector<vgui_displaylist2D_tableau_sptr> selected(2);
    selected[0] = tabs[val1];
    selected[1] = tabs[val2];

    vcl_vector<vcl_vector<vsol_point_2d_sptr> > corr_pts =
      tool_->correspondences(selected);

    FMatrix F;

    if(is_skew){
      vcl_vector<vgl_line_2d<double> > lines;
      for ( unsigned int i=0; i<corr_pts[0].size(); ++i ){
        if(corr_pts[0][i] && corr_pts[1][i]){
          lines.push_back(vgl_line_2d<double>(corr_pts[0][i]->get_p(),
                                              corr_pts[1][i]->get_p()));
          lines.back().normalize();
        }
      }
      int num = lines.size();
      if(num < 2)
        return;
      vnl_matrix<double> A(num,2);
      vnl_vector<double> c(num);
      for ( int i=0; i<num; ++i ){
        A(i,0) = lines[i].a();
        A(i,1) = lines[i].b();
        c(i)   = lines[i].c();
      }
      vnl_vector<double> pt = vnl_svd<double>(A).solve(c);
      vcl_cout << "point = " << pt << vcl_endl;
      vnl_matrix<double> S(3,3);
      S(0,0) = S(1,1) = S(2,2) = 0.0;
      S(1,2) = pt[0];  S(2,1) = -pt[0];
      S(0,2) = -pt[1]; S(2,0) = pt[1];
      S(0,1) = 1.0;    S(1,0) = -1.0;
      F.set(S);

    }
    else{
      vcl_vector< vgl_homg_point_2d< double > > s1;
      vcl_vector< vgl_homg_point_2d< double > > s2;
      for ( unsigned int i=0; i<corr_pts[0].size(); ++i ){
        if(corr_pts[0][i] && corr_pts[1][i]){
          s1.push_back(vgl_homg_point_2d<double>(corr_pts[0][i]->get_p()));
          s2.push_back(vgl_homg_point_2d<double>(corr_pts[1][i]->get_p()));
        }
      }
      FMatrixComputeRANSAC fmc(true, 0.25);
      F = fmc.compute(s1,s2);
    }

    vcl_cout << "F = \n"<< F << vcl_endl;
    vgl_homg_point_2d< double > e1, e2;
    F.get_epipoles(e1, e2);
    vcl_cout << "epipole for " << choices[val1] << " " << e1 << vcl_endl;
    vcl_cout << "epipole for " << choices[val2] << " " << e2 << vcl_endl;

  }

  bvis1_correspond_point_tool* tool_;
};


//: Add popup menu items
void 
bvis1_correspond_point_tool::get_popup(const vgui_popup_params& params, 
                                            vgui_menu &menu)
{
  menu.add("Fundamental Matrix", new bvis1_fmatrix_command(this));
}
