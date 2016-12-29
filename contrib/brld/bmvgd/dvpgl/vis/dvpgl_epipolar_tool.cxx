#include "dvpgl_epipolar_tool.h"
#include <vgui/vgui_projection_inspector.h>
#include <vgui/vgui.h>
#include <vgui/vgui_dialog.h>
#include <bpro1/bpro1_storage_sptr.h>
#include <bvis1/bvis1_manager.h>
#include <bvis1/bvis1_view_tableau.h>
#include <vidpro1/vidpro1_repository.h>

#include <dvpgl/pro/dvpgl_camera_storage.h>


#define MANAGER bvis1_manager::instance()


dvpgl_epipolar_tool::
dvpgl_epipolar_tool()
  : pt_(0),activated_(false)
{
  gesture0 = vgui_event_condition(vgui_LEFT, vgui_MODIFIER_NULL, true);
}

vcl_string
dvpgl_epipolar_tool::name() const
{
  return "Epipolar";
}

void   
dvpgl_epipolar_tool::activate ()
{
  vcl_cout << "dvpgl_epipolar_tool ON\n";


  vcl_vector< bvis1_view_tableau_sptr > views;
  views = MANAGER->get_views();

  if (views.size() < 2) {
    vgui::out << "Error: need at least 2 views for this tool" << vcl_endl;
    vcl_cerr  << "Error: need at least 2 views for this tool" << vcl_endl;
    return;
  }
  
  int v1, v2; //:< view numbers to work on
  if(!get_epipolar_params(&v1, &v2)) {
    vcl_cerr << "Error: in getting params" << vcl_endl;
    return;
  }
  if ( v1+1 > static_cast<int>(views.size()) || v2 +1 > static_cast<int>(views.size()) ) {
    vcl_cerr << "Error: invalid view numbers" << vcl_endl;
    return;
  }

  int frame_v1 = views[v1]->frame();
  int frame_v2 = views[v2]->frame();

  vcl_cout << "Working in frames " << frame_v1 << " and " << frame_v2 << vcl_endl;


  // -------- Get camera matrices from each frame

  {

  // camera 1
  bpro1_storage_sptr 
    p = MANAGER->repository()->get_data_at("vpgl camera", frame_v1);

  dvpgl_camera_storage_sptr cam_storage;

  cam_storage.vertical_cast(p);

  
  if (cam_storage->get_camera()->type_name() != "vpgl_perspective_camera") {
    vcl_cerr << "Error: tool requires a perspective camera" << vcl_endl;
    return;
  }

  p1_ = static_cast<const vpgl_perspective_camera<double> *> (cam_storage->get_camera());

  vcl_cout << "NAME: " << cam_storage->name() << vcl_endl;
  vcl_cout << "Camera1: \n" << p1_->get_matrix();
//vcl_cout << "Camera1: \n" << *p1_;

  // camera 2
  p = MANAGER->repository()->get_data_at("vpgl camera",frame_v2);

  cam_storage.vertical_cast(p);

  if (cam_storage->get_camera()->type_name() != "vpgl_perspective_camera") {
    vcl_cerr << "Error: tool requires a perspective camera" << vcl_endl;
    return;
  }

  p2_ = static_cast<const vpgl_perspective_camera<double> *> (cam_storage->get_camera());

  vcl_cout << "\nNAME2: " << cam_storage->name() << vcl_endl;
  vcl_cout << "Camera2: \n" << p2_->get_matrix();

  // Fundamental matrix
  fm_ = new vpgl_fundamental_matrix <double> (*p1_,*p2_);

  
  }


  // -------- Add tableaus to draw on
 
  vcl_string type("vsol2D");
  vcl_string name("epipolar");
  vcl_string type2("vsol2D");
  vcl_string name2("epipolar");


  bpro1_storage_sptr 
    n_data  = MANAGER->repository()->new_data_at(type,name,frame_v1),
    n_data2 = MANAGER->repository()->new_data_at(type2,name2,frame_v2);

  if (n_data && n_data2) {
     MANAGER->add_to_display(n_data);
     MANAGER->add_to_display(n_data2);
     MANAGER->display_current_frame();
  } else {
     vcl_cerr << "error: unable to register new data\n";
     return ;
  }

  vgui_tableau_sptr tab_ptr1 = views[v1]->selector()->get_tableau(name);
  vgui_tableau_sptr tab_ptr2 = views[v2]->selector()->get_tableau(name2);
  if (tab_ptr1 && tab_ptr2) {
    tab_l_.vertical_cast(tab_ptr1);
    tab_r_.vertical_cast(tab_ptr2);
  } else {
    vcl_cerr << "error: Could not find child tableaus in selector\n";
    return ;
  }

  vcl_string active;
  active = views[v1]->selector()->active_name();
  views[v1]->selector()->set_active(name);
  views[v1]->selector()->active_to_top();
  views[v1]->selector()->set_active(active);

  active = views[v2]->selector()->active_name();
  views[v2]->selector()->set_active(name2);
  views[v2]->selector()->set_active(name2);
  views[v2]->selector()->active_to_top();
  views[v2]->selector()->set_active(active);


  // Draw epipoles


  vgl_homg_point_2d<double> h_er, h_el;

  fm_->get_epipoles(h_er,h_el);

  vgl_point_2d<double> er(h_er), el(h_el);
   
  vcl_cout << "\nEpipoles: er =  " << er << " ; el = " << el << vcl_endl;

  
  tab_l_->set_foreground(1,0,0.2);
  tab_l_->set_point_radius(7);
  tab_l_->set_current_grouping( "Drawing" );
  tab_l_->add_point(er.x(),er.y());
  tab_l_->set_current_grouping( "default" );
  tab_l_->post_redraw();

  tab_r_->set_foreground(1,0,0.2);
  tab_r_->set_point_radius(7);
  tab_r_->set_current_grouping( "Drawing" );
  tab_r_->add_point(el.x(),el.y());
  tab_r_->set_current_grouping( "defaurt" );
  tab_r_->post_redraw();

  /*
  -8.5555e-08  
    -1.0139e-06   
    0.00041411

   1.4661e-06  
   -3.4055e-06   
   -0.0022325
  -0.00040957    
  0.0038262    
  -0.049207
  // XXX
  vnl_matrix_fixed<double,3,3>& F ;

  F(0,0) = 
  F(0,0) = 
  F(0,0) = 
  F(0,0) = 
  F(0,0) = 
  F(0,0) = 
  F(0,0) = 
  fm_->set_matrix(F);
  */
}

