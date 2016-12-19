//This is bvis1/menu/bvis1_repository_menu.h

#ifndef bvis1_repository_menu_h_
#define bvis1_repository_menu_h_

//:
// \file
// \brief The bvis1_repository_menu 
// \author Amir Tamrakar
// \date April 21, 2004
//
// This menu contains all menus to manage the repository
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

class bvis1_repository_menu : public vgui_menu
{

public:

  bvis1_repository_menu() {}
  virtual ~bvis1_repository_menu() {}

  void add_to_menu( vgui_menu & menu );

  static void load_repository();
  static void add_to_repository();
  static void save_repository();
  static void view_repository();
  //: clears all data and regenerate tableaux
  static void clear_all();
  
protected:

private:

};

#endif
