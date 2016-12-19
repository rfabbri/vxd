//This is bvis1/menu/bvis1_video_menu.h

#ifndef bvis1_video_menu_header
#define bvis1_video_menu_header

//:
// \file
// \brief The bvis1_video_menu 
// \author Mark Johnson (mrj)
// \date Fri Jul 18 14:05:46 EDT 2003
//
// \verbatim
//  Modifications
//    Amir Tamrakar April 21, 2004
//    -Made it a subclass of vgui_menu to make it more flexible
//     There is no reason not to subclass it from vgui_menu
//    -This also removes the need to have a vgui_menu (items) defined inside this class
// \endverbatim

#include <vgui/vgui_menu.h>

class bvis1_video_menu: public vgui_menu 
{

public:

  bvis1_video_menu();
  virtual ~bvis1_video_menu();

  void add_to_menu( vgui_menu & menu );
 
  static void play_video();
  static void loop_play_video();
  static void stop_video();
  static void process_and_play_video();

  static void first_frame();
  static void next_frame();
  static void prev_frame();
  static void last_frame();
  static void goto_frame();

  static void process_frame();
   
  static void resetskipframes();
protected:

private:

};

#endif
