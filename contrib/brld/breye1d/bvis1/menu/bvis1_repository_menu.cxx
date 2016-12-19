
#include "bvis1_repository_menu.h"
#include <vgui/vgui_dialog.h>
#include <vidpro1/vidpro1_repository.h>


void
bvis1_repository_menu::add_to_menu( vgui_menu & menu )
{
  add( "Clear All" , clear_all);
  separator();
  add( "Load Repository" , load_repository );
  add( "Add Repository" , add_to_repository );
  add( "Save Repository" , save_repository );
  separator();
  add( "View Repository" , view_repository );
  
  menu.add( "Repository" , (*this));
}

void
bvis1_repository_menu::load_repository()
{
  bvis1_manager::instance()->load_repository();
}

void
bvis1_repository_menu::add_to_repository()
{
  bvis1_manager::instance()->add_to_repository();
}



void
bvis1_repository_menu::save_repository()
{
  bvis1_manager::instance()->save_repository();
}

void
bvis1_repository_menu::view_repository()
{
  bvis1_manager::instance()->view_repository();
}

void 
bvis1_repository_menu::clear_all()
{
   bvis1_manager::instance()->repository()->initialize(1);
   bvis1_manager::instance()->regenerate_all_tableaux();
   bvis1_manager::instance()->display_current_frame(true);
}

