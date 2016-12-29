//This is vpgld/menu/vpgld_camera_menu.h

#ifndef vpgld_camera_menu_header
#define vpgld_camera_menu_header

//:
// \file
// \brief The vpgld_camera_menu 
// \author Based on original code by  J.L. Mundy
// \date April 23, 2007
//
// This menu contains all the menus related to creating and managing multiple cameras
//
// \verbatim
//  Modifications
// \endverbatim

#include <vgui/vgui_menu.h>
class vpgld_camera_menu : public vgui_menu
{

public:

  vpgld_camera_menu() {}
  virtual ~vpgld_camera_menu() {}

  void add_to_menu( vgui_menu & menu );

  static void add_camera_view();
  static void remove_camera_view();
  
protected:

private:

};

#endif
