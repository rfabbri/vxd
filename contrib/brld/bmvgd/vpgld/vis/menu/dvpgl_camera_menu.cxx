
#include "dvpgl_camera_menu.h"
#include <vgui/vgui_dialog.h>
#include <bvis/bvis_mview_manager.h>
void
dvpgl_camera_menu::add_to_menu( vgui_menu & menu )
{
  add( "Add Camaera View" , add_camera_view );
  add( "Remove Camera View" , remove_camera_view );
  
  menu.add( "Camera " , (*this));
}


void
dvpgl_camera_menu::add_camera_view()
{
  //Prompt the user to select a frame offset
  vgui_dialog view_dialog( "New Camera View" );
  if( view_dialog.ask() )
    bvis_mview_manager::instance()->add_new_camera_view();
}


void
dvpgl_camera_menu::remove_camera_view()
{
  bvis_mview_manager::instance()->remove_selected_camera_view();
}


