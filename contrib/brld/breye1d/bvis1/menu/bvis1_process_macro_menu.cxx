//This is bvis1/bvis1_process_macro_menu.h

#include "bvis1_process_macro_menu.h"

bvis1_process_macro_menu::bvis1_process_macro_menu()
{
}

bvis1_process_macro_menu::~bvis1_process_macro_menu()
{
}

void
bvis1_process_macro_menu::add_to_menu( vgui_menu & menu )
{
  add( "Start Recording" , start_recording );
  add( "Stop Recording", stop_recording );
  separator();
  add( "Delete Last Step" , delete_last );
  add( "Clear All" , clear_all );
  separator();
  add( "Display Process List", view_process_list);

  menu.add( "Process Macro" , (*this) );
}

void
bvis1_process_macro_menu::start_recording()
{
  bvis1_manager::instance()->start_recording_macro();
}

void
bvis1_process_macro_menu::stop_recording()
{
  bvis1_manager::instance()->stop_recording_macro();
}

void
bvis1_process_macro_menu::clear_all()
{
  bvis1_manager::instance()->process_manager()->clear_process_queue();
}


void
bvis1_process_macro_menu::delete_last()
{
  bvis1_manager::instance()->process_manager()->delete_last_process();
}

void
bvis1_process_macro_menu::view_process_list()
{
  vgui_dialog display_dialog("Current process Queue" );

  //display current process list
  display_dialog.message("Current Process Queue:");
  
  vcl_vector<vcl_string> process_list = bvis1_manager::instance()->process_manager()->get_process_queue_list();
  vcl_vector<vcl_string>::iterator i = process_list.begin();
  for (; i!=process_list.end(); i++)
    display_dialog.message( (*i).c_str()); 

  //onlyshow ok button
  display_dialog.set_cancel_button(0);

  display_dialog.ask();
}
