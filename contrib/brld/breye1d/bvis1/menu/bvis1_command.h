// This is breye1/bvis1/menu/bvis1_tool_command.h
#ifndef bvis1_command_h_
#define bvis1_command_h_

//:
// \file
// \brief This file defines vgui_command classes for bvis
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 2/09/04
//
// Each class in this file is derived from vgui_command and
// each must define an execute member function which is called
// when the menu item is selected.
//
// \verbatim
//  Modifications
//      Amir Tamrakar 06/02/04: added the file load/save commands for the new file menu
//      Matt Leotta   07/08/04: merged the load/save commands back into the original command
//                              with a new parameter to disable the queue
// \endverbatim

#include <vgui/vgui_command.h>
#include <bvis1/bvis1_tool_sptr.h>
#include <bpro1/bpro1_process_sptr.h>

// Macros
#define MENU_ADD_TOOL(tool, menu) { bvis1_tool_sptr t = new tool; menu.add(t->name(), new bvis1_tool_command(t)); }

#define MENU_ADD_PROCESS_NAME(name, menu) menu.add( name, \
            new bvis1_process_command( bvis1_manager::instance()->process_manager()->get_process_by_name(name)) )

#define MENU_ADD_PROCESS_NAME2(name, process_name, menu) menu.add( name, \
            new bvis1_process_command( bvis1_manager::instance()->process_manager()->get_process_by_name( process_name ) ) )

#define FILE_MENU_ADD_PROCESS_NAME(name, process_name, menu) menu.add(name, \
            new bvis1_process_command(bvis1_manager::instance()->process_manager()->get_process_by_name(process_name), false))



//: \brief The command for selecting the active tool from a menu
class bvis1_tool_command : public vgui_command
{
 public:
  bvis1_tool_command(const bvis1_tool_sptr& tool) : tool_(tool) {}
  void execute();

  bvis1_tool_sptr tool_;
};


//: \brief The command for selecting a process from a menu
class bvis1_process_command : public vgui_command
{
 public:
  bvis1_process_command(const bpro1_process_sptr& process, bool allow_queue = true)
   : process_(process), allow_queue_(allow_queue) {}
  void execute();

  bpro1_process_sptr process_;
  bool allow_queue_;
};


#endif //bvis1_command_h_
