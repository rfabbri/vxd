//This is dvpgl/menu/dvpgl_camera_menu.h

#ifndef dvpgl_camera_menu_header
#define dvpgl_camera_menu_header

//:
// \file
// \brief The dvpgl_camera_menu 
// \author J.L. Mundy
// \date April 23, 2007
//
// This menu contains all the menus related to creating and managing multiple cameras
//
// \verbatim
//  Modifications
// \endverbatim

#include <vgui/vgui_menu.h>
class dvpgl_camera_menu : public vgui_menu
{

public:

  dvpgl_camera_menu() {}
  virtual ~dvpgl_camera_menu() {}

  void add_to_menu( vgui_menu & menu );

  static void add_camera_view();
  static void remove_camera_view();
  
protected:

private:

};

#endif
