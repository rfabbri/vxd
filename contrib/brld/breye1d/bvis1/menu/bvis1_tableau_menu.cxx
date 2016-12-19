
#include "bvis1_tableau_menu.h"
#include <vgui/vgui_dialog.h>

void
bvis1_tableau_menu::add_to_menu( vgui_menu & menu )
{
  add( "New Storage" , make_empty_storage );
  add( "Get Info on Active Tableau" , get_info_on_active_tableau );
  add( "Regenerate All Tableaux" , regenerate_tableaux );
  add( "Cache Tableaux" , cache_tableaux );
   
  menu.add( "Tableaux" , (*this));
}

void
bvis1_tableau_menu::get_info_on_active_tableau()
{
  bvis1_manager::instance()->get_info_on_active_tableau();
}


void
bvis1_tableau_menu::regenerate_tableaux()
{
  bvis1_manager::instance()->regenerate_all_tableaux();
  bvis1_manager::instance()->display_current_frame(true);
}

void
bvis1_tableau_menu::cache_tableaux()
{
  bvis1_manager::instance()->toggle_tableau_cache();
}


void
bvis1_tableau_menu::make_empty_storage()
{
  bvis1_manager::instance()->make_empty_storage();
}
