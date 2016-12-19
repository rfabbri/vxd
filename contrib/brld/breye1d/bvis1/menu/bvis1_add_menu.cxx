//This is bvis1/bvis1_add_menu.cxx

#include "bvis1_add_menu.h"
#include <bvis1/bvis1_manager.h>

bvis1_add_menu::bvis1_add_menu()
{
}

bvis1_add_menu::~bvis1_add_menu()
{
}

void
bvis1_add_menu::add_to_menu( vgui_menu & menu )
{
  add( "Add new frame" , add_new_frame );

  menu.add( "Add" , (*this) );
}

void
bvis1_add_menu::add_new_frame()
{
  bvis1_manager::instance()->add_new_frame();
}
