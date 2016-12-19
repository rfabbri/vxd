
#include "bvis1_view_menu.h"
#include <vgui/vgui_dialog.h>

void
bvis1_view_menu::add_to_menu( vgui_menu & menu )
{
  add( "Add View" , add_view );
  add( "Remove View" , remove_view );
  add( "Layer per View", layer_per_view );
  
  menu.add( "View" , (*this));
}


void
bvis1_view_menu::add_view()
{
  int offset = 0;
  bool absolute = false;
  //Prompt the user to select a frame offset
  vgui_dialog view_dialog( "New View" );
  view_dialog.field( "Frame offset" , offset );
  view_dialog.checkbox( "Absolute Offset", absolute );
  
  if( view_dialog.ask() )
    bvis1_manager::instance()->add_new_view(offset, absolute);
  bvis1_manager::instance()->display_current_frame();
}


void
bvis1_view_menu::remove_view()
{
  bvis1_manager::instance()->remove_active_view();
  bvis1_manager::instance()->display_current_frame();
}


void
bvis1_view_menu::layer_per_view()
{
  bvis1_manager::instance()->layer_per_view();
  bvis1_manager::instance()->display_current_frame();
}