void   
dvpgl_epipolar_tool::deactivate ()
{
  delete fm_;
  vcl_cout << "dvpgl_epipolar_tool OFF\n";
}

bool 
dvpgl_epipolar_tool::set_tableau( const vgui_tableau_sptr& /*tableau*/ )
{
  return true;
}

bool
dvpgl_epipolar_tool::handle( const vgui_event & e, 
                             const bvis1_view_tableau_sptr& /*view*/ )
{
   if (!tab_l_)
     return false;

   float ix, iy;
   vgui_projection_inspector().window_to_image_coordinates(e.wx, e.wy, ix, iy);

   if (e.button  == vgui_LEFT) {
      activated_ = !activated_;
//      vcl_cout << "Offset: " << view_r_->token.offsetX << " " << view_r_->token.offsetY << vcl_endl;
   }

   if (gesture0(e) || (activated_ && e.type == vgui_MOTION)) {
      if (pt_) {
         tab_l_->remove(pt_);
         tab_r_->remove(soview_r_);
      }
      tab_l_->set_foreground(0,1,0.2);
      tab_l_->set_point_radius(5);
      tab_l_->set_current_grouping( "Drawing" );
      pt_ = tab_l_->add_point(ix,iy);
      tab_l_->set_current_grouping( "default" );
      tab_l_->post_redraw();

      vgl_homg_point_2d<double> img1_pt(ix,iy);
      vgl_homg_line_2d<double> l;
      l = fm_->l_epipolar_line(img1_pt);
      tab_r_->set_foreground(0,1,0);
      tab_r_->set_current_grouping( "Drawing" );
      soview_r_ = tab_r_->add_infinite_line(l.a(),l.b(),l.c());
      tab_r_->set_current_grouping( "default" );
      tab_r_->post_redraw();
      return true;
   }

   //  We are not interested in other events,
   //  so pass event to base class:


  return false;
}

//: Display dialog to get params
bool dvpgl_epipolar_tool::
get_epipolar_params(int *initial_view, int *final_view)
{
  vgui_dialog* epipolar_dialog = new vgui_dialog("Epipolar Explorer Tool");
  epipolar_dialog->message("USAGE: Choose initial view and second view then click on the initial.\nNote: do not change frames while using this tool. If you do, please restart the tool.");

//  epipolar_dialog->checkbox("Use canny", iparams->canny);
//  epipolar_dialog->checkbox("Use image", iparams->use_given_image);

  *initial_view = 0;
  epipolar_dialog->field("First  view", *initial_view);

  *final_view = 1;
  epipolar_dialog->field("Second view", *final_view);

  return epipolar_dialog->ask();
}
