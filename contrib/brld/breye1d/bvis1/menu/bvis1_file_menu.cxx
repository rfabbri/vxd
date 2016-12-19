
#include "bvis1_file_menu.h"
#include <bvis1/bvis1_manager.h>

bvis1_file_menu::bvis1_file_menu()
{
}

bvis1_file_menu::~bvis1_file_menu()
{
}

void
bvis1_file_menu::add_to_menu( vgui_menu & menu )
{
  add( "Save as movie" ,   save_view_as_movie );
  separator();
  add( "Quit" , quit );

  menu.add( "File" , (*this) );
}

void
bvis1_file_menu::quit()
{
  bvis1_manager::instance()->quit();
}

void
bvis1_file_menu::save_view_as_movie()
{
  bvis1_manager::instance()->save_view_as_movie();
}


