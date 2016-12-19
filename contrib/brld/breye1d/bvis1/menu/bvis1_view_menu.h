//This is bvis1/menu/bvis1_view_menu.h

#ifndef bvis1_view_menu_header
#define bvis1_view_menu_header

//:
// \file
// \brief The bvis1_video_menu 
// \author Amir Tamrakar
// \date April 21, 2004
//
// This menu contains all the menus related to creating and managing multiple views of the data
//
// \verbatim
//  Modifications
//    Amir Tamrakar April 21, 2004
//    -Made it a subclass of vgui_menu to make it more flexible
//     There is no reason not to subclass it from vgui_menu
//    -This also removes the need to have a vgui_menu (items) defined inside this class
// \endverbatim

#include <bvis1/bvis1_manager.h>
#include <vgui/vgui_menu.h>

class bvis1_view_menu : public vgui_menu
{

public:

  bvis1_view_menu() {}
  virtual ~bvis1_view_menu() {}

  void add_to_menu( vgui_menu & menu );

  static void add_view();
  static void remove_view();
  static void layer_per_view();
  
protected:

private:

};

#endif
